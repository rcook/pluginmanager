#include "catch.hpp"
#include "interfacelib/OpFunc.h"

static void consumeOpFunc(OpFunc func) { }

static int testOpFunc(int x, int y)
{
    return x + y;
}

TEST_CASE("OpFunc", "OpFunc")
{
    SECTION("basics")
    {
        consumeOpFunc(testOpFunc);
    }
}

