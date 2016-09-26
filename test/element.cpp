#include "helper.h"

struct Fixture {
    Fixture()
    {
        param = PairingParam::gen_type_a(160, 512);
        pairing = Pairing::init_from_param(param);
    }
    ~Fixture() {}
    PairingParamPtr param;
    PairingPtr pairing;
};

BOOST_FIXTURE_TEST_SUITE(pairing, Fixture)

BOOST_AUTO_TEST_CASE(init)
{
    Element g1, g2, gt, zr;
    BOOST_TEST(g1.type() == ElementType::NotInitialized);
    g1.init_g1(pairing);
    BOOST_TEST(g1.type() == ElementType::G1);
    g2.init_g2(pairing);
    BOOST_TEST(g2.type() == ElementType::G2);
    gt.init_gt(pairing);
    BOOST_TEST(gt.type() == ElementType::GT);
    zr.init_zr(pairing);
    BOOST_TEST(zr.type() == ElementType::Zr);
}

BOOST_AUTO_TEST_SUITE_END()
