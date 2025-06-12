import cv2
import numpy as np
import onnxruntime as ort

def angle_between(v1, v2):
    """Returns the angle in radians between two vectors."""
    v1_u = v1 / np.linalg.norm(v1 + 1e-6)
    v2_u = v2 / np.linalg.norm(v2 + 1e-6)
    return np.arccos(np.clip(np.dot(v1_u, v2_u), -1.0, 1.0))

def get_orientation(points):
    """Estimates the orientation of a set of points using linear regression."""
    if len(points) < 2:
        return None
    x = np.array([p[0] for p in points])
    y = np.array([p[1] for p in points])
    A = np.vstack([x, np.ones(len(x))]).T
    try:
        m, _ = np.linalg.lstsq(A, y, rcond=None)[0]
        return np.arctan(m) # Angle in radians
    except np.linalg.LinAlgError:
        return np.pi / 2.0 # Vertical line

def merge_nearby_lanes(lane_groups, max_distance=50, max_angle_diff=np.pi / 18):
    """Merges lane groups that are spatially close and have similar orientations."""
    merged = [False] * len(lane_groups)
    final_lanes = []

    for i in range(len(lane_groups)):
        if not merged[i]:
            current_lane = lane_groups[i]
            merged[i] = True
            for j in range(i + 1, len(lane_groups)):
                if not merged[j]:
                    # Calculate distance between centroids
                    centroid1 = np.mean(current_lane, axis=0)
                    centroid2 = np.mean(lane_groups[j], axis=0)
                    distance = np.linalg.norm(centroid1 - centroid2)

                    # Calculate orientation difference
                    orientation1 = get_orientation(current_lane)
                    orientation2 = get_orientation(lane_groups[j])

                    if orientation1 is not None and orientation2 is not None:
                        angle_diff = abs(orientation1 - orientation2)
                        angle_diff = min(angle_diff, np.pi - angle_diff) # Ensure smallest angle

                        if distance < max_distance and angle_diff < max_angle_diff:
                            current_lane.extend(lane_groups[j])
                            merged[j] = True
            final_lanes.append(np.array(current_lane).tolist())
    return final_lanes

def group_lane_coordinates_improved_v2(segmentation_mask, min_component_size=50,
                                        morph_kernel_size=(5, 5), max_merge_distance=50,
                                        max_merge_angle_diff_deg=10, cutoff_ratio=0.5):
    """
    Improved function to group lane coordinates, aiming for two main lanes.
    Includes pre-processing, size filtering, and merging of nearby, similarly oriented components.
    The mask is modified to remove points above the cutoff before processing.
    The final filtering prioritizes lanes closest to the horizontal center.
    """
    height = segmentation_mask.shape[0]
    width = segmentation_mask.shape[1]
    cutoff_y = int(height * cutoff_ratio)
    center_x = width // 2

    # 1. Modify the Mask: Remove points above the cutoff
    modified_mask = segmentation_mask.copy()
    modified_mask[:cutoff_y, :] = 0

    # 2. Pre-processing: Morphological Opening (on the modified mask)
    kernel = np.ones(morph_kernel_size, np.uint8)
    opened_mask = cv2.morphologyEx(modified_mask, cv2.MORPH_OPEN, kernel)

    # 3. Connected Components Analysis (on the pre-processed, modified mask)
    num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(opened_mask, connectivity=8)

    potential_lanes = []
    for label in range(1, num_labels):
        area = stats[label, cv2.CC_STAT_AREA]
        if area >= min_component_size:
            component_pixels_indices = np.where(labels == label)
            component_coordinates = np.stack((component_pixels_indices[1], component_pixels_indices[0]), axis=-1).tolist()
            potential_lanes.append(component_coordinates)

    # 4. Merge nearby and similarly oriented lanes
    merged_lanes = merge_nearby_lanes(potential_lanes, max_distance=max_merge_distance,
                                        max_angle_diff=np.deg2rad(max_merge_angle_diff_deg))

    # 5. Filtering: Choose the two lanes closest to the horizontal center
    if len(merged_lanes) > 2:
        def get_lane_center_distance(lane):
            if not lane:
                return float('inf')
            avg_x = np.mean([p[0] for p in lane])
            return abs(avg_x - center_x)

        # Sort lanes by their distance to the center
        sorted_lanes_by_center = sorted(merged_lanes, key=get_lane_center_distance)
        # Take the two closest to the center
        closest_two = sorted_lanes_by_center[:2]

        # If the two closest are different, return them.
        # If there's only one or zero, return what we have.
        if len(closest_two) == 2:
            return closest_two
        elif len(closest_two) == 1:
            # If only one close lane, maybe return it with the largest other?
            remaining_lanes = [lane for lane in merged_lanes if lane not in closest_two]
            if remaining_lanes:
                remaining_lanes.sort(key=len, reverse=True)
                return closest_two + remaining_lanes[:1]
            else:
                return closest_two
        else:
            return []

    elif len(merged_lanes) > 0:
        return merged_lanes
    else:
        return []
    
def group_lane_coordinates_improved_v3(segmentation_mask, min_component_size=50,
                                        morph_kernel_size=(5, 5), max_merge_distance=50,
                                        max_merge_angle_diff_deg=10, cutoff_ratio=0.5):
    """
    Improved function to group lane coordinates, aiming for the two largest lanes.
    Includes pre-processing, size filtering, and merging of nearby, similarly oriented components.
    The mask is modified to remove points above the cutoff before processing.
    The final filtering prioritizes the two largest lanes based on the number of points.
    """
    height = segmentation_mask.shape[0]
    width = segmentation_mask.shape[1]
    cutoff_y = int(height * cutoff_ratio)

    # 1. Modify the Mask: Remove points above the cutoff
    modified_mask = segmentation_mask.copy()
    modified_mask[:cutoff_y, :] = 0

    # 2. Pre-processing: Morphological Opening (on the modified mask)
    kernel = np.ones(morph_kernel_size, np.uint8)
    opened_mask = cv2.morphologyEx(modified_mask, cv2.MORPH_OPEN, kernel)

    # 3. Connected Components Analysis (on the pre-processed, modified mask)
    num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(opened_mask, connectivity=8)

    potential_lanes = []
    for label in range(1, num_labels):
        area = stats[label, cv2.CC_STAT_AREA]
        if area >= min_component_size:
            component_pixels_indices = np.where(labels == label)
            component_coordinates = np.stack((component_pixels_indices[1], component_pixels_indices[0]), axis=-1).tolist()
            potential_lanes.append(component_coordinates)

    # 4. Merge nearby and similarly oriented lanes
    merged_lanes = merge_nearby_lanes(potential_lanes, max_distance=max_merge_distance,
                                        max_angle_diff=np.deg2rad(max_merge_angle_diff_deg))

    # 5. Filtering: Choose the two largest lanes
    if len(merged_lanes) > 2:
        # Sort lanes by their size (number of points) in descending order
        sorted_lanes_by_size = sorted(merged_lanes, key=len, reverse=True)
        # Take the two largest
        largest_two = sorted_lanes_by_size[:2]
        return largest_two
    elif len(merged_lanes) > 0:
        return merged_lanes
    else:
        return []
    
def draw_grouped_lanes(mask_shape, grouped_lanes, colors=None, thickness=5):
    """
    Draws the grouped lane coordinates onto a black mask.

    Args:
        mask_shape (tuple): The (height, width) of the mask to create.
        grouped_lanes (list): A list of lists, where each inner list contains
                              (x, y) coordinates of a single lane.
        colors (list or None): A list of BGR colors to use for each lane.
                               If None, uses a default set of colors.
        thickness (int): The thickness of the lines to draw.

    Returns:
        numpy.ndarray: A black mask with the grouped lanes drawn as lines.
    """
    height, width = mask_shape
    mask = np.zeros((height, width, 3), dtype=np.uint8)

    if colors is None:
        default_colors = [(255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 255, 0)] # Red, Green, Blue, Yellow
        colors = default_colors[:len(grouped_lanes)]
    elif len(colors) < len(grouped_lanes):
        print("Warning: Not enough colors provided, some lanes might have default colors.")
        default_colors = [(255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 255, 0)]
        colors.extend(default_colors[len(colors) - len(grouped_lanes):])

    for i, lane_points in enumerate(grouped_lanes):
        color = colors[i % len(colors)]
        if len(lane_points) > 1:
            for j in range(len(lane_points) - 1):
                start_point = (int(lane_points[j][0]), int(lane_points[j][1]))
                end_point = (int(lane_points[j + 1][0]), int(lane_points[j + 1][1]))
                cv2.line(mask, start_point, end_point, color, thickness)
        elif len(lane_points) == 1:
            # Draw a single point if the lane has only one pixel
            center = (int(lane_points[0][0]), int(lane_points[0][1]))
            cv2.circle(mask, center, thickness // 2, color, -1)

    return mask

class YOLOPv2():
    def __init__(self, model_path, confThreshold=0.5):
        self.classes = list(map(lambda x: x.strip(), open('lane-detector-yolo/models/coco.names', 'r').readlines()))
        so = ort.SessionOptions()
        so.log_severity_level = 3
        self.session = ort.InferenceSession(model_path, so, providers=['CUDAExecutionProvider', 'CPUExecutionProvider'])
        print(self.session.get_providers())
        model_inputs = self.session.get_inputs()
        self.input_name = model_inputs[0].name
        self.input_names = [model_inputs[i].name for i in range(len(model_inputs))]
        self.input_shape = model_inputs[0].shape
        self.input_height = int(self.input_shape[2])
        self.input_width = int(self.input_shape[3])
        self.confThreshold = confThreshold

    def detect(self, frame):
        image_width, image_height = frame.shape[1], frame.shape[0]
        ratioh = image_height / self.input_height
        ratiow = image_width / self.input_width

        # Pre process:Resize, BGR->RGB, float32 cast
        input_image = cv2.resize(frame, dsize=(self.input_width, self.input_height))
        input_image = cv2.cvtColor(input_image, cv2.COLOR_BGR2RGB)
        input_image = input_image.transpose(2, 0, 1)
        input_image = np.expand_dims(input_image, axis=0)
        input_image = input_image.astype('float16')
        input_image = input_image / 255.0

        # Inference
        #print(type(input_image))
        results = self.session.run(None, {self.input_name: input_image})
        print("shape of results:", [result.shape for result in results])

        scores_ = results[2]
        batchno_classid_y1x1y2x2 = results[3]
        # Traffic Object Detection
        bboxes, scores, class_ids = [], [], []
        for score, batchno_classid_y1x1y2x2_ in zip(scores_, batchno_classid_y1x1y2x2):
            if score < self.confThreshold:
                continue

            class_id = int(batchno_classid_y1x1y2x2_[1])
            y1 = batchno_classid_y1x1y2x2_[2]
            x1 = batchno_classid_y1x1y2x2_[3]
            y2 = batchno_classid_y1x1y2x2_[4]
            x2 = batchno_classid_y1x1y2x2_[5]
            y1 = int(y1 * ratioh)
            x1 = int(x1 * ratiow)
            y2 = int(y2 * ratioh)
            x2 = int(x2 * ratiow)

            bboxes.append([x1, y1, x2, y2])
            class_ids.append(class_id)
            scores.append(score)

        for bbox, score, class_id in zip(bboxes, scores, class_ids):
            x1, y1 = int(bbox[0]), int(bbox[1])
            x2, y2 = int(bbox[2]), int(bbox[3])

            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 2)
            cv2.putText(frame, '%s:%.2f' % (self.classes[class_id-1], score), (x1, y1 - 5), 0,
                       0.7, (0, 255, 0), 2)

        # Drivable Area Segmentation
        drivable_area = np.squeeze(results[0], axis=0)
        mask = np.argmax(drivable_area, axis=0).astype(np.uint8)
        mask = cv2.resize(mask, (image_width, image_height), interpolation=cv2.INTER_NEAREST)
        frame[mask==1] = [0, 255, 0]
        # Lane Line
        lane_line = np.squeeze(results[1])
        mask = np.where(lane_line > 0.5, 1, 0).astype(np.uint8)
        mask = cv2.resize(mask, (image_width, image_height), interpolation=cv2.INTER_NEAREST)


        # grouped_lanes = group_lane_coordinates_improved_v3(mask, min_component_size=20,
        #                                                    morph_kernel_size=(5, 5),
        #                                                    max_merge_distance=25,
        #                                                    max_merge_angle_diff_deg=15,
        #                                                    cutoff_ratio=0.4)
        #print(f"Lanes after improvement: {len(grouped_lanes)}")
        grouped_lanes = []

        # Save mask after clustering
        #lane_mask_default_colors = draw_grouped_lanes(mask.shape, grouped_lanes, thickness=3)
        #cv2.imwrite("/workspaces/Team04/lane-detector-yolo/mask.jpg", lane_mask_default_colors)

        frame[mask==1] = [255, 0, 0]
        return frame, grouped_lanes
