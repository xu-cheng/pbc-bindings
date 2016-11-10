#include "helper.h"

struct Fixture {
    Fixture() { pairing = Pairing::init_from_param_str(TYPE_A_PARAM); }
    ~Fixture() {}
    PairingPtr pairing;
};

BOOST_FIXTURE_TEST_SUITE(pairing, Fixture)

BOOST_AUTO_TEST_CASE(element_power)
{
    Element base, e, z;
    BOOST_CHECK_THROW(FixedBasePower a(base), NotInitializedError);
    base.init_g1(pairing);
    base.random();
    FixedBasePower p(base);
    BOOST_CHECK_THROW(p.apply(e), NotInitializedError);
    e.init_g1(pairing);
    BOOST_CHECK_THROW(p.apply(e), ElementTypeError);
    z.init_zr(pairing);
    z.random();
    BOOST_TEST(p.apply(z) == base.pow(z));
    BOOST_TEST(p.apply(2) == base.pow(2));
}

BOOST_AUTO_TEST_SUITE_END()
