import numpy as np
from scipy.interpolate import CubicSpline
from typing import List, Tuple


def split_lanes_by_proximity(
    lanes_points: List[List[Tuple[float, float]]],
) -> Tuple[List[Tuple[float, float]], List[Tuple[float, float]]]:
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
    return sort_lane_by_y(left_lane), sort_lane_by_y(right_lane)


def sort_lane_by_y(lane_points: List[np.ndarray]) -> List[np.ndarray]:
    return sorted(lane_points, key=lambda point: point[1])


def calculate_centerline(left_lane: np.ndarray, right_lane: np.ndarray) -> np.ndarray:
    num_points = max(len(left_lane), len(right_lane))
    left_lane_sorted = left_lane[np.argsort(left_lane[:, 1])]
    right_lane_sorted = right_lane[np.argsort(right_lane[:, 1])]
    min_y = max(left_lane_sorted[0, 1], right_lane_sorted[0, 1])
    max_y = min(left_lane_sorted[-1, 1], right_lane_sorted[-1, 1])
    y_values = np.linspace(min_y, max_y, num_points)
    left_interp = CubicSpline(left_lane_sorted[:, 1], left_lane_sorted[:, 0])
    right_interp = CubicSpline(right_lane_sorted[:, 1], right_lane_sorted[:, 0])
    left_x = left_interp(y_values)
    right_x = right_interp(y_values)
    center_x = (left_x + right_x) / 2
    centerline = np.column_stack((center_x, y_values, np.zeros_like(y_values)))
    return centerline


def get_position_heading_from_centerline(centerline: np.ndarray) -> np.ndarray:
    start_pos = centerline[0]
    next_pos = centerline[1]
    return np.arctan2(next_pos[1] - start_pos[1], next_pos[0] - start_pos[0])


def interpolate_lane_points_polyfit(
    points: np.ndarray, num_points: int, degree: int = 3
) -> np.ndarray:
    if points.shape[0] <= degree:
        raise ValueError(
            "Number of input points must be greater than the polynomial degree."
        )

    t = np.zeros(len(points))
    for i in range(1, len(points)):
        t[i] = t[i - 1] + np.linalg.norm(points[i, :2] - points[i - 1, :2])

    if t[-1] > 0:
        t /= t[-1]
    else:
        return np.tile(points[0], (num_points, 1))

    poly_x_func = np.poly1d(np.polyfit(t, points[:, 0], degree))
    poly_y_func = np.poly1d(np.polyfit(t, points[:, 1], degree))
    if points.shape[1] > 2:
        poly_z_func = np.poly1d(np.polyfit(t, points[:, 2], degree))
    else:
        poly_z_func = lambda x: np.zeros_like(x)

    t_new = np.linspace(0, 1, num_points)
    x_new = poly_x_func(t_new)
    y_new = poly_y_func(t_new)
    z_new = poly_z_func(t_new)
    return np.column_stack((x_new, y_new, z_new))


def run_mpc(mpc, left_lane: np.ndarray, right_lane: np.ndarray, desired_velocity: float, current_velocity: float) -> Tuple[List[float], np.ndarray, np.ndarray]:
    centerline = calculate_centerline(left_lane, right_lane)
    pos_heading = get_position_heading_from_centerline(centerline)  # Estimate current heading from centerline
    current_state = [0.0, max(left_lane[0][1], right_lane[0][1]), pos_heading, current_velocity]  # Set current position to image center
    mpc.set_state(current_state)
    optimal_control = mpc.estimate_new_control_from_lane_points(
        left_lane, right_lane, desired_velocity
    )
    new_state = mpc.get_new_state(mpc.get_state(), np.array(optimal_control))
    return optimal_control, new_state, centerline


def normalize_minus1_and_1(point: float, min_val: float, max_val: float) -> float:
    return round(((2 * (point - min_val)) / (max_val - min_val)) - 1, 3)
