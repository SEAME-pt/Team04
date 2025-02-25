Here's your updated Carla Simulator guide in markdown format with the Jupyter instructions included:

# Carla Simulator Setup Guide

This guide walks through setting up and running the CARLA autonomous driving simulator with NVIDIA GPU support, along with configuring a Python environment for development.

## 1 Installing CARLA

#### https://carla.readthedocs.io/en/latest/start_quickstart/#b-package-installation
First install CARLA from the package installation method. I installed version 0.9.15

## 2.1 Hardware Acceleration Setup
### IMPORTANT: This step solves the problem of not reconizing NVIDIA board in Ubuntu

Before running CARLA, set up the environment variables for proper NVIDIA GPU utilization:

```bash
# Configure Vulkan and shader caching
export VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/nvidia_icd.json
export __GL_SHADER_DISK_CACHE=1
export __GL_SHADER_DISK_CACHE_PATH=/tmp
```

## 2.2 Running CARLA with GPU Offloading
### IMPORTANT: This step solves the problem of having insuficient hardware (CPU/GPU)
For optimal performance with NVIDIA GPUs:

```bash
# Run with NVIDIA GPU offloading at 720p in windowed mode
DISPLAY=:0 __NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia __GL_SYNC_TO_VBLANK=0 ./CarlaUE4.sh -ResX=1280 -ResY=720 -windowed
```

## 3. Performance Mode

If you need better performance on lower-end hardware:

```bash
# Run with low quality settings in windowed mode
./CarlaUE4.sh -windowed -quality-level=Low
```

## 4. Python Environment Setup

Set up a dedicated Python virtual environment for CARLA development:

```bash
# Create and activate virtual environment
python3 -m venv carla-sim
source carla-sim/bin/activate

# Install required packages
pip install carla
pip install pygame
pip install numpy
pip install jupyter
pip install opencv-python
```

## 5. Using Jupyter with Carla

### Starting the Environment

First, make sure your Carla server is running in a terminal:

```bash
# Navigate to your Carla directory
cd path/to/carla

# Start Carla (using one of the methods mentioned above)
./CarlaUE4.sh -windowed -quality-level=Low
```

Leave this terminal running, and open a new terminal window to start Jupyter:

```bash
# Activate your virtual environment
source carla-sim/bin/activate

# Start Jupyter notebook server
jupyter notebook
```

This will automatically open your web browser with the Jupyter interface. If it doesn't, look for a URL in the terminal that looks like: `http://localhost:8888/?token=some_long_token`

### Creating Your First Carla Notebook

In the Jupyter web interface:
1. Click the "New" button in the top right
2. Select "Python 3" from the dropdown menu

This creates a new notebook with an empty cell waiting for your code.

### Connecting to Carla

In the first cell of your notebook, enter this code:

```python
import carla
import time

# Connect to the Carla server
client = carla.Client('localhost', 2000)
client.set_timeout(10.0)  # seconds

# Get the world
world = client.get_world()

# Print some info to verify connection
print(f"Connected to Carla version: {client.get_server_version()}")
print(f"Client version: {client.get_client_version()}")
print(f"Current map: {world.get_map().name}")
```

To run this cell, click the "Run" button at the top, or press Shift+Enter.

### Exploring Available Vehicles

Add a new cell (click the "+" button in the toolbar) and enter:

```python
# Get the blueprint library
blueprint_library = world.get_blueprint_library()

# List all available vehicles
vehicles = blueprint_library.filter('vehicle.*')
print(f"Available vehicles ({len(vehicles)}):")
for i, vehicle in enumerate(vehicles):
    print(f"{i}: {vehicle.id}")
```

Run this cell with Shift+Enter.

### Spawning a Vehicle

In a new cell, enter:

```python
import random

# Get spawn points
spawn_points = world.get_map().get_spawn_points()
print(f"Found {len(spawn_points)} spawn points")

# Choose a random vehicle blueprint
vehicle_bp = random.choice(vehicles)
print(f"Selected vehicle: {vehicle_bp.id}")

# Try to spawn the vehicle
transform = random.choice(spawn_points)
vehicle = world.try_spawn_actor(vehicle_bp, transform)

if vehicle:
    print(f"Successfully spawned {vehicle_bp.id} at {transform.location}")
else:
    print("Failed to spawn vehicle. Location might be occupied.")
```

### Making the Vehicle Move

In a new cell:

```python
# Make sure we have a vehicle spawned
if vehicle:
    # Set the vehicle to autopilot
    vehicle.set_autopilot(True)
    print("Vehicle set to autopilot mode")
    
    # Let it drive for a while
    print("Letting the vehicle drive for 30 seconds...")
    for i in range(30):
        # Print the current location every 5 seconds
        if i % 5 == 0:
            location = vehicle.get_location()
            print(f"Current location: ({location.x:.1f}, {location.y:.1f}, {location.z:.1f})")
        time.sleep(1)
    
    # Disable autopilot
    vehicle.set_autopilot(False)
    print("Autopilot disabled")
```

### Cleaning Up

In a final cell:

```python
# Destroy our vehicle when done
if vehicle:
    vehicle.destroy()
    print("Vehicle removed from simulation")
    
print("Session complete!")
```
