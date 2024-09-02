#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include "slaveEventManager.h"
#include "masterEventManager.h"

using namespace std;
using namespace std::chrono;






static deque<FileInfo> fileQueue;
static int current = 0;

void print(vector<Event> events)
{
	cout << "Events_list:" << endl;
	for (auto& event : events) {
		cout << event.toString() << endl;
	}
	cout << "-------------------------" << endl;
}

#include "masterEventManager.h"



//int main()
//{
//	//// יצירת זמן נוכחי
//	time_point<system_clock> now = system_clock::now();
//
//
//	time_t now_time_t = system_clock::to_time_t(now);
//	tm* now_tm = localtime(&now_time_t);
//	tm now_tm_copy = *now_tm;
//
//	if (now_tm != nullptr) {
//		cout << "Current time: " << put_time(now_tm, "%d-%m-%Y_%H.%M.%S") << endl;
//	}
//	Event e1(*now_tm, "event1");
//
//	now_tm->tm_sec += 2;
//	Event e2(*now_tm, "event2");
//
//	now_tm->tm_sec += 2;
//	Event e3(*now_tm, "event3");
//
//
//	now_tm->tm_sec += 1;
//	Event e4(*now_tm, "event4");
//
//	now_tm->tm_sec += 1;
//	Event e5(*now_tm, "event5");
//
//	now_tm->tm_sec += 1;
//	Event e6(*now_tm, "event6");
//
//	now_tm->tm_sec += 1;
//	Event e7(*now_tm, "event7");
//
//	now_tm->tm_sec += 1;
//	Event e8(*now_tm, "event8");
//
//	now_tm->tm_sec += 1;
//	Event e9(*now_tm, "event9");
//
//	now_tm->tm_sec += 2;
//	Event e10(*now_tm, "event10");
//
//	now_tm->tm_sec += 2;
//	Event e11(*now_tm, "event11");
//
//	vector<Event> events = {
//		e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11
//	};
//
//
//
//	size_t totalSize = 0;
//	for (auto& event : events) {
//		totalSize += event.getSize();
//	}
//	cout << "total_size: " << totalSize << endl;
//
//
//	time_t now_seconds1 = mktime(&now_tm_copy);
//	time_t now_seconds2 = mktime(&now_tm_copy);
//	now_seconds1 += 100;
//	now_seconds2 += 120;
//
//	//הגדרת הרחפן הראשון והוספת אירועים 
//	SlaveEventManager SEM1(3, "SEM1_file", 2);
//	SEM1.addEvent(e1);
//	SEM1.addEvent(e2);
//	SEM1.addEvent(e5);
//	SEM1.addEvent(e6);
//	SEM1.addEvent(e7);
//	SEM1.addEvent(e8);
//
//
//
//	//הגדרת הרחפן השני והוספת אירועים
//	SlaveEventManager SEM2(2, "SEM2_file", 2);
//	SEM2.addEvent(e3);
//	SEM2.addEvent(e4);
//	SEM2.addEvent(e9);
//	SEM2.addEvent(e10);
//
//
//	vector<Event> vEvents;
//	now_seconds1 -= 80;
//
//
//	MasterEventManager MEM;
//
//	vector<int> v ;
//	v.push_back(SEM1.getSlaveId());
//	v.push_back(SEM2.getSlaveId());
//	vector<Message> messages = MEM.sendReqToEventListFromSlaves(v, now_tm_copy, *(localtime(&(now_seconds2))), 900);
//	SEM1.getEvents(&messages[0]);
//	SEM1.getEvents(&messages[1]);
//	//vEvents = MEM.sendReqToEventListFromSlaves(v, now_tm_copy, *(localtime(&(now_seconds2))), 500);
//	//print(vEvents);
//}
