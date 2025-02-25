# CARLA Simulator Methods Documentation

This document provides an explanation of the key methods and components used in the improved CARLA simulation script.

## Table of Contents
1. [Connection and World Setup](#connection-and-world-setup)
2. [Pygame Setup](#pygame-setup)
3. [Synchronous Mode](#synchronous-mode)
4. [Traffic Manager](#traffic-manager)
5. [Vehicle Blueprints and Spawning](#vehicle-blueprints-and-spawning)
6. [Ego Vehicle Configuration](#ego-vehicle-configuration)
7. [Camera Setup](#camera-setup)
8. [Image Processing](#image-processing)
9. [Collision Sensor](#collision-sensor)
10. [NPC Vehicle Configuration](#npc-vehicle-configuration)
11. [Spectator Configuration](#spectator-configuration)
12. [Main Simulation Loop](#main-simulation-loop)
13. [Cleanup](#cleanup)

## Connection and World Setup

The first step in any CARLA script is establishing a connection to the CARLA server and retrieving the simulation world. This is essential as all subsequent operations require access to the world object.

```python
client = carla.Client('localhost', 2000)
client.set_timeout(10.0)
world = client.get_world()
```

- `carla.Client`: Creates a connection to the CARLA server
  - Parameters: IP address and port (default: localhost:2000)
- `client.set_timeout()`: Sets maximum time to wait for operations
- `client.get_world()`: Retrieves the active CARLA world

## Pygame Setup

Pygame is used to create a visualization window where the camera feed will be displayed. This setup initializes the pygame library and creates the display surface with appropriate settings for performance.

```python
pygame.init()
display = pygame.display.set_mode((display_width, display_height), pygame.HWSURFACE | pygame.DOUBLEBUF)
```

- `pygame.init()`: Initializes all pygame modules
- `pygame.display.set_mode()`: Creates a display surface
  - `pygame.HWSURFACE`: Creates a hardware-accelerated surface
  - `pygame.DOUBLEBUF`: Enables double buffering for smoother updates
- `pygame.display.set_caption()`: Sets the window title
- `pygame.font.Font()`: Creates a font object for rendering text
- `pygame.time.Clock()`: Creates a clock object to control frame rate

## Synchronous Mode

Synchronous mode is crucial for coherent vehicle behavior as it ensures all simulation components advance in lockstep. This prevents timing issues that can cause unrealistic behavior in traffic simulations.

```python
settings = world.get_settings()
settings.synchronous_mode = True
settings.fixed_delta_seconds = 0.05  # 20 FPS
world.apply_settings(settings)
```

- `world.get_settings()`: Retrieves current simulation settings
- `settings.synchronous_mode`: When set to True, simulation advances only when `world.tick()` is called
- `settings.fixed_delta_seconds`: Sets the simulation time step (in seconds)
- `world.apply_settings()`: Applies modified settings to the world

## Traffic Manager

The Traffic Manager is CARLA's built-in system for controlling autonomous vehicles. It handles navigation, collision avoidance, and traffic rule compliance. Configuring it properly is essential for realistic traffic behavior.

```python
traffic_manager = client.get_trafficmanager(8000)
traffic_manager.set_synchronous_mode(True)
```

- `client.get_trafficmanager()`: Creates or connects to a traffic manager instance
  - Parameter: Port number (default is 8000)
- `traffic_manager.set_synchronous_mode()`: Synchronizes traffic manager with world's synchronous mode
- `traffic_manager.set_global_distance_to_leading_vehicle()`: Sets default distance between vehicles
- `traffic_manager.global_percentage_speed_difference()`: Sets global speed adjustment

## Vehicle Blueprints and Spawning

Blueprints define the characteristics of actors in CARLA. For vehicles, blueprints determine the vehicle model, color, and other properties. Spawn points are predefined locations in the map where vehicles can be safely placed.

```python
blueprint_library = world.get_blueprint_library()
vehicle_blueprints = blueprint_library.filter('vehicle.*')
spawn_points = world.get_map().get_spawn_points()
```

- `world.get_blueprint_library()`: Gets access to blueprint definitions
- `blueprint_library.filter()`: Filters blueprints by pattern
- `world.get_map()`: Gets the current map
- `map.get_spawn_points()`: Gets predefined spawn points in the map
- `world.try_spawn_actor()`: Attempts to spawn an actor at specified location
  - Returns actor if successful, None if spawn point is blocked

## Ego Vehicle Configuration

The ego vehicle is the main vehicle that the camera will follow. It requires special configuration to ensure it behaves appropriately in traffic and is visually distinct from other vehicles.

```python
ego_vehicle_bp = blueprint_library.find('vehicle.tesla.model3')
ego_vehicle_bp.set_attribute('color', '204, 0, 0')
ego_vehicle = world.try_spawn_actor(ego_vehicle_bp, ego_spawn_point)
```

- `blueprint_library.find()`: Gets a specific blueprint by ID
- `blueprint.set_attribute()`: Sets attributes of the blueprint
- `ego_vehicle.set_autopilot()`: Enables autopilot mode for the vehicle
  - Uses traffic manager for behavior control

## Camera Setup

Cameras in CARLA are sensor actors that generate images. Proper camera configuration is essential for good visualization. The camera transform defines its position and orientation relative to the vehicle it's attached to.

```python
camera_bp = blueprint_library.find('sensor.camera.rgb')
camera_bp.set_attribute('image_size_x', str(display_width))
camera_transform = carla.Transform(carla.Location(x=-5.0, z=2.8), carla.Rotation(pitch=-15))
camera = world.spawn_actor(camera_bp, camera_transform, attach_to=ego_vehicle)
```

- `blueprint_library.find('sensor.camera.rgb')`: Gets RGB camera blueprint
- Setting camera attributes:
  - `image_size_x/y`: Resolution width/height
  - `fov`: Field of view (in degrees)
- `carla.Transform`: Defines position and rotation
  - `carla.Location`: 3D location coordinates
  - `carla.Rotation`: Rotation in pitch, yaw, roll
- `world.spawn_actor(blueprint, transform, attach_to=parent)`: Spawns sensor attached to vehicle

## Image Processing

CARLA cameras generate raw image data that needs to be processed before it can be displayed. This involves converting the data format, handling color channels, and transforming the image for pygame compatibility.

```python
def process_image(image):
    array = np.frombuffer(image.raw_data, dtype=np.dtype("uint8"))
    array = np.reshape(array, (image.height, image.width, 4))
    array = array[:, :, :3]  # Remove alpha channel
    array = array[:, :, ::-1]  # Convert BGR to RGB
    surface = pygame.surfarray.make_surface(array.swapaxes(0, 1))
    image_queue.put(surface)

camera.listen(lambda image: process_image(image))
```

- `image.raw_data`: Raw sensor data (1D array of bytes)
- `np.frombuffer()`: Converts raw bytes to numpy array
- `np.reshape()`: Reshapes array to image dimensions (height, width, channels)
- `array[:, :, :3]`: Slices array to get RGB channels (removes alpha)
- `array[:, :, ::-1]`: Reverses channel order (BGR to RGB)
- `pygame.surfarray.make_surface()`: Converts numpy array to pygame surface
- `array.swapaxes(0, 1)`: Transposes array (needed for pygame)
- `camera.listen()`: Registers callback function for sensor data
- `image_queue.put()`: Adds processed image to thread-safe queue

## Collision Sensor

Collision sensors detect when a vehicle collides with other objects in the simulation. This is important for safety evaluation and for creating realistic responses to collisions.

```python
collision_bp = blueprint_library.find('sensor.other.collision')
collision_sensor = world.spawn_actor(collision_bp, carla.Transform(), attach_to=ego_vehicle)

def collision_callback(event):
    collision_history.append(event)
    print(f"Collision detected: {event}")

collision_sensor.listen(lambda event: collision_callback(event))
```

- `blueprint_library.find('sensor.other.collision')`: Gets collision sensor blueprint
- `collision_sensor.listen()`: Registers callback for collision events
- `event`: Contains information about collision (impulse, contact point, etc.)

## NPC Vehicle Configuration

NPC (Non-Player Character) vehicles form the traffic around the ego vehicle. Their behavior needs to be configured to create realistic traffic patterns with appropriate variance in driving styles.

```python
npc.set_autopilot(True, traffic_manager.get_port())
traffic_manager.ignore_lights_percentage(npc, random.uniform(0, 20))
traffic_manager.auto_lane_change(npc, random.random() > 0.3)
traffic_manager.vehicle_percentage_speed_difference(npc, speed_difference)
traffic_manager.distance_to_leading_vehicle(npc, random.uniform(1.0, 4.0))
```

- `vehicle.set_autopilot()`: Enables autopilot mode for the vehicle
- `traffic_manager.ignore_lights_percentage()`: Probability to ignore traffic lights
- `traffic_manager.auto_lane_change()`: Enables/disables automatic lane changes
- `traffic_manager.vehicle_percentage_speed_difference()`: Adjusts vehicle speed
  - Negative: faster than speed limit
  - Positive: slower than speed limit
- `traffic_manager.distance_to_leading_vehicle()`: Sets distance to vehicle ahead
  - Default unit is meters

## Spectator Configuration

The spectator is the camera view visible in the CARLA window (not the pygame window). Positioning it appropriately helps with debugging and monitoring the simulation.

```python
spectator = world.get_spectator()
spectator_transform = carla.Transform(
    ego_vehicle.get_transform().location + carla.Location(z=20, x=-15),
    carla.Rotation(pitch=-90)
)
spectator.set_transform(spectator_transform)
```

- `world.get_spectator()`: Gets the spectator camera (visible in CARLA window)
- `vehicle.get_transform()`: Gets current transform (location/rotation) of vehicle
- `spectator.set_transform()`: Positions the spectator camera

## Main Simulation Loop

The main loop is the heart of the simulation. It advances the simulation time, processes events, updates the display, and controls the frame rate. This loop runs continuously until the program is terminated.

```python
world.tick()  # Advances simulation

# Get vehicle speed
velocity = ego_vehicle.get_velocity()
speed = 3.6 * np.sqrt(velocity.x**2 + velocity.y**2 + velocity.z**2)  # in km/h

# Display update
surface = image_queue.get(block=False)
display.blit(surface, (0, 0))
pygame.display.flip()

# Frame rate control
clock.tick(20)
```

- `world.tick()`: Advances simulation by one step when in synchronous mode
- `ego_vehicle.get_velocity()`: Gets 3D velocity vector of vehicle
- `display.blit()`: Draws one surface onto another
- `pygame.display.flip()`: Updates the full display
- `clock.tick()`: Limits the frame rate and returns time since last call

## Cleanup

Proper cleanup is essential to prevent memory leaks and ensure the simulation can be restarted correctly. This involves destroying all actors and resetting simulation settings.

```python
for actor in spawned_actors:
    if actor is not None and actor.is_alive:
        actor.destroy()

settings = world.get_settings()
settings.synchronous_mode = False
settings.fixed_delta_seconds = None
world.apply_settings(settings)

pygame.quit()
```

- `actor.is_alive`: Checks if actor still exists in simulation
- `actor.destroy()`: Removes actor from simulation
- Restoring asynchronous mode settings
- `pygame.quit()`: Closes pygame properly

## Traffic Manager Methods Reference

| Method | Purpose |
|--------|---------|
| `set_synchronous_mode()` | Synchronizes traffic manager with world |
| `set_global_distance_to_leading_vehicle()` | Sets default distance between vehicles |
| `global_percentage_speed_difference()` | Sets global speed adjustment |
| `ignore_lights_percentage()` | Sets probability to ignore traffic lights |
| `ignore_signs_percentage()` | Sets probability to ignore signs |
| `auto_lane_change()` | Enables/disables automatic lane changes |
| `vehicle_percentage_speed_difference()` | Sets vehicle-specific speed adjustment |
| `distance_to_leading_vehicle()` | Sets vehicle-specific following distance |
| `keep_right_rule_percentage()` | Controls lane discipline behavior |
| `collision_detection()` | Enables/disables collision detection |
| `get_port()` | Gets traffic manager port |
