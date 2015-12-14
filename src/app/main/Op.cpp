#include "Op.h"

using namespace std;

Op::Op(Op&& other)
    : m_name(move(other.m_name))
    , m_func(move(other.m_func))
{
}

Op::Op(const string& name, OpFunc func)
    : m_name(name)
    , m_func(func)
{
}
