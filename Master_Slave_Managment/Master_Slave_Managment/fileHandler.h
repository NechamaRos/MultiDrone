#pragma once
#include "event.h"
#include "fileInfo.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <filesystem>
#include <sstream>
#include <iomanip>
using namespace std;

class FileHandler
{	
	string baseFileName;
	size_t maxFiles;
	deque<FileInfo> fileNamesQueue;

	void loadMetadata();
	void saveMetadata();
	bool parseTimeString(const string& timeStr, tm& tm);
	string writeFileName(string startTimeFormatted, string endTimeFormatted, bool isSortedList);
	string readContentFromFile(string filePath);
	void addRelevantEvents(const string& fileContent, vector<Event>& events, size_t maxSize, const tm& startTime, const tm& endTime);

public:
	FileHandler(const string& baseFileName, size_t maxFiles);
	FileHandler(const FileHandler& other);
	FileHandler& operator=(const FileHandler& other);
	string writeToFile(vector<Event> events, bool isSortedList=false);
	void readFromDiskAndAddToEvents(vector<Event>& events, const tm startTime, const tm endTime, size_t maxSize);

};

