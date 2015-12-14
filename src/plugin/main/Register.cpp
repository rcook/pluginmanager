#include "interfacelib/Registrar.h"

REGISTER_PLUGIN_OPS(registrar)
{
    registrar.registerOp("add", [](int x, int y) { return x + y; });
    registrar.registerOp("multiply", [](int x, int y) { return x * y; });
}
