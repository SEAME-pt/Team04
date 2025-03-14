# Autonomous Vehicle Control

## Table of Contents

1. [Overview](#overview)
2. [Control Schemes](#control-schemes)
3. [Types of Controllers](#types-of-controllers)
   - [Model-Free Controllers](#model-free-controllers)
   - [Model-Based Controllers](#model-based-controllers)
4. [Summary of Control Strategies](#summary-of-control-strategies-for-autonomous-vehicles)
5. [Model Predictive Control (MPC)](#model-predictive-control-mpc)
   - [Mathematical Model](#mathematical-model)
   - [Four-Wheel Model](#four-wheel-model)
   - [MPC Design Parameters](#mpc-design-parameters)
   - [Optimization](#optimization)
6. [Image-Based Model Predictive Control](#image-based-model-predictive-control-for-autonomous-driving)
   - [Perspective Mapping](#perspective-mapping)
   - [Inverse Perspective Mapping](#inverse-perspective-mapping)
   - [Camera Calibration](#camera-calibration)
   - [State Estimation](#state-estimation)
   - [MPC based on images](#control-method)
7. [References](#references)

---

## Overview

The control of an autonomous vehicle is divided into the following two components:

- **Longitudinal Control**: Controls throttle and brake inputs.
- **Lateral Control**: Controls steering input.

## Control Schemes

- **Coupled Control**: The lateral and longitudinal controllers influence each other.
- **De-coupled Control**: The lateral and longitudinal controllers are independent of each other.

## Types of Controllers

### Model-Free Controllers

- **Definition**: Controllers that do not rely on a mathematical model of the system. They correct errors based on the difference between the setpoint and the current state.
- **Advantages**: Easy to implement.
- **Disadvantages**: Difficult to tune, no guarantee of optimal performance, unable to handle complex systems with multiple constraints. Can become overly reactive and generate erratic actions.
- **Example**: PID controllers.

### Model-Based Controllers

- **Definition**: Controllers that utilize a mathematical model of the system.
- **Advantages**: Can model complex systems and achieve.
- **Disadvantages**: Complex to implement, and can be computationally expensive.
- **Example**: Model Predictive Control (MPC).

## Summary of Control Strategies for Autonomous Vehicles

| Comparison Metric            | Bang-Bang Control | PID Control | Geometric Control | Model Predictive Control | Imitation Learning Based Control | Reinforcement Learning Based Control |
|------------------------------|-------------------|-------------|-------------------|--------------------------|----------------------------------|--------------------------------------|
| **Principle of Operation**   | Error Driven (FSM) | Error Driven (Corrective) | Model Based (Corrective) | Model Based (Optimal) | Supervised Learning | Reinforcement Learning |
| **Tracking**                 | Poor              | Good        | Very Good         | Excellent                | Good                             | -                                    |
| **Robustness**               | Poor              | Very Good   | Very Good         | Excellent                | Good                             | Good                                |
| **Stability**                | Poor              | Very Good   | Very Good         | Excellent                | Good                             | Good                                |
| **Reliability**              | Very Low          | Very High   | Very High         | Extremely High           | Low                              | Low                                 |
| **Technical Complexity**     | Very Low          | Low         | Low               | Very High                | Low                              | High                                |
| **Computational Overhead**   | Very Low          | Low         | Low               | Very High                | High                             | High                                |
| **Constraint Satisfaction**  | Poor              | Good        | Good              | Excellent                | -                                | -                                   |
| **Technical Maturity**       | Very High         | Very High   | High              | Moderate                 | Low                              | Very Low                            |

## Model Predictive Control (MPC)

MPC involves horizon optimization and is based on mathematical models.

### Mathematical Model

- **Particle Motion Model**
- **Bicycle Model**
- **Four-Wheel Model**

### Four-Wheel Model

The vehicle’s state is defined by four variables: $x$, $y$, $\theta$, and $\phi$ (vehicle’s position and orientation, and the steering angle). The vehicle is controlled by two inputs: $v_1$  (the angular velocity of the rolling wheels) and $v_2$​ (the steering wheel angular velocity).

#### Vehicle Motion

The vehicle moves forward with rear wheels driven, described by the following equations:

$$
\begin{bmatrix}
\dot{x} \\
\dot{y} \\
\dot{\theta} \\
\dot{\phi}
\end{bmatrix} =
\begin{bmatrix}
\cos\theta \\
\sin\theta \\
\frac{\tan\phi}{l} \\
0
\end{bmatrix} rv_1 +
\begin{bmatrix}
0 \\
0 \\
0 \\
1
\end{bmatrix} v_2
$$

Where:

- $x$, $y$, $\theta$, and $\phi$ represent vehicle states.
- $v_1$ and $v_2$ are the control inputs for vehicle speed and steering angular velocity, respectively.
- $r$ is the wheel radius.
- $l$ is the distance between the front and rear wheels.
- $rv_1$ is the vehicle speed in km/h, and $v_2$ is the steering angular velocity in rpm.

The vehicle model is nonlinear but can be linearized and transformed into a discretized time model:

$$
\dot{X} = f(x, u)
$$

Where:

- $x = [x, y, \theta, \phi]^T$ is the vehicle state.
- $u = [u_1, u_2]^T$ are the control inputs.

The system's state at time $k+1$ is described as:

$$
\tilde{X}(k+1) = A(k) \tilde{X}(k) + B(k) \tilde{u}(k)
$$

Where $A(k)$, $B(k)$, and $C(k)$ are matrices describing the system dynamics.

$$
A(k) =
\begin{bmatrix}
1 & 0 & -u_{r1}(k) \sin\theta_r(k) \Delta t & 0 \\
0 & 1 & u_{r1}(k) \cos\theta_r(k) \Delta t & 0 \\
0 & 0 & 1 & \frac{u_{r1}(k)}{l\cos^2\phi_r(k)} \Delta t \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

$$
B(k) =
\begin{bmatrix}
\cos \theta_r(k) \Delta t & 0 \\
\sin \theta_r(k) \Delta t & 0 \\
\frac{\tan \phi_r(k)}{l}\Delta t & 0 \\
0 & \Delta t
\end{bmatrix}
$$

$$
C(k) =
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

and

$$
\tilde{X}(k) = X(k) - X_r(k) =
\begin{bmatrix}
x(k) - x_r(k) \\
y(k) - y_r(k) \\
\theta(k) - \theta_r(k) \\
\phi(k) - \phi_r(k)
\end{bmatrix}
$$

$$
\tilde{u}(k) = u(k) - u_r(k) =
\begin{bmatrix}
u_1(k) - u_{r1}(k) \\
u_2(k) - u_{r2}(k)
\end{bmatrix}
$$

There are two control inputs of vehicle speed, $𝑢_1(𝑘)−𝑢𝑟_1(𝑘)$, and the steering angular velocity, $𝑢_2(𝑘)−𝑢𝑟_2(𝑘)$.

$$
y(k) = \tilde{Y}(k) = C(k) \tilde{X}(k)
$$

$$
\min_{U \equiv \{\Delta u_k, \dots, \Delta u_{k+N_u-1} \}} J(U, x(k)) =
\sum_{i=0}^{N_y-1} \left[ (y_{k+i|k} - r_{k+i|k})' Q (y_{k+i|k} - r_{k+i|k}) + \Delta u_{k+i|k}' R \Delta u_{k+i|k} \right]
$$

where:

- $x(k)$ are the state variables at the present discrete time k
- $U$ is the solution of the predictive input horizon from k to Nu
- $N_y$ is the predictive output horizon
- $y_{k+i|k}$ are the outputs at the present discrete time k
- $r_{k+i|k}$ are the tracking trajectory setpoints
- $\Delta u_{k+i|k}$ are the input predictive increments
- $Q$ and $R$ are the weighting matrices for outputs and inputs.

## MPC Design Parameters

- **Sample Time ($T_s$)**: This parameter determines the rate at which the control loop executes. A proper sample time ensures the controller’s response is fast enough to avoid accidents and follow the reference trajectory closely. A sample time of 5% to 10% of the system’s rise time is recommended:

$$
0.05 t_r < T_s < 0.1 t_r
$$

- **Prediction Horizon ($p$)**: This is the number of future time steps over which predictions are made. A prediction horizon too short may lead to reactive control, while one too long can result in unnecessary computational effort. A recommended range is:

$$
\frac{t_s}{T_s} < p < 1.5 \frac{t_s}{T_s}
$$

- **Control Horizon ($m$)**: This is the number of time steps over which the control inputs are optimized. The control horizon should typically be 10%-20% of the prediction horizon

$$
0.1 p < m < 0.2 p
$$

- **Weights**: Weights: In MPC, weights are used to prioritize different objectives such as minimizing tracking error and limiting the rate of change of control inputs. It is often more critical to track the vehicle’s position than velocity, and therefore, position tracking might receive higher weight.

## Optimization

Different optimization methods can be used in MPC depending on whether the problem is convex or non-convex.

- **Convex Optimization**: Gradient descent and other convex optimization techniques are suitable for linear problems.
- **Non-Linear Optimization**: For more complex, non-convex problems, metaheuristic methods such as Particle Swarm Optimization may be employed.

## Image-Based Model Predictive Control for Autonomous Driving

### Perspective Mapping

The projection of 3D camera coordinates to 2D image coordinates is described by the equation:

$$
\lambda \begin{bmatrix} u \\ v \\ 1 \end{bmatrix} =
\begin{bmatrix} \alpha_u & 0 & u_0 \\ 0 & \alpha_v & v_0 \\ 0 & 0 & 1 \end{bmatrix}
\begin{bmatrix} X_c \\ Y_c \\ Z_c \end{bmatrix}
$$

Where:

- $u$ and $v$ are the pixel coordinates.
- $alpha_u$ and $alpha_v$ are the focal lengths.
- $u_0$ and $v_0$ are the principal point coordinates.

### Inverse Perspective Mapping

Assuming the road is flat, the inverse mapping to convert from image coordinates to camera coordinates is given by:

$$
\begin{bmatrix} X_c \\ Y_c \\ Z_c \end{bmatrix} = \frac{h}{ \mathbf{n_c}^T \mathbf{K}^{-1} (u,v,1)^T} \mathbf{K}^{-1} \begin{bmatrix} u \\ v \\ 1 \end{bmatrix}
$$

Where:

- $h$ is the height of the camera.
- $K$ is the intrinsic camera matrix.
- $n_c$ is the camera reference frame.

### Camera Calibration

Camera calibration is typically done using methods such as  **Zhang's Method**.

### State Estimation

Estimate the vehicle’s reference trajectory (straight line to vanishing point). The error is determined by the angle and distance between the lines.

### Control Method

1. Detect ego lanes (lane detection).
2. Sample points in the middle of the lanes.
3. Fit a second-order polynomial to the sampled points.
4. Sample points from the polynomial for the prediction horizon.
5. Estimate the vehicle state from the image using inverse perspective mapping.
6. Convert from pixel space to world space.
7. Minimize the objective function over the prediction horizon.
8. Update control inputs based on new state estimates.

### References

1. Samak, C., Samak, T., & Kandhasamy, S. - Control Strategies for Autonomous Vehicles.
2. Weiskircher, T., Wang, Q., & Ayalew, B. - Predictive Guidance and Control Framework for (Semi-)Autonomous Vehicles in Public Traffic. IEEE.
3. Vu, T. M., Moezzi, R., Cyrus, J., & Hlava, J. - Model Predictive Control for Autonomous Driving Vehicles.
4. Hiremath, S. A., Gummadi, P. K., & Bajcinca, N. - Image-Based Model Predictive Controller for Autonomous Driving.
5. Fermi, T. - Algorithms for Automated Driving: Lane Detection & Inverse Perspective Mapping. Online resource.
6. Zhang, Z. - A Flexible New Technique for Camera Calibration.
