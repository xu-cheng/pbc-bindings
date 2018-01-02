/*
  The MIT License (MIT)
  Copyright (c) 2016-2018 Xu Cheng
  See licensing at:
    https://opensource.org/licenses/MIT
*/

#pragma once

#include <gmpxx.h>
#include <memory>
#include <string>
#include "./backend.hpp"
#include "./exceptions.hpp"
#include "./pairing_param.hpp"

namespace pbc
{
    class Pairing;

    typedef std::shared_ptr<Pairing> PairingPtr;

    class Pairing
    {
    public:
        Pairing() = default;
        Pairing(const backend::pairing_s& pairing) : _pairing(pairing) {}
        Pairing(const Pairing&) = delete;
        ~Pairing() { pairing_clear(&_pairing); }
        static PairingPtr init_from_param_str(const std::string& param_str)
        {
            return Pairing::init_from_param(
                PairingParam::init_from_str(param_str));
            PairingPtr ptr = std::make_shared<Pairing>();
            if (backend::pairing_init_set_str(&ptr->_pairing,
                                              param_str.c_str()) != 0)
                throw InitializationError("Failed to initialize pairing.");
            return ptr;
        }
        static PairingPtr init_from_param(const PairingParamPtr& param)
        {
            PairingPtr ptr = std::make_shared<Pairing>();
            backend::pairing_init_pbc_param(
                &ptr->_pairing,
                const_cast<backend::pbc_param_s*>(param->c_param()));
            return ptr;
        }

        backend::pairing_s* c_pairing() { return &_pairing; }
        const backend::pairing_s* c_pairing() const { return &_pairing; }
        bool symmetric() const
        {
            return backend::pairing_is_symmetric(
                const_cast<backend::pairing_s*>(c_pairing()));
        }
        int g1_bytes_length() const
        {
            return backend::pairing_length_in_bytes_G1(
                const_cast<backend::pairing_s*>(c_pairing()));
        }
        int g1_x_only_bytes_length() const
        {
            return backend::pairing_length_in_bytes_x_only_G1(
                const_cast<backend::pairing_s*>(c_pairing()));
        }
        int g1_compressed_bytes_length() const
        {
            return backend::pairing_length_in_bytes_compressed_G1(
                const_cast<backend::pairing_s*>(c_pairing()));
        }
        int g2_bytes_length() const
        {
            return backend::pairing_length_in_bytes_G2(
                const_cast<backend::pairing_s*>(c_pairing()));
        }
        int g2_x_only_bytes_length() const
        {
            return backend::pairing_length_in_bytes_x_only_G2(
                const_cast<backend::pairing_s*>(c_pairing()));
        }
        int g2_compressed_bytes_length() const
        {
            return backend::pairing_length_in_bytes_compressed_G2(
                const_cast<backend::pairing_s*>(c_pairing()));
        }
        int gt_bytes_length() const
        {
            return backend::pairing_length_in_bytes_GT(
                const_cast<backend::pairing_s*>(c_pairing()));
        }
        int zr_bytes_length() const
        {
            return backend::pairing_length_in_bytes_Zr(
                const_cast<backend::pairing_s*>(c_pairing()));
        }
        mpz_class g1_order() const { return mpz_class(_pairing.G1->order); }
        mpz_class g2_order() const { return mpz_class(_pairing.G2->order); }
        mpz_class gt_order() const { return mpz_class(_pairing.GT->order); }
        mpz_class zr_order() const { return mpz_class(_pairing.Zr[0].order); }

    private:
        backend::pairing_s _pairing;
    };
}  // namespace pbc
