#pragma once
#ifndef API_H
#define API_H
#include <string>
#include <vector>
#include <stdexcept>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class API {
public:
    unsigned char* GenerateKey(size_t length = 16);
    static void writeLog(const std::string& message);
};

#endif // API_H