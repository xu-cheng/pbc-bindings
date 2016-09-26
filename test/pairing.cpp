#include "helper.h"

struct Fixture {
    Fixture() { param = PairingParam::gen_type_a(160, 512); }
    ~Fixture() {}
    PairingParamPtr param;
};

BOOST_FIXTURE_TEST_SUITE(param, Fixture)

BOOST_AUTO_TEST_CASE(init_from_param_and_str)
{
    PairingPtr pariring1 = Pairing::init_from_param(param);
    PairingPtr pariring2 = Pairing::init_from_param_str(param->to_str());
    BOOST_TEST(pariring1->symmetric() == pariring2->symmetric());
    BOOST_TEST(pariring1->g1_bytes_length() == pariring2->g1_bytes_length());
    BOOST_TEST(pariring1->g2_bytes_length() == pariring2->g2_bytes_length());
}

BOOST_AUTO_TEST_CASE(pairing_info)
{
    PairingPtr pariring = Pairing::init_from_param(param);
    BOOST_TEST(pariring->symmetric() == true);
    BOOST_TEST(pariring->g1_bytes_length() == 128);
    BOOST_TEST(pariring->g1_x_only_bytes_length() == 64);
    BOOST_TEST(pariring->g1_compressed_bytes_length() == 65);
    BOOST_TEST(pariring->g2_bytes_length() == 128);
    BOOST_TEST(pariring->g2_x_only_bytes_length() == 64);
    BOOST_TEST(pariring->g2_compressed_bytes_length() == 65);
    BOOST_TEST(pariring->gt_bytes_length() == 128);
    BOOST_TEST(pariring->zr_bytes_length() == 20);
}

BOOST_AUTO_TEST_SUITE_END()
