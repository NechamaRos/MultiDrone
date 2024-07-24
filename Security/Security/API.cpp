
#include <iostream>
#include "API.h"

unsigned char* GenerateKey(size_t length = 16) { // 128 ביטים = 16 בתים (characters)
    const string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()";
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, characters.size() - 1);
    unsigned char* key = new unsigned char[length];
    for (size_t i = 0; i < length; ++i) {
        key[i] = characters[distribution(generator)];
    }
    return key;
}

void API::writeLog(const std::string& message) {
    std::ofstream logFile;
    logFile.open("log.txt", std::ios_base::app); // פתיחת הקובץ במצב הוספה
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    }
    else {
        std::cerr << "Unable to open log file" << std::endl;
    }
}


