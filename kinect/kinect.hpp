#pragma once

#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/logger.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/registration.h>

#include <opencv/cv.hpp>

namespace Kinect
{

struct Images {
    cv::Mat rgb;
    cv::Mat ir;
    cv::Mat depth;
};

void init();
void update();
Images getImages();

}  // namespace Kinect
