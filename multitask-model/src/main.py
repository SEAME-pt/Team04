import cv2 
import argparse
import onnxruntime as ort
import time
import zmq
import struct
from models import YOLOPv2, MultiTask
from typing import List, Tuple
import matplotlib.pyplot as plt

def send_controls(publisher, acceleration, steering):
    message_to_send = struct.pack("ff", acceleration, steering) 
    publisher.send(message_to_send)
    print(f"Sent controls: {acceleration}, {steering} as bytes: {message_to_send.hex()}")

if __name__=='__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--modelpath", type=str, default='lane-detector-yolo/models/yolopv3_post_192x320.onnx', help="model path")
    parser.add_argument("--imgpath", type=str, default='lane-detector-yolo/images/5.jpg', help="image path")
    parser.add_argument("--confThreshold", default=0.5, type=float, help='class confidence')
    args = parser.parse_args()

    print(ort.__version__)
    print(cv2.__version__)

    net = MultiTask(args.modelpath, confThreshold=args.confThreshold)
    srcimg = cv2.imread(args.imgpath)
    print(type(srcimg))
    srcimg, stats = net.detect(srcimg)

    num_runs=100
    total_time_inference = 0.0
    total_time_nms = 0.0
    start_time = time.time()
    for i in range(num_runs):
        srcimg, (inf_time, nms_time) = net.detect(srcimg)
        total_time_inference += inf_time
        total_time_nms += nms_time
    end_time = time.time()

    total_time = end_time - start_time
    fps_total = num_runs / total_time

    fps_inf = num_runs / total_time_inference
    fps_nms = num_runs / total_time_nms
    print(f"FPS Test: {fps_inf:.2f} frames per second")
    print(f"FPS NMS: {fps_nms:.2f} frames per second")
    print(f'FPS: {fps_total:.2f} frames per second')

    cv2.imwrite("/workspaces/Team04/lane-detector-yolo/test.jpg", srcimg)

    # ZeroMQ Publisher Initialization
    context = zmq.Context()
    publisher = context.socket(zmq.PUB)
    publisher.setsockopt(zmq.LINGER, 0) 
    publisher.bind("tcp://*:5555")

    print("Controls publisher started. Press Ctrl+C to stop...")
    try:
        while True:
            acceleration, steering = 1, 1
            send_controls(publisher, acceleration, steering)
            time.sleep(1)  # Publish every 1 second
    except KeyboardInterrupt:
        print("\nInterrupt received, shutting down publisher...")
    finally:
        publisher.close()
        context.term()
        print("Publisher resources closed.")