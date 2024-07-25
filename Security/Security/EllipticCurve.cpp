#include "EllipticCurve.h"
#include "Logger.h"
#include "Point.h"
#include "Utils.h"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

// Constructor for the EllipticCurve class
EllipticCurve::EllipticCurve(cpp_int a, cpp_int b, cpp_int p)
{
    // Calculate the discriminant of the curve to ensure it is valid over F_p
    cpp_int discriminant = mod(4 * a * a * a + 27 * b * b, p);
    if (discriminant == 0) {
        Logger::getInstance().log("c'tor EllipticCurve: The curve is not valid over F_p.");
        throw std::runtime_error("The curve is not valid over F_p.");
    }
    // Define the generator point G (Gx, Gy) and the order N of the curve
    cpp_int Gx("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
    cpp_int Gy("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
    Point g(Gx, Gy);

    // Initialize the curve parameters
    this->a = a;
    this->b = b;
    this->p = p;
    this->G = g;
    this->N = cpp_int("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");

    // Check if the generator point G is on the curve
    if (!isOnCurve(this->G)) {
        Logger::getInstance().log("c'tor EllipticCurve: Generator point G is not on the curve");
        throw std::runtime_error("Generator point G is not on the curve");
    }
}

// Default constructor for the EllipticCurve class
EllipticCurve::EllipticCurve()
{
    // Initialize curve parameters for the secp256k1 curve
    this->a = 0;
    this->b = 7;
    cpp_int Gx("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
    cpp_int Gy("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
    Point g(Gx, Gy);
    this->G = g;
    this->p = cpp_int("0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    this->N = cpp_int("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
}

// Function to check if a point is on the elliptic curve
bool EllipticCurve::isOnCurve(const Point& P) const {
    if (P.isInfinity()) return true; // Point at infinity is always on the curve
    cpp_int lhs = mod(P.getY() * P.getY(), p); // Calculate the left-hand side of the curve equation
    cpp_int rhs = mod(P.getX() * P.getX() * P.getX() + a * P.getX() + b, p); // Calculate the right-hand side of the curve equation
    bool onCurve = lhs == rhs;
    if (!onCurve) {
        Logger::getInstance().log("isOnCurve: The point is not on the curve. LHS: " + lhs.str() + ", RHS: " + rhs.str());
    }
    return onCurve;
}

// Function to add two points on the elliptic curve
Point EllipticCurve::add(const Point& P, const Point& Q) const
{
    // Check if both points are on the curve
    if (!isOnCurve(P) || !isOnCurve(Q)) {
        Logger::getInstance().log("EllipticCurve::add: One of the points is not on the curve");
        throw std::runtime_error("One of the points is not on the curve");
    }

    Logger::getInstance().log("EllipticCurve::add: Adding points P and Q");
    Logger::getInstance().log("P: (" + P.getX().str() + ", " + P.getY().str() + ")");
    Logger::getInstance().log("Q: (" + Q.getX().str() + ", " + Q.getY().str() + ")");

    if (P.isInfinity()) return Q; // If P is the point at infinity, return Q
    if (Q.isInfinity()) return P; // If Q is the point at infinity, return P

    cpp_int lambda;
    try {
        if (P.getX() == Q.getX()) {
            if (P.getY() != Q.getY() || P.getY() == 0) {
                return Point(0, 0); // Result is the point at infinity
            }
            cpp_int num = mod(3 * P.getX() * P.getX() + a, p); // Calculate the numerator for lambda in the case of P == Q
            cpp_int denom = mod(2 * P.getY(), p); // Calculate the denominator for lambda
            lambda = mod(num * modInverse(denom, p), p); // Calculate lambda using modular inverse
        }
        else {
            cpp_int num = mod(Q.getY() - P.getY(), p); // Calculate the numerator for lambda in the case of P != Q
            cpp_int denom = mod(Q.getX() - P.getX(), p); // Calculate the denominator for lambda
            lambda = mod(num * modInverse(denom, p), p); // Calculate lambda using modular inverse
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error calculating lambda: " << e.what() << std::endl;
        throw;
    }

    Logger::getInstance().log("EllipticCurve::add: lambda = " + lambda.str());

    cpp_int xr = mod(lambda * lambda - P.getX() - Q.getX(), p); // Calculate x coordinate of the resulting point
    cpp_int yr = mod(lambda * (P.getX() - xr) - P.getY(), p); // Calculate y coordinate of the resulting point

    Logger::getInstance().log("EllipticCurve::add: xr = " + xr.str());
    Logger::getInstance().log("EllipticCurve::add: yr = " + yr.str());

    Point R(xr, yr);

    // Check if the resulting point is on the curve
    if (!isOnCurve(R)) {
        Logger::getInstance().log("EllipticCurve::add: Resulting point is not on the curve");
        throw std::runtime_error("Resulting point is not on the curve");
    }
    return R;
}

// Function to multiply a point by a scalar on the elliptic curve
Point EllipticCurve::multiply(const Point& P, cpp_int scalar) const {
    // Check if the point is on the curve
    if (!isOnCurve(P)) {
        Logger::getInstance().log("EllipticCurve::multiply: The point is not on the curve");
        throw std::runtime_error("The point is not on the curve");
    }
    if (scalar == 0 || P.isInfinity()) {
        return Point(0, 0); // If scalar is 0 or P is the point at infinity, return the point at infinity
    }

    Point R(0, 0); // Initialize result point R to the point at infinity
    Point Q = P; // Initialize Q to P
    cpp_int k = scalar; // Initialize k to scalar

    // Double-and-add algorithm for point multiplication
    while (k > 0) {
        if (k % 2 == 1) {
            R = add(R, Q); // Add Q to R if the current bit of k is 1
        }
        Q = add(Q, Q); // Double Q
        k /= 2; // Divide k by 2

        Logger::getInstance().log("EllipticCurve::multiply: Current R: (" + R.getX().str() + ", " + R.getY().str() + ")");
        Logger::getInstance().log("EllipticCurve::multiply: Current Q: (" + Q.getX().str() + ", " + Q.getY().str() + ")");
    }

    // Check if the resulting point is on the curve
    if (!isOnCurve(R)) {
        Logger::getInstance().log("EllipticCurve::multiply: Resulting point is not on the curve");
        throw std::runtime_error("Resulting point is not on the curve");
    }

    return R;
}
