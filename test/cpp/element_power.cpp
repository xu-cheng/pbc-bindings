#include "helper.h"

TEST_CASE_METHOD(Fixture, "element_power")
{
    Element base, e, z;
    CHECK_THROWS_AS(FixedBasePower(base), NotInitializedError);
    base.init_g1(pairing);
    base.random();
    FixedBasePower p(base);
    CHECK(p.get_base() == base);
    CHECK_THROWS_AS(p.apply(e), NotInitializedError);
    e.init_g1(pairing);
    CHECK_THROWS_AS(p.apply(e), ElementTypeError);
    z.init_zr(pairing);
    z.random();
    CHECK(p.apply(z) == base.pow(z));
    CHECK(p.pow(z) == base.pow(z));
    CHECK(p.apply(2) == base.pow(2));
    CHECK(p.pow(2) == base.pow(2));
}

TEST_CASE_METHOD(Fixture, "element_pow2")
{
    Element a1, n1, a2, n2;
    CHECK_THROWS_AS(pow2(a1, n1, a2, n2), NotInitializedError);
    a1.init_g1(pairing);
    a1.random();
    a2.init_g1(pairing);
    a2.random();
    CHECK_THROWS_AS(pow2(a1, a1, a2, a2), ElementTypeError);
    n1.init_zr(pairing);
    n1.random();
    n2.init_zr(pairing);
    n2.random();
    CHECK(pow2(a1, n1, a2, n2) == a1.pow(n1) * a2.pow(n2));
}

TEST_CASE_METHOD(Fixture, "element_pow3")
{
    Element a1, n1, a2, n2, a3, n3;
    CHECK_THROWS_AS(pow3(a1, n1, a2, n2, a3, n3), NotInitializedError);
    a1.init_g1(pairing);
    a1.random();
    a2.init_g1(pairing);
    a2.random();
    a3.init_g1(pairing);
    a3.random();
    CHECK_THROWS_AS(pow3(a1, a1, a2, a2, a3, a3), ElementTypeError);
    n1.init_zr(pairing);
    n1.random();
    n2.init_zr(pairing);
    n2.random();
    n3.init_zr(pairing);
    n3.random();
    CHECK(pow3(a1, n1, a2, n2, a3, n3) == a1.pow(n1) * a2.pow(n2) * a3.pow(n3));
}
