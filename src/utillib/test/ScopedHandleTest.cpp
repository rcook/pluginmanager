#include "catch.hpp"
#include "utillib/ScopedHandle.h"
#include <cassert>
#include <unordered_map>

using namespace std;

enum class HandleState
{
    Created,
    Opened,
    Closed
};

using RawHandle = void*;

class HandleInfo
{
public:
    HandleInfo() = delete;
    HandleInfo(const HandleInfo&) = delete;
    HandleInfo& operator=(const HandleInfo&) = delete;

public:
    ~HandleInfo() = default;

    HandleInfo(HandleInfo&& other)
        : m_rawHandle(move(other.m_rawHandle))
        , m_name(move(other.m_name))
        , m_state(move(other.m_state))
    {
    }

    HandleInfo(RawHandle rawHandle, const string& name)
        : m_rawHandle(rawHandle)
        , m_name(name)
        , m_state(HandleState::Created)
    {
    }

    RawHandle rawHandle() const { return m_rawHandle; }
    const string& name() const { return m_name; }
    HandleState state() const { return m_state; }
    void setState(HandleState state) { m_state = state; }

private:
    const RawHandle m_rawHandle;
    const string m_name;
    HandleState m_state;
};

static unordered_map<RawHandle, HandleInfo> s_handleInfos;

static RawHandle OpenRawHandle(const string& name)
{
    auto rawHandle = reinterpret_cast<void*>(s_handleInfos.size() + 100);
    s_handleInfos.emplace(rawHandle, HandleInfo(rawHandle, name));

    auto& handleInfo = s_handleInfos.at(rawHandle);
    handleInfo.setState(HandleState::Opened);
    return rawHandle;
}

static const string& GetRawHandleName(RawHandle rawHandle)
{
    auto& handleInfo = s_handleInfos.at(rawHandle);
    assert(handleInfo.state() == HandleState::Opened);
    return handleInfo.name();
}

static HandleState GetRawHandleState(RawHandle rawHandle)
{
    auto& handleInfo = s_handleInfos.at(rawHandle);
    return handleInfo.state();
}

/*static*/ void CloseRawHandle(RawHandle rawHandle)
{
    auto& handleInfo = s_handleInfos.at(rawHandle);
    assert(handleInfo.state() == HandleState::Opened);
    handleInfo.setState(HandleState::Closed);
}

using TestHandle = ScopedHandle<RawHandle, nullptr, decltype(CloseRawHandle)*, CloseRawHandle>;

static TestHandle openTestHandle(const string& name)
{
    // Uses move constructor
    TestHandle handle(OpenRawHandle(name));
    return handle;
}

TEST_CASE("ScopedHandle", "ScopedHandle")
{
    SECTION("basics")
    {
        vector<RawHandle> rawHandles;

        // Check that handle is moved between ScopeHandle instances correctly
        {
            auto handle = openTestHandle("aaa");
            rawHandles.push_back(handle.get());
            REQUIRE(GetRawHandleName(handle.get()).compare("aaa") == 0);
            REQUIRE(GetRawHandleState(handle.get()) == HandleState::Opened);
        }

        for (auto rawHandle : rawHandles)
        {
            REQUIRE(GetRawHandleState(rawHandle) == HandleState::Closed);
        }
    }
}
