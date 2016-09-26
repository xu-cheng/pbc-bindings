/*
  The MIT License (MIT)
  Copyright (c) 2016 Xu Cheng
  See licensing at:
    https://opensource.org/licenses/MIT
*/

#pragma once

#include <exception>
#include <string>
#include "pbc-streamopen.hpp"

namespace pbc
{
    namespace backend
    {
#include <pbc/pbc.h>
    };

    class NotAllocatedException : public std::runtime_error
    {
    public:
        NotAllocatedException() : runtime_error("Object is not allocated.") {}
    };

    class AlreadyAllocatedException : public std::runtime_error
    {
    public:
        AlreadyAllocatedException()
            : runtime_error("Object is already allocated.")
        {
        }
    };

    class InitializationError : public std::runtime_error
    {
    public:
        InitializationError() : runtime_error("Failed to initialize object.") {}
        InitializationError(const std::string& msg) : runtime_error(msg) {}
    };

    class Pairing;
    class PairingParam;
    class Element;
    typedef std::shared_ptr<Pairing> PairingPtr;
    typedef std::shared_ptr<PairingParam> PairingParamPtr;
    typedef std::shared_ptr<Element> ElementPtr;

    class PairingParam
    {
    public:
        PairingParam() = default;
        PairingParam(const PairingParam&) = delete;
        ~PairingParam() { backend::pbc_param_clear(_param); }
        std::string to_str()
        {
            std::stringstream buf;
            backend::stream_fd_ptr sfd = backend::streamopen(buf);
            backend::pbc_param_out_str(sfd->fd, _param);
            backend::streamclose(sfd);
            return buf.str();
        }

        static PairingParamPtr init_from_str(const std::string& param_str)
        {
            PairingParamPtr ptr = std::make_shared<PairingParam>();
            if (backend::pbc_param_init_set_str(ptr->_param,
                                                param_str.c_str()) != 0)
                throw InitializationError(
                    "Failed to initialize pairing parameter.");
            return ptr;
        }
        static PairingParamPtr gen_type_a(int rbits = 160, int qbits = 512)
        {
            PairingParamPtr ptr = std::make_shared<PairingParam>();
            backend::pbc_param_init_a_gen(ptr->_param, rbits, qbits);
            return ptr;
        }
        static PairingParamPtr gen_type_i(int group_size = 696)
        {
            PairingParamPtr ptr = std::make_shared<PairingParam>();
            backend::pbc_param_init_i_gen(ptr->_param, group_size);
            return ptr;
        }
        static PairingParamPtr gen_type_e(int rbits = 160, int qbits = 1024)
        {
            PairingParamPtr ptr = std::make_shared<PairingParam>();
            backend::pbc_param_init_e_gen(ptr->_param, rbits, qbits);
            return ptr;
        }
        static PairingParamPtr gen_type_f(int bits = 160)
        {
            PairingParamPtr ptr = std::make_shared<PairingParam>();
            backend::pbc_param_init_f_gen(ptr->_param, bits);
            return ptr;
        }

    private:
        friend class Pairing;
        backend::pbc_param_t _param;
    };

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
            backend::pairing_init_pbc_param(ptr->_pairing, param->_param);
            return ptr;
        }

        bool symmetric() { return backend::pairing_is_symmetric(_pairing); }
        int g1_bytes_length()
        {
            return backend::pairing_length_in_bytes_G1(_pairing);
        }
        int g1_x_only_bytes_length()
        {
            return backend::pairing_length_in_bytes_x_only_G1(_pairing);
        }
        int g1_compressed_bytes_length()
        {
            return backend::pairing_length_in_bytes_compressed_G1(_pairing);
        }
        int g2_bytes_length()
        {
            return backend::pairing_length_in_bytes_G2(_pairing);
        }
        int g2_x_only_bytes_length()
        {
            return backend::pairing_length_in_bytes_x_only_G2(_pairing);
        }
        int g2_compressed_bytes_length()
        {
            return backend::pairing_length_in_bytes_compressed_G2(_pairing);
        }
        int gt_bytes_length()
        {
            return backend::pairing_length_in_bytes_GT(_pairing);
        }
        int zr_bytes_length()
        {
            return backend::pairing_length_in_bytes_Zr(_pairing);
        }

    private:
        backend::pairing_t _pairing;
    };

    class Element
    {
    public:
        Element() : _allocated(false), _size(0) {}
        Element(const Element& e) : _allocated(e._allocated), _size(e._size)
        {
            if (_allocated) {
                backend::element_init_same_as(
                    _element, *(backend::element_t*)&e._element);
                backend::element_set(_element,
                                     *(backend::element_t*)&e._element);
            }
        }
        ~Element()
        {
            if (_allocated) {
                backend::element_clear(_element);
                _allocated = false;
                _size = 0;
            }
        }

    private:
        bool _allocated;
        std::size_t _size;
        backend::element_t _element;
    };
};
