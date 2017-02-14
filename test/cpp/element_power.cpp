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
    BOOST_TEST(p.get_base() == base);
    BOOST_CHECK_THROW(p.apply(e), NotInitializedError);
    e.init_g1(pairing);
    BOOST_CHECK_THROW(p.apply(e), ElementTypeError);
    z.init_zr(pairing);
    z.random();
    BOOST_TEST(p.apply(z) == base.pow(z));
    BOOST_TEST(p.pow(z) == base.pow(z));
    BOOST_TEST(p.apply(2) == base.pow(2));
    BOOST_TEST(p.pow(2) == base.pow(2));
}

BOOST_AUTO_TEST_CASE(element_pow2)
{
    Element a1, n1, a2, n2;
    BOOST_CHECK_THROW(pow2(a1, n1, a2, n2), NotInitializedError);
    a1.init_g1(pairing);
    a1.random();
    a2.init_g1(pairing);
    a2.random();
    BOOST_CHECK_THROW(pow2(a1, a1, a2, a2), ElementTypeError);
    n1.init_zr(pairing);
    n1.random();
    n2.init_zr(pairing);
    n2.random();
    BOOST_TEST(pow2(a1, n1, a2, n2) == a1.pow(n1) * a2.pow(n2));
}

BOOST_AUTO_TEST_CASE(element_pow3)
{
    Element a1, n1, a2, n2, a3, n3;
    BOOST_CHECK_THROW(pow3(a1, n1, a2, n2, a3, n3), NotInitializedError);
    a1.init_g1(pairing);
    a1.random();
    a2.init_g1(pairing);
    a2.random();
    a3.init_g1(pairing);
    a3.random();
    BOOST_CHECK_THROW(pow3(a1, a1, a2, a2, a3, a3), ElementTypeError);
    n1.init_zr(pairing);
    n1.random();
    n2.init_zr(pairing);
    n2.random();
    n3.init_zr(pairing);
    n3.random();
    BOOST_TEST(pow3(a1, n1, a2, n2, a3, n3) ==
               a1.pow(n1) * a2.pow(n2) * a3.pow(n3));
}

BOOST_AUTO_TEST_SUITE_END()
