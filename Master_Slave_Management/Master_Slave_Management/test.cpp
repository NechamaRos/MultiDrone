#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define _CRT_SECURE_NO_WARNINGS
//
#pragma warning(disable:4996)
//
#include "doctest.h"
#include "dynamicBuffer.h"
#include "event.h"
#include "fileHandler.h"
#include "slaveEventManager.h"
#include "masterEventManager.h"
#include "mockDataManager.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> // For rand() and srand() functions
#include <cstdio> // for remove() function

using namespace std;
using namespace std::chrono;

tm addSecondsToTime(const tm& time, int secondsToAdd)// Adds seconds to the hour on the date
{
    time_t time_t_form = mktime(const_cast<tm*>(&time));
    time_t_form += secondsToAdd;
    tm* updated_time = localtime(&time_t_form);
    return *updated_time;
}

TEST_CASE("Event: getMessage")//check the string
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm current_date = *tm_now;
    Event event(current_date, "get event1");//create event
    DOCTEST_CHECK(event.getMessage() == "get event1");
}

//a function for comparing dates of type tm
bool operator==(const std::tm& lhs, const std::tm& rhs) {
    return lhs.tm_year == rhs.tm_year &&
        lhs.tm_mon == rhs.tm_mon &&
        lhs.tm_mday == rhs.tm_mday &&
        lhs.tm_hour == rhs.tm_hour &&
        lhs.tm_min == rhs.tm_min &&
        lhs.tm_sec == rhs.tm_sec;
}

TEST_CASE("Event: getDate")//check date
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm time = *tm_now;
    Event event(time, "event!");
    CHECK(event.getTime() == time);
}

TEST_CASE("Event: getDateToString") // Convert date to string
{
    time_t now = time(nullptr);  // current time
    tm* tm_now = localtime(&now);  // Conversion to tm
    tm time = *tm_now;
    tm tmi;
    Event event(time, "event!");
    istringstream ss(event.getFormatTime());
    ss >> get_time(&tmi, "%d-%m-%Y_%H.%M.%S");
    CHECK(time == tmi);
}

TEST_CASE("Event: other: getMessage")//check string I got from other
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm current_date = *tm_now;
    Event event1(current_date, "event!");
    Event event2(event1);
    DOCTEST_CHECK(event2.getMessage() == "event!");
}

TEST_CASE("Event: other: getDate")//check date I got from other
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm time = *tm_now;
    Event event1(time, "event!");
    Event event2(event1);
    CHECK(event2.getTime() == time);
}

TEST_CASE("Event::operator= when same")// check operator = when they same
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event event2(timing, "event1");
    Event b = event1 = event2;
    CHECK(event1.getMessage() == b.getMessage());
    CHECK(event1.getTime() == b.getTime());
}

TEST_CASE("Event::operator= not same")// check operator = when they not same
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    now = time(nullptr);    // current time
    tm_now = localtime(&now);    // Conversion to tm
    timing = *tm_now;
    Event event2(timing, "event2");
    Event b = event1 = event2;
    CHECK(event1.getMessage() == b.getMessage());
    CHECK(event1.getTime() == b.getTime());
}

TEST_CASE("Event::operator= sand 1 twice")// check operator = when he sent one twice
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event b = event1 = event1;
    CHECK(event1.getMessage() == b.getMessage());
    CHECK(event1.getTime() == b.getTime());
}

TEST_CASE("Event::operator== not same")// check operator == when they not same
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);// adds 2 seconds to the hour on the date
    Event event2(*tm_now, "event2");
    bool b = event1 == event2;
    CHECK(b == false);
}

TEST_CASE("Event::operator== same")// check operator == when they same
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event event2(timing, "event2");
    CHECK(event1 == event2);
}

TEST_CASE("Event::operator< when small")// check operator < when small
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);// adds 2 seconds to the hour on the date
    Event event2(timing, "event2");
    CHECK(event1 < event2);
}

TEST_CASE("Event::operator< when big")//check operator < when big
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);// adds 2 seconds to the hour on the date
    Event event2(timing, "event2");
    bool b = event2 < event1;
    CHECK(b == false);
}

TEST_CASE("Event::operator< when same")// check operator < when same
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event event2(timing, "event2");
    bool b = event1 < event2;
    CHECK(b == false);
}

TEST_CASE("Event::operator> when small")// check operator > when small
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);// adds 2 seconds to the hour on the date
    Event event2(timing, "event2");
    bool b = event1 > event2;
    CHECK(b == false);
}

TEST_CASE("Event::operator> when big")// check operator > when big
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);// adds 2 seconds to the hour on the date
    Event event2(timing, "event2");
    CHECK(event2 > event1);
}

TEST_CASE("Event::operator> when same")// check operator > when same
{
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event event2(timing, "event2");
    bool b = event1 > event2;
    CHECK(b == false);
}

TEST_CASE("DynamicBuffer: addEvent- if add to buffer, getBufferIndex, getBuffer")//Checking if he entered the correct data in the correct array
{
    int random = rand() % 10 + 1; // Generate a random number between 1 and 10
    DynamicBuffer dB(random);
    int buf1, buf2;
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    tm date;
    for (int i = 1; i <= random + 2; i++)
    {
        timing = addSecondsToTime(timing, 2);// adds 2 seconds to the hour on the date
        Event event(timing, "The event " + to_string(i));
        date = event.getTime();
        dB.addEvent(event);
        if (i == random + 1)// if i greater than the size of the buffer
        {
            buf1 = dB.getBufferIndex();
        }
        else
        {
            if (i == random)//if i equal to the size of the buffer
            {
                buf2 = dB.getBufferIndex();
            }
        }
    }
    CHECK(buf1 == 1);
    CHECK(dB.getBufferIndex() == 1);
    CHECK(buf2 == 0);
    DOCTEST_CHECK(dB.getBuffer().at(dB.getBufferIndex()).getMessage() == "The event " + to_string(random + 2));
    DOCTEST_CHECK(date == timing);
}

TEST_CASE("DynamicBuffer: addEvent- enter more the len buffer if add to other buffer")//Checking if when the place in the first array is finished, it moves to the second array
{
    int random = rand() % 10 + 1; // Generate a random number between 1 and 100
    DynamicBuffer dB(random);
    int buf1, buf2;
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    tm date;
    for (int i = 1; i <= random + 1; i++)
    {
        timing = addSecondsToTime(timing, 2);// Adding to the hour on the date 2 seconds
        Event event(timing, "The event " + to_string(i));
        date = event.getTime();
        dB.addEvent(event);
        if (i == 1)//if it is the first event
        {
            buf1 = dB.getBufferIndex();
        }
        else
        {
            if (i == random + 1)// if i greater than the size of the buffer
            {
                buf2 = dB.getBufferIndex();
            }
        }
    }
    CHECK(buf1 == 0);
    CHECK(buf2 == 1);
}

TEST_CASE("DynamicBuffer: clearAltBuffer, getAltBuffer")//Checking if the array is emptied
{
    DynamicBuffer dB(1);
    time_t now = time(nullptr);    // current time
    tm* tm_now = localtime(&now);    // Conversion to tm
    tm timing = *tm_now;
    Event e1(timing, "event1");
    dB.addEvent(e1);
    timing = addSecondsToTime(timing, 2);// Adding to the hour on the date 2 seconds
    Event e2(*tm_now, "event2");
    dB.addEvent(e2);
    dB.clearAltBuffer();
    CHECK(dB.getAltBuffer().size() == 0);
}

// function to delete a file
void RemoveTestFile(const std::string& filename) {
    if (std::remove(filename.c_str()) != 0) {
        std::perror("לא ניתן למחוק את הקובץ");
    }
}

TEST_CASE("SlaveEventManager: WriteToFile: when there are more files")//Checking if writing to a file that has not yet exceeded the maximum number of files
{
    string startDate;
    string endDate;
    int numFile = 0;
    time_point<system_clock> now = system_clock::now();
    time_t now_time_t = system_clock::to_time_t(now);
    tm* now_tm = localtime(&now_time_t);
    tm timing = *now_tm;
    vector<Event> events;
    int random = rand() % 10 + 1; // Generate a random number between 1 and 100
    for (int i = 1; i <= random + 1; i++)
    {
        timing = addSecondsToTime(timing, 2);// Adding to the hour on the date 2 seconds
        Event e(*now_tm, "event_" + to_string(i));
        if (i == 1)//if i is first event
        {
            startDate = e.getFormatTime();
        }
        else if (i == random)//if i  is last event
        {
            endDate = e.getFormatTime();
        }
        events.push_back(e);// add event to vector
    }
    SlaveEventManager SEM1(random, "SEM1_file", 2);//create slave
    for (const auto& e : events) {//add events
        SEM1.addEvent(e);
    }
    string filename = "0_SEM1_file_from_" + startDate + "_to_" + endDate + ".log";//save name file
    bool b;
    ifstream file(filename);//open file

    CHECK(file.is_open());
    string line;
    size_t index = 0;
    for (auto& e : events) {
        if (getline(file, line))
        {
            if (line != e.getFormatTime() + " " + e.getMessage())
                b = false;
        }
    }
    if (getline(file, line))
    {
        b = false;
    }
    // Close the file
    file.close();

    // Deleting the file
    RemoveTestFile(filename);
    RemoveTestFile("SEM1_file_metadata.txt");

    CHECK(b);
}

TEST_CASE("SlaveEventManager: WriteToFile: when need go to next file")//Checking if writing to the next file
{
    string startDate1;
    string endDate1;
    string startDate;
    string endDate;
    int numFile = 0;
    time_point<system_clock> now = system_clock::now();
    time_t now_time_t = system_clock::to_time_t(now);
    tm* now_tm = localtime(&now_time_t);
    tm timing = *now_tm;
    vector<Event> events1;
    vector<Event> events2;
    int random = rand() % 10 + 2; // Generate a random number between 1 and 100
    for (int i = 1; i <= random * 2 + 1; i++)
    {
        timing = addSecondsToTime(timing, 2);// Adding to the hour on the date 2 seconds
        Event e(*now_tm, "event_is_" + to_string(i));
        if (i == 1)//if it is first event in first file
        {
            startDate1 = e.getFormatTime();
        }
        else if (i == random)//if it is last event in first file
        {
            endDate1 = e.getFormatTime();
        }
        if (i <= random)//if it is for first file
            events1.push_back(e);
        else
        {
            if (i == random + 1)//if it is first event in second file
            {
                numFile = 1;
                startDate = e.getFormatTime();
            }
            else if (i == random * 2)//if it is last event in second file
                endDate = e.getFormatTime();
            events2.push_back(e);
        }
    }
    SlaveEventManager SEM1(random, "SEM1_file", 1);
    for (const auto& e : events1) {
        SEM1.addEvent(e);
    }
    for (const auto& e : events2) {
        SEM1.addEvent(e);
    }
    string filename1 = "0_SEM1_file_from_" + startDate1 + "_to_" + endDate1 + ".log";//save name first file
    string filename = "1_SEM1_file_from_" + startDate + "_to_" + endDate + ".log";//save name second file
    bool b;
    ifstream file(filename);
    CHECK(file.is_open());
    string line;
    for (auto& e : events2) {
        if (getline(file, line))
        {
            if (line != e.getFormatTime() + " " + e.getMessage())
                b = false;
        }
    }
    if (getline(file, line))
    {
        b = false;
    }

    // Close the file
    file.close();

    // Deleting the file
    RemoveTestFile(filename1);
    RemoveTestFile(filename);
    RemoveTestFile("SEM1_file_metadata.txt");

    CHECK(b);
}

TEST_CASE("SlaveEventManager: WriteToFile: when need override file")//Checking if a file is written that exceeds the maximum number of files
{
    string startDate1;
    string endDate1;
    string startDate;
    string endDate;
    int numFile = 0;
    time_point<system_clock> now = system_clock::now();
    time_t now_time_t = system_clock::to_time_t(now);
    tm* now_tm = localtime(&now_time_t);
    tm timing = *now_tm;
    vector<string> files;
    vector<Event> events1;
    vector<Event> events2;
    int random = rand() % 10 + 1; // Generate a random number between 1 and 100
    SlaveEventManager SEM1(random, "SEM1_file", 2);
    for (int i = 1; i <= random * 3 + 1; i++)
    {
        timing = addSecondsToTime(timing, 2);// Adding to the hour on the date 2 seconds
        Event e(timing, "the_event_is_" + to_string(i));
        SEM1.addEvent(e);
        if (i == random + 1)//if it is first event in second file
        {
            startDate1 = e.getFormatTime();
        }
        else if (i == random * 2)//if it is last event in second file
        {
            endDate1 = e.getFormatTime();
        }
        else
        {
            if (i == random * 2 + 1)//if it is first event in third file that override firs file
            {
                numFile = 1;
                startDate = e.getFormatTime();
            }
            else if (i == random * 3)//if it is last event in third file that override firs file
                endDate = e.getFormatTime();
        }
    }
    //save name of file
    string filename1 = "1_SEM1_file_from_" + startDate1 + "_to_" + endDate1 + ".log";
    string filename = "2_SEM1_file_from_" + startDate + "_to_" + endDate + ".log";
    string Mfilename1 = "1_SEM1_file_from_" + startDate1 + "_to_" + endDate1 + ".log " + startDate1 + " " + endDate1;
    string Mfilename = "2_SEM1_file_from_" + startDate + "_to_" + endDate + ".log " + startDate + " " + endDate;
    files.push_back(Mfilename1);
    files.push_back(Mfilename);
    bool flag;
    string s = "SEM1_file_metadata.txt";
    ifstream file(s);
    CHECK(file.is_open());
    string line;
    for (auto& e : files) {
        if (getline(file, line))
        {
            if (line != e)
                flag = false;
        }
    }
    if (getline(file, line))
    {
        flag = false;
    }

    // Close the file
    file.close();

    // Deleting the file
    RemoveTestFile(filename1);
    RemoveTestFile(filename);
    RemoveTestFile("SEM1_file_metadata.txt");

    CHECK(flag);
}

//function that get string and remove all the sapces in the string
string removeSpaces(const string& str) {
    string result;
    for (char c : str) {
        if (c != ' ' && c != '\n') {
            result += c;
        }
    }
    return result;
}

bool checkFileContent(const string& filename, const vector<string>& expectedLines) {
    ifstream file(filename);
    REQUIRE(file.is_open());

    vector<string> fileLines;
    string line;
    while (getline(file, line)) {
        fileLines.push_back(removeSpaces(line));
    }
    file.close();

    // Checking the compatibility of the number of lines
    if (fileLines.size() != expectedLines.size())
        return false;

    // Comparing the rows one by one
    for (size_t i = 0; i < expectedLines.size(); ++i) {
        if (fileLines[i] != expectedLines[i])
            return false;
    }
    return true;
}

TEST_CASE("MasterEventManager:sort: from 1 slave and 2 file")//Checking the contents of the sorted file created when one drone requested and 2 files were created
{
#undef TEST_MODE
#define TEST_MODE 1
    string startDate1;
    string endDate1;
    string startDate2;
    string endDate2;
    tm startDateTM;
    tm endDateTM;
    string end;
    size_t sumSize = 0;
    time_point<system_clock> now = system_clock::now();
    time_t now_time_t = system_clock::to_time_t(now);
    tm* now_tm = localtime(&now_time_t);
    tm timing = *now_tm;
    vector<Event> events;
    int random = rand() % 10 + 1; // Generate a random number between 1 and 100
    for (int i = 1; i <= random * 2 + 1; i++)// adding events
    {
        timing = addSecondsToTime(timing, 2);// Adding to the hour on the date 2 seconds
        Event e(timing, "The_Event_" + to_string(i));// create new event
        if (i == 1)// if this is first event in the first event's file
        {
            startDate1 = e.getFormatTime();
            startDateTM = e.getTime();
        }
        else if (i == random)// If this is the last event in the first event's file
        {
            endDate1 = e.getFormatTime();
        }
        else if (i == random + 1)// if this is first event in the second event's file
        {
            startDate2 = e.getFormatTime();
        }
        else if (i == random * 2)// if this is last event in the second event's file
        {
            endDate2 = e.getFormatTime();
        }
        else if (i == random * 2 + 1)// if this is last event in the sort file
        {
            endDateTM = e.getTime();
            end = e.getFormatTime();
        }
        sumSize += e.getSize();
        events.push_back(e);
    }
    SlaveEventManager SEM1(random, "SEM1_file", 2);//create slave
    for (const auto& e : events) {// adding events to slave
        SEM1.addEvent(e);
    }
    string filename1 = "0_SEM1_file_from_" + startDate1 + "_to_" + endDate1 + ".log";// save name file1
    string filename2 = "1_SEM1_file_from_" + startDate2 + "_to_" + endDate2 + ".log";// save name file2

    MasterEventManager master;

    mockDataManager.slaveManagers[1] = &SEM1;

    mockDataManager.masterManager = master;

    vector<int> slaveIds = { 1 };
    tm startTime = startDateTM;
    tm endTime = endDateTM;
    size_t sizeInBytes = sumSize;

    mockDataManager.masterManager.sendReqToEventListFromSlaves(slaveIds, startTime, endTime, sizeInBytes);// Sending a request from the master
    for (const auto& entry : mockDataManager.receivedMessages) {
        const auto& slaveId = entry.first;
        const auto& messages = entry.second;

        for (Message* message : messages) {
            mockSendMessage(message);
        }
    }

    vector<string> eventsInString;
    for (Event event : events)
    {
        eventsInString.push_back(removeSpaces(event.getFormatTime() + "  " + event.getMessage()));
    }
    bool isContentFileTrue = checkFileContent(mockDataManager.sortedListFileName, eventsInString);

    CHECK(isContentFileTrue == true);

    //Deleting all the files created in the test
    RemoveTestFile(filename1);
    RemoveTestFile(filename2);
    RemoveTestFile("SEM1_file_metadata.txt");
    RemoveTestFile("sorted_event_list_by_time_range_from_" + startDate1 + "_to_" + end + ".log");
}

TEST_CASE("MasterEventManager:sort: from 2 slave")//Checking if you sorted what you got from two slaves
{
#undef TEST_MODE
#define TEST_MODE 1

    string fileName1;
    string fileName2;
    string startDate1;
    string startDate2;
    string endDate1;
    string endDate2;
    tm startDateTM;
    tm endDateTM;
    string end;
    size_t sumSize = 0;
    time_point<system_clock> now = system_clock::now();
    time_t now_time_t = system_clock::to_time_t(now);
    tm* now_tm = localtime(&now_time_t);
    tm timing = *now_tm;
    vector<Event> events1;
    vector<Event> events2;
    vector<Event> events;
    int random = (rand() % 10 + 1) * 2; // Generate a random number between 1 and 100
    for (int i = 1; i <= random + 2; i++)//adding events
    {
        timing = addSecondsToTime(timing, 2);// Adding to the hour on the date 2 seconds
        Event e(timing, "the_event_" + to_string(i));
        if (i == 1)//if it is first event
        {
            startDate1 = e.getFormatTime();
            startDateTM = e.getTime();
        }
        else if (i == random)//if it is last event
        {
            endDate2 = e.getFormatTime();
        }
        else if (i == random + 2)//if it is last event in sort file
        {
            endDateTM = e.getTime();
            end = e.getFormatTime();
        }
        if (i % 2 == 0)// if i is double
        {
            if (i == 2)// if it is the first event in slave2
                startDate2 = e.getFormatTime();
            events2.push_back(e);
        }
        else
        {
            if (i == random - 1)//if it is last event in slave1
                endDate1 = e.getFormatTime();
            events1.push_back(e);
        }
        events.push_back(e);
        sumSize += e.getSize();
    }

    //Setting up the first slave and adding events 
    SlaveEventManager slave1(random / 2, "SEM1_file", 2);
    for (const auto& e : events1) {
        slave1.addEvent(e);
    }
    fileName1 = "0_SEM1_file_from_" + startDate1 + "_to_" + endDate1 + ".log";//save file name

    //Setting up the second slave and adding events
    SlaveEventManager slave2(random / 2, "SEM2_file", 2);
    for (const auto& e : events2) {
        slave2.addEvent(e);
    }
    fileName2 = "0_SEM2_file_from_" + startDate2 + "_to_" + endDate2 + ".log";//save file name

    MasterEventManager master;

    mockDataManager.slaveManagers[1] = &slave1;
    mockDataManager.slaveManagers[2] = &slave2;

    mockDataManager.masterManager = master;


    vector<int> slaveIds = { 1,2 };
    tm startTime = startDateTM;
    tm endTime = endDateTM;
    size_t sizeInBytes = sumSize;

    mockDataManager.masterManager.sendReqToEventListFromSlaves(slaveIds, startTime, endTime, sizeInBytes);//sand equest from manager
    for (const auto& entry : mockDataManager.receivedMessages) {
        const auto& slaveId = entry.first;
        const auto& messages = entry.second;

        for (Message* message : messages) {
            mockSendMessage(message);
        }
    }

    vector<string> eventsInString;
    for (Event event : events)
    {
        eventsInString.push_back(removeSpaces(event.getFormatTime() + "  " + event.getMessage()));
    }
    bool isContentFileTrue = checkFileContent(mockDataManager.sortedListFileName, eventsInString);

    CHECK(isContentFileTrue == true);

    //Deleting all the files created in the test
    RemoveTestFile(fileName1);
    RemoveTestFile(fileName2);
    RemoveTestFile("SEM1_file_metadata.txt");
    RemoveTestFile("SEM2_file_metadata.txt");
    RemoveTestFile("sorted_event_list_by_time_range_from_" + startDate1 + "_to_" + end + ".log");
}

TEST_CASE("MasterEventManager:sort: from 2 slave When the number of events requested is less than the number of events created")//Checking if the sorting file contains only some of the events generated according to the request
{
#undef TEST_MODE
#define TEST_MODE 1

    string fileName1;
    string fileName2;
    string startDate1;
    string startDate2;
    string endDate1;
    string endDate2;
    tm startDateTM;
    tm endDateTM;
    string start;
    string end;
    size_t sumSize = 0;
    time_point<system_clock> now = system_clock::now();
    time_t now_time_t = system_clock::to_time_t(now);
    tm* now_tm = localtime(&now_time_t);
    tm timing = *now_tm;
    vector<Event> events1;
    vector<Event> events2;
    vector<Event> events;
    int random = (rand() % 10 + 1) * 2; // Generate a random number between 1 and 100
    for (int i = 1; i <= random + 2; i++)//adding events
    {
        timing = addSecondsToTime(timing, 2);// Adding to the hour on the date 2 seconds
        Event e(timing, "the_event_" + to_string(i));
        if (i == 1)//if it is first event
        {
            startDate1 = e.getFormatTime();
            startDateTM = e.getTime();
        }
        else if (i == 3)//if it is last event in sort file
        {
            start = e.getFormatTime();
        }
        else if (i == random)//if it is last event
        {
            endDate2 = e.getFormatTime();
        }
        else if (i == random + 2)//if it is last event in sort file
        {
            endDateTM = e.getTime();
            end = e.getFormatTime();
        }
        if (i % 2 == 0)// if i is double
        {
            if (i == 2)// if it is the first event in slave2
                startDate2 = e.getFormatTime();
            events2.push_back(e);
        }
        else
        {
            if (i == random - 1)//if it is last event in slave1
                endDate1 = e.getFormatTime();
            events1.push_back(e);
        }
        events.push_back(e);
        sumSize += e.getSize();
    }

    //Setting up the first slave and adding events 
    SlaveEventManager slave1(random / 2, "SEM1_file", 2);
    for (const auto& e : events1) {
        slave1.addEvent(e);
    }
    fileName1 = "0_SEM1_file_from_" + startDate1 + "_to_" + endDate1 + ".log";//save file name

    //Setting up the second slave and adding events
    SlaveEventManager slave2(random / 2, "SEM2_file", 2);
    for (const auto& e : events2) {
        slave2.addEvent(e);
    }
    fileName2 = "0_SEM2_file_from_" + startDate2 + "_to_" + endDate2 + ".log";//save file name

    MasterEventManager master;

    mockDataManager.slaveManagers[1] = &slave1;
    mockDataManager.slaveManagers[2] = &slave2;

    mockDataManager.masterManager = master;


    vector<int> slaveIds = { 1,2 };
    tm startTime = startDateTM;
    tm endTime = endDateTM;
    size_t sizeInBytes = sumSize - 64 * 2;

    mockDataManager.masterManager.sendReqToEventListFromSlaves(slaveIds, startTime, endTime, sizeInBytes);//sand equest from manager
    for (const auto& entry : mockDataManager.receivedMessages) {
        const auto& slaveId = entry.first;
        const auto& messages = entry.second;

        for (Message* message : messages) {
            mockSendMessage(message);
        }
    }

    vector<string> eventsInString;
    size_t numEvents = events.size();
    for (size_t i = 2; i < numEvents; ++i) {
        Event event = events[i];
        eventsInString.push_back(removeSpaces(event.getFormatTime() + "  " + event.getMessage()));
    }
    bool isContentFileTrue = checkFileContent(mockDataManager.sortedListFileName, eventsInString);

    CHECK(isContentFileTrue == true);

    //Deleting all the files created in the test
    RemoveTestFile(fileName1);
    RemoveTestFile(fileName2);
    RemoveTestFile("SEM1_file_metadata.txt");
    RemoveTestFile("SEM2_file_metadata.txt");
    RemoveTestFile("sorted_event_list_by_time_range_from_" + start + "_to_" + end + ".log");
}

TEST_CASE("MasterEventManager:sort: from random number of slaves") // Test with a random number of slaves
{
#undef TEST_MODE
#define TEST_MODE 1

    string fileNamePrefix = "SEM_file_"; // Prefix for filenames for slaves
    vector<SlaveEventManager> slaveManagers; // List to store slaves
    vector<string> fileNames; // List for filenames for slaves
    vector<string> fileMetadata; // List for filenames for slaves
    vector<Event> allEvents; // List of all events from slaves
    size_t sumSize = 0; // Variable to store the total size of all events

    int numSlaves = (rand() % 5) + 1; // Random number of slaves between 1 and 5

    // Define start time
    time_point<system_clock> now = system_clock::now(); // Current time
    time_t now_time_t = system_clock::to_time_t(now); // Convert time to time_t
    tm* now_tm = localtime(&now_time_t); // Convert time to tm structure
    tm timing = *now_tm; // Copy time to timing variable

    // Create slaves and events for each slave
    for (int i = 1; i <= numSlaves; ++i) {
        vector<Event> events; // List of events for the current slave
        int numEvents = (rand() % 10) + 1; // Random number of events between 1 and 10
        string startDate; // Start date
        string endDate; // End date
        tm startDateTM; // Start date in tm structure
        tm endDateTM; // End date in tm structure
        size_t slaveSize = 0; // Size of all events for the current slave

        // Add events to the list for the current slave
        for (int j = 1; j <= numEvents + 1; ++j) {
            timing = addSecondsToTime(timing, 2);// Add 2 seconds to the current time
            Event e(timing, "event_" + to_string(j)); // Create a new event
            if (j == 1) {
                startDate = e.getFormatTime(); // Set start date if this is the first event
                startDateTM = e.getTime(); // Set start date in tm structure
            }
            if (j == numEvents) {
                endDate = e.getFormatTime(); // Set end date if this is the last event
            }
            else if (j == numEvents + 1)
            {
                endDateTM = e.getTime(); // Set end date in tm structure
            }
            events.push_back(e); // Add event to the list
            allEvents.push_back(e); // Add event to the overall list
            slaveSize += e.getSize(); // Add event size to the total size
        }
        int maxFiles = (std::rand() % 5) + 1; // Random number of max files between 1 and 5

        // Create a new slave and add events to it
        SlaveEventManager slave(numEvents, fileNamePrefix + to_string(i), maxFiles);
        for (const auto& e : events) {
            slave.addEvent(e);
        }
        slaveManagers.push_back(slave); // Add the slave to the list of slaves
        fileNames.push_back("0_" + fileNamePrefix + to_string(i) + "_from_" + startDate + "_to_" + endDate + ".log"); // Create filename for the slave
        fileMetadata.push_back(fileNamePrefix + to_string(i) + "_metadata.txt");
        if (slaveSize > sumSize)
            sumSize = slaveSize; // Add the slave's events size to the total size
    }

    // Create a master manager
    MasterEventManager master;

    // Set slaves in the mock data manager
    for (int i = 0; i < numSlaves; ++i) {
        mockDataManager.slaveManagers[i + 1] = &slaveManagers[i];
    }
    mockDataManager.masterManager = master; // Set master manager in the mock data manager

    vector<int> slaveIds; // List to hold slave IDs
    for (int i = 1; i <= numSlaves; ++i) {
        slaveIds.push_back(i); // Add slave IDs to the list
    }

    tm startTime = allEvents.front().getTime(); // Set start time from the first event
    tm endTime = allEvents.back().getTime(); // Set end time from the last event
    size_t sizeInBytes = sumSize * numSlaves; // Set total size of all events

    // Request the master to get the event list from the slaves
    mockDataManager.masterManager.sendReqToEventListFromSlaves(slaveIds, startTime, endTime, sizeInBytes);

    // Process incoming messages
    for (const auto& entry : mockDataManager.receivedMessages) {
        const auto& slaveId = entry.first; // Slave ID
        const auto& messages = entry.second; // List of messages

        for (Message* message : messages) {
            mockSendMessage(message); // Send message
        }
    }

    // Create a list of events in string format
    vector<string> eventsInString;
    for (Event event : allEvents) {
        eventsInString.push_back(removeSpaces(event.getFormatTime() + "  " + event.getMessage())); // Create a string with the event time and message
    }

    // Check if the content of the sorted file matches the expected events
    bool isContentFileTrue = checkFileContent(mockDataManager.sortedListFileName, eventsInString);
    CHECK(isContentFileTrue == true); // Verify the result

    // Delete files created during the test
    for (const auto& fileName : fileNames) {
        RemoveTestFile(fileName);
    }
    for (const auto& fileName : fileMetadata) {
        RemoveTestFile(fileName);
    }
    RemoveTestFile("sorted_event_list_by_time_range_from_" + allEvents.front().getFormatTime() + "_to_" + allEvents.back().getFormatTime() + ".log");
}