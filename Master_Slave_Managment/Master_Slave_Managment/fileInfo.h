#pragma once
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;
class FileInfo
{
public:
	string fileName;
	tm startTime;
	tm endTime;

	FileInfo();
	FileInfo(const string& fileName, const tm& start,
		const tm& end);
};

