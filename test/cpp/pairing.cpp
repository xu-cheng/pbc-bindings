#include "helper.h"

TEST_CASE_METHOD(Fixture, "init_from_param_and_str")
{
    PairingParamPtr param = PairingParam::init_from_str(type_a_param);
    CHECK_THROWS_AS(Pairing::init_from_param_str("type a"),
                    InitializationError);
    PairingPtr pairing1 = Pairing::init_from_param(param);
    PairingPtr pairing2 = Pairing::init_from_param_str(param->to_str());
    CHECK(pairing1->symmetric() == pairing2->symmetric());
    CHECK(pairing1->g1_bytes_length() == pairing2->g1_bytes_length());
    CHECK(pairing1->g2_bytes_length() == pairing2->g2_bytes_length());
}

TEST_CASE_METHOD(Fixture, "pairing_info")
{
    CHECK(pairing->symmetric() == true);
    CHECK(pairing->g1_bytes_length() == 128);
    CHECK(pairing->g1_x_only_bytes_length() == 64);
    CHECK(pairing->g1_compressed_bytes_length() == 65);
    CHECK(pairing->g2_bytes_length() == 128);
    CHECK(pairing->g2_x_only_bytes_length() == 64);
    CHECK(pairing->g2_compressed_bytes_length() == 65);
    CHECK(pairing->gt_bytes_length() == 128);
    CHECK(pairing->zr_bytes_length() == 20);
    CHECK(pairing->g1_order() ==
          mpz_class("730750818665451621361119245571504901405976559617"));
    CHECK(pairing->g2_order() ==
          mpz_class("730750818665451621361119245571504901405976559617"));
    CHECK(pairing->gt_order() ==
          mpz_class("730750818665451621361119245571504901405976559617"));
    CHECK(pairing->zr_order() ==
          mpz_class("730750818665451621361119245571504901405976559617"));
}
