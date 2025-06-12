import onnx
from onnxconverter_common import float16

model = onnx.load("yolopv2_post_192x320.onnx")
model_fp16 = float16.convert_float_to_float16(model)
onnx.save(model_fp16, "model_fp16.onnx")
