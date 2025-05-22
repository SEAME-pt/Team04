import zmq
import struct
import time

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://10.21.221.30:5555")

# Move forward and turn 80% right for 5 seconds
data = struct.pack('ff', 0.5, 0.8)  # 50% forward, 80% right
socket.send(data)
response = socket.recv()
print(f"Response: {response}")
time.sleep(5)

# Centralize the steering and stop the motor
data = struct.pack('ff', 0.0, 0.0)  # Stop motor, centralize steering
socket.send(data)
response = socket.recv()
print(f"Response: {response}")

# Move backward and turn 80% left for 5 seconds
data = struct.pack('ff', -0.5, -0.8)  # 50% backward, 80% left
socket.send(data)
response = socket.recv()
print(f"Response: {response}")
time.sleep(5)

# Centralize the steering and stop the motor
data = struct.pack('ff', 0.0, 0.0)  # Stop motor, centralize steering
socket.send(data)
response = socket.recv()
print(f"Response: {response}")