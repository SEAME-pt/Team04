# Setting Up a CARLA 0.9.15 Server on Ubuntu

This guide focuses specifically on setting up a CARLA 0.9.15 server using the pre-built package on Ubuntu. CARLA (Car Learning to Act) is an open-source simulator for autonomous driving research.

## Table of Contents
1. [Pre-built Package Installation for Ubuntu](#pre-built-package-installation-for-ubuntu)
2. [Running the CARLA Server](#running-the-carla-server)
3. [Setting Up the Python API](#setting-up-the-python-api)
4. [Performance Optimization](#performance-optimization)

## Pre-built Package Installation for Ubuntu

This is the recommended method for most users as it's straightforward and requires minimal setup.

### Step 1: Update System Packages

Ensure your system is up to date:

```bash
sudo apt-get update
sudo apt-get upgrade -y
```

### Step 2: Install Required Dependencies

Install the necessary libraries for CARLA to run properly:

```bash
sudo apt-get install -y libsdl2-2.0-0 libsdl2-image-2.0-0 libpng16-16 libjpeg-turbo8 libtiff5 libomp5 libvulkan1
```

For newer Ubuntu versions (20.04+), you might also need:

```bash
sudo apt-get install -y libgl1-mesa-glx libopengl0
```

### Step 3: Install NVIDIA Drivers (if using NVIDIA GPU)

For optimal performance, ensure you have the latest NVIDIA drivers:

```bash
sudo add-apt-repository ppa:graphics-drivers/ppa -y
sudo apt-get update
sudo apt-get install -y nvidia-driver-515  # or newer version
```

After installation, reboot your system:

```bash
sudo reboot
```

### Step 4: Download CARLA 0.9.15

Create a directory for CARLA and download the package:

```bash
mkdir -p ~/CARLA
cd ~/CARLA
wget https://carla-releases.s3.eu-west-3.amazonaws.com/Linux/CARLA_0.9.15.tar.gz
```

### Step 5: Extract the Archive

Extract the downloaded archive:

```bash
tar -xvzf CARLA_0.9.15.tar.gz -C ~/CARLA
```

This will extract all files to the CARLA directory.

### Step 6: Verify Installation

Check if the installation was successful by listing the contents of the CARLA directory:

```bash
ls -la ~/CARLA
```

You should see the CarlaUE4.sh script and other files and directories.

## Running the CARLA Server

### Basic Server Start

To start the CARLA server with default settings:

```bash
cd ~/CARLA
./CarlaUE4.sh
```

This will launch CARLA in full-screen mode with default settings.

### Custom Server Configuration

CARLA server can be customized with various command-line options:

#### Running in Windowed Mode

```bash
./CarlaUE4.sh -windowed -ResX=800 -ResY=600
```

#### Setting Graphics Quality

```bash
./CarlaUE4.sh -quality-level=Low
```

Available quality levels: Low, Medium, High, Epic

#### Setting Frame Rate

```bash
./CarlaUE4.sh -fps=30
```

#### Launching with Specific Map

```bash
./CarlaUE4.sh -quality-level=Low -map=Town03
```

Available maps: Town01, Town02, Town03, Town04, Town05, Town06, Town07

#### Starting Without Rendering (Headless Mode)

```bash
./CarlaUE4.sh -RenderOffScreen
```

#### Setting RPC and Streaming Ports

```bash
./CarlaUE4.sh -carla-rpc-port=3000 -carla-streaming-port=3001
```

### Comprehensive Example

To launch CARLA with optimized settings for development:

```bash
./CarlaUE4.sh -windowed -ResX=1280 -ResY=720 -quality-level=Medium -fps=30 -carla-rpc-port=2000
```

## Setting Up the Python API

The Python API allows you to interact with the CARLA server through Python scripts.

### Step 1: Install Python Dependencies

Install the required Python packages:

```bash
pip3 install --user pygame numpy matplotlib pillow
```

For more advanced features, you might also need:

```bash
pip3 install --user opencv-python networkx scipy
```

### Step 2: Set Up PYTHONPATH

Add the CARLA Python module to your PYTHONPATH:

For temporary use in current terminal:

```bash
export PYTHONPATH=$PYTHONPATH:~/CARLA/PythonAPI/carla/dist/carla-0.9.15-py3.7-linux-x86_64.egg
```

For permanent setup, add this line to your ~/.bashrc file:

```bash
echo 'export PYTHONPATH=$PYTHONPATH:~/CARLA/PythonAPI/carla/dist/carla-0.9.15-py3.7-linux-x86_64.egg' >> ~/.bashrc
source ~/.bashrc
```

⚠️ Note: The exact filename might vary depending on your Python version. Check the actual filename in the dist directory.

### Step 3: Verify Python API Installation

Create a simple test script to verify the Python API is working:

```bash
mkdir -p ~/carla_test
cd ~/carla_test
touch test_carla.py
```

Add this content to test_carla.py:

```python
import carla
import time

try:
    # Connect to the client
    client = carla.Client('localhost', 2000)
    client.set_timeout(10.0)
    
    # Get the world
    world = client.get_world()
    
    # Print available maps
    print(f"Available maps: {client.get_available_maps()}")
    
    # Get the blueprint library
    blueprint_library = world.get_blueprint_library()
    
    # Count available blueprints
    vehicle_bps = blueprint_library.filter('vehicle.*')
    print(f"Available vehicle blueprints: {len(vehicle_bps)}")
    
    # Get the spectator
    spectator = world.get_spectator()
    transform = spectator.get_transform()
    print(f"Spectator location: {transform.location}")
    
    print("CARLA Python API is working correctly!")
    
except Exception as e:
    print(f"Error: {e}")
```

Run the test script (make sure the CARLA server is running):

```bash
python3 test_carla.py
```

If successful, you should see information about available maps, vehicle blueprints, and the spectator's location.

### Step 4: Running Example Scripts

CARLA comes with several example scripts. Try running the manual control example:

```bash
cd ~/CARLA/PythonAPI/examples
python3 manual_control.py
```

This will open a window that allows you to control a vehicle manually.

Other useful examples include:

- `automatic_control.py`: Demonstrates autonomous driving
- `spawn_npc.py`: Spawns NPC vehicles and pedestrians
- `dynamic_weather.py`: Shows how to change weather dynamically

## Performance Optimization

Optimizing CARLA performance is crucial, especially for complex simulations. Here are detailed settings for different scenarios:

### For Development and Testing

When developing and testing your autonomous driving algorithms:

```bash
./CarlaUE4.sh -quality-level=Low -windowed -ResX=1280 -ResY=720 -fps=30
```

### For Headless Simulation (No Rendering)

When running simulations on a server or when visualization is not needed:

```bash
./CarlaUE4.sh -RenderOffScreen -quality-level=Low -carla-server -fps=10
```

### For Maximum Performance

When running on lower-end hardware or when maximum performance is needed:

```bash
./CarlaUE4.sh -quality-level=Low -windowed -ResX=800 -ResY=600 -fps=20 -carla-no-hud
```

### Python-side Optimizations

Add these settings in your Python scripts for better performance:

```python
import carla

# Connect to the client
client = carla.Client('localhost', 2000)
client.set_timeout(10.0)

# Get the world
world = client.get_world()

# Configure world settings for performance
settings = world.get_settings()
settings.synchronous_mode = True  # Synchronous mode for better control
settings.fixed_delta_seconds = 0.05  # 20 FPS
settings.no_rendering_mode = False  # Set to True if you don't need rendering
world.apply_settings(settings)

# Configure traffic manager for performance
traffic_manager = client.get_trafficmanager(8000)
traffic_manager.set_synchronous_mode(True)
traffic_manager.set_global_distance_to_leading_vehicle(5.0)  # Increase for better performance
```

### Reducing CPU and GPU Load

To reduce load on your system:

1. Limit the number of NPCs:
   ```bash
   python3 spawn_npc.py -n 50 -w 0  # 50 vehicles, 0 pedestrians
   ```

2. Use simpler maps (Town01 or Town02):
   ```bash
   ./CarlaUE4.sh -map=Town01
   ```

3. Disable weather effects in your Python scripts:
   ```python
   weather = carla.WeatherParameters.ClearNoon
   world.set_weather(weather)
   ```

These optimizations should help you run CARLA 0.9.15 smoothly on your Ubuntu system, even with moderate hardware.
