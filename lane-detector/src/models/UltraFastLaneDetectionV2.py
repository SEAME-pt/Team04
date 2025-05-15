import cv2 
import numpy as np
import os
import onnxruntime

class UltraFastLaneDetectionV2:
    def __init__(self, modelpath):
        # Initialize model
        session_option = onnxruntime.SessionOptions()
        session_option.log_severity_level = 3
        # self.session = onnxruntime.InferenceSession(path, providers=['CUDAExecutionProvider', 'CPUExecutionProvider'])
        self.session = onnxruntime.InferenceSession(modelpath, sess_options=session_option, providers=['CUDAExecutionProvider', 'CPUExecutionProvider'])
        model_inputs = self.session.get_inputs()
        self.input_name = model_inputs[0].name
        self.input_shape = model_inputs[0].shape
        print(self.input_shape)
        self.input_height = int(self.input_shape[2])
        self.input_width = int(self.input_shape[3])

        model_outputs = self.session.get_outputs()
        self.output_names = [model_outputs[i].name for i in range(len(model_outputs))]
        dataset = os.path.basename(modelpath).split('_')[1]
        if dataset == 'culane':
            num_row = 72
            num_col = 81
            self.row_anchor = np.linspace(0.42, 1, num_row)
            self.col_anchor = np.linspace(0, 1, num_col)
            self.train_width = 1600
            self.train_height = 320
            self.crop_ratio = 0.6
        else:
            num_row = 56
            num_col = 41
            self.row_anchor = np.linspace(160, 710, num_row) / 720
            self.col_anchor = np.linspace(0, 1, num_col)
            self.train_width = 800
            self.train_height = 320
            self.crop_ratio = 0.8
        self.mean_ = np.array([0.485, 0.456, 0.406], dtype=np.float32).reshape((1, 1, 3))
        self.std_ = np.array([0.229, 0.224, 0.225], dtype=np.float32).reshape((1, 1, 3))

    def softmax(self, x, axis=0):
        exp_x = np.exp(x)
        return exp_x / np.sum(exp_x, axis=axis)
    def pred2coords(self, pred, local_width=1, original_image_width=1640, original_image_height=590):
        loc_row, loc_col, exist_row, exist_col = pred
        batch_size, num_grid_row, num_cls_row, num_lane_row = loc_row.shape
        batch_size, num_grid_col, num_cls_col, num_lane_col = loc_col.shape

        max_indices_row = loc_row.argmax(axis=1)
        # n , num_cls, num_lanes
        valid_row = exist_row.argmax(axis=1)
        # n, num_cls, num_lanes

        max_indices_col = loc_col.argmax(axis=1)
        # n , num_cls, num_lanes
        valid_col = exist_col.argmax(axis=1)
        # n, num_cls, num_lanes

        coords = []

        row_lane_idx = [1, 2]
        col_lane_idx = [0, 3]

        for i in row_lane_idx:
            tmp = []
            if valid_row[0, :, i].sum() > num_cls_row / 2:
                for k in range(valid_row.shape[1]):
                    if valid_row[0, k, i]:
                        all_ind = np.array(range(max(0, max_indices_row[0, k, i] - local_width), min(num_grid_row - 1, max_indices_row[0, k, i] + local_width) + 1))

                        out_tmp = (self.softmax(loc_row[0, all_ind, k, i], axis=0) * all_ind.astype(np.float32)).sum() + 0.5
                        out_tmp = out_tmp / (num_grid_row - 1) * original_image_width
                        tmp.append((int(out_tmp), int(self.row_anchor[k] * original_image_height)))
                coords.append(tmp)

        for i in col_lane_idx:
            tmp = []
            if valid_col[0, :, i].sum() > num_cls_col / 4:
                for k in range(valid_col.shape[1]):
                    if valid_col[0, k, i]:
                        all_ind = np.array(range(max(0, max_indices_col[0, k, i] - local_width), min(num_grid_col - 1, max_indices_col[0, k, i] + local_width) + 1))

                        out_tmp = (self.softmax(loc_col[0, all_ind, k, i], axis=0) * all_ind.astype(np.float32)).sum() + 0.5

                        out_tmp = out_tmp / (num_grid_col - 1) * original_image_height
                        tmp.append((int(self.col_anchor[k] * original_image_width), int(out_tmp)))
                coords.append(tmp)
        return coords

    def detect(self, srcimg):
        img_h, img_w = srcimg.shape[:2]
        img = cv2.resize(srcimg, (self.train_width, int(self.train_height / self.crop_ratio)))
        img = (img.astype(np.float32) / 255.0 - self.mean_) / self.std_
        img = img[-self.train_height:, :, :]
        img = img.transpose(2, 0, 1)
        input_tensor = img[np.newaxis, :, :, :]
        pred = self.session.run(None, {self.input_name: input_tensor})
        coords = self.pred2coords(pred, original_image_width=img_w, original_image_height=img_h)
        return coords