#pragma once

#include <libfreenect2/logger.h>

#include <cstdlib>
#include <fstream>

namespace Kinect
{

class FileLogger : public libfreenect2::Logger
{
private:
    std::ofstream logfile_;

public:
    FileLogger(const char* filename)
    {
        if (filename) {
            logfile_.open(filename);
        }
        level_ = Debug;
    }

    bool good()
    {
        return logfile_.is_open() && logfile_.good();
    }

    virtual void log(Level level, const std::string& message)
    {
        logfile_ << "[" << libfreenect2::Logger::level2str(level) << "] " << message << std::endl;
    }
};

}  // namespace Kinect
