# Create Cross-Compilation Sysroot Environment for Raspberry Pi

To build Qt6 from source for aarch64, a cross-compilation sysroot is necessary. This environment allows you to compile software on a host system for ARM-based Raspberry Pi systems.

### Build the Sysroot Locally Using Docker

#### 1. **Ensure Docker is Installed**

Make sure Docker is installed:

```bash
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

#### 2. **Run the Setup Script**

Run the setup script to create the sysroot:

```bash
sudo ./docker/run.sh
```

### Resources

- [Qt6 Cross-Compilation Guide for Raspberry Pi](https://wiki.qt.io/Cross-Compile_Qt_6_for_Raspberry_Pi)
- [QTonRaspberryPi GitHub Repository](https://github.com/PhysicsX/QTonRaspberryPi/tree/main)
