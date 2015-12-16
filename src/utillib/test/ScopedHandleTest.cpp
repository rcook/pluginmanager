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
    using TestUniquePtr = unique_ptr<remove_pointer<RawHandle>::type, decltype(CloseRawHandle)*>;

    SECTION("invalid")
    {
        TestHandle handle;

        REQUIRE(!handle);

        bool handleAsBool = handle;
        REQUIRE(!handleAsBool);

        REQUIRE_THROWS_AS(handle.get(), runtime_error);
    }

    SECTION("valid")
    {
        auto handle = openTestHandle("valid");

        REQUIRE(handle);

        bool handleAsBool = handle;
        REQUIRE(handleAsBool);

        REQUIRE_NOTHROW(handle.get());

        REQUIRE(handle.get());

        REQUIRE(GetRawHandleState(handle.get()) == HandleState::Opened);
    }

    SECTION("move")
    {
        vector<RawHandle> rawHandles;

        // Check that handle is moved between ScopeHandle instances correctly
        {
            auto handle = openTestHandle("move");
            rawHandles.push_back(handle.get());
            REQUIRE(GetRawHandleName(handle.get()).compare("move") == 0);
            REQUIRE(GetRawHandleState(handle.get()) == HandleState::Opened);
        }

        for (auto rawHandle : rawHandles)
        {
            REQUIRE(GetRawHandleState(rawHandle) == HandleState::Closed);
        }
    }

    SECTION("swap")
    {
        auto handle0 = openTestHandle("swap0");
        auto rawHandle0 = handle0.get();

        auto handle1 = openTestHandle("swap1");
        auto rawHandle1 = handle1.get();

        REQUIRE(rawHandle0 != rawHandle1);

        handle0.swap(handle1);

        REQUIRE(handle0.get() == rawHandle1);
        REQUIRE(GetRawHandleState(handle0.get()) == HandleState::Opened);

        REQUIRE(handle1.get() == rawHandle0);
        REQUIRE(GetRawHandleState(handle1.get()) == HandleState::Opened);
    }

    SECTION("release")
    {
        auto handle = openTestHandle("release");
        auto rawHandle = handle.get();
        REQUIRE(GetRawHandleState(handle.get()) == HandleState::Opened);

        TestUniquePtr ptr(handle.release(), CloseRawHandle);

        auto releasedRawHandle = ptr.get();
        REQUIRE(releasedRawHandle == rawHandle);
        REQUIRE_THROWS_AS(handle.get(), runtime_error);

        REQUIRE(GetRawHandleState(rawHandle) == HandleState::Opened);
    }

    SECTION("reset")
    {
        SECTION("invalid")
        {
            auto handle = openTestHandle("reset-invalid");
            auto rawHandle = handle.get();

            REQUIRE(GetRawHandleState(rawHandle) == HandleState::Opened);

            handle.reset();

            REQUIRE(GetRawHandleState(rawHandle) == HandleState::Closed);
        }

        SECTION("other")
        {
            TestUniquePtr ptr0(OpenRawHandle("reset-other0"), CloseRawHandle);
            auto rawHandle0 = ptr0.get();

            auto handle1 = openTestHandle("reset-other1");
            auto rawHandle1 = handle1.get();

            REQUIRE(GetRawHandleState(rawHandle0) == HandleState::Opened);
            REQUIRE(GetRawHandleState(rawHandle1) == HandleState::Opened);

            handle1.reset(ptr0.release());

            REQUIRE(GetRawHandleState(rawHandle0) == HandleState::Opened);
            REQUIRE(GetRawHandleState(rawHandle1) == HandleState::Closed);
        }
    }

    for (const auto& pair : s_handleInfos)
    {
        REQUIRE(pair.second.state() == HandleState::Closed);
    }
}
