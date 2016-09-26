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

    class NotInitializedError : public std::runtime_error
    {
    public:
        NotInitializedError() : runtime_error("Object is not initialized.") {}
    };

    class AlreadyInitializedError : public std::runtime_error
    {
    public:
        AlreadyInitializedError()
            : runtime_error("Object is already initialized.")
        {
        }
    };

    class InitializationError : public std::runtime_error
    {
    public:
        InitializationError() : runtime_error("Failed to initialize object.") {}
        InitializationError(const std::string& msg) : runtime_error(msg) {}
    };

    class ElementTypeError : public std::runtime_error
    {
    public:
        ElementTypeError() : runtime_error("Element has wrong type.") {}
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
        std::string to_str() const
        {
            std::stringstream buf;
            backend::stream_fd_ptr sfd = backend::streamopen(buf);
            backend::pbc_param_out_str(sfd->fd,
                                       *(backend::pbc_param_t*)&_param);
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

        bool symmetric() const
        {
            return backend::pairing_is_symmetric(
                *(backend::pairing_t*)&_pairing);
        }
        int g1_bytes_length() const
        {
            return backend::pairing_length_in_bytes_G1(
                *(backend::pairing_t*)&_pairing);
        }
        int g1_x_only_bytes_length() const
        {
            return backend::pairing_length_in_bytes_x_only_G1(
                *(backend::pairing_t*)&_pairing);
        }
        int g1_compressed_bytes_length() const
        {
            return backend::pairing_length_in_bytes_compressed_G1(
                *(backend::pairing_t*)&_pairing);
        }
        int g2_bytes_length() const
        {
            return backend::pairing_length_in_bytes_G2(
                *(backend::pairing_t*)&_pairing);
        }
        int g2_x_only_bytes_length() const
        {
            return backend::pairing_length_in_bytes_x_only_G2(
                *(backend::pairing_t*)&_pairing);
        }
        int g2_compressed_bytes_length() const
        {
            return backend::pairing_length_in_bytes_compressed_G2(
                *(backend::pairing_t*)&_pairing);
        }
        int gt_bytes_length() const
        {
            return backend::pairing_length_in_bytes_GT(
                *(backend::pairing_t*)&_pairing);
        }
        int zr_bytes_length() const
        {
            return backend::pairing_length_in_bytes_Zr(
                *(backend::pairing_t*)&_pairing);
        }

    private:
        friend class Element;
        backend::pairing_t _pairing;
    };

    enum class ElementType { G1, G2, GT, Zr, NotInitialized };

    std::ostream& operator<<(std::ostream& o, ElementType type)
    {
        switch (type) {
            case ElementType::G1: o << "G1"; break;
            case ElementType::G2: o << "G2"; break;
            case ElementType::GT: o << "GT"; break;
            case ElementType::Zr: o << "Zr"; break;
            case ElementType::NotInitialized: o << "NotInitialized"; break;
        }
        return o;
    }

    class Element
    {
    public:
        Element() : _type(ElementType::NotInitialized) {}
        Element(const Element& e) : _type(e._type)
        {
            if (_type != ElementType::NotInitialized) {
                backend::element_init_same_as(
                    _element, *(backend::element_t*)&e._element);
                backend::element_set(_element,
                                     *(backend::element_t*)&e._element);
            }
        }
        Element(Element&& e) noexcept : _type(e._type)
        {
            std::memcpy(&_element[0], &e._element[0],
                        sizeof(backend::element_t));
            e._type = ElementType::NotInitialized;
        }
        ~Element()
        {
            if (_type != ElementType::NotInitialized) {
                backend::element_clear(_element);
                _type = ElementType::NotInitialized;
            }
        }

#define Element_Init_Func(type, type_lowercase)                    \
    void init_##type_lowercase(const PairingPtr pairing)           \
    {                                                              \
        if (_type != ElementType::NotInitialized)                  \
            throw AlreadyInitializedError();                       \
        backend::element_init_##type(_element, pairing->_pairing); \
        _type = ElementType::type;                                 \
    }

        Element_Init_Func(G1, g1);
        Element_Init_Func(G2, g2);
        Element_Init_Func(GT, gt);
        Element_Init_Func(Zr, zr);
#undef Element_Init_Func

        void init_same_as(const Element& e)
        {
            if (_type != ElementType::NotInitialized)
                throw AlreadyInitializedError();
            if (e._type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_init_same_as(_element,
                                          *(backend::element_t*)&e._element);
            _type = e._type;
        }

        ElementType type() const { return _type; }
        std::string to_str(int base = 10) const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            std::stringstream buf;
            backend::stream_fd_ptr sfd = backend::streamopen(buf);
            backend::element_out_str(sfd->fd, base,
                                     *(backend::element_t*)&_element);
            backend::streamclose(sfd);
            return buf.str();
        }

        Element& operator=(Element&& e)
        {
            std::memcpy(&_element[0], &e._element[0],
                        sizeof(backend::element_t));
            _type = e._type;
            e._type = ElementType::NotInitialized;
            return *this;
        }
        Element& operator=(const Element& e)
        {
            if (_type != ElementType::NotInitialized && _type != e._type) {
                backend::element_clear(_element);
                if (e._type != ElementType::NotInitialized)
                    backend::element_init_same_as(
                        _element, *(backend::element_t*)&e._element);
            }
            if (e._type != ElementType::NotInitialized)
                backend::element_set(_element,
                                     *(backend::element_t*)&e._element);
            _type = e._type;
            return *this;
        }
        Element& operator=(int i)
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_set_si(_element, i);
            return *this;
        }
        Element& random()
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_random(_element);
            return *this;
        }
        Element& from_str(const std::string& s, int base = 10)
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_set_str(_element, s.c_str(), base);
            return *this;
        }

        bool operator==(const Element& e) const
        {
            if (_type == ElementType::NotInitialized ||
                e._type == ElementType::NotInitialized)
                return false;
            else if (_type != e._type)
                return false;
            else
                return backend::element_cmp(
                           *(backend::element_t*)&_element,
                           *(backend::element_t*)&e._element) == 0;
        }
        bool operator==(int i) const
        {
            if (_type == ElementType::NotInitialized) return false;
            Element e;
            e.init_same_as(*this);
            e = i;
            return operator==(e);
        }
        bool operator!=(const Element& e) const { return !operator==(e); }
        bool operator!=(int i) const { return !operator==(i); }
        Element operator+(const Element& e) const
        {
            if (_type == ElementType::NotInitialized ||
                e._type == ElementType::NotInitialized)
                throw NotInitializedError();
            if (_type != e._type) throw ElementTypeError();
            Element out;
            out.init_same_as(*this);
            backend::element_add(out._element, *(backend::element_t*)&_element,
                                 *(backend::element_t*)&e._element);
            return out;
        }
        Element operator+=(const Element& e)
        {
            operator=(operator+(e));
            return *this;
        }

        Element operator-(const Element& e) const
        {
            if (_type == ElementType::NotInitialized ||
                e._type == ElementType::NotInitialized)
                throw NotInitializedError();
            if (_type != e._type) throw ElementTypeError();
            Element out;
            out.init_same_as(*this);
            backend::element_sub(out._element, *(backend::element_t*)&_element,
                                 *(backend::element_t*)&e._element);
            return out;
        }
        Element operator-=(const Element& e)
        {
            operator=(operator-(e));
            return *this;
        }

        Element operator*(int i) const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            Element out;
            out.init_same_as(*this);
            backend::element_mul_si(out._element,
                                    *(backend::element_t*)&_element, i);
            return out;
        }
        Element operator*(const Element& e) const
        {
            if (_type == ElementType::NotInitialized ||
                e._type == ElementType::NotInitialized)
                throw NotInitializedError();
            if (e._type != ElementType::Zr) throw ElementTypeError();
            Element out;
            out.init_same_as(*this);
            backend::element_mul_zn(out._element,
                                    *(backend::element_t*)&_element,
                                    *(backend::element_t*)&e._element);
            return out;
        }
        Element operator*=(int i)
        {
            operator=(operator*(i));
            return *this;
        }
        Element operator*=(const Element& e)
        {
            operator=(operator*(e));
            return *this;
        }

        Element operator/(const Element& e) const
        {
            if (_type == ElementType::NotInitialized ||
                e._type == ElementType::NotInitialized)
                throw NotInitializedError();
            if (_type != e._type) throw ElementTypeError();
            Element out;
            out.init_same_as(*this);
            backend::element_div(out._element, *(backend::element_t*)&_element,
                                 *(backend::element_t*)&e._element);
            return out;
        }
        Element operator/=(const Element& e)
        {
            operator=(operator/(e));
            return *this;
        }

        Element operator-() const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            Element out;
            out.init_same_as(*this);
            backend::element_neg(out._element, *(backend::element_t*)&_element);
            return out;
        }

    private:
        backend::element_t _element;
        ElementType _type;
    };

    std::ostream& operator<<(std::ostream& o, const Element& e)
    {
        o << e.to_str();
        return o;
    }
};
