import cv2 
import argparse
import onnxruntime as ort
import time
import zmq
import struct
from models import YOLOPv2

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
    parser.add_argument("--modelpath", type=str, default='lane-detector-yolo/models/yolopv2_post_192x320.onnx', help="model path")
    parser.add_argument("--imgpath", type=str, default='lane-detector-yolo/images/1.jpg', help="image path")
    parser.add_argument("--confThreshold", default=0.5, type=float, help='class confidence')
    args = parser.parse_args()

    print(ort.__version__)
    print(cv2.__version__)

    net = YOLOPv2(args.modelpath, confThreshold=args.confThreshold)
    srcimg = cv2.imread(args.imgpath)
    srcimg = net.detect(srcimg)

    # num_runs=1000
    # start_time = time.time()
    # for i in range(num_runs):
    #     print(i)
    #     net.detect(srcimg)
    # end_time = time.time()

    # total_time = end_time - start_time
    # fps = num_runs / total_time
    # print(f'FPS: {fps}')

    cv2.imwrite("/workspaces/Team04/lane-detector-yolo/test.jpg", srcimg)

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