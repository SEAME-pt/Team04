# Requirements

## System overview

The JetRacer instrument cluster Qt application project is aimed at creating a real-time 
speedometer for a JetRacer car. The application will run on a Jetson Nano and receive speed data 
from a speed sensor via the in-vehicle communication using Controller Area Network (CAN) protocol. 
This project will provide an opportunity for students to gain practical experience in software 
engineering, specifically in the areas of embedded systems, software architecture, and 
communication protocols. The project will also allow students to gain knowledge of the 
GUI frameworks (eg. Qt), that are widely used in the automotive industry for developing 
many embedded applications. The successful completion of this project will demonstrate the 
students' ability to design and implement a real-world software solution, and their ability 
to effectively communicate their results.

## Functional Requirements

* The user should be able to control the car
* The user should be able to see real time speed data on the GUI
* The user should be able to see the battery level of the car in the GUI

## Non-Functional Requirements

The non functional (or quality) requirements are described by the **FURPS+** model. The
FURPS model was first elaborated by Grady and Caswell 1987 at Hewlett-Packard. The
letters represent functionality (capabilities), usability (aesthetic, consistency, documentation), 
reliability (accuracy, frequency of failure), performance (speed, response time), and
supportability (testing, compatibility).

### Functionality

* The GUI should start when the Jetson Nano boots
* System should be prepared to update CAN data to the cloud
* CAN-BUS communication should be used between the Jetson Nano and the Arduino

### Usability

* The GUI should be clear on the used metric system (km/h or mph)
* The GUI should be responsive (adapt to screen size)

### Reliability

* The system should not fail mid drive.
* The speed should be accurate

### Performance

* The speed should be presented in real time

### Supportability

* Unit tests should be developed for all software
* System should be scalable
* System should keep logging data for future analysis
* It should support over-the-air (OTA) updates.

### Design Constraints (+)

* GUI should be implemented using QT/QML and C++

### Implementation Constraints (+)

* Static code analysis should be performed with clang-tidy
* MISRA C++ 2023 should ideally be followed
* GitHub actions should be used for static analysis and automatic deployments

### Interface Constraints (+)

* SSH is to be used to connect to the Jetson Nano
* Updates should be fetched via HTTPS

### Physical Constraints (+)

* The speed sensor system should be Arduino based, and send the data via CAN BUS
* The GUI application should run on a Jetson Nano or Raspberry Pi ECU
* The GUI application should run on a 7.9' LCD
* The JetRacer should be controllable via the Gamepad
