#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE pairing
#include <boost/test/unit_test.hpp>
#include "pbc/pbc.hpp"

using namespace std;
using namespace pbc;

BOOST_AUTO_TEST_CASE(init_from_param_and_str)
{
    PairingParamPtr param = PairingParam::gen_type_a();
    PairingPtr pariring1 = Pairing::init_from_param(param);
    PairingPtr pariring2 = Pairing::init_from_param_str(param->to_str());
    BOOST_TEST(pariring1->symmetric() == pariring2->symmetric());
    BOOST_TEST(pariring1->g1_bytes_length() == pariring2->g1_bytes_length());
    BOOST_TEST(pariring1->g2_bytes_length() == pariring2->g2_bytes_length());
}
