import numpy as np
from typing import Tuple, List
from utils import sort_lane_by_y


def convert_all_points_to_world_coordinates(
    image_left_lane_points: List[Tuple[float, float]],
    image_right_lane_points: List[Tuple[float, float]],
    intrinsic_matrix: np.ndarray,
    rotation_matrix: np.ndarray,
    translation_vector: np.ndarray,
    image_size: Tuple[int, int],
) -> Tuple[List[np.ndarray], List[np.ndarray]]:
    left_lane_world = convert_points(
        image_left_lane_points,
        intrinsic_matrix,
        rotation_matrix,
        translation_vector,
        image_size,
    )
    right_lane_world = convert_points(
        image_right_lane_points,
        intrinsic_matrix,
        rotation_matrix,
        translation_vector,
        image_size,
    )
    return sort_lane_by_y(left_lane_world), sort_lane_by_y(right_lane_world)


def convert_points(
    points: List[Tuple[float, float]],
    intrinsic_matrix: np.ndarray,
    rotation_matrix: np.ndarray,
    translation_vector: np.ndarray,
    image_size: Tuple[int, int],
) -> List[np.ndarray]:
    return [
        convert_point_image_to_world_space(
            np.array(point),
            intrinsic_matrix,
            rotation_matrix,
            translation_vector,
            image_size,
        )
        for point in points
    ]


def convert_point_image_to_world_space(
    image_point: np.ndarray,
    intrinsic_matrix: np.ndarray,
    rotation_matrix: np.ndarray,
    translation_vector: np.ndarray,
    image_size: Tuple[int, int],
) -> np.ndarray:
    width, height = image_size

    x_centered = image_point[0] - width / 2
    y_centered = image_point[1] - height / 2
    pixel_homogeneous = np.array([x_centered, y_centered, 1.0])

    K_inv = np.linalg.inv(intrinsic_matrix)
    camera_ray = K_inv @ pixel_homogeneous

    R_inv = np.linalg.inv(rotation_matrix)
    r3 = R_inv[:, 2]
    t = translation_vector.flatten()
    scale = (0 - t[2]) / (r3 @ camera_ray)  # ground plane z = 0
    world_point = R_inv @ (scale * camera_ray) + t
    return world_point
