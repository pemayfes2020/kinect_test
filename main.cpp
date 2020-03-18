#include "kinect.hpp"

#include <opencv/cv.hpp>

#include <cstdlib>
#include <iostream>
#include <signal.h>


int main(int argc, char* argv[])
{

    Kinect::init();

    while (true) {
        Kinect::update();
    
        auto [rgb, ir, depth] = Kinect::getImages();

        cv::imshow("rgb", rgb);
        cv::imshow("ir", ir);
        cv::imshow("depth", depth);

        cv::waitKey(1);
    }

    return 0;
}
