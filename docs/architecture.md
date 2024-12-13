# Instrument Cluster Architecture

## Physical Layer

![PiRacer](images/piracer_schematics.png)

## Components

* Jetson NANO
* Arduino Uno R3
* RS485 CAN for Jetson Nano (MCP2515)
* MCP2515 CAN Module
* LM393 Speedsensor
* 7.9inch DSI LCD

## Logical View

### System (Level 1)

![Architecture](diagrams/out/System-System_Diagram_for_JetRacer_System.png)

### Container (Level 2)

![Container](diagrams/out/techtribesjs-Container_Diagram_for_JetRacer_System.png)

### Component (Level 3)

## Physical View

![Deployment](diagrams/out/deployment-Deployment_Diagram_for_JetRacer_System.png)

## Process View

### Arduino

Incoming pulse

![Arduino](diagrams/out/process/arduino_speed_pulse.png)

Speed calculation / can message

![Arduino](diagrams/out/process/arduino_speed_calc.png)

### COM - Middleware

![Arduino](diagrams/out/process/com_can.png)

### Instrument Cluster

![Arduino](diagrams/out/process/cluster_subscriber.png)
