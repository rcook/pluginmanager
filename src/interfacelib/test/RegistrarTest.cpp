#include "catch.hpp"
#include "interfacelib/Registrar.h"

using namespace std;

TEST_CASE("Registrar", "Registrar")
{
    SECTION("basics")
    {
        class TestRegistrar : public Registrar
        {
        public:
            void registerOp(const string& name, OpFunc func) override { }
        };

        TestRegistrar registrar;
    }
}

