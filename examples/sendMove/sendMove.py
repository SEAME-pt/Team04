import zmq
import struct

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://10.21.221.30:5555")

# Send command: acceleration 0.5 (50% forward), steering -0.2 (20% left)
data = struct.pack('ff', 0.5, -0.2)
socket.send(data)

# Wait for response
response = socket.recv()
print(f"Response: {response}")