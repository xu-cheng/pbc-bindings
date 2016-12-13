#include "helper.h"

struct Fixture {
    Fixture() { param = PairingParam::init_from_str(TYPE_A_PARAM); }
    ~Fixture() {}
    PairingParamPtr param;
};

BOOST_FIXTURE_TEST_SUITE(param, Fixture)

BOOST_AUTO_TEST_CASE(init_from_param_and_str)
{
    BOOST_CHECK_THROW(Pairing::init_from_param_str("type a"),
                      InitializationError);
    PairingPtr pairing1 = Pairing::init_from_param(param);
    PairingPtr pairing2 = Pairing::init_from_param_str(param->to_str());
    BOOST_TEST(pairing1->symmetric() == pairing2->symmetric());
    BOOST_TEST(pairing1->g1_bytes_length() == pairing2->g1_bytes_length());
    BOOST_TEST(pairing1->g2_bytes_length() == pairing2->g2_bytes_length());
}

BOOST_AUTO_TEST_CASE(pairing_info)
{
    PairingPtr pairing = Pairing::init_from_param(param);
    BOOST_TEST(pairing->symmetric() == true);
    BOOST_TEST(pairing->g1_bytes_length() == 128);
    BOOST_TEST(pairing->g1_x_only_bytes_length() == 64);
    BOOST_TEST(pairing->g1_compressed_bytes_length() == 65);
    BOOST_TEST(pairing->g2_bytes_length() == 128);
    BOOST_TEST(pairing->g2_x_only_bytes_length() == 64);
    BOOST_TEST(pairing->g2_compressed_bytes_length() == 65);
    BOOST_TEST(pairing->gt_bytes_length() == 128);
    BOOST_TEST(pairing->zr_bytes_length() == 20);
    BOOST_TEST(pairing->g1_order() ==
               mpz_class("730750818665451621361119245571504901405976559617"));
    BOOST_TEST(pairing->g2_order() ==
               mpz_class("730750818665451621361119245571504901405976559617"));
    BOOST_TEST(pairing->gt_order() ==
               mpz_class("730750818665451621361119245571504901405976559617"));
    BOOST_TEST(pairing->zr_order() ==
               mpz_class("730750818665451621361119245571504901405976559617"));
}

BOOST_AUTO_TEST_SUITE_END()
