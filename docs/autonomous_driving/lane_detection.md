# Lane Detection

## 1. Datasets

| **Dataset** | **Description** | **Paper** | **Year** | **Citations** | **Location** | **Number of Images** |
|---|---|----|---|---|---|---|
| [**CULane**](https://xingangpan.github.io/projects/CULane.html) | A large dataset for lane detection with diverse road conditions (urban, rural, highway). | [Spatial As Deep: Spatial CNN for Traffic Scene Understanding](https://arxiv.org/abs/1712.06080) | 2016 | 1200 | China | 133235 |
| [**TuSimple**](https://github.com/TuSimple/tusimple-benchmark) | Dataset for lane detection in highway scenarios with annotated lane images. | ? | ? |? | USA | 6408 |
| [**ApolloScape**](http://apolloscape.auto) | A dataset with diverse traffic conditions, road types, and weather conditions. | [The ApolloScape Open Dataset for Autonomous Driving and its Application](https://arxiv.org/abs/1803.06184) | 2018 | 1198 | China | 140000 |
| [**KITTILane**](http://www.cvlibs.net/datasets/kitti/) | Lane detection dataset from the KITTI benchmark, covering urban and highway scenarios. | [Are we ready for autonomous driving? The KITTI vision benchmark suite](https://ieeexplore.ieee.org/document/6248074) | 2012 | 16597 | Germany | 15000 |
| [**Berkeley DeepDrive (BDD100K)**](https://bair.berkeley.edu/blog/2018/05/30/bdd/) | A large dataset for driving-related tasks, including lane detection in diverse environments. | [BDD100K: A Diverse Driving Dataset for Heterogeneous Multitask Learning](https://arxiv.org/abs/1805.04687) | 2018 | 2592 | USA | 100000 |
| [**Cityscapes**](https://www.cityscapes-dataset.com/) | A dataset for semantic urban scene understanding, with lane detection in urban settings. | [Cityscapes 3D: Dataset and Benchmark for 9 DoF Vehicle Detection](https://arxiv.org/abs/2006.07864) | 2016 | 71 | Germany | 5000 |
| [**Oxford RobotCar**](https://robotcar-dataset.robots.ox.ac.uk/) | A dataset for autonomous driving in urban environments with dynamic lane markings. | [Real-time Kinematic Ground Truth for the Oxford RobotCar Dataset](https://arxiv.org/abs/2002.10152) | 2016 | 7 | UK | 20M |
| [**Mapillary Vistas**](https://www.mapillary.com/dataset/vistas) | A global dataset for urban scene understanding with lane detection in different cities. | [The Mapillary Vistas Dataset for Semantic Understanding of Street Scenes](https://ieeexplore.ieee.org/document/8237796) | 2017 | 1585 | Global | 25000 |

---

## 2. State-of-the-Art Methods

### 2.1. Traditional Computer Vision Methods

- Edge Detection (Canny, Sobel)
- Hough Transform

### 2.2. Limitations

| **Issue** | **Explanation** |
|---|---|
| **Sensitivity to Lighting** | Fails in low light, shadows, and glare. |
| **Poor Occlusion Handling** | Cannot predict lanes when they are partially blocked. |
| **Struggles with Complex Roads** | Cannot detect lane merges, splits, or curved lanes effectively. |
| **No Context Awareness** | Cannot differentiate lane markings from road cracks, curbs, or debris. |
| **Lack of Real-Time Adaptation** | Hardcoded rules fail in dynamic environments like rain, snow, or construction zones. |

### 2.3. Conclusion

[Deep learning-based approaches (YOLO, Faster R-CNN, CLRNet, etc.) have replaced traditional methods (Canny, Sobel, Hough Transform) because they can learn lane patterns, adapt to different environments, and generalize better](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=10006813).

---

## 3. Lane Detection Models

| **Model** | **Paper** | **Year** | **Citations** |
|---|---|---|---|
| [YOLO (YOLOP, YOLOv11)](https://github.com/ultralytics/ultralytics) | [YOLOP: You Only Look Once for Panoptic Driving Perception](https://arxiv.org/abs/2108.11250) | 2021 | 347 |
| [Faster R-CNN](https://github.com/chenyuntc/simple-faster-rcnn-pytorch) | [Faster R-CNN: Towards Real-Time Object Detection with Region Proposal Networks](https://arxiv.org/abs/1506.01497) | 2015 | 52348 |
| [CLRNet](https://github.com/Turoad/CLRNet) | [CLRNet: Cross Layer Refinement Network for Lane Detection](https://openaccess.thecvf.com/content/CVPR2022/papers/Zheng_CLRNet_Cross_Layer_Refinement_Network_for_Lane_Detection_CVPR_2022_paper.pdf) | 2022 | 235 |
| DLT-Net | [DLT-Net: Joint Detection of Drivable Areas, Lane Lines, and Traffic Objects](https://ieeexplore.ieee.org/document/8937825) | 2020 | 108 |
| [HybridNets](https://github.com/datvuthanh/HybridNets) | [HybridNets: End-to-End Perception Network](https://arxiv.org/abs/2203.09035) | 2020 | 105 |

### 3.1. Description

#### YOLO (YOLOP, YOLOv11)

- Very fast but treats lanes as objects rather than structured curves.
- Good for object detection but not ideal for complex lane scenarios such as merging and occlusions.

#### Faster R-CNN

- Very high accuracy but too slow for real-time lane detection.

#### CLRNet

- Anchor-based lane detection model optimized for real-time performance.
- Accurate but lacks structured lane reasoning for merges and splits.

#### DLT-Net

- Uses graph-based learning to model lane structures and topology.
- Handles occlusions and merges better than CLRNet but is slightly slower.
- Not publicly available.

#### HybridNets

- Multi-task learning model that detects lanes, objects, and drivable areas.
- Highly optimized for edge devices (Jetson Nano, Raspberry Pi).
- Slightly less accurate for lane detection than CLRNet or DLT-Net.

---

### 3.2. Conclusion

- Speed: YOLO (best for object detection, not ideal for lanes).
- Lane detection accuracy: CLRNet.
- Multi-task learning (lanes, objects, drivable area): HybridNets.

---
