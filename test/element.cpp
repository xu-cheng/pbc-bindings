#include "helper.h"

struct Fixture {
    Fixture() { pairing = Pairing::init_from_param_str(TYPE_A_PARAM); }
    ~Fixture() {}
    PairingPtr pairing;
};

BOOST_FIXTURE_TEST_SUITE(pairing, Fixture)

BOOST_AUTO_TEST_CASE(move_constructor)
{
    Element e1;
    e1.init_zr(pairing);
    Element e2 = move(e1);
    BOOST_TEST(e1.type() == ElementType::NotInitialized);
    BOOST_TEST(e2.type() == ElementType::Zr);
}

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
    BOOST_CHECK_THROW(g1.init_g2(pairing), AlreadyInitializedError);
}

BOOST_AUTO_TEST_SUITE_END()
