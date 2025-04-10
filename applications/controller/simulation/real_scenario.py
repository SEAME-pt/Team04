import argparse
from pathlib import Path
from typing import List, Tuple
import matplotlib

from runner import runner

matplotlib.use("Agg")
import matplotlib.pyplot as plt

from data import IMAGE_POINTS


def save_points_as_image(
    left_lane: List[Tuple[float, float]],
    right_lane: List[Tuple[float, float]],
    image_path: str,
) -> None:
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


def parser():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-d",
        "--desired_velocity",
        type=float,
        required=False,
        default=10.0,
        help="Desired velocity",
    )
    parser.add_argument(
        "-c",
        "--current_velocity",
        type=float,
        required=False,
        default=1.0,
        help="Desired velocity",
    )
    parser.add_argument(
        "-o",
        "--output_image_path",
        type=Path,
        required=False,
        default=Path(__file__).parent / "simulation_results.png",
        help="Output path to the lane points after image to world conversion",
    )
    return parser.parse_args()


if __name__ == "__main__":
    args = parser()
    points = IMAGE_POINTS[2]
    controls, lanes = runner(points, args.desired_velocity, args.current_velocity)
    save_points_as_image(lanes[0], lanes[1], args.output_image_path)
    print(f"Acceleration: {controls[0]} | Steering: {controls[1]}")
