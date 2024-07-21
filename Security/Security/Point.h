#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include "Logger.h"
using namespace boost::multiprecision;

class Point
{
private:
	cpp_int x;
	cpp_int y;

public:
	Point(cpp_int x, cpp_int y);
	Point();
	bool isInfinity() const { return x == 0 && y == 0; }

	cpp_int getX() const {
		return x;
	}
	cpp_int getY() const {
		return y;
	}
	bool operator==(const Point& other) const;
	bool operator!=(const Point& other) const;
	Point inverse() const;
};

