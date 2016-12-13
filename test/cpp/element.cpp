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
    Element e2(move(e1));
    BOOST_TEST(e1.type() == ElementType::NotInitialized);
    BOOST_TEST(e2.type() == ElementType::Zr);
    Element e3 = move(e2);
    BOOST_TEST(e2.type() == ElementType::NotInitialized);
    BOOST_TEST(e3.type() == ElementType::Zr);
}

BOOST_AUTO_TEST_CASE(init)
{
    Element g1, gt, zr, e;
    BOOST_TEST(g1.type() == ElementType::NotInitialized);
    g1.init_g1(pairing);
    BOOST_TEST(g1.type() == ElementType::G1);
    gt.init_gt(pairing);
    BOOST_TEST(gt.type() == ElementType::GT);
    zr.init_zr(pairing);
    BOOST_TEST(zr.type() == ElementType::Zr);
    BOOST_CHECK_THROW(g1.init_g2(pairing), AlreadyInitializedError);
    BOOST_CHECK_THROW(e.init_same_as(e), NotInitializedError);
    e.init_same_as(g1);
    BOOST_TEST(e.type() == ElementType::G1);
    BOOST_CHECK_THROW(e.init_same_as(g1), AlreadyInitializedError);
}

BOOST_AUTO_TEST_CASE(init_g2)
{
    Element e1, e2;
    e1.init_g2(pairing);
    BOOST_TEST(e1.type() == ElementType::G1);
    PairingParamPtr pp = PairingParam::gen_type_f();
    PairingPtr p = Pairing::init_from_param(pp);
    e2.init_g2(p);
    BOOST_TEST(e2.type() == ElementType::G2);
}

BOOST_AUTO_TEST_CASE(assign_and_compare)
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
    BOOST_TEST(e1 == e2);
    BOOST_TEST(e1 == 0);
    BOOST_TEST(e1 != e3);
    BOOST_TEST(e1 != 1);
    BOOST_TEST(e1 != e4);
    e4 = e1;
    BOOST_TEST(e1 == e4);

    Element e5, e6;
    e5 = e1;
    BOOST_TEST(e5 == e1);
    BOOST_CHECK_THROW(e6 = 1, NotInitializedError);
}

BOOST_AUTO_TEST_CASE(random)
{
    Element e;
    e.init_g1(pairing);
    e.random();
    BOOST_TEST(e != 0);
}

BOOST_AUTO_TEST_CASE(from_integer_and_to_from_mpz_class)
{
    Element e1 = Element::from_integer(pairing, 42);
    BOOST_TEST(e1 == 42);
    Element e2 = Element::from_mpz_class(pairing, mpz_class(42));
    BOOST_TEST(e2 == 42);
    mpz_class z = e2.to_mpz_class();
    BOOST_TEST(z == 42);
}

BOOST_AUTO_TEST_CASE(to_and_from_str)
{
    Element e1, e2, e3, e4;
    e1.init_g1(pairing);
    e2.init_zr(pairing);
    e1.random();
    e2 = 42;
    BOOST_TEST(Element::from_str(pairing, e1.type(), e1.to_str()) == e1);
    BOOST_TEST(Element::from_str(pairing, e2.type(), e2.to_str()) == e2);

    Element e;
    BOOST_CHECK_THROW(e.to_str(), NotInitializedError);
    BOOST_CHECK_THROW(
        Element::from_str(pairing, ElementType::NotInitialized, ""),
        ElementTypeError);
}

BOOST_AUTO_TEST_CASE(to_and_from_byptes)
{
    Element e1, e2, e3, e4;
    e1.init_g1(pairing);
    e2.init_zr(pairing);
    e1.random();
    e2 = 42;
    BOOST_TEST(Element::from_bytes(pairing, e1.type(), e1.to_bytes()) == e1);
    BOOST_TEST(Element::from_bytes(pairing, e2.type(), e2.to_bytes()) == e2);

    Element e;
    BOOST_CHECK_THROW(e.to_bytes(), NotInitializedError);
    BOOST_CHECK_THROW(Element::from_bytes(pairing, ElementType::NotInitialized,
                                          basic_string<unsigned char>()),
                      ElementTypeError);
}

BOOST_AUTO_TEST_CASE(random_method)
{
    Element e;
    BOOST_CHECK_THROW(e.random(), NotInitializedError);
    e.init_zr(pairing);
    e.random();
    BOOST_TEST(e != 0);
}

BOOST_AUTO_TEST_CASE(set_zero_and_one)
{
    Element e;
    BOOST_CHECK_THROW(e.random(), NotInitializedError);
    e.init_zr(pairing);
    e.set_zero();
    BOOST_TEST(e == 0);
    e.set_one();
    BOOST_TEST(e == 1);
}

BOOST_AUTO_TEST_CASE(arithmetic)
{
    Element e1, e2, e3, e4;
    e1.init_g1(pairing);
    e2.init_zr(pairing);
    e3.init_zr(pairing);
    e1.random();
    e2 = 7;
    e3 = 6;

    BOOST_CHECK_THROW(e4 + e1, NotInitializedError);
    BOOST_CHECK_THROW(e4 - e1, NotInitializedError);
    BOOST_CHECK_THROW(e4 * e2, NotInitializedError);
    BOOST_CHECK_THROW(e4 * 2, NotInitializedError);
    BOOST_CHECK_THROW(e4 / e1, NotInitializedError);
    BOOST_CHECK_THROW(e4.pow(e2), NotInitializedError);
    BOOST_CHECK_THROW(e4 += e1, NotInitializedError);
    BOOST_CHECK_THROW(e4 -= e1, NotInitializedError);
    BOOST_CHECK_THROW(e4 *= e2, NotInitializedError);
    BOOST_CHECK_THROW(e4 *= 2, NotInitializedError);
    BOOST_CHECK_THROW(e4 /= e1, NotInitializedError);

    BOOST_CHECK_THROW(e2 + e1, ElementTypeError);
    BOOST_CHECK_THROW(e2 - e1, ElementTypeError);
    BOOST_CHECK_THROW(e2 / e1, ElementTypeError);
    BOOST_CHECK_THROW(e2 += e1, ElementTypeError);
    BOOST_CHECK_THROW(e2 -= e1, ElementTypeError);
    BOOST_CHECK_THROW(e2 /= e1, ElementTypeError);

    BOOST_TEST(e2 + e3 == 13);
    e2 += e3;
    BOOST_TEST(e2 == 13);
    BOOST_TEST(e2 - e3 == 7);
    e2 -= e3;
    BOOST_TEST(e2 == 7);
    BOOST_TEST(e2 * e3 == 42);
    e2 *= e3;
    BOOST_TEST(e2 == 42);
    BOOST_TEST(e2 / e3 == 7);
    e2 /= e3;
    BOOST_TEST(e2 == 7);
    e2 = e2.pow(e3);
    BOOST_TEST(e2 == 117649);
    e2 = e3.pow(2);
    BOOST_TEST(e2 == 36);
    BOOST_TEST(-e3 == -6);
}

BOOST_AUTO_TEST_CASE(inverse)
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
    BOOST_TEST(e1.to_str(16) == "6de0243f778127dbbe487f6f0221fb7c090fede0");
}

BOOST_AUTO_TEST_SUITE_END()
