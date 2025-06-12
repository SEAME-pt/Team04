import onnx
import onnx.helper # We need this for the new helper function
from onnx import TensorProto # Just in case you want to use it for dummy models later
import numpy as np # Import numpy to work with dtypes
import os # For checking if the model file exists

def print_onnx_graph_info(model_path):
    """
    Loads an ONNX model and prints its graph structure.
    Adapted to handle deprecated ONNX mappings and numpy dtype objects.
    """
    if not os.path.exists(model_path):
        print(f"Error: Model file not found at '{model_path}'")
        return

    try:
        onnx_model = onnx.load(model_path)
        graph = onnx_model.graph

        print(f"Model Name: {graph.name}")
        print(f"Model Version: {onnx_model.ir_version}") # ONNX IR version

        print("\n--- Graph Inputs ---")
        for input_info in graph.input:
            # Get the NumPy dtype object using the recommended helper
            np_dtype = onnx.helper.tensor_dtype_to_np_dtype(input_info.type.tensor_type.elem_type)
            print(f"  Name: {input_info.name}")
            # Access the 'name' attribute of the numpy dtype object
            print(f"  Type: {np_dtype.name if isinstance(np_dtype, np.dtype) else str(np_dtype)}")
            # Handle dynamic shapes (dim_param) vs. static shapes (dim_value)
            shape = [d.dim_value if d.dim_value != 0 else d.dim_param for d in input_info.type.tensor_type.shape.dim]
            print(f"  Shape: {shape}")
            print("-" * 20)

        print("\n--- Graph Outputs ---")
        for output_info in graph.output:
            # Get the NumPy dtype object using the recommended helper
            np_dtype = onnx.helper.tensor_dtype_to_np_dtype(output_info.type.tensor_type.elem_type)
            print(f"  Name: {output_info.name}")
            # Access the 'name' attribute of the numpy dtype object
            print(f"  Type: {np_dtype.name if isinstance(np_dtype, np.dtype) else str(np_dtype)}")
            shape = [d.dim_value if d.dim_value != 0 else d.dim_param for d in output_info.type.tensor_type.shape.dim]
            print(f"  Shape: {shape}")
            print("-" * 20)

        print("\n--- Graph Initializers (Weights/Biases) ---")
        if graph.initializer: # Added check for empty initializer list
            for initializer in graph.initializer:
                # Get the NumPy dtype object using the recommended helper
                np_dtype = onnx.helper.tensor_dtype_to_np_dtype(initializer.data_type)
                print(f"  Name: {initializer.name}")
                # Access the 'name' attribute of the numpy dtype object
                print(f"  Type: {np_dtype.name if isinstance(np_dtype, np.dtype) else str(np_dtype)}")
                print(f"  Shape: {initializer.dims}")
                print("-" * 20)
        else:
            print("  No initializers found.")


        print("\n--- Graph Nodes (Operations) ---")
        if graph.node: # Added check for empty node list
            for i, node in enumerate(graph.node):
                print(f"Node {i+1}:")
                print(f"  Name: {node.name if node.name else 'N/A'}")
                print(f"  Operator Type: {node.op_type}")
                print(f"  Inputs: {', '.join(node.input)}")
                print(f"  Outputs: {', '.join(node.output)}")
                if node.attribute:
                    print("  Attributes:")
                    for attr in node.attribute:
                        attr_value = onnx.helper.get_attribute_value(attr)
                        # Decode bytes attributes for better readability
                        if isinstance(attr_value, bytes):
                            try:
                                attr_value = attr_value.decode('utf-8')
                            except UnicodeDecodeError:
                                pass # Keep as bytes if not decodable
                        print(f"    - {attr.name}: {attr_value}")
                print("-" * 30)
        else:
            print("  No nodes found.")

    except Exception as e:
        print(f"An unexpected error occurred while processing the ONNX model: {e}")

def main():
    """
    Main function to handle model loading and printing.
    """
    # Option 1: Use a predefined model path
    model_file = 'yolopv2_post_192x320.onnx'

    # Option 3: Get model path from command-line arguments
    # if len(sys.argv) > 1:
    #     model_file = sys.argv[1]
    # else:
    #     print("Usage: python your_script_name.py <path_to_onnx_model>")
    #     print("No model path provided, using dummy model for demonstration.")
    #     model_file = create_dummy_onnx_model()


    print(f"\n--- Analyzing ONNX model: {model_file} ---")
    print_onnx_graph_info(model_file)
    print("\n--- Analysis Complete ---")

if __name__ == "__main__":
    main()
