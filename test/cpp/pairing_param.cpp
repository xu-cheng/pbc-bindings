#include "helper.h"

TEST_CASE_METHOD(Fixture, "to_and_init_from_str")
{
    CHECK_THROWS_AS(PairingParam::init_from_str("type a"), InitializationError);
    PairingParamPtr param = PairingParam::init_from_str(type_a_param);
    string param_str = param->to_str();
    CHECK(param_str == type_a_param);
}

TEST_CASE_METHOD(Fixture, "gen_type_a")
{
    PairingParamPtr param = PairingParam::gen_type_a(160, 512);
    string param_str = param->to_str();
    CHECK_THAT(param_str, Contains("type a"));
}

TEST_CASE_METHOD(Fixture, "gen_type_i")
{
    PairingParamPtr param = PairingParam::gen_type_i(150);
    string param_str = param->to_str();
    CHECK_THAT(param_str, Contains("type i"));
}

TEST_CASE_METHOD(Fixture, "gen_type_e")
{
    PairingParamPtr param = PairingParam::gen_type_e(160, 1024);
    string param_str = param->to_str();
    CHECK_THAT(param_str, Contains("type e"));
}

TEST_CASE_METHOD(Fixture, "gen_type_f")
{
    PairingParamPtr param = PairingParam::gen_type_f(160);
    string param_str = param->to_str();
    CHECK_THAT(param_str, Contains("type f"));
}
