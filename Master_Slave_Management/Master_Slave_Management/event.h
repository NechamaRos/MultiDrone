#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std;
using namespace std::chrono;
struct Event
{
private:
	tm time;
	string message;
public:
	Event();
	Event(string message);
	Event(tm time, string message);
	Event(const Event& other);
	Event& operator=(const Event& other);
	bool operator<(const Event& other) const;
	bool operator>(const Event& other) const;
	bool operator==(const Event& other) const;
	tm getTime();
	string getMessage();
	string getFormatTime();
	size_t getSize();
	string toString();
};

