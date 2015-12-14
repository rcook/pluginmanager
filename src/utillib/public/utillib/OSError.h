#pragma once

#include <stdexcept>
#include <string>

class OSError : public std::runtime_error
{
public:
    OSError() = delete;
    OSError& operator=(const OSError&) = delete;

public:
    OSError(const OSError&) = default;
    ~OSError() = default;

    OSError(int code, const char* osFunctionName, const char* message);

    static void throwCurrentError(const char* osFunctionName);

    int code() const { return m_code; }
    const std::string& osFunctionName() const { return m_osFunctionName; }

private:
    const int m_code;
    const std::string m_osFunctionName;
};
