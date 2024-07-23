#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "EllipticCurve.h"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

TEST_CASE("Elliptic Curve Constructor") {
    cpp_int a = 2;
    cpp_int b = 3;
    cpp_int p = 17;

    CHECK_NOTHROW(EllipticCurve(a, b, p));

    cpp_int invalid_a = 0;
    cpp_int invalid_b = 0;
    cpp_int invalid_p = 17;

    CHECK_THROWS_AS(EllipticCurve(invalid_a, invalid_b, invalid_p), std::runtime_error);
}

TEST_CASE("Point On Curve") {

    cpp_int a = 0;
    cpp_int b = 7;
    cpp_int p = cpp_int("0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    EllipticCurve curve(a, b, p);
    Point validPoint(cpp_int("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"),
        cpp_int("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"));
    Point invalidPoint(5, 2);
    CHECK(curve.isOnCurve(validPoint));
    CHECK_FALSE(curve.isOnCurve(invalidPoint));
}

TEST_CASE("Add Points") {

    cpp_int a = 0;
    cpp_int b = 7;
    cpp_int p = cpp_int("0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    EllipticCurve curve(a, b, p);
    Point P(cpp_int("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"),
        cpp_int("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"));
    Point Q(cpp_int("0xf9308a019258c31049344f85f89d5229b531c845836f99b08601f113bce036f9"),
        cpp_int("0x388f7b0f632de8140fe337e62a37f3566500a99934c2231b6cb9fd7584b8e672"));
    CHECK(curve.isOnCurve(Q));

    Point result = curve.add(P, Q);
    Point r(cpp_int("0xc6047f9441ed7d6d3045406e95c07cd85a86a1678953e44f54006e452df93ea0")
        , cpp_int("0x1ae168fea63dc339a3c58419466ceae04a2d8a0eaef756a01d2b7906a8d2e035"));
    CHECK(curve.isOnCurve(result));

    Point doubled = curve.add(P, P);
    CHECK(curve.isOnCurve(doubled));

    Point infinity = curve.add(P, Point(0, 0));
    CHECK(infinity.getX() == P.getX());
    CHECK(infinity.getY() == P.getY());

    Point invalidPoint(5, 2);
    CHECK_THROWS_AS(curve.add(P, invalidPoint), std::runtime_error);
}

TEST_CASE("Multiply Point by Scalar") {

    cpp_int a = 0;
    cpp_int b = 7;
    cpp_int p = cpp_int("0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    EllipticCurve curve(a, b, p);
    Point P(cpp_int("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"),
        cpp_int("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"));

    Point R = curve.multiply(P, curve.getN()-100);
    CHECK(curve.isOnCurve(R));

}
 