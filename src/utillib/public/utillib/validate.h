#pragma once

#include <stdexcept>

#define XYZ_REQUIRE(expr) \
    do \
    { \
        if (!(expr)) \
        { \
            throw std::runtime_error("General failure"); \
        } \
    } \
    while (false)
