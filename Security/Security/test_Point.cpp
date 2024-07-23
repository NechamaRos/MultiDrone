#include "doctest.h"
#include "Point.h"

TEST_CASE("Point constructor and equality operators") {
    cpp_int x1 = 3, y1 = 4;
    cpp_int x2 = 5, y2 = 6;

    Point p1(x1, y1);
    Point p2(x1, y1);
    Point p3(x2, y2);

    SUBCASE("Default constructor") {
        Point p;
        CHECK(p.getX() == 0);
        CHECK(p.getY() == 0);
    }

    SUBCASE("Parameterized constructor") {
        CHECK(p1.getX() == x1);
        CHECK(p1.getY() == y1);
    }

    SUBCASE("Equality operator") {
        CHECK(p1 == p2);
        CHECK(!(p1 == p3));
    }

    SUBCASE("Inequality operator") {
        CHECK(p1 != p3);
        CHECK(!(p1 != p2));
    }
}

TEST_CASE("Point inverse method") {
    cpp_int x = 7, y = 8;
    Point p(x, y);
    Point inv = p.inverse();

    CHECK(inv.getX() == x);
    CHECK(inv.getY() == -y);
}
