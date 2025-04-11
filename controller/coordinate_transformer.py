import numpy as np


def convert_point_image_to_world_space(
    image_point: np.ndarray,
    intrinsic_matrix: np.ndarray,
    rotation_matrix: np.ndarray,
    translation_vector: np.ndarray,
    image_size,
) -> np.ndarray:
    W, H = image_size

    # Adjust the pixel coordinate system
    x_ajusted = image_point[0] - (W / 2)
    y_ajusted = image_point[1] - (H / 2)
    uv_homog = np.array([x_ajusted, y_ajusted, 1.0])

    K_inv = np.linalg.inv(intrinsic_matrix)
    camera_ray = K_inv @ uv_homog

    R_inv = np.linalg.inv(rotation_matrix)
    r1, r2, r3 = R_inv[:, 0], R_inv[:, 1], R_inv[:, 2]

    t_x, t_y, t_z = translation_vector.flatten()

    ground_plane_z = 0
    lambda_val = (ground_plane_z - t_z) / (r3 @ camera_ray)

    world_coords = (R_inv @ (lambda_val * camera_ray)) + translation_vector.flatten()
    return world_coords


def convert_all_points_to_world_coordinates(
    image_left_lane_points,
    image_right_lane_points,
    intrinsic_matrix,
    rotation_matrix,
    translation_vector,
    image_size,
):
    left_lane_world = []
    right_lane_world = []

    for point in image_left_lane_points:
        point_array = np.array(point)
        world_point = convert_point_image_to_world_space(
            point_array,
            intrinsic_matrix,
            rotation_matrix,
            translation_vector,
            image_size,
        )
        left_lane_world.append(world_point)
    for point in image_right_lane_points:
        point_array = np.array(point)
        world_point = convert_point_image_to_world_space(
            point_array,
            intrinsic_matrix,
            rotation_matrix,
            translation_vector,
            image_size,
        )
        right_lane_world.append(world_point)

    return left_lane_world, right_lane_world
