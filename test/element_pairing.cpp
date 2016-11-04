#include <vector>
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

BOOST_AUTO_TEST_CASE(element_pairing_prod)
{
    BOOST_CHECK_THROW(e_prod(vector<Element>{}, vector<Element>{}),
                      std::invalid_argument);
    Element e1, e2;
    e1.init_g1(pairing);
    e2.init_g2(pairing);
    e1.random();
    e2.random();
    BOOST_CHECK_THROW(e_prod(vector<Element>{e1, e1}, vector<Element>{e2}),
                      std::invalid_argument);
    BOOST_CHECK_THROW(e_prod(vector<Element>{e1}, vector<Element>{Element()}),
                      NotInitializedError);
    Element et1 = e_prod(vector<Element>{e1, e1}, vector<Element>{e2, e2});
    Element et2 = e(e1, e2);
    BOOST_TEST(et1 == et2 * et2);
}

BOOST_AUTO_TEST_CASE(element_pairing_fixed_g1)
{
    Element e1, e2;
    BOOST_CHECK_THROW(FixedG1Pairing a(e1), ElementTypeError);
    e1.init_g1(pairing);
    e1.random();
    FixedG1Pairing p(e1);
    BOOST_CHECK_THROW(p.apply(e2), NotInitializedError);
    e2.init_g2(pairing);
    e2.random();
    BOOST_TEST(p.apply(e2) == e(e1, e2));
}

BOOST_AUTO_TEST_SUITE_END()
