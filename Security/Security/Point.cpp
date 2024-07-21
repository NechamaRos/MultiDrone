#include "Point.h"

Point::Point(cpp_int x, cpp_int y)
{
	this->x = x;
	this->y = y;
}

Point::Point()
{
	x = 0;
	y = 0;
}

bool Point::operator==(const Point& other) const
{
	if (this->x != other.x || this->y != other.y) {
		return false;
	}
	return true;
}

bool Point::operator!=(const Point& other) const
{
	if (this->x == other.x && this->y == other.y) {
		return false;
	}
	return true;
}
Point Point::inverse() const {
	return Point(x, -y);
}