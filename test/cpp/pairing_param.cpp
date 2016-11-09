#include "helper.h"

BOOST_AUTO_TEST_CASE(to_and_init_from_str)
{
    BOOST_CHECK_THROW(PairingParam::init_from_str("type a"),
                      InitializationError);
    PairingParamPtr param = PairingParam::init_from_str(TYPE_A_PARAM);
    string param_str = param->to_str();
    BOOST_TEST(param_str == TYPE_A_PARAM);
}

BOOST_AUTO_TEST_CASE(gen_type_a)
{
    PairingParamPtr param = PairingParam::gen_type_a(160, 512);
    string param_str = param->to_str();
    BOOST_TEST(param_str.find("type a") != string::npos);
}

BOOST_AUTO_TEST_CASE(gen_type_i)
{
    PairingParamPtr param = PairingParam::gen_type_i(150);
    string param_str = param->to_str();
    BOOST_TEST(param_str.find("type i") != string::npos);
}

BOOST_AUTO_TEST_CASE(gen_type_e)
{
    PairingParamPtr param = PairingParam::gen_type_e(160, 1024);
    string param_str = param->to_str();
    BOOST_TEST(param_str.find("type e") != string::npos);
}

BOOST_AUTO_TEST_CASE(gen_type_f)
{
    PairingParamPtr param = PairingParam::gen_type_f(160);
    string param_str = param->to_str();
    BOOST_TEST(param_str.find("type f") != string::npos);
}
