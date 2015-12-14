#include "PluginManager.h"

#include "utillib/OSError.h"
#include "utillib/ScopedHandle.h"
#include "utillib/validate.h"
#include <unordered_map>

#ifdef BUILD_WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using namespace std;

class PluginManager::Impl
{
private:
#ifdef BUILD_WINDOWS
    using ModuleHandle = ScopedHandle<HMODULE, nullptr, decltype(FreeLibrary)*, FreeLibrary>;
#else
    using ModuleHandle = ScopedHandle<void*, nullptr, decltype(dlclose)*, dlclose>;
#endif

public:
    Impl(const Impl&) = delete;
    Impl& operator=(const Impl&) = delete;

public:
    Impl() = default;
    ~Impl() = default;

    bool tryRegisterPluginOps(const string& fileName, Registrar& registrar)
    {
        const auto* module = tryGetModule(fileName);
        if (!module)
        {
            return false;
        }

        auto func = tryGetFunc<RegisterPluginOpsFunc>(*module, REGISTER_PLUGIN_OPS_FUNCTION_NAME);
        if (!func)
        {
            return false;
        }

        func(registrar);
        return true;
    }

private:
    const ModuleHandle* tryGetModule(const string& fileName)
    {
        auto iter = m_modules.find(fileName);
        if (iter != m_modules.end())
        {
            return &iter->second;
        }

        auto module = tryLoadModule(fileName.data());
        if (!module)
        {
            return nullptr;
        }

        auto result = m_modules.emplace(fileName, move(module));
        XYZ_REQUIRE(result.second);

        return &result.first->second;
    }

    static ModuleHandle tryLoadModule(const string& fileName)
    {
#ifdef BUILD_WINDOWS
        // Temporarily suppress critical error dialog
        class SuppressCriticalErrorHandler
        {
        public:
            SuppressCriticalErrorHandler() : m_oldErrorMode(GetErrorMode()) { SetErrorMode(m_oldErrorMode | SEM_FAILCRITICALERRORS); }
            ~SuppressCriticalErrorHandler() { SetErrorMode(m_oldErrorMode); }

        private:
            const UINT m_oldErrorMode;
        } instance;

        auto module = ModuleHandle(LoadLibrary(fileName.data()));
        if (!module && GetLastError() != ERROR_BAD_EXE_FORMAT)
        {
            OSError::throwCurrentError("LoadLibrary");
        }

        return module;
#else
        return ModuleHandle(dlopen(fileName.data(), RTLD_LAZY));
#endif
    }

    template<typename F>
    F tryGetFunc(const ModuleHandle& module, const char* name)
    {
#ifdef BUILD_WINDOWS
        auto func = GetProcAddress(module.get(), name);
        if (!func && GetLastError() != ERROR_PROC_NOT_FOUND)
        {
            OSError::throwCurrentError("GetProcAddress");
        }

        return reinterpret_cast<F>(func);
#else
        return reinterpret_cast<F>(dlsym(module.get(), name));
#endif
    }

private:
    unordered_map<string, ModuleHandle> m_modules;
};

PluginManager::PluginManager() : m_impl(make_unique<Impl>())
{
}

PluginManager::~PluginManager()
{
}

bool PluginManager::tryRegisterPluginOps(const string& fileName, Registrar& registrar)
{
    return m_impl->tryRegisterPluginOps(fileName, registrar);
}
