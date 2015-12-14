#include "PluginManager.h"
#include "RegistrarImpl.h"
#include "utillib/OSError.h"
#include "utillib/ScopedHandle.h"
#include "utillib/fs.h"
#include "utillib/string.h"
#if defined(BUILD_LINUX) || defined(BUILD_OSX)
#include <dirent.h>
#endif
#include <iostream>
#ifdef BUILD_WINDOWS
#include <windows.h>
#endif

using namespace std;

#ifdef BUILD_WINDOWS

static vector<string> getCandidatePluginFileNames(const string& pluginDir)
{
    using FindHandle = ScopedHandle<HANDLE, INVALID_HANDLE_VALUE, decltype(FindClose)*, FindClose>;

    vector<string> fileNames;

    string filter(pluginDir + "\\*.dll");
    WIN32_FIND_DATA findData;
    FindHandle handle(FindFirstFile(filter.data(), &findData));
    if (!handle)
    {
        if (GetLastError() == ERROR_PATH_NOT_FOUND)
        {
            return fileNames;
        }

        OSError::throwCurrentError("FindFirstFile");
    }

    do 
    {
        fileNames.emplace_back(joinPaths(pluginDir, findData.cFileName));
    }
    while (FindNextFile(handle.get(), &findData));

    return fileNames;
}

#else

static vector<string> getFileNames(const string& dir, function<bool(const dirent*)> predicate)
{
    using DirHandle = ScopedHandle<DIR*, nullptr, decltype(closedir)*, closedir>;

    vector<string> fileNames;

    DirHandle handle(opendir(dir.data()));
    if (!handle)
    {
        return fileNames;
    }

    dirent* entry;
    while ((entry = readdir(handle.get())))
    {
        if (predicate(entry))
        {
            fileNames.emplace_back(joinPaths(dir, entry->d_name));
        }
    }

    return fileNames;
}

static vector<string> getCandidatePluginFileNames(const string& pluginDir)
{
    return getFileNames(pluginDir, [](const dirent* entry)
    {
        return entry->d_type == DT_REG
            && stringBeginsWith(entry->d_name, "lib")
#ifdef BUILD_LINUX
            && stringEndsWith(entry->d_name, ".so");
#else
            && stringEndsWith(entry->d_name, ".dylib");
#endif
    });
}

#endif

int main(int argc, char* const argv[])
{
    try
    {
        PluginManager pluginManager;
        RegistrarImpl registrar;

        string dir(getDir(argv[0]));
        string pluginDir(joinPaths(dir, "plugins"));
        for (const auto& fileName : getCandidatePluginFileNames(pluginDir))
        {
            if (pluginManager.tryRegisterPluginOps(fileName, registrar))
            {
                cout << "Registered ops in plugin " << fileName << endl;
            }
        }

        for (const auto& pair : registrar.ops())
        {
            cout << pair.first << ": result=" << pair.second.func()(5, 6) << endl;
        }

        cout << "result=" << registrar.invoke("a", 5, 6) << endl;
    }
    catch (const OSError& ex)
    {
        cout
            << "Unhandled OS error:" << endl
            << "  code: " << ex.code() << endl
            << "  osFunctionName: " << ex.osFunctionName() << endl
            << "  message: " << ex.what() << endl;
    }
    catch (const runtime_error& ex)
    {
        cout << "Unhandled exception: " << ex.what() << endl;
    }
}
