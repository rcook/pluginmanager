#include "catch.hpp"
#include "utillib/string.h"

TEST_CASE("string", "string")
{
    SECTION("basics")
    {
        REQUIRE(stringBeginsWith("foobar", "foo"));
        REQUIRE(!stringBeginsWith("foobar", "fob"));
    }
}

