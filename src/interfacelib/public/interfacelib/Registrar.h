#pragma once

#include "interfacelib/OpFunc.h"
#include "interfacelib/defs.h"
#include <string>

#define REGISTER_PLUGIN_OPS_FUNCTION_NAME "__registerPluginOps"
#define REGISTER_PLUGIN_OPS(__registrar) \
    extern "C" DLLEXPORT void __registerPluginOps(Registrar& __registrar)

class INTERFACELIB_API Registrar
{
protected:
    Registrar();

public:
    virtual void registerOp(const std::string& name, OpFunc func) = 0;
};

using RegisterPluginOpsFunc = void(*)(Registrar&);
