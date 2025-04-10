import os
import numpy as np
from scipy.interpolate import CubicSpline
from typing import List, Tuple
import matplotlib
from mpc import MPC

matplotlib.use("Agg")
import matplotlib.pyplot as plt

from coordinate_transformer import convert_all_points_to_world_coordinates
from data import IMAGE_POINTS, intrinsic_matrix, rotation_matrix, translation_vector


def split_lanes_by_proximity(
    lanes_points,
) -> Tuple[List[Tuple[int, int]], List[Tuple[int, int]]]:
    all_x_coords = [point[0] for lane in lanes_points for point in lane]
    middle_x = (min(all_x_coords) + max(all_x_coords)) / 2

    left_lanes = []
    right_lanes = []

    for lane in lanes_points:
        avg_x = sum(point[0] for point in lane) / len(lane)

        if avg_x < middle_x:
            left_lanes.append((lane, abs(avg_x - middle_x)))
        else:
            right_lanes.append((lane, abs(avg_x - middle_x)))

    left_lane = [] if not left_lanes else min(left_lanes, key=lambda x: x[1])[0]
    right_lane = [] if not right_lanes else min(right_lanes, key=lambda x: x[1])[0]

    return left_lane, right_lane


def save_points_as_image(left_lane, right_lane, image_path) -> None:
    fig, ax = plt.subplots(figsize=(12, 8))

    x_coords = [point[0] for point in left_lane]
    y_coords = [point[1] for point in left_lane]
    ax.scatter(x_coords, y_coords, color="yellow", s=30, alpha=0.8)
    ax.plot(x_coords, y_coords, color="yellow", linewidth=2, alpha=0.5)

    x_coords = [point[0] for point in right_lane]
    y_coords = [point[1] for point in right_lane]
    ax.scatter(x_coords, y_coords, color="white", s=30, alpha=0.8)
    ax.plot(x_coords, y_coords, color="white", linewidth=2, alpha=0.5)

    ax.set_xlabel("X (pixels)")
    ax.set_ylabel("Y (pixels)")
    ax.set_title("Lane Points Visualization")

    ax.scatter([], [], color="yellow", label="Left Lane")
    ax.scatter([], [], color="white", label="Right Lane")
    ax.legend()

    ax.grid(True, alpha=0.3)
    ax.set_facecolor("black")

    plt.savefig(image_path, dpi=300)
    print(f"Image visualization saved to: {image_path}")


def interpolate_lane_points_polyfit(points, num_points=None, degree=3):
    if num_points is None:
        num_points = len(points) * 5
    t = np.zeros(len(points))
    for i in range(1, len(points)):
        t[i] = t[i - 1] + np.sqrt(np.sum((points[i, :2] - points[i-1, :2]) ** 2))
    if t[-1] > 0:
        t = t / t[-1]
    poly_x = np.polyfit(t, points[:, 0], degree)
    poly_y = np.polyfit(t, points[:, 1], degree)
    if points.shape[1] > 2:
        poly_z = np.polyfit(t, points[:, 2], degree)
    poly_x_func = np.poly1d(poly_x)
    poly_y_func = np.poly1d(poly_y)
    if points.shape[1] > 2:
        poly_z_func = np.poly1d(poly_z)
    t_new = np.linspace(0, 1, num_points)
    x_new = poly_x_func(t_new)
    y_new = poly_y_func(t_new)

    if points.shape[1] > 2:
        z_new = poly_z_func(t_new)
    else:
        z_new = np.zeros_like(x_new)
    return np.column_stack((x_new, y_new, z_new))


def calculate_centerline(left_lane, right_lane):
    max_points = max(len(left_lane), len(right_lane))
    left_lane_interp = interpolate_lane_points_polyfit(left_lane, num_points=max_points)
    right_lane_interp = interpolate_lane_points_polyfit(right_lane, num_points=max_points)
    left_lane_sorted = left_lane_interp[np.argsort(left_lane_interp[:, 1])]
    right_lane_sorted = right_lane_interp[np.argsort(right_lane_interp[:, 1])]
    min_y = max(left_lane_sorted[0, 1], right_lane_sorted[0, 1])
    max_y = min(left_lane_sorted[-1, 1], right_lane_sorted[-1, 1])
    y_values = np.linspace(min_y, max_y, max_points)
    left_interp = CubicSpline(left_lane_sorted[:, 1], left_lane_sorted[:, 0])
    right_interp = CubicSpline(right_lane_sorted[:, 1], right_lane_sorted[:, 0])
    left_x = left_interp(y_values)
    right_x = right_interp(y_values)
    center_x = (left_x + right_x) / 2
    centerline = np.column_stack((center_x, y_values, np.zeros_like(y_values)))
    return centerline


def run_simulation(left_lane_original, right_lane_original, output_path):
    desired_speed = 1.0
    dt = 0.5
    sim_time = 4.5
    steps = int(sim_time / dt)

    num_points = min(len(left_lane_original), len(right_lane_original))
    left_lane = interpolate_lane_points_polyfit(left_lane_original, 200, 3)
    right_lane = interpolate_lane_points_polyfit(right_lane_original, 200, 3)

    save_points_as_image(left_lane, right_lane, os.path.abspath("interp.png"))

    mpc = MPC()

    centerline = calculate_centerline(left_lane, right_lane)
    start_pos = centerline[0]
    next_pos = centerline[1]
    start_heading = np.arctan2(next_pos[1] - start_pos[1], next_pos[0] - start_pos[0])
    state = np.array([start_pos[0], start_pos[1], start_heading, 0.0])

    mpc.update_state(state)

    states = [state.copy()]
    controls = []
    detected_lefts = []
    detected_rights = []

    for i in range(steps):
        accel, steering = mpc.estimate_new_control_from_lane_points(
            left_lane, right_lane, desired_speed
        )
        state = mpc.get_new_state(state, [accel, steering])
        mpc.update_state(state)
        mpc.update_control([accel, steering])

        controls.append(
            [
                # mpc.normalize(accel, mpc.max_deceleration, mpc.max_acceleration),
                # mpc.normalize(
                #     steering, -mpc.max_steering_angle, mpc.max_steering_angle
                # ),
                accel, steering
            ]
        )
        states.append(state.copy())

    states = np.array(states)
    controls = np.array(controls)
    time = np.arange(0, sim_time + dt, dt)

    plot_results(
        states,
        controls,
        time,
        centerline,
        left_lane,
        right_lane,
        detected_lefts,
        detected_rights,
        output_path,
    )


def plot_results(
    states,
    controls,
    time,
    centerline,
    left_lane,
    right_lane,
    detected_lefts,
    detected_rights,
    output_path,
):
    plt.figure(figsize=(12, 8))

    # Plot trajectory
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

    # Plot velocity
    plt.subplot(2, 2, 2)
    plt.plot(time, states[:, 3], "b-")
    # plt.axhline(y=5.0, color="r", linestyle="--", label="Desired Speed")
    plt.title("Vehicle Velocity")
    plt.xlabel("Time (s)")
    plt.ylabel("Velocity (m/s)")
    plt.grid(True)
    plt.legend()

    # Plot acceleration control
    plt.subplot(2, 2, 3)
    plt.plot(time[:-1], controls[:, 0], "r-")
    plt.title("Acceleration Control")
    plt.xlabel("Time (s)")
    plt.ylabel("Acceleration (m/s²)")
    plt.grid(True)

    # Plot steering control
    plt.subplot(2, 2, 4)
    plt.plot(time[:-1], controls[:, 1], "g-")
    plt.title("Steering Control")
    plt.xlabel("Time (s)")
    plt.ylabel("Steering Angle (rad)")
    plt.grid(True)

    plt.tight_layout()
    plt.savefig(output_path)
    print(f"Results saved as {output_path}")


if __name__ == "__main__":
    points = IMAGE_POINTS[2]
    save_points_as_image(
        points[0], points[1], os.path.abspath("original_points.png")
    )
    left_lane, right_lane = split_lanes_by_proximity(points)
    save_points_as_image(left_lane, right_lane, os.path.abspath("selected_points.png"))
    left_lane_world, right_lane_world = convert_all_points_to_world_coordinates(
        left_lane,
        right_lane,
        intrinsic_matrix,
        rotation_matrix,
        translation_vector,
        (720, 1280),
        # (2464, 3264),
    )
    save_points_as_image(
        left_lane_world, right_lane_world, os.path.abspath("world_points.png")
    )
    run_simulation(
        np.array(left_lane_world),
        np.array(right_lane_world),
        os.path.abspath("simulation2.png"),
    )
