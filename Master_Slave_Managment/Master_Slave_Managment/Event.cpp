#include "event.h"
using namespace std;

//compare times
bool Event::compareTimes(const tm& tm1, const tm& tm2)
{
    if (tm1.tm_year != tm2.tm_year) return tm1.tm_year < tm2.tm_year;
    if (tm1.tm_mon != tm2.tm_mon) return tm1.tm_mon < tm2.tm_mon;
    if (tm1.tm_mday != tm2.tm_mday) return tm1.tm_mday < tm2.tm_mday;
    if (tm1.tm_hour != tm2.tm_hour) return tm1.tm_hour < tm2.tm_hour;
    if (tm1.tm_min != tm2.tm_min) return tm1.tm_min < tm2.tm_min;
    return tm1.tm_sec < tm2.tm_sec;
}

Event::Event()
{
}

Event::Event(string message)
	: message(message)
{
    time_point<system_clock> now = system_clock::now();
    time_t now_time_t = system_clock::to_time_t(now);
    time = *(localtime(&now_time_t));
}

Event::Event(tm time, string message)
    :time(time),message(message)
{
}

Event::Event(const Event& other)
    : time(other.time), message(other.message)
{
}

Event& Event::operator=(const Event& other)
{
    if (this != &other)
    {
        time = other.time;
        message = other.message;
    }
    return *this;
}

bool Event::operator<(const Event& other) const
{
    return compareTimes(time, other.time);
}

bool Event::operator<(const tm& otherTime) const
{
    return compareTimes(time,otherTime);
}

bool Event::operator>(const Event& other) const
{
    return other < *this;
}

bool Event::operator>(const tm& otherTime) const
{
    return compareTimes(otherTime,time);
}

bool Event::operator==(const Event& other) const
{
    return !compareTimes(time, other.time) && !compareTimes(other.time, time);
}

bool Event::operator==(const tm& otherTime) const
{
    return !compareTimes(time,otherTime) && !compareTimes(otherTime,time);
}

bool Event::operator<=(const tm& otherTime) const
{
    return !(*this > otherTime);
}

bool Event::operator>=(const tm& otherTime) const
{
    return !(*this < otherTime);
}


tm Event::getTime()
{
    return time;
}

string Event::getMessage()
{
    return this->message;
}

string Event::getFormatTime()
{
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H.%M.%S", &time);
    return string(buffer);
}

size_t Event::getSize()
{
    return sizeof(*this);
}

string Event::toString()
{
    return "date: "+getFormatTime()+ ", message: "+getMessage();
}

bool operator<(const std::tm& tm1, const std::tm& tm2)
{
    return Event::compareTimes(tm1,tm2);
}

bool operator>=(const std::tm& tm1, const std::tm& tm2)
{
    return !Event::compareTimes(tm1,tm2);
}

bool operator<=(const std::tm& tm1, const std::tm& tm2)
{
    return !Event::compareTimes(tm2,tm1);
}
