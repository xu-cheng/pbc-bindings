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
#include "./streamopen.hpp"

namespace pbc
{
    class PairingParam;

    typedef std::shared_ptr<PairingParam> PairingParamPtr;

    class PairingParam
    {
    public:
        PairingParam() = default;
        PairingParam(const PairingParam&) = delete;
        ~PairingParam() { backend::pbc_param_clear(_param); }
        std::string to_str() const
        {
            std::stringstream buf;
            streamopen::stream_fd_ptr sfd = streamopen::streamopen(buf);
            backend::pbc_param_out_str(sfd->fd,
                                       *(backend::pbc_param_t*)&_param);
            streamopen::streamclose(sfd);
            return buf.str();
        }
        backend::pbc_param_ptr c_param() const
        {
            return (backend::pbc_param_ptr)&_param[0];
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
        backend::pbc_param_t _param;
    };
}
