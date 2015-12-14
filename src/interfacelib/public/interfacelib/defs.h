#pragma once

#include "config.h"

#ifdef _MSC_VER
#   define DLLEXPORT __declspec(dllexport)
#   define DLLIMPORT __declspec(dllimport)
#else
#   define DLLEXPORT __attribute__((visibility("default")))
#   define DLLIMPORT
#endif

#ifdef BUILD_INTERFACELIB
#   define INTERFACELIB_API DLLEXPORT
#else
#   define INTERFACELIB_API DLLIMPORT
#endif

#ifndef HAVE_MAKE_UNIQUE
#include <memory>

namespace std
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif
