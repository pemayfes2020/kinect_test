#include "kinect.hpp"
#include "logger.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

namespace Kinect
{

// 実装は https://openkinect.github.io/libfreenect2/ に基づく

class KinectManager
{
private:
    libfreenect2::Freenect2 freenect2;
    std::shared_ptr<libfreenect2::PacketPipeline> pipeline;
    libfreenect2::Freenect2Device* device;
    std::shared_ptr<libfreenect2::SyncMultiFrameListener> listener;
    libfreenect2::FrameMap frames;

    Images images;

public:
    KinectManager()
        : freenect2{}, pipeline{nullptr}, device{nullptr},
          listener{}, frames{}
    {
        // Logging
        libfreenect2::setGlobalLogger(libfreenect2::createConsoleLogger(libfreenect2::Logger::Debug));

        FileLogger* filelogger = new FileLogger(getenv("LOGFILE"));
        if (filelogger->good()) {
            libfreenect2::setGlobalLogger(filelogger);
        } else {
            delete filelogger;
        }

        // Discover Devices
        if (freenect2.enumerateDevices() == 0) {
            std::cerr << "no device connected!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::string serial = freenect2.getDefaultDeviceSerialNumber();

        // Open Pipeline
        pipeline = std::make_shared<libfreenect2::PacketPipeline>();

        // Open and Configure the Device
        device = freenect2.openDevice(serial, pipeline.get());

        int types = libfreenect2::Frame::Color
                    | libfreenect2::Frame::Ir
                    | libfreenect2::Frame::Depth;
        listener = std::make_shared<libfreenect2::SyncMultiFrameListener>(types);
        libfreenect2::FrameMap frames;
        device->setColorFrameListener(listener.get());
        device->setIrAndDepthFrameListener(listener.get());

        // Start the Device
        if (!device->start()) {
            std::cerr << "Cannot Open Device" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Output Informations
        std::cout << "Version: " << LIBFREENECT2_VERSION << std::endl;

        std::cout << "device serial: " << device->getSerialNumber() << std::endl;
        std::cout << "device firmware: " << device->getFirmwareVersion() << std::endl;
    }
    ~KinectManager()
    {
        device->stop();
        device->close();
    }

    void update()
    {
        if (!listener->waitForNewFrame(frames, 10 * 1000)) {
            std::cerr << "timeout!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        libfreenect2::Frame* rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame* ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame* depth = frames[libfreenect2::Frame::Depth];

        images.rgb = cv::Mat{
            (int)rgb->height, (int)rgb->width,
            CV_8UC4, rgb->data}
                         .clone();

        images.ir = cv::Mat{
            (int)ir->height, (int)ir->width,
            CV_32FC1, ir->data}
                        .clone();

        images.depth = cv::Mat{
            (int)depth->height, (int)depth->width,
            CV_32FC1, depth->data}
                           .clone();

        listener->release(frames);
    }

    Images getImages()
    {
        return images;
    }

    static KinectManager* getInstance()
    {
        static KinectManager instance;
        return &instance;
    }
};

std::unique_ptr<KinectManager> kinect;

void init()
{
    kinect = std::make_unique<KinectManager>();
}

void update()
{
    kinect->update();
}

Images getImages()
{
    return kinect->getImages();
}

}  // namespace Kinect
