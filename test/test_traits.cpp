#include "catch.hpp"
#include <traits.h>

struct Foo : Eq {
    Foo(bool x) : x(x) {}
    bool operator==(Foo const& b) const { return x == b.x; }

    bool x;
};

TEST_CASE("Eq") {
    SECTION("comapirng two equivalent values") {
        const Foo a{true};
        const Foo b{true};

        REQUIRE(a == b);
        REQUIRE_FALSE(a != b);
    }

    SECTION("comapirng two different values") {
        const Foo a{true};
        const Foo b{false};

        REQUIRE_FALSE(a == b);
        REQUIRE(a != b);
    }
}

struct Bar : Ord {
    Bar(int x) : x(x) {}

    bool operator==(Bar const& b) const { return x == b.x; }
    bool operator<(Bar const& b) const { return x < b.x; }

    int x;
};

TEST_CASE("Ord") {
    SECTION("comapirng two equivalent values") {
        const Bar a{0};
        const Bar b{0};

        REQUIRE_FALSE(a < b);
        REQUIRE_FALSE(a > b);

        REQUIRE(a <= b);
        REQUIRE(a >= b);

        REQUIRE(a == b);
        REQUIRE_FALSE(a != b);
    }

    SECTION("comapirng two different values") {
        const Bar a{0};
        const Bar b{1};

        REQUIRE(a < b);
        REQUIRE_FALSE(a > b);

        REQUIRE(a <= b);
        REQUIRE_FALSE(a >= b);

        REQUIRE_FALSE(a == b);
        REQUIRE(a != b);
    }
}
