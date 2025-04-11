import zmq
import struct

def receive_and_print_floats(subscriber_socket, rounding_digits=2):
    """Receives a byte string, unpacks it into two floats (single precision),
       rounds them, and prints the result."""
    received_data = subscriber_socket.recv()
    float1, float2 = struct.unpack("ff", received_data)
    rounded_float1 = round(float1, rounding_digits)
    rounded_float2 = round(float2, rounding_digits)
    print(f"Received controls: {rounded_float1:.{rounding_digits}f}, {rounded_float2:.{rounding_digits}f}")
    return rounded_float1, rounded_float2

def main():
    context = zmq.Context()
    subscriber = context.socket(zmq.SUB)
    subscriber.setsockopt(zmq.LINGER, 0) 
    subscriber.connect("tcp://localhost:5555")
    subscriber.setsockopt_string(zmq.SUBSCRIBE, "")  # Subscribe to all messages

    print("Controls subscriber started. Press Ctrl+C to exit...")
    try:
        while True:
            receive_and_print_floats(subscriber)
    except KeyboardInterrupt:
        print("\nInterrupt received, shutting down subscriber...")
    finally:
        subscriber.close()
        context.term()
        print("Subscriber resources closed.")

if __name__ == "__main__":
    main()