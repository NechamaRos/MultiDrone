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
#include <cstdio> // לצורך שימוש בפונקציה remove

using namespace std;
using namespace std::chrono;

tm addSecondsToTime(const tm& time, int secondsToAdd)// פונקציה שמוסיפה שניות לשעה בתאריך
{
    time_t time_t_form = mktime(const_cast<tm*>(&time));
    time_t_form += secondsToAdd;
    tm* updated_time = localtime(&time_t_form);
    return *updated_time;
}

TEST_CASE("Event: getMessage")//אם קיבל את המחרות טוב
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    // יצירת משתנה tm חדש שיכיל את התאריך הנוכחי
    tm current_date = *tm_now; // שעתיים שעתיים, בלי הקשיחות של שעה
    Event event(current_date, "get event1");//יצירת אירוע
    DOCTEST_CHECK(event.getMessage() == "get event1");
}

// פונקציה להשוואת תאריכים מסוג tm
bool operator==(const std::tm& lhs, const std::tm& rhs) {
    return lhs.tm_year == rhs.tm_year &&
        lhs.tm_mon == rhs.tm_mon &&
        lhs.tm_mday == rhs.tm_mday &&
        lhs.tm_hour == rhs.tm_hour &&
        lhs.tm_min == rhs.tm_min &&
        lhs.tm_sec == rhs.tm_sec;
}

TEST_CASE("Event: getDate")//אם קיבל את התאריך טוב
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm time = *tm_now;
    Event event(time, "event!");
    CHECK(event.getTime() == time);
}

TEST_CASE("Event: getDateToString")//אם קיבל את התאריך טוב והמיר למחרוזת טוב
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm time = *tm_now;
    tm tmi;
    Event event(time, "event!");
    istringstream ss(event.getFormatTime());
    ss >> get_time(&tmi, "%d-%m-%Y_%H.%M.%S");
    CHECK(time == tmi);
}

TEST_CASE("Event: other: getMessage")//other אם קיבל את המחרות טוב שמקבל 
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    // יצירת משתנה tm חדש שיכיל את התאריך הנוכחי
    tm current_date = *tm_now; // שעתיים שעתיים, בלי הקשיחות של שעה
    Event event1(current_date, "event!");
    Event event2(event1);
    DOCTEST_CHECK(event2.getMessage() == "event!");
}

TEST_CASE("Event: other: getDate")//other אם קיבל את התאריך טוב שמקבל
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm time = *tm_now;
    Event event1(time, "event!");
    Event event2(event1);
    CHECK(event2.getTime() == time);
}

TEST_CASE("Event::operator= same")// בדיקה אם זהים
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event event2(timing, "event1");
    Event b = event1 = event2;
    CHECK(event1.getMessage() == b.getMessage());
    CHECK(event1.getTime() == b.getTime());
}

TEST_CASE("Event::operator= not same")// בדיקה אם לא זהים
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    now = time(nullptr);    // קבלת הזמן הנוכחי
    tm_now = localtime(&now);    // המרת הזמן למבנה tm
    timing = *tm_now;
    Event event2(timing, "event2");
    Event b = event1 = event2;
    CHECK(event1.getMessage() == b.getMessage());
    CHECK(event1.getTime() == b.getTime());
}

TEST_CASE("Event::operator= sand 1 twice")// בדיקה אם שלח את אותו אחד
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event b = event1 = event1;
    CHECK(event1.getMessage() == b.getMessage());
    CHECK(event1.getTime() == b.getTime());
}

TEST_CASE("Event::operator== not same")// בדיקה אם שלח את אותו אחד
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);// הסספת 2 שניות לשעה בתאריך
    Event event2(*tm_now, "event2");
    bool b = event1 == event2;
    CHECK(b == false);
}

TEST_CASE("Event::operator== realy same")// בדיקה אם שלח את אותו אחד
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event event2(timing, "event2");
    CHECK(event1 == event2);
}

TEST_CASE("Event::operator< if small")// בדיקה אם שלח את אותו אחד
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);
    Event event2(timing, "event2");
    CHECK(event1 < event2);
}

TEST_CASE("Event::operator< if big")// בדיקה אם שלח את אותו אחד
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);
    Event event2(timing, "event2");
    bool b = event2 < event1;
    CHECK(b == false);
}

TEST_CASE("Event::operator< if same")// בדיקה אם שלח את אותו אחד
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event event2(timing, "event2");
    bool b = event1 < event2;
    CHECK(b == false);
}

TEST_CASE("Event::operator> if small")// בדיקה אם שלח את אותו אחד
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);
    Event event2(timing, "event2");
    bool b = event1 > event2;
    CHECK(b == false);
}

TEST_CASE("Event::operator> if big")// בדיקה אם שלח את אותו אחד
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    timing = addSecondsToTime(timing, 2);
    Event event2(timing, "event2");
    CHECK(event2 > event1);
}

TEST_CASE("Event::operator> if same")// בדיקה אם שלח את אותו אחד
{
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    Event event1(timing, "event1");
    Event event2(timing, "event2");
    bool b = event1 > event2;
    CHECK(b == false);
}

TEST_CASE("DynamicBuffer: addEvent- if add to buffer, getBufferIndex, getBuffer")//בדיקה אם הכניס למערך הנכון את הנתונם הנכונים
{
    int random = rand() % 10 + 1; // Generate a random number between 1 and 100
    DynamicBuffer dB(random);
    int buf1, buf2;
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *tm_now;
    tm date;
    for (int i = 1; i <= random + 2; i++)
    {
        timing = addSecondsToTime(timing, 2);// הוספת 2 שניות לשעה בתאריך
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

TEST_CASE("DynamicBuffer: addEvent- enter more the len buffer if add to other buffer")//בדיקה אם כשנגמר המקום במערך עובר למערך השני
{
    int random = rand() % 10 + 1; // Generate a random number between 1 and 100
    DynamicBuffer dB(random);
    int buf1, buf2;
    time_t now = time(nullptr);   // קבלת הזמן הנוכחי
    tm* tm_now = localtime(&now);    // המרת הזמן למבנה tm
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

TEST_CASE("DynamicBuffer: clearAltBuffer, getAltBuffer")//בדיקה אם מרוקן את המערך
{
    DynamicBuffer dB(1);
    time_t now = time(nullptr);    // קבלת הזמן הנוכחי
    tm* now_tm = localtime(&now);    // המרת הזמן למבנה tm
    tm timing = *now_tm;
    Event e1(timing, "event1");
    dB.addEvent(e1);
    timing = addSecondsToTime(timing, 2);// Adding to the hour on the date 2 seconds
    Event e2(*now_tm, "event2");
    dB.addEvent(e2);
    dB.clearAltBuffer();
    CHECK(dB.getAltBuffer().size() == 0);
}

// פונקציה למחיקת קובץ
void RemoveTestFile(const std::string& filename) {
    if (std::remove(filename.c_str()) != 0) {
        std::perror("לא ניתן למחוק את הקובץ");
    }
}

TEST_CASE("SlaveEventManager: WriteToFile: when have palce")//בדיקה אם כותב לקובץ שעדיין לא עבר את מ"ס הקבצים המקסימלי
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
    // סגירת הקובץ
    file.close();

    // מחיקת הקובץ
    RemoveTestFile(filename);
    RemoveTestFile("SEM1_file_metadata.txt");

    CHECK(b);
}

TEST_CASE("SlaveEventManager: WriteToFile: when need go to next file")//בדיקה אם כותב לקובץ הבא
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

    // סגירת הקובץ
    file.close();

    // מחיקת הקובץ
    RemoveTestFile(filename1);
    RemoveTestFile(filename);
    RemoveTestFile("SEM1_file_metadata.txt");

    CHECK(b);
}

TEST_CASE("SlaveEventManager: WriteToFile: when need override file")//בדיקה אם כותב לקובץ כשעבר את מ"ס הקבצים המקסימלי
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

    // סגירת הקובץ
    file.close();

    // מחיקת הקובץ
    RemoveTestFile(filename1);
    RemoveTestFile(filename);
    RemoveTestFile("SEM1_file_metadata.txt");

    CHECK(flag);
}

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

TEST_CASE("MasterEventManager:sort: from 1 slave")//בדיקה אם מיין את מה בקיבל מרחפן אחד
{
#undef TEST_MODE
#define TEST_MODE 1
    string startDate;
    string endDate;
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
    for (int i = 1; i <= random + 1; i++)// הוספת ארועים
    {
        timing = addSecondsToTime(timing, 2);// שינוי השעה בתאריך לעוד 2 שניות
        Event e(timing, "The_Event_" + to_string(i));// יצירת אירוע חדש
        if (i == 1)// אם זה האירוע הראשון
        {
            startDate = e.getFormatTime();
            startDateTM = e.getTime();
        }
        else if (i == random)// אם זה האירוע האחרון בקובץ של הרחפן
        {
            endDate = e.getFormatTime();
        }
        else if (i == random + 1)// אם זה האירוע בקובץ המיון
        {
            endDateTM = e.getTime();
            end = e.getFormatTime();
        }
        sumSize += e.getSize();
        events.push_back(e);
    }
    SlaveEventManager SEM1(random, "SEM1_file", 2);//יצירת רחפן
    for (const auto& e : events) {// הכנסת אירועים
        SEM1.addEvent(e);
    }
    string filename = "0_SEM1_file_from_" + startDate + "_to_" + endDate + ".log";// שמירת שם הקובץ

    MasterEventManager master;

    mockDataManager.slaveManagers[1] = &SEM1;

    mockDataManager.masterManager = master;

    vector<int> slaveIds = { 1 };
    tm startTime = startDateTM;
    tm endTime = endDateTM;
    size_t sizeInBytes = sumSize;

    mockDataManager.masterManager.sendReqToEventListFromSlaves(slaveIds, startTime, endTime, sizeInBytes);// שליחת בקשה מהמאסטר
    for (const auto& entry : mockDataManager.receivedMessages) {
        const auto& slaveId = entry.first; // המפתח של המיפוי
        const auto& messages = entry.second; // הערך המקושר למפתח

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

    //מחיקת כל הקבצים שנוצרו בטסט
    RemoveTestFile(filename);
    RemoveTestFile("SEM1_file_metadata.txt");
    RemoveTestFile("sorted_event_list_by_time_range_from_" + startDate + "_to_" + end + ".log");
}

TEST_CASE("MasterEventManager:sort: from 2 slave")//בדיקה אם מיין את מה בקיבל משתי רחפנים
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

    //הגדרת הרחפן הראשון והוספת אירועים 
    SlaveEventManager slave1(random / 2, "SEM1_file", 2);
    for (const auto& e : events1) {
        slave1.addEvent(e);
    }
    fileName1 = "0_SEM1_file_from_" + startDate1 + "_to_" + endDate1 + ".log";

    //הגדרת הרחפן השני והוספת אירועים
    SlaveEventManager slave2(random / 2, "SEM2_file", 2);
    for (const auto& e : events2) {
        slave2.addEvent(e);
    }
    fileName2 = "0_SEM2_file_from_" + startDate2 + "_to_" + endDate2 + ".log";

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
        const auto& slaveId = entry.first; // המפתח של המיפוי
        const auto& messages = entry.second; // הערך המקושר למפתח

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

    // מחיקת הקובץ
    RemoveTestFile(fileName1);
    RemoveTestFile(fileName2);
    RemoveTestFile("SEM1_file_metadata.txt");
    RemoveTestFile("SEM2_file_metadata.txt");
    RemoveTestFile("sorted_event_list_by_time_range_from_" + startDate1 + "_to_" + end + ".log");
}

TEST_CASE("MasterEventManager:sort: from random number of slaves") // בדיקה עם מספר רנדומלי של רחפנים
{
#undef TEST_MODE
#define TEST_MODE 1

    string fileNamePrefix = "SEM_file_"; // קידומת לשמות הקבצים עבור הרחפנים
    vector<SlaveEventManager> slaveManagers; // רשימה לשמירת הרחפנים
    vector<string> fileNames; // רשימה לשמות הקבצים עבור הרחפנים
    vector<Event> allEvents; // רשימה לכל האירועים מהרחפנים
    size_t sumSize = 0; // משתנה לשמירת סך הגודל של כל האירועים

    int numSlaves = (rand() % 5) + 1; // מספר רנדומלי של רחפנים בין 1 ל-5

    // הגדרת זמן התחלתי
    time_point<system_clock> now = system_clock::now(); // זמן נוכחי
    time_t now_time_t = system_clock::to_time_t(now); // המרת הזמן לנקודת זמן מסוג time_t
    tm* now_tm = localtime(&now_time_t); // המרת הזמן למבנה tm
    tm timing = *now_tm; // העתקת הזמן למשתנה timing

    // יצירת רחפנים ואירועים עבור כל רחפן
    for (int i = 1; i <= numSlaves; ++i) {
        vector<Event> events; // רשימה לאירועים עבור הרחפן הנוכחי
        int numEvents = (rand() % 10) + 1; // מספר רנדומלי של אירועים בין 1 ל-10
        string startDate; // תאריך התחלה
        string endDate; // תאריך סיום
        tm startDateTM; // תאריך התחלה במבנה tm
        tm endDateTM; // תאריך סיום במבנה tm
        size_t slaveSize = 0; // גודל סך האירועים של הרחפן הנוכחי

        // הוספת אירועים לרשימה עבור הרחפן הנוכחי
        for (int j = 1; j <= numEvents + 1; ++j) {
            timing = addSecondsToTime(timing, 2); // הוספת 2 שניות לזמן הנוכחי
            Event e(timing, "event_" + to_string(j)); // יצירת אירוע חדש
            if (j == 1) {
                startDate = e.getFormatTime(); // הגדרת תאריך התחלה אם זה האירוע הראשון
                startDateTM = e.getTime(); // הגדרת תאריך התחלה במבנה tm
            }
            if (j == numEvents) {
                endDate = e.getFormatTime(); // הגדרת תאריך סיום אם זה האירוע האחרון
            }
            else if (j = numEvents + 1)
            {
                endDateTM = e.getTime(); // הגדרת תאריך סיום במבנה tm
            }
            events.push_back(e); // הוספת האירוע לרשימה
            allEvents.push_back(e); // הוספת האירוע לרשימה הכללית
            slaveSize += e.getSize(); // הוספת גודל האירוע לסך הכולל
        }

        // יצירת רחפן חדש והוספת האירועים אליו
        SlaveEventManager slave(numEvents, fileNamePrefix + to_string(i), 2);
        for (const auto& e : events) {
            slave.addEvent(e);
        }
        slaveManagers.push_back(slave); // הוספת הרחפן לרשימה של הרחפנים
        fileNames.push_back(fileNamePrefix + to_string(i) + "_from_" + startDate + "_to_" + endDate + ".log"); // יצירת שם קובץ עבור הרחפן
        sumSize += slaveSize; // הוספת גודל האירועים של הרחפן לסך הכולל
    }

    // יצירת מאסטר ניהולי
    MasterEventManager master;

    // הגדרת הרחפנים במאגר המידע המזויף
    for (int i = 0; i < numSlaves; ++i) {
        mockDataManager.slaveManagers[i + 1] = &slaveManagers[i];
    }
    mockDataManager.masterManager = master; // הגדרת מאסטר ניהולי במאגר המידע המזויף

    vector<int> slaveIds; // רשימה להכנסת מזהי הרחפנים
    for (int i = 1; i <= numSlaves; ++i) {
        slaveIds.push_back(i); // הוספת מזהי הרחפנים לרשימה
    }

    tm startTime = allEvents.front().getTime(); // קביעת זמן התחלה מהאירוע הראשון
    tm endTime = allEvents.back().getTime(); // קביעת זמן סיום מהאירוע האחרון
    size_t sizeInBytes = sumSize; // קביעת סך הגודל של כל האירועים

    // שליחת בקשה למאסטר לקבלת רשימת האירועים מהרחפנים
    mockDataManager.masterManager.sendReqToEventListFromSlaves(slaveIds, startTime, endTime, sizeInBytes);

    // טיפול בהודעות המתקבלות
    for (const auto& entry : mockDataManager.receivedMessages) {
        const auto& slaveId = entry.first; // מזהה הרחפן
        const auto& messages = entry.second; // רשימת ההודעות

        for (Message* message : messages) {
            mockSendMessage(message); // שליחת הודעה
        }
    }

    // יצירת רשימה של האירועים בפורמט מחרוזת
    vector<string> eventsInString;
    for (Event event : allEvents) {
        eventsInString.push_back(removeSpaces(event.getFormatTime() + "  " + event.getMessage())); // יצירת מחרוזת עם הזמן וההודעה של כל אירוע
    }

    // בדיקת אם התוכן של הקובץ המסודר תואם לאירועים שציפינו להם
    bool isContentFileTrue = checkFileContent(mockDataManager.sortedListFileName, eventsInString);
    CHECK(isContentFileTrue == true); // בדיקת תוצאה

    // מחיקת הקבצים שנוצרו במהלך הבדיקה
    for (const auto& fileName : fileNames) {
        RemoveTestFile(fileName);
    }
    RemoveTestFile("sorted_event_list_by_time_range_from_" + allEvents.front().getFormatTime() + "_to_" + allEvents.back().getFormatTime() + ".log");
}
