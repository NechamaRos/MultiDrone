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
	bool operator<(const tm& otherTime) const;
	bool operator>(const Event& other) const;
	bool operator>(const tm& otherTime) const;
	bool operator==(const Event& other) const;
	bool operator==(const tm& otherTime) const;
	bool operator<=(const tm& otherTime) const;
	bool operator>=(const tm& otherTime) const;
	friend bool operator<(const std::tm& tm1, const std::tm& tm2);
	friend bool operator>=(const std::tm& tm1, const std::tm& tm2);
	friend bool operator<=(const std::tm& tm1, const std::tm& tm2);
	static bool compareTimes(const tm& tm1, const tm& tm2) ;
	tm getTime();
	string getMessage();
	string getFormatTime();
	size_t getSize();
	string toString();
};

bool operator<(const std::tm& tm1, const std::tm& tm2);
bool operator>=(const std::tm& tm1, const std::tm& tm2);
bool operator<=(const std::tm& tm1, const std::tm& tm2);