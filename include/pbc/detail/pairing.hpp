/*
  The MIT License (MIT)
  Copyright (c) 2016 Xu Cheng
  See licensing at:
    https://opensource.org/licenses/MIT
*/

#pragma once

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
        Pairing(const Pairing&) = delete;
        ~Pairing() { pairing_clear(_pairing); }
        static PairingPtr init_from_param_str(const std::string& param_str)
        {
            return Pairing::init_from_param(
                PairingParam::init_from_str(param_str));
            PairingPtr ptr = std::make_shared<Pairing>();
            if (backend::pairing_init_set_str(ptr->_pairing,
                                              param_str.c_str()) != 0)
                throw InitializationError("Failed to initialize pairing.");
            return ptr;
        }
        static PairingPtr init_from_param(const PairingParamPtr& param)
        {
            PairingPtr ptr = std::make_shared<Pairing>();
            backend::pairing_init_pbc_param(ptr->_pairing, param->c_param());
            return ptr;
        }

        backend::pairing_ptr c_pairing() const
        {
            return (backend::pairing_ptr)&_pairing[0];
        }
        bool symmetric() const
        {
            return backend::pairing_is_symmetric(c_pairing());
        }
        int g1_bytes_length() const
        {
            return backend::pairing_length_in_bytes_G1(c_pairing());
        }
        int g1_x_only_bytes_length() const
        {
            return backend::pairing_length_in_bytes_x_only_G1(c_pairing());
        }
        int g1_compressed_bytes_length() const
        {
            return backend::pairing_length_in_bytes_compressed_G1(c_pairing());
        }
        int g2_bytes_length() const
        {
            return backend::pairing_length_in_bytes_G2(c_pairing());
        }
        int g2_x_only_bytes_length() const
        {
            return backend::pairing_length_in_bytes_x_only_G2(c_pairing());
        }
        int g2_compressed_bytes_length() const
        {
            return backend::pairing_length_in_bytes_compressed_G2(c_pairing());
        }
        int gt_bytes_length() const
        {
            return backend::pairing_length_in_bytes_GT(c_pairing());
        }
        int zr_bytes_length() const
        {
            return backend::pairing_length_in_bytes_Zr(c_pairing());
        }

    private:
        backend::pairing_t _pairing;
    };
};
