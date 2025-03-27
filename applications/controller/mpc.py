import numpy as np
from scipy.optimize import minimize
from scipy.interpolate import interp1d
from typing import List, Tuple, Callable


class MPC:
    def __init__(
        self, pred_horizon: int = 10, control_horizon: int = 5, dt: float = 0.1
    ):
        # MPC parameters
        self.N_p = pred_horizon
        self.N_c = control_horizon
        self.dt = dt
        # Model
        self.wheelbase = 0.1  # m
        self.max_steering_angle = 0.5  # rad, around 30 degrees
        self.max_acceleration = 3.0  # m/s^2
        self.max_deceleration = -3.0  # m/s^2
        # Weights
        self.weight_path = 1.0
        self.weight_heading = 0.6
        self.weight_velocity = 0.1
        self.weight_acceleration_smoothness = 0.05
        self.weight_steering_angle_smoothness = 0.1
        self.weight_lane_center = 1.5
        # State
        self.x = 0.0
        self.y = 0.0
        self.heading_angle = 0.0
        self.velocity = 0.0
        # Controls
        self.acceleration = 0.0
        self.steering_angle = 0.0

    def set_state(self, state):
        self.x = state[0]
        self.y = state[1]
        self.heading_angle = state[2]
        self.velocity = state[3]

    def set_control(self, control):
        self.acceleration = control[0]
        self.steering_angle = control[1]

    def get_state(self):
        return [self.x, self.y, self.heading_angle, self.velocity]

    def get_control(self):
        return [self.acceleration, self.steering_angle]

    def estimate_new_control_from_lane_points(
        self, left_lane: List, right_lane: List, desired_velocity: float
    ) -> Tuple:
        x_points = np.array([p[0] for p in left_lane])

        interp_left_lane = self.interpolate_lane_points(x_points, left_lane)
        interp_right_lane = self.interpolate_lane_points(x_points, right_lane)

        reference_states = self.get_reference_states_over_horizon(
            x_points, interp_left_lane, interp_right_lane, desired_velocity
        )
        current_state = np.array([self.x, self.y, self.heading_angle, self.velocity])

        optimal_controls = self.minimize_cost_function(
            current_state, reference_states, interp_left_lane, interp_right_lane
        )
        optimal_acceleration, optimal_steering_angle = optimal_controls[0]
        return optimal_acceleration, optimal_steering_angle

    def interpolate_lane_points(
        self, x_points: np.ndarray, lane_points: List
    ) -> Callable:
        y_points = np.array([p[1] for p in lane_points])
        return interp1d(x_points, y_points, fill_value="extrapolate")

    def get_reference_states_over_horizon(
        self,
        x_points: np.ndarray,
        left_lane: Callable,
        right_lane: Callable,
        desired_velocity: float,
    ) -> List:
        centerline_y = (left_lane(x_points) + right_lane(x_points)) / 2
        centerline_path = [(x, y) for x, y in zip(x_points, centerline_y)]
        reference_states = []
        for i in range(min(self.N_p, len(centerline_path))):
            x, y = centerline_path[i]
            if i < len(centerline_path) - 1:
                next_x, next_y = centerline_path[i + 1]
                heading_angle = np.arctan2(next_y - y, next_x - x)
            else:
                heading_angle = self.heading_angle
            reference_states.append([x, y, heading_angle, desired_velocity])

        while len(reference_states) < self.N_p:
            last = reference_states[-1]
            reference_states.append(last)
        return reference_states

    def minimize_cost_function(
        self,
        current_state: np.ndarray,
        reference_state: List,
        left_lane: Callable,
        right_lane: Callable,
    ) -> np.ndarray:
        initial_control_over_horizon = np.zeros(
            2 * self.N_c
        )  # acceleration = 0 and steering_angle = 0
        bounds = [
            (self.max_deceleration, self.max_acceleration),
            (-self.max_steering_angle, self.max_steering_angle),
        ] * self.N_c

        result = minimize(
            self.calculate_cost,
            initial_control_over_horizon,  # must only have one dimension
            args=(current_state, reference_state, left_lane, right_lane),
            method="SLSQP",
            bounds=bounds,
        )

        return result.x.reshape(-1, 2)

    def calculate_cost(
        self,
        control_over_horizon: np.ndarray,
        current_state: np.ndarray,
        reference_state: List,
        left_lane: Callable,
        right_lane: Callable,
    ) -> float:
        control_over_horizon = control_over_horizon.reshape(-1, 2)
        cost = 0
        state = current_state

        for i in range(self.N_p):
            control = control_over_horizon[min(i, self.N_c - 1)]
            next_state = self.get_new_state(state, control)
            path_error, heading_error, velocity_error, lane_center_error = (
                self.calculate_errors(
                    next_state, reference_state[i], left_lane, right_lane
                )
            )

            acceleration, steering_angle = control
            acceleration_cost = acceleration**2
            steering_cost = steering_angle**2

            cost += (
                self.weight_path * path_error
                + self.weight_heading * heading_error
                + self.weight_velocity * velocity_error
                + self.weight_acceleration_smoothness * acceleration_cost
                + self.weight_steering_angle_smoothness * steering_cost
                + self.weight_lane_center * lane_center_error
            )
            state = next_state
        return cost

    def get_new_state(self, state: np.ndarray, control: np.ndarray) -> np.ndarray:
        x, y, heading_angle, velocity = state
        acceleration, steering_angle = control
        acceleration = np.clip(
            acceleration, self.max_deceleration, self.max_acceleration
        )
        steering_angle = np.clip(
            steering_angle, -self.max_steering_angle, self.max_steering_angle
        )
        x_next = x + velocity * np.cos(heading_angle) * self.dt
        y_next = y + velocity * np.sin(heading_angle) * self.dt
        heading_next = (
            heading_angle
            + (velocity / self.wheelbase) * np.tan(steering_angle) * self.dt
        )
        v_next = max(0.0, velocity + acceleration * self.dt)
        return np.array([x_next, y_next, heading_next, v_next])

    @staticmethod
    def calculate_errors(
        next_state: np.ndarray,
        reference_state: np.ndarray,
        left_lane: Callable,
        right_lane: Callable,
    ) -> Tuple:
        next_x, next_y, next_heading, next_velocity = next_state
        ref_x, ref_y, ref_heading, ref_velocity = reference_state

        path_error = np.sqrt((next_x - ref_x) ** 2 + (next_y - ref_y) ** 2)
        heading_error = np.abs(next_heading - ref_heading)
        vel_error = np.abs(next_velocity - ref_velocity)

        new_lane_center_y = (left_lane(next_x) + right_lane(next_x)) / 2
        lane_center_error = np.abs(next_y - new_lane_center_y)

        return path_error, heading_error, vel_error, lane_center_error

    def update_control(self, new_controls: np.ndarray) -> None:
        self.acceleration, self.steering_angle = new_controls

    def update_state(self, new_state: np.ndarray) -> None:
        self.x, self.y, self.heading_angle, self.velocity = new_state
