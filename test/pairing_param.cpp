#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE pairing_param
#include <boost/test/unit_test.hpp>
#include "pbc/pbc.hpp"

using namespace std;
using namespace pbc;

BOOST_AUTO_TEST_CASE(to_and_init_from_str)
{
    string param_str = R"(type i
m 97
t 12
n 2726865189058261010774960798134976187171462721
n2 7
)";
    PairingParamPtr param = PairingParam::init_from_str(param_str);
    string param_str_out = param->to_str();
    BOOST_TEST(param_str == param_str_out);
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
