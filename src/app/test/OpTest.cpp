#include "Op.h"
#include "catch.hpp"

TEST_CASE("Op", "Op")
{
    SECTION("basics")
    {
        Op op("name", [](int x, int y) { return x + y; });
        REQUIRE(op.name().compare("name") == 0);
        REQUIRE(op.func());
    }
}

