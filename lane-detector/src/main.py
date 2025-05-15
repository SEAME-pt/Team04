import cv2 
import numpy as np
import time
import zmq
import struct
import argparse
from models import UltraFastLaneDetectionV2

def fit_and_draw_polynomial(img, lane_points, color=(0, 0, 255), degree=2):
    """Fits a polynomial to lane points and draws it on the image."""

    if len(lane_points) < degree + 1:  # Need enough points for the fit
        return

    x_coords, y_coords = zip(*lane_points)  # Separate x and y coordinates

    try:
        coefficients = np.polyfit(y_coords, x_coords, degree)
        polynomial = np.poly1d(coefficients)

        min_y = min(y_coords)
        max_y = max(y_coords)
        y_range = np.arange(min_y, max_y, 1)
        x_fitted = polynomial(y_range).astype(np.int32)

        for x, y in zip(x_fitted, y_range):
            if 0 <= x < img.shape[1] and 0 <= y < img.shape[0]:
                cv2.circle(img, (x, y), 3, color, -1)  # Draw fitted points
    except np.linalg.LinAlgError:
        print("Polynomial fitting failed for a lane.")

def get_lane_center_x(lane_points, image_width):
    """Calculates the approximate horizontal center of a lane."""
    if not lane_points:
        return None
    x_coords, _ = zip(*lane_points)
    return sum(x_coords) / len(x_coords)

def get_controls(lanes):
    if not lanes:
        return None
    
    acceleration = -0.3
    steering = 0.56
    return acceleration, steering

def send_controls(publisher, acceleration, steering):
    message_to_send = struct.pack("ff", acceleration, steering) 
    publisher.send(message_to_send)
    print(f"Sent controls: {acceleration}, {steering} as bytes: {message_to_send.hex()}")

if __name__=='__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--imgpath", type=str, default='lane-detector/images/5.jpg', help="image path")
    parser.add_argument("--modelpath", type=str, default='lane-detector/models/ufldv3_tusimple_res18_320x800.onnx', help="onnx modelpath")
    args = parser.parse_args()

    net = UltraFastLaneDetectionV2(args.modelpath)
    srcimg = cv2.imread(args.imgpath)

    # INFERENCE
    coords = net.detect(srcimg)
    for lane in coords:
        for coord in lane:
            cv2.circle(srcimg, coord, 3, (0, 255, 0), -1)

    cv2.imwrite("/workspaces/Team04/lane-detector/test.jpg", srcimg)

    # FPS Test
    # num_runs=1000
    # start_time = time.time()
    # for _ in range(num_runs):
    #     coords = net.detect(srcimg)
    # end_time = time.time()

    # total_time = end_time - start_time
    # fps = num_runs / total_time
    # print(f'FPS: {fps}')

    # image_center_x = srcimg.shape[1] / 2  # Calculate the image center
    # if len(coords) > 2:
    #     lane_centers = []
    #     for lane in coords:
    #         center_x = get_lane_center_x(lane, srcimg.shape[1])
    #         if center_x is not None:
    #             lane_centers.append((abs(center_x - image_center_x), lane)) # store the distance from the center, and the lane.

    #     lane_centers.sort(key=lambda x: x[0])  # Sort by distance from center, ascending
    #     closest_lanes = [lane_centers[0][1], lane_centers[1][1]]  # Get the two closest lanes

    #     for lane in closest_lanes:
    #         fit_and_draw_polynomial(srcimg, lane, color=(0, 0, 255))
    # elif len(coords) > 0:
    #     for lane in coords:
    #         fit_and_draw_polynomial(srcimg, lane, color=(0, 0, 255))

    # ZeroMQ Publisher Initialization
    context = zmq.Context()
    publisher = context.socket(zmq.PUB)
    publisher.setsockopt(zmq.LINGER, 0) 
    publisher.bind("tcp://*:5555")

    print("Controls publisher started. Press Ctrl+C to stop...")
    try:
        while True:
            controls = get_controls([3])

            if controls is not None:
                acceleration, steering = controls
                send_controls(publisher, acceleration, steering)
            time.sleep(1)  # Publish every 1 second
    except KeyboardInterrupt:
        print("\nInterrupt received, shutting down publisher...")
    finally:
        publisher.close()
        context.term()
        print("Publisher resources closed.")
