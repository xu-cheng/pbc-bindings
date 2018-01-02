#include "helper.h"

#include <vector>

TEST_CASE_METHOD(Fixture, "element_pairing")
{
    Element e1, e2, z;
    CHECK_THROWS_AS(e(e1, e2), NotInitializedError);
    e1.init_g1(pairing);
    e2.init_g2(pairing);
    z.init_zr(pairing);
    e1.random();
    e2.random();
    z.random();
    Element et = e(e1, e2);
    CHECK(et.type() == ElementType::GT);
    CHECK(et.pow(z) == e(e1.pow(z), e2));
}

TEST_CASE_METHOD(Fixture, "element_pairing_prod")
{
    CHECK_THROWS_AS(e_prod(vector<Element>{}, vector<Element>{}),
                    std::invalid_argument);
    Element e1, e2, one;
    e1.init_g1(pairing);
    e2.init_g2(pairing);
    one.init_g1(pairing);
    e1.random();
    e2.random();
    one.set_one();
    CHECK_THROWS_AS(e_prod(vector<Element>{e1, e1}, vector<Element>{e2}),
                    std::invalid_argument);
    CHECK_THROWS_AS(e_prod(vector<Element>{e1}, vector<Element>{Element()}),
                    NotInitializedError);
    Element et1 = e_prod(vector<Element>{e1, e1}, vector<Element>{e2, e2});
    Element et2 = e(e1, e2);
    CHECK(et1 == et2 * et2);
    Element et3 = e_prod(vector<Element>{one, one}, vector<Element>{e2, e2});
    Element et4 = e(one, e2);
    CHECK(et3 == et4 * et4);
    Element et5 = e_prod(vector<Element>{e1, one}, vector<Element>{e2, e2});
    CHECK(et5 == et2 * et4);
}

TEST_CASE_METHOD(Fixture, "element_pairing_fixed_g1")
{
    Element e1, e2, zr;
    CHECK_THROWS_AS(FixedG1Pairing(e1), NotInitializedError);
    zr.init_zr(pairing);
    CHECK_THROWS_AS(FixedG1Pairing(zr), ElementTypeError);
    e1.init_g1(pairing);
    e1.random();
    FixedG1Pairing p(e1);
    CHECK(p.get_g1() == e1);
    CHECK_THROWS_AS(p.apply(e2), NotInitializedError);
    e2.init_g2(pairing);
    e2.random();
    CHECK(p.apply(e2) == e(e1, e2));
}
