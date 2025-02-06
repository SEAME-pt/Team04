# State of Art of AI in automotive

## Automated Car Systems

According to the Society of Automotive Engineers (SAE international), driving automation can be categorized into six levels (SAE J3016):

| Level | Name | Driver | Driving Environment Monitoring (DEM) | Dynamic Driving Task Fallback (DDTF) |
| --- | --- | --- | --- | --- |
| 0 | No automation | Human | Human | Human |
| 1 | Driver assistance | Human & System | Human | Human |
| 2 | Partial automation | System | Human | Human |
| 3 | Conditional automation | System | System | Human |
| 4 | High automation | System | System | System |
| 5 | Full automation | System | System | System |

The state of art Automated Driving Systems (ADS) are mainly at level 2 and 3.

## Automated Car Software

Rui Fan et all divides the software architecture of Autonomous Cars (AC) into five modules: 

- **Perception**: analyzes raw sensor data (such as cameras, Radar, Lidar) and outputs into something that can be used by the AC. This can be broken into computer vision-based (optimization techniques) and machine learning-based (data-driven classification or regression - CNN).
- **Localization and Mapping**: Using sensor data and perception output to estimate AC location and build/update 3D world map.
- **Prediction**: analyzes motion patterns of other traffic agents and predict future AC trajectories
- **Planning**: determines possible safe AC navigation routes based on all the previous modules’ information.
- **Control**: sends the appropriate commands to throttle, braking or steering torque, based on the predicted trajectory and the estimated vehicle states, enabling the AC to follow the planned trajectory.