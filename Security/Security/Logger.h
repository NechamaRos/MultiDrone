// Logger.h
#pragma once

#include <fstream>
#include <string>
#include <mutex>

class Logger {
public:
    static Logger& getInstance();

    void log(const std::string& message);

private:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream logFile_;
    std::mutex mutex_;
};
 