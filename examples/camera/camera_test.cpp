#include <opencv2/opencv.hpp>
//#include "/usr/include/opencv4"

std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
        return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=640, height=360, "
                               "format=(string)NV12, framerate=30/1 ! nvvidconv ! video/x-raw, format=BGRx ! "
                               "videoconvert ! video/x-raw, format=BGR ! appsink drop=1 max-buffers=1";
    }

int main()
{
    // int capture_width = 1280 ;
    // int capture_height = 720 ;
    // int display_width = 1280 ;
    // int display_height = 720 ;
    // int framerate = 30 ;
    // int flip_method = 0 ;

    std::string pipeline = gstreamer_pipeline(capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);
    std::cout << "Using pipeline: \n\t" << pipeline << "\n";
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if(!cap.isOpened()) {
	std::cout<<"Failed to open camera."<<std::endl;
	return (-1);
    }

    cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;

    std::cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
    	if (!cap.read(img)) {
		std::cout<<"Capture read error"<<std::endl;
		break;
	}
	cv::imshow("CSI Camera",img);
	int keycode = cv::waitKey(10) & 0xff ;
        if (keycode == 27) break ;
    }

    cap.release();
    cv::destroyAllWindows() ;
    return 0;
}
