simple_camera.cpp
The last example is a simple C++ program which reads from the camera and displays to a window on the screen using OpenCV:

$ g++ -std=c++11 -Wall -I/usr/lib/opencv -I/usr/include/opencv4 simple_camera.cpp -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -o simple_camera

$ ./simple_camera
This program is a simple outline, and does not handle needed error checking well.
https://github.com/dusty-nv/jetson-utils