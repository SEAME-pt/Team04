import numpy as np
from mpc import MPC
from coordinate_transformer import convert_all_points_to_world_coordinates
from typing import List, Tuple
from utils import (
    split_lanes_by_proximity,
    interpolate_lane_points_polyfit,
    run_mpc,
    normalize_minus1_and_1,
)

# Camera Parameters
INTRINSIC_MATRIX = np.array([
    [640,   0, 320],  # fx,  0, cx
    [  0, 640, 240],  #  0, fy, cy
    [  0,   0,   1]
])
ROTATION_MATRIX = np.array([
    [-1, 0, 0],
    [ 0, 1, 0],
    [ 0, 0, 1]
])
TRANSLATION_VECTOR = np.array([
    [0],
    [0],
    [1]
])
IMAGE_SIZE = (720, 1280)
FPS = 20

# MPC Parameters
DT = round(1 / FPS, 3)
PREDICTION_HORIZON = 10
CONTROL_HORIZON = 5

# Other
POLY_DEGREE = 3


def runner(
    points: List[List[Tuple[float, float]]],
    desired_velocity: float,
    current_velocity: float
) -> Tuple[Tuple[float, float], Tuple[List, List]]:

    left_lane_orig, right_lane_orig = split_lanes_by_proximity(points)
    left_lane_world, right_lane_world = convert_all_points_to_world_coordinates(
        left_lane_orig,
        right_lane_orig,
        INTRINSIC_MATRIX,
        ROTATION_MATRIX,
        TRANSLATION_VECTOR,
        IMAGE_SIZE,
    )

    poly_num_points = max(len(left_lane_world), len(right_lane_world)) * 5
    left_lane = interpolate_lane_points_polyfit(np.array(left_lane_world), poly_num_points, POLY_DEGREE)
    right_lane = interpolate_lane_points_polyfit(np.array(right_lane_world), poly_num_points, POLY_DEGREE)

    mpc = MPC(PREDICTION_HORIZON, CONTROL_HORIZON, DT)
    optimal_control, _, _ = run_mpc(
        mpc,
        np.array(left_lane),
        np.array(right_lane),
        desired_velocity,
        current_velocity
    )

    bounded_throttle = normalize_minus1_and_1(optimal_control[0], mpc.max_deceleration, mpc.max_acceleration)
    bounded_streering = normalize_minus1_and_1(optimal_control[1], -mpc.max_steering_angle, mpc.max_steering_angle)

    return [bounded_throttle, bounded_streering], [left_lane, right_lane]
