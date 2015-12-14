#pragma once

#include "interfacelib/Registrar.h"
#include <memory>
#include <string>

class PluginManager
{
public:
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

public:
    PluginManager();
    ~PluginManager();

    bool tryRegisterPluginOps(const std::string& fileName, Registrar& registrar);

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};
