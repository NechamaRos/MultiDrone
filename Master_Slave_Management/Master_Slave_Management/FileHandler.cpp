#include "fileHandler.h"

FileHandler::FileHandler(const string& baseFileName, size_t maxFiles)
	:baseFileName(baseFileName), maxFiles(maxFiles)
{
	loadMetadata();
}

FileHandler::FileHandler(const FileHandler& other)
	:baseFileName(other.baseFileName), maxFiles(other.maxFiles)
{
	fileNamesQueue = other.fileNamesQueue;
}

FileHandler& FileHandler::operator=(const FileHandler& other)
{
	if (this != &other)
	{
		baseFileName = other.baseFileName;
		maxFiles = other.maxFiles;
		fileNamesQueue = other.fileNamesQueue;
	}
	return *this;
}

string FileHandler::writeFileName(string startTimeFormatted, string endTimeFormatted,bool isSortedList)
{
	string fileName;

	if (isSortedList)
		fileName = "sorted_event_list_by_time_range";
	else
		fileName = to_string(fileNamesQueue.size()) + "_" + baseFileName;
	fileName+= "_from_" + startTimeFormatted + "_to_" + endTimeFormatted + ".log";
	return fileName;
}


void FileHandler::loadMetadata()
{
	ifstream metaFile(baseFileName + "_metadata.txt");
	if (!metaFile.is_open())
		return;

	string line,fileName, startTimeStr, endTimeStr;
	istringstream iss;
	tm tmStart = {}, tmEnd = {};
	time_point<system_clock> startTime, endTime;
	while (getline(metaFile,line))
	{
		iss.clear();
		iss.str(line);

		if (!(iss >> fileName >> startTimeStr >> endTimeStr))
		{
			cerr << "Failed to read metadata line: " << line << endl;
			continue;
		}

		if (!parseTimeString(startTimeStr, tmStart))
			continue;

		if (!parseTimeString(endTimeStr, tmEnd))
			continue;

		fileNamesQueue.emplace_back(fileName, tmStart, tmEnd);
	}
	metaFile.close();
}

void FileHandler::saveMetadata()
{
	char buffer1[80];
	char buffer2[80];
	ofstream metaFile(baseFileName + "_metadata.txt", ios::out | ios::trunc);
	for (const auto& fileInfo : fileNamesQueue)
	{
		strftime(buffer1, sizeof(buffer1), "%d-%m-%Y_%H.%M.%S", &fileInfo.startTime);
		strftime(buffer2, sizeof(buffer2), "%d-%m-%Y_%H.%M.%S", &fileInfo.endTime);
		metaFile << fileInfo.fileName << " "
			<< string(buffer1) << " "
			<< string(buffer2) << "\n";
	}
}


bool FileHandler::parseTimeString(const string& timeStr, tm& tm)
{
	istringstream ss(timeStr);
	ss >> get_time(&tm, "%d-%m-%Y_%H.%M.%S");
	if(ss.fail())
		cerr << "Failed to parse time string: " << timeStr << endl;
	return !ss.fail();
}

/// <summary>
/// Reads the contents of the file found in the received path and returns it as a string.
/// </summary>
/// <param name="filePath">The file path to read.</param>
/// <returns>the file content.</returns>
string FileHandler::readContentFromFile(string filePath)
{
	ifstream inFile;
	inFile.open(filePath, ios::in | ios::ate);
	if (inFile.is_open())
	{
		streampos fileSize = inFile.tellg();
		inFile.seekg(0, ios::beg); //Returns the pointer to the beginning of the file

		string fileContent((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
		inFile.close();
		return fileContent;
	}	
	else
	{
		return "";
		//throw runtime_error("Unable to open file: " + filePath);
	}
}

/// <summary>
/// adding the relevant events to the vector events.
/// </summary>
/// <param name="fileContent">The contents of the file.</param>
/// <param name="events">The event vector to which it is added.</param>
/// <param name="maxSize">The maximum size to adding events</param>
/// <param name="startTime">The beginning of the time range of the desired events</param>
/// <param name="endTime">The end of the time range of the desired events</param>
void FileHandler::addRelevantEvents(const string& fileContent, vector<Event>& events, size_t maxSize,const tm& startTime,const tm& endTime)
{
	//Creates a stream from the content of the file.
	istringstream fileStream(fileContent), iss;
	string line, timeStr, eventString;
	size_t currentSize = 0, eventSize;
	tm tm = {};


	// Reads the lines and add them to vector
	vector<string> lines;

	while (getline(fileStream, line))
	{
		lines.push_back(line);
	}

	//Go over the lines from the end to the beginning.
	for (auto lineIt = lines.rbegin(); lineIt != lines.rend(); ++lineIt)
	{
		iss.clear();
		iss.str(*lineIt);
		iss >> timeStr; //read the time from thr line.
		getline(iss, eventString);//read the string of the event.

		//Checks if the read date is correct and is within the desired time range.
		if (parseTimeString(timeStr, tm) && tm >= startTime && tm <= endTime)
		{
			Event event(tm, eventString);
			eventSize = event.getSize();
			if (currentSize + eventSize > maxSize)
			{
				return; //Exits the function if the event size has reached the maximum.
			}
			events.push_back(event); // Add the event to the temporary vector.
			currentSize += eventSize;
		}
	}
}

/// <summary>
/// Reads the events in the range from the disk and adds them to the event vector. 
/// </summary>
/// <param name="events">The event vector to which it is added.</param>
/// <param name="startTime">The beginning of the time range of the desired events</param>
/// <param name="endTime">The end of the time range of the desired events</param>
/// <param name="maxSize">The maximum size to adding events</param>
void FileHandler::readFromDiskAndAddToEvents(vector<Event>& events, const tm startTime, const tm endTime, size_t maxSize)
{
	string fileContent;
	tm tm = {};
	istringstream iss, ss, fileStream;

	//Go through the queue of the list of file names.
	for (auto it = fileNamesQueue.rbegin(); it != fileNamesQueue.rend(); ++it)
	{
		const auto& fileInfo = *it;
		
		//If the time of the last event in the file is less than the start of the desired time range,
		//we will terminate the function because there will be no more suitable files.
		if (fileInfo.endTime< startTime)
			return;
		//If the events in the file are not within the time range,
		//we will continue to the next file.
		if (endTime < fileInfo.startTime)
			continue;

		//reads the contents of the file.
		fileContent= readContentFromFile(fileInfo.fileName);

		//If the content is read correctly, 		
		//adds the relevant events to the events list.
		if(fileContent!= "")
			addRelevantEvents(fileContent, events, maxSize, startTime, endTime);
	}
}


/// <summary>
/// Writes the list of received events to the file.
/// If the list is a list accumulated in the buffer - adds the path to the queue that saves the file names.
/// </summary>
/// <param name="events">The list of events to write to the file.</param>
/// <param name="isSortedList">True if it is a sorted list of events from several drones 
/// and false if it is a normal list of events accumulated in the buffer and written to memory.</param>
/// <returns>returns the path to the file</returns>
string FileHandler::writeToFile(vector<Event> events, bool isSortedList)
{
	if (events.empty())
		return "";

	tm startTime = events.front().getTime();
	tm endTime = events.back().getTime();

	string startTimeFormatted = events.front().getFormatTime();
	string endTimeFormatted = events.back().getFormatTime();

	string fileName = writeFileName(startTimeFormatted, endTimeFormatted, isSortedList);
	
	//If we have already written a maximum possible number of files, we will overwrite the oldest one
	if (fileNamesQueue.size() >= maxFiles)
	{
		remove(fileNamesQueue.front().fileName.c_str());
		fileNamesQueue.pop_front();
	}

	ofstream outFile(fileName);

	if (!outFile) {
		cerr << "Failed to open file for writing: " << fileName << endl;
		return "";
	}

	for (auto& event : events)
	{
		outFile << event.getFormatTime() << " " << event.getMessage() << "\n";
	}
	outFile.close();
	
	//adding the file name, start and end time to the fileNamesQueue that save it.
	if(!isSortedList)
	{
		fileNamesQueue.emplace_back(fileName, startTime, endTime);
		saveMetadata();
	}
	return fileName;
}

