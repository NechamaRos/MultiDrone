// Logger.cpp
#include "Logger.h"
#include <stdexcept>
#include <ctime>


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

    std::time_t currentTime;
    std::time(&currentTime);  // Get current time

    std::tm localTime;
    localtime_s(&localTime, &currentTime);

    logFile_
        << localTime.tm_mday << '/'
        << (localTime.tm_mon + 1) << '/'
        << (localTime.tm_year + 1900) << ' '
        << localTime.tm_hour << ':'
        << localTime.tm_min << ':'
        << localTime.tm_sec << " - ";

    logFile_ << message << std::endl;
}
