#include "Logger.h"
#include <stdexcept>
#include <ctime>
#include <mutex>
#include <fstream>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : logFile_("log.txt", std::ios_base::app) {
    if (!logFile_.is_open()) {
        throw std::runtime_error("Unable to open log file");
    }
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> guard(mutex_);

    std::time_t currentTime = std::time(nullptr);  // Get current time
    std::tm localTime;

    // Use localtime_r to get the local time
    if (localtime_r(&currentTime, &localTime) == nullptr) {
        // Handle error
        return;
    }

    logFile_
        << localTime.tm_mday << '/'
        << (localTime.tm_mon + 1) << '/'
        << (localTime.tm_year + 1900) << ' '
        << localTime.tm_hour << ':'
        << localTime.tm_min << ':'
        << localTime.tm_sec << " - ";

    logFile_ << message << std::endl;
}
