#include "helper.h"

struct Fixture {
    Fixture() { pairing = Pairing::init_from_param_str(TYPE_A_PARAM); }
    ~Fixture() {}
    PairingPtr pairing;
};

BOOST_FIXTURE_TEST_SUITE(pairing, Fixture)

BOOST_AUTO_TEST_CASE(element_pairing)
{
    Element e1, e2, z;
    BOOST_CHECK_THROW(e(e1, e2), NotInitializedError);
    e1.init_g1(pairing);
    e2.init_g2(pairing);
    z.init_zr(pairing);
    e1.random();
    e2.random();
    z.random();
    Element et = e(e1, e2);
    BOOST_TEST(et.type() == ElementType::GT);
    BOOST_TEST(et.pow(z) == e(e1.pow(z), e2));
}

BOOST_AUTO_TEST_SUITE_END()
