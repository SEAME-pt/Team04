# Networking and Communication - Glossary

## CAN Bus  
**Controller Area Network (CAN)**: A robust communication protocol designed to enable real-time data exchange between microcontrollers and embedded devices within a vehicle. It operates without the need for a central computer and is essential for transmitting data such as speed, engine performance, and diagnostics.

## Interoperability  
**Interoperability**: The ability of different systems, devices, or protocols to work together seamlessly. In the context of the PiRacer, it ensures smooth communication between hardware components like the speed sensor, Raspberry Pi, and the CAN bus.

## MCP2515  
**MCP2515**: A standalone CAN controller that interfaces microcontrollers (or Raspberry Pi) with the CAN bus. It processes CAN messages and interacts with the TJA1050 transceiver to send and receive data.

## Network Latency  
**Network Latency**: The time delay between sending a message and receiving a response. Low latency is critical for real-time communication in vehicle systems to ensure timely data processing and decision-making.

## OBD-II  
**OBD-II (On-Board Diagnostics II)**: A standardized vehicle diagnostic interface that allows external devices to access real-time data and diagnostic information from the vehicle's systems. An OBD-II cable connects the Raspberry Pi to the CAN bus.

## Over-the-Air Updates (OTA)  
**Over-the-Air Updates (OTA)**: A method of updating software on a device remotely via a network connection, without requiring physical access. This feature is increasingly common in modern vehicles and IoT systems.

## Real-Time Data  
**Real-Time Data**: Information that is collected, processed, and transmitted with minimal delay, allowing the system to respond to changes instantaneously. In the PiRacer project, this involves displaying live speed data from the sensor.

## TJA1050  
**TJA1050**: A high-speed CAN transceiver used alongside the MCP2515. It converts the CAN protocol signals to a form that the Raspberry Pi or microcontroller can understand.

## Vehicle-to-Everything (V2X)  
**Vehicle-to-Everything (V2X)**: Communication technology that enables vehicles to exchange information with infrastructure, other vehicles, and even pedestrians. It enhances safety and efficiency in smart transportation systems.