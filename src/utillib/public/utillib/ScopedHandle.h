#pragma once

#include "utillib/validate.h"

template<typename THandle, THandle InvalidHandleValue, typename TCloseFunc, TCloseFunc closeFunc>
class ScopedHandle
{
public:
    ScopedHandle(const ScopedHandle&) = delete;
    ScopedHandle& operator=(const ScopedHandle&) = delete;

public:
    explicit ScopedHandle(THandle handle = InvalidHandleValue)
        : m_handle(handle)
    {
    }

    ScopedHandle(ScopedHandle&& other)
        : m_handle(other.release())
    {
    }

    ~ScopedHandle()
    {
        if (m_handle != InvalidHandleValue)
        {
            closeFunc(m_handle);
            m_handle = InvalidHandleValue;
        }
    }

    operator bool() const { return m_handle != InvalidHandleValue; }

    bool operator!() const { return m_handle == InvalidHandleValue; }

    void swap(ScopedHandle& other)
    {
        using std::swap;

        swap(m_handle, other.m_handle);
    }

    THandle release()
    {
        THandle result = m_handle;
        m_handle = InvalidHandleValue;
        return result;
    }

    void reset(THandle handle = InvalidHandleValue)
    {
        ScopedHandle temp(m_handle);
        m_handle = handle;
    }

    THandle get() const
    {
        XYZ_REQUIRE(m_handle != InvalidHandleValue);

        return m_handle;
    }

private:
    THandle m_handle;
};

