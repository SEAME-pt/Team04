import carla
import random
import pygame
import numpy as np
import time
import queue
import weakref

# Initialize pygame for display
pygame.init()
display_width = 1280
display_height = 720
display = pygame.display.set_mode((display_width, display_height), pygame.HWSURFACE | pygame.DOUBLEBUF)
pygame.display.set_caption("CARLA Camera View")
font = pygame.font.Font(None, 36)
clock = pygame.time.Clock()

# Connect to the client and retrieve the world object
client = carla.Client('localhost', 2000)
client.set_timeout(10.0)  # Set timeout to 10 seconds
world = client.get_world()

# Set synchronous mode for better control
settings = world.get_settings()
settings.synchronous_mode = True
settings.fixed_delta_seconds = 0.05  # 20 FPS
world.apply_settings(settings)

# Get the traffic manager and set global parameters
traffic_manager = client.get_trafficmanager(8000)
traffic_manager.set_synchronous_mode(True)
traffic_manager.set_global_distance_to_leading_vehicle(2.0)  # More realistic following distance
traffic_manager.global_percentage_speed_difference(0.0)  # Default speed

# Get the blueprint library and filter for vehicle blueprints
blueprint_library = world.get_blueprint_library()
vehicle_blueprints = blueprint_library.filter('vehicle.*')
safe_vehicle_blueprints = [bp for bp in vehicle_blueprints if int(bp.get_attribute('number_of_wheels')) >= 4]

# Get all the spawn points in the world
spawn_points = world.get_map().get_spawn_points()
random.shuffle(spawn_points)  # Randomize spawn points

# Create a list to keep track of spawned actors
spawned_actors = []

# Define the ego vehicle blueprint (Tesla Model 3)
ego_vehicle_bp = blueprint_library.find('vehicle.tesla.model3')
ego_vehicle_bp.set_attribute('color', '204, 0, 0')  # Red color for better visibility

# Choose a good spawn point for the ego vehicle
ego_spawn_point = random.choice(spawn_points)
spawn_points.remove(ego_spawn_point)  # Remove this spawn point from the list

# Spawn the ego vehicle
ego_vehicle = world.try_spawn_actor(ego_vehicle_bp, ego_spawn_point)
if ego_vehicle is None:
    print("Failed to spawn ego vehicle, trying another spawn point...")
    ego_spawn_point = random.choice(spawn_points)
    spawn_points.remove(ego_spawn_point)
    ego_vehicle = world.try_spawn_actor(ego_vehicle_bp, ego_spawn_point)

if ego_vehicle:
    spawned_actors.append(ego_vehicle)
    print(f"Spawned ego vehicle at {ego_spawn_point.location}")
    
    # Set up the traffic manager for the ego vehicle
    ego_vehicle.set_autopilot(True, traffic_manager.get_port())
    traffic_manager.ignore_lights_percentage(ego_vehicle, 0)
    traffic_manager.auto_lane_change(ego_vehicle, True)
    traffic_manager.vehicle_percentage_speed_difference(ego_vehicle, -5.0)  # Slightly slower than others
    traffic_manager.distance_to_leading_vehicle(ego_vehicle, 5.0)  # Safer distance
    
    # Create a camera blueprint
    camera_bp = blueprint_library.find('sensor.camera.rgb')
    camera_bp.set_attribute('image_size_x', str(display_width))
    camera_bp.set_attribute('image_size_y', str(display_height))
    camera_bp.set_attribute('fov', '110')  # Wider field of view
    
    # Create an ideal camera location (slightly above and behind the ego vehicle)
    camera_transform = carla.Transform(carla.Location(x=-5.0, z=2.8), carla.Rotation(pitch=-15))
    
    # Spawn the camera
    camera = world.spawn_actor(camera_bp, camera_transform, attach_to=ego_vehicle)
    spawned_actors.append(camera)
    
    # Create a queue to store camera images
    image_queue = queue.Queue()
    
    # Define a callback function to process the images
    def process_image(image):
        # Convert CARLA raw image to Pygame surface
        array = np.frombuffer(image.raw_data, dtype=np.dtype("uint8"))
        array = np.reshape(array, (image.height, image.width, 4))
        array = array[:, :, :3]  # Remove alpha channel
        array = array[:, :, ::-1]  # Convert BGR to RGB
        surface = pygame.surfarray.make_surface(array.swapaxes(0, 1))
        image_queue.put(surface)
    
    # Listen to the camera sensor
    camera.listen(lambda image: process_image(image))
    
    # Create a collision sensor for the ego vehicle
    collision_bp = blueprint_library.find('sensor.other.collision')
    collision_sensor = world.spawn_actor(collision_bp, carla.Transform(), attach_to=ego_vehicle)
    spawned_actors.append(collision_sensor)
    
    # List to store collision events
    collision_history = []
    
    def collision_callback(event):
        collision_history.append(event)
        print(f"Collision detected: {event}")
    
    collision_sensor.listen(lambda event: collision_callback(event))
    
    # Spawn the NPC vehicles
    num_npcs = 20  # Number of NPC vehicles to spawn
    for i in range(num_npcs):
        if not spawn_points:
            print("Run out of spawn points!")
            break
            
        spawn_point = spawn_points.pop(0)
        blueprint = random.choice(safe_vehicle_blueprints)
        
        # Try to spawn the vehicle
        npc = world.try_spawn_actor(blueprint, spawn_point)
        if npc:
            spawned_actors.append(npc)
            print(f"Spawned NPC vehicle {i+1}/{num_npcs}")
            
            # Set up traffic manager for coherent behavior
            npc.set_autopilot(True, traffic_manager.get_port())
            
            # Randomize behavior slightly for realism
            # Some vehicles obey traffic rules strictly
            if random.random() > 0.8:
                traffic_manager.ignore_lights_percentage(npc, random.uniform(0, 20))
            else:
                traffic_manager.ignore_lights_percentage(npc, 0)
                
            # Randomize lane changing behavior
            traffic_manager.auto_lane_change(npc, random.random() > 0.3)
            
            # Randomize speed - most cars drive at similar speeds, but some variance
            speed_difference = random.normalvariate(0, 10)  # Normal distribution with mean 0, std 10
            traffic_manager.vehicle_percentage_speed_difference(npc, speed_difference)
            
            # Set distance to leading vehicle for realistic following
            traffic_manager.distance_to_leading_vehicle(npc, random.uniform(1.0, 4.0))
    
    # Position the spectator to view the ego vehicle
    spectator = world.get_spectator()
    spectator_transform = carla.Transform(
        ego_vehicle.get_transform().location + carla.Location(z=20, x=-15),
        carla.Rotation(pitch=-90)
    )
    spectator.set_transform(spectator_transform)
    
    # Main game loop
    try:
        start_time = time.time()
        running = True
        while running:
            # Advance the simulation
            world.tick()
            
            # Process Pygame events
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
            
            # Update the display if we have a new image
            try:
                surface = image_queue.get(block=False)
                
                # Add some HUD information
                elapsed_time = time.time() - start_time
                velocity = ego_vehicle.get_velocity()
                speed = 3.6 * np.sqrt(velocity.x**2 + velocity.y**2 + velocity.z**2)  # in km/h
                
                # Create text for HUD
                text_surface = font.render(f"Speed: {speed:.1f} km/h | Time: {elapsed_time:.1f}s", True, (255, 255, 255))
                
                # Draw the camera image
                display.blit(surface, (0, 0))
                # Draw the HUD on top
                display.blit(text_surface, (10, 10))
                
                pygame.display.flip()
                
            except queue.Empty:
                pass
            
            # Cap the frame rate
            clock.tick(20)
            
    finally:
        # Clean up
        print("Cleaning up actors...")
        for actor in spawned_actors:
            if actor is not None and actor.is_alive:
                actor.destroy()
                
        # Reset to asynchronous mode
        settings = world.get_settings()
        settings.synchronous_mode = False
        settings.fixed_delta_seconds = None
        world.apply_settings(settings)
        
        pygame.quit()
        print("Simulation ended")
else:
    print("Failed to spawn ego vehicle!")