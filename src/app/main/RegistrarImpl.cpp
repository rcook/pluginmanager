#include "RegistrarImpl.h"
#include "utillib/validate.h"

using namespace std;

void RegistrarImpl::registerOp(const string& name, OpFunc func)
{
	XYZ_REQUIRE(m_names.insert(name));

    m_ops.emplace(name, Op(name, func));
}

int RegistrarImpl::invoke(const string& command, int x, int y) const
{
	const auto* name = m_names.findByPrefix(command);
	XYZ_REQUIRE(name);

	const auto& op = m_ops.at(*name);
	return op.func()(x, y);
}
