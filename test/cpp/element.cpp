#include "helper.h"

TEST_CASE_METHOD(Fixture, "move_constructor")
{
    Element e1;
    e1.init_zr(pairing);
    Element e2(move(e1));
    CHECK(e1.type() == ElementType::NotInitialized);
    CHECK(e2.type() == ElementType::Zr);
    Element e3 = move(e2);
    CHECK(e2.type() == ElementType::NotInitialized);
    CHECK(e3.type() == ElementType::Zr);
}

TEST_CASE_METHOD(Fixture, "init")
{
    Element g1, gt, zr, e;
    CHECK(g1.type() == ElementType::NotInitialized);
    g1.init_g1(pairing);
    CHECK(g1.type() == ElementType::G1);
    gt.init_gt(pairing);
    CHECK(gt.type() == ElementType::GT);
    zr.init_zr(pairing);
    CHECK(zr.type() == ElementType::Zr);
    CHECK_THROWS_AS(g1.init_g2(pairing), AlreadyInitializedError);
    CHECK_THROWS_AS(e.init_same_as(e), NotInitializedError);
    e.init_same_as(g1);
    CHECK(e.type() == ElementType::G1);
    CHECK_THROWS_AS(e.init_same_as(g1), AlreadyInitializedError);
}

TEST_CASE_METHOD(Fixture, "init_g2")
{
    Element e1, e2;
    e1.init_g2(pairing);
    CHECK(e1.type() == ElementType::G1);
    PairingParamPtr pp = PairingParam::gen_type_f();
    PairingPtr p = Pairing::init_from_param(pp);
    e2.init_g2(p);
    CHECK(e2.type() == ElementType::G2);
}

TEST_CASE_METHOD(Fixture, "swap")
{
    Element e1, e2;
    e2.init_zr(pairing);
    e2 = 42;
    e1.swap(e2);
    CHECK(e1.type() == ElementType::Zr);
    CHECK(e2.type() == ElementType::NotInitialized);
    CHECK(e1 == 42);
    CHECK(e1.pairing());
    CHECK(!e2.pairing());
}

TEST_CASE_METHOD(Fixture, "assign_and_compare")
{
    Element e1, e2, e3, e4;
    e1.init_zr(pairing);
    e2.init_zr(pairing);
    e3.init_zr(pairing);
    e4.init_g1(pairing);
    e1 = 0;
    e2 = e1;
    e3 = 1;
    e4 = 0;
    CHECK(e1 == e2);
    CHECK(e1 == 0);
    CHECK(e1 != e3);
    CHECK(e1 != 1);
    CHECK(e1 != e4);
    e4 = e1;
    CHECK(e1 == e4);

    Element e5, e6;
    e5 = e1;
    CHECK(e5 == e1);
    CHECK_THROWS_AS(e6 = 1, NotInitializedError);
}

TEST_CASE_METHOD(Fixture, "random")
{
    Element e;
    e.init_g1(pairing);
    e.random();
    CHECK(e != 0);
}

TEST_CASE_METHOD(Fixture, "from_integer_and_to_from_mpz_class")
{
    Element e1 = Element::from_integer(pairing, 42);
    CHECK(e1 == 42);
    Element e2 = Element::from_mpz_class(pairing, mpz_class(42));
    CHECK(e2 == mpz_class(42));
    mpz_class z = e2.to_mpz_class();
    CHECK(z == 42);
}

TEST_CASE_METHOD(Fixture, "to_and_from_str")
{
    Element e1, e2, e3, e4;
    e1.init_g1(pairing);
    e2.init_g2(pairing);
    e3.init_gt(pairing);
    e4.init_zr(pairing);
    e1.random();
    e2.random();
    e3.random();
    e4.random();
    CHECK(Element::from_str(pairing, e1.type(), e1.to_str()) == e1);
    CHECK(Element::from_str(pairing, e2.type(), e2.to_str()) == e2);
    CHECK(Element::from_str(pairing, e3.type(), e3.to_str()) == e3);
    CHECK(Element::from_str(pairing, e4.type(), e4.to_str()) == e4);

    Element e;
    CHECK_THROWS_AS(e.to_str(), NotInitializedError);
    CHECK_THROWS_AS(Element::from_str(pairing, ElementType::NotInitialized, ""),
                    ElementTypeError);
}

TEST_CASE_METHOD(Fixture, "to_and_from_byptes")
{
    Element e1, e2, e3, e4;
    e1.init_g1(pairing);
    e2.init_g2(pairing);
    e3.init_gt(pairing);
    e4.init_zr(pairing);
    e1.random();
    e2.random();
    e3.random();
    e4.random();
    CHECK(Element::from_bytes(pairing, e1.type(), e1.to_bytes()) == e1);
    CHECK(Element::from_bytes(pairing, e2.type(), e2.to_bytes()) == e2);
    CHECK(Element::from_bytes(pairing, e3.type(), e3.to_bytes()) == e3);
    CHECK(Element::from_bytes(pairing, e4.type(), e4.to_bytes()) == e4);
    CHECK(Element::from_bytes_compressed(pairing, e1.type(),
                                         e1.to_bytes_compressed()) == e1);
    CHECK(Element::from_bytes_compressed(pairing, e2.type(),
                                         e2.to_bytes_compressed()) == e2);

    Element e;
    CHECK_THROWS_AS(e.to_bytes(), NotInitializedError);
    CHECK_THROWS_AS(e.to_bytes_compressed(), NotInitializedError);
    CHECK_THROWS_AS(e3.to_bytes_compressed(), ElementTypeError);
    CHECK_THROWS_AS(e4.to_bytes_compressed(), ElementTypeError);
    CHECK_THROWS_AS(
        Element::from_bytes(pairing, ElementType::NotInitialized, string()),
        ElementTypeError);
    CHECK_THROWS_AS(Element::from_bytes_compressed(
                        pairing, ElementType::NotInitialized, string()),
                    ElementTypeError);
}

TEST_CASE_METHOD(Fixture, "random_method")
{
    Element e;
    CHECK_THROWS_AS(e.random(), NotInitializedError);
    e.init_zr(pairing);
    e.random();
    CHECK(e != 0);
}

TEST_CASE_METHOD(Fixture, "set_zero_and_one")
{
    Element e;
    CHECK_THROWS_AS(e.random(), NotInitializedError);
    e.init_g1(pairing);
    e.set_zero();
    CHECK(e.is_zero());
    e.set_one();
    CHECK(e.is_one());
}

TEST_CASE_METHOD(Fixture, "arithmetic")
{
    Element e1, e2, e3, e4;
    e1.init_g1(pairing);
    e2.init_zr(pairing);
    e3.init_zr(pairing);
    e1.random();
    e2 = 7;
    e3 = 6;

    CHECK_THROWS_AS(e4 + e1, NotInitializedError);
    CHECK_THROWS_AS(e4 - e1, NotInitializedError);
    CHECK_THROWS_AS(e4 * e2, NotInitializedError);
    CHECK_THROWS_AS(e4 * 2, NotInitializedError);
    CHECK_THROWS_AS(e4 / e1, NotInitializedError);
    CHECK_THROWS_AS(e4.pow(e2), NotInitializedError);
    CHECK_THROWS_AS(e4 += e1, NotInitializedError);
    CHECK_THROWS_AS(e4 -= e1, NotInitializedError);
    CHECK_THROWS_AS(e4 *= e2, NotInitializedError);
    CHECK_THROWS_AS(e4 *= 2, NotInitializedError);
    CHECK_THROWS_AS(e4 /= e1, NotInitializedError);

    CHECK_THROWS_AS(e2 + e1, ElementTypeError);
    CHECK_THROWS_AS(e2 - e1, ElementTypeError);
    CHECK_THROWS_AS(e2 / e1, ElementTypeError);
    CHECK_THROWS_AS(e2 += e1, ElementTypeError);
    CHECK_THROWS_AS(e2 -= e1, ElementTypeError);
    CHECK_THROWS_AS(e2 /= e1, ElementTypeError);

    CHECK(e2 + e3 == 13);
    e2 += e3;
    CHECK(e2 == 13);
    CHECK(e2 - e3 == 7);
    e2 -= e3;
    CHECK(e2 == 7);
    CHECK(e2 * e3 == 42);
    e2 *= e3;
    CHECK(e2 == 42);
    CHECK(e2 / e3 == 7);
    e2 /= e3;
    CHECK(e2 == 7);
    e2 = e2.pow(e3);
    CHECK(e2 == 117649);
    e2 = e3.pow(2);
    CHECK(e2 == 36);
    CHECK(-e3 == -6);
}

TEST_CASE_METHOD(Fixture, "inverse")
{
    Element e1, e2;
    e1.init_zr(pairing);
    e2.init_zr(pairing);
    e1 = 2;
    e2 = 89;
    e1 = e1.pow(e2);
    e2 = 1;
    e1 -= e2;
    e1 = e1.inverse();
    CHECK(e1.to_str(16) == "6de0243f778127dbbe487f6f0221fb7c090fede0");
}
