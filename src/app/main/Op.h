#pragma once

#include "interfacelib/defs.h"
#include "interfacelib/Registrar.h"

class Op
{
public:
    Op() = delete;
    Op(const Op&) = delete;
    Op& operator=(const Op&) = delete;

public:
    Op(Op&& other);
    Op(const std::string& name, OpFunc func);

    const std::string& name() const { return m_name; }
    OpFunc func() const { return m_func; }

private:
    const std::string m_name;
    const OpFunc m_func;
};
