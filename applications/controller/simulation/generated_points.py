import argparse
from pathlib import Path
from typing import Tuple, List

import numpy as np
import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

from mpc import MPC


def generate_lane_edges(
    centerline: np.ndarray, lane_width: float = 1
) -> Tuple[np.ndarray, np.ndarray]:
    left_edge = []
    right_edge = []

    for i in range(len(centerline)):
        x, y = centerline[i]

        if i < len(centerline) - 1:
            next_x, next_y = centerline[i + 1]
            heading = np.arctan2(next_y - y, next_x - x)
        else:
            prev_x, prev_y = centerline[i - 1]
            heading = np.arctan2(y - prev_y, x - prev_x)

        normal = heading + np.pi / 2

        left_x = x + lane_width / 2 * np.cos(normal)
        left_y = y + lane_width / 2 * np.sin(normal)
        right_x = x - lane_width / 2 * np.cos(normal)
        right_y = y - lane_width / 2 * np.sin(normal)

        left_edge.append([left_x, left_y])
        right_edge.append([right_x, right_y])

    return np.array(left_edge), np.array(right_edge)


def simulate_camera_detection(
    vehicle_pos: np.ndarray,
    left_lane: interp1d,
    right_lane: interp1d,
    view_distance: float = 20,
    noise_std: float = 0.1,
) -> Tuple[np.ndarray, np.ndarray]:
    x, y = vehicle_pos[:2]
    x_ahead = np.linspace(x, x + view_distance, 10)

    left_y = left_lane(x_ahead)
    right_y = right_lane(x_ahead)

    if noise_std > 0:
        left_y += np.random.normal(0, noise_std, left_y.shape)
        right_y += np.random.normal(0, noise_std, right_y.shape)

    left_points = np.column_stack((x_ahead, left_y))
    right_points = np.column_stack((x_ahead, right_y))

    return left_points, right_points


def run_simulation(output_path: str) -> None:
    x = np.linspace(0, 500, 500)
    y = 20 * np.sin(x / 20) + 20 * np.cos(x / 50) + 5 * np.sin(x / 10)

    centerline = np.column_stack((x, y))
    left_lane_full, right_lane_full = generate_lane_edges(centerline, lane_width=3.5)

    left_lane = interp1d(
        left_lane_full[:, 0], left_lane_full[:, 1], fill_value="extrapolate"
    )
    right_lane = interp1d(
        right_lane_full[:, 0], right_lane_full[:, 1], fill_value="extrapolate"
    )

    mpc = MPC()

    start_pos = centerline[0]
    next_pos = centerline[1]

    start_heading = np.arctan2(next_pos[1] - start_pos[1], next_pos[0] - start_pos[0])
    state = np.array([start_pos[0], start_pos[1], start_heading, 0.0])
    mpc.update_state(state)

    desired_speed = 5.0
    dt = 0.1
    sim_time = 30.0
    steps = int(sim_time / dt)

    states = [state.copy()]
    controls = []
    detected_lefts = []
    detected_rights = []

    for _ in range(steps):
        left_points, right_points = simulate_camera_detection(
            state, left_lane, right_lane, view_distance=20, noise_std=0.05
        )

        detected_lefts.append(left_points.copy())
        detected_rights.append(right_points.copy())

        accel, steering = mpc.estimate_new_control_from_lane_points(
            left_points, right_points, desired_speed
        )
        state = mpc.get_new_state(state, [accel, steering])
        mpc.update_state(state)
        mpc.update_control([accel, steering])

        controls.append(
            [
                normalize_minus1_and_1(
                    accel, mpc.max_deceleration, mpc.max_acceleration
                ),
                normalize_minus1_and_1(
                    steering, -mpc.max_steering_angle, mpc.max_steering_angle
                ),
            ]
        )
        states.append(state.copy())

    time = np.arange(0, sim_time + dt, dt)

    plot_results(
        np.array(states),
        np.array(controls),
        time,
        centerline,
        left_lane_full,
        right_lane_full,
        detected_lefts,
        detected_rights,
        output_path,
    )


def normalize_minus1_and_1(point: float, min_val: float, max_val: float) -> float:
    return ((2 * (point - min_val)) / (max_val - min_val)) - 1


def plot_results(
    states: np.ndarray,
    controls: np.ndarray,
    time: np.ndarray,
    centerline: np.ndarray,
    left_lane: np.ndarray,
    right_lane: np.ndarray,
    detected_lefts: List[np.ndarray],
    detected_rights: List[np.ndarray],
    output_path: str,
) -> None:
    plt.figure(figsize=(12, 8))
    plt.subplot(2, 2, 1)
    plt.plot(centerline[:, 0], centerline[:, 1], "g--", label="Road Centerline")
    plt.plot(left_lane[:, 0], left_lane[:, 1], "k-", alpha=0.5, label="Lane Edges")
    plt.plot(right_lane[:, 0], right_lane[:, 1], "k-", alpha=0.5)
    plt.plot(states[:, 0], states[:, 1], "b-", label="Vehicle Trajectory")
    plt.scatter(states[0, 0], states[0, 1], color="r", s=100, label="Start")

    detection_interval = max(1, len(detected_lefts) // 3)
    for i in range(0, len(detected_lefts), detection_interval):
        plt.scatter(
            detected_lefts[i][:, 0],
            detected_lefts[i][:, 1],
            color="orange",
            s=5,
            alpha=0.3,
        )
        plt.scatter(
            detected_rights[i][:, 0],
            detected_rights[i][:, 1],
            color="orange",
            s=5,
            alpha=0.3,
        )

    plt.title("Vehicle Trajectory with Lane Detection")
    plt.xlabel("X Position (m)")
    plt.ylabel("Y Position (m)")
    plt.axis("equal")
    plt.grid(True)
    plt.legend()

    plt.subplot(2, 2, 2)
    plt.plot(time, states[:, 3], "b-")
    plt.axhline(y=5.0, color="r", linestyle="--", label="Desired Speed")
    plt.title("Vehicle Speed")
    plt.xlabel("Time (s)")
    plt.ylabel("Speed (m/s)")
    plt.grid(True)
    plt.legend()

    plt.subplot(2, 2, 3)
    plt.plot(time[:-1], controls[:, 0], "r-")
    plt.title("Acceleration Control")
    plt.xlabel("Time (s)")
    plt.ylabel("Acceleration (m/s²)")
    plt.grid(True)

    plt.subplot(2, 2, 4)
    plt.plot(time[:-1], controls[:, 1], "g-")
    plt.title("Steering Control")
    plt.xlabel("Time (s)")
    plt.ylabel("Steering Angle (rad)")
    plt.grid(True)

    plt.tight_layout()
    plt.savefig(output_path)
    print(f"Results saved as {output_path}")


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Run lane-following MPC simulation and save results."
    )
    parser.add_argument(
        "-o",
        "--output",
        type=str,
        default=str(Path(__file__).parent / "simulation_results.png"),
        help="Path to save the simulation results image.",
    )
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_arguments()
    run_simulation(args.output)
