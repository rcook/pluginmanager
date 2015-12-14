#include "utillib/OSError.h"
#include "utillib/ScopedHandle.h"
#ifdef BUILD_WINDOWS
#include <windows.h>
#endif

using namespace std;

#ifdef BUILD_WINDOWS
static_assert(sizeof(DWORD) <= sizeof(int), "Must be able to store DWORD error code in int");

using MessageHandle = ScopedHandle<HLOCAL, nullptr, decltype(LocalFree)*, LocalFree>;

static MessageHandle FormatErrorMessage(DWORD dwError)
{
    HLOCAL hMessage;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&hMessage),
        0,
        nullptr);
    return MessageHandle(hMessage);
}
#endif

OSError::OSError(int code, const char* osFunctionName, const char* message)
    : runtime_error(message)
    , m_osFunctionName(osFunctionName)
    , m_code(code)
{
}

void OSError::throwCurrentError(const char* osFunctionName)
{
#ifdef BUILD_WINDOWS
    DWORD dwError = GetLastError();
    auto message = FormatErrorMessage(dwError);
    throw OSError(dwError, osFunctionName, static_cast<const char*>(message.get()));
#else
    throw OSError(-1, osFunctionName, "<no message>");
#endif
}
