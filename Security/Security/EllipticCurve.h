#ifndef ELLIPTIC_CURVE_H
#define ELLIPTIC_CURVE_H
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
#include "Point.h"

class EllipticCurve {
private:

    cpp_int a;
    cpp_int b;
    cpp_int p;
    Point G;
    cpp_int N;


public:
    EllipticCurve();
    EllipticCurve(cpp_int a, cpp_int b, cpp_int p);
    bool isOnCurve(const Point& point) const;
    Point add(const Point& P, const Point& Q) const;
    Point multiply(const Point& P, cpp_int scalar) const;

    cpp_int getA() const {
        return a;
    }
    cpp_int getB() const {
        return b;
    }
    cpp_int getP() const {
        return p;
    } 
    Point getG() const {
        return G;
    }
    cpp_int getN() const {
        return N;
    }
};

#endif // ELLIPTIC_CURVE_H
