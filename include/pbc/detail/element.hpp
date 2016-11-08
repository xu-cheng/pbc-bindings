/*
  The MIT License (MIT)
  Copyright (c) 2016 Xu Cheng
  See licensing at:
    https://opensource.org/licenses/MIT
*/

#pragma once

#include <ostream>
#include <string>
#include "./backend.hpp"
#include "./element_type.hpp"
#include "./exceptions.hpp"
#include "./pairing.hpp"
#include "./streamopen.hpp"

namespace pbc
{
    class Element;
    class FixedG1Pairing;
    class FixedBasePower;

    typedef std::shared_ptr<Element> ElementPtr;

    class Element
    {
    public:
        Element() : _type(ElementType::NotInitialized), _pairing(nullptr) {}
        Element(const Element& e) : _type(e._type), _pairing(e._pairing)
        {
            if (_type != ElementType::NotInitialized) {
                backend::element_init_same_as(
                    &_element, const_cast<backend::element_s*>(e.c_element()));
                backend::element_set(
                    &_element, const_cast<backend::element_s*>(e.c_element()));
            }
        }
        Element(Element&& e) noexcept : _type(e._type), _pairing(e._pairing)
        {
            _element = e._element;
            e._type = ElementType::NotInitialized;
        }
        ~Element()
        {
            if (_type != ElementType::NotInitialized) {
                backend::element_clear(&_element);
                _type = ElementType::NotInitialized;
            }
        }

#define Element_Init_Func(type, type_lowercase)                                \
    void init_##type_lowercase(const PairingPtr& pairing)                       \
    {                                                                          \
        if (_type != ElementType::NotInitialized)                              \
            throw AlreadyInitializedError();                                   \
        backend::element_init_##type(                                          \
            &_element, const_cast<backend::pairing_s*>(pairing->c_pairing())); \
        _type = ElementType::type;                                             \
        _pairing = pairing;                                                    \
    }

        Element_Init_Func(G1, g1);
        Element_Init_Func(GT, gt);
        Element_Init_Func(Zr, zr);
#undef Element_Init_Func
        void init_g2(const PairingPtr& pairing)
        {
            if (_type != ElementType::NotInitialized)
                throw AlreadyInitializedError();
            backend::element_init_G2(&_element, const_cast<backend::pairing_s*>(
                                                    pairing->c_pairing()));
            if (pairing->symmetric())
                _type = ElementType::G1;
            else
                _type = ElementType::G2;
            _pairing = pairing;
        }
        void init_same_as(const Element& e)
        {
            if (_type != ElementType::NotInitialized)
                throw AlreadyInitializedError();
            if (e._type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_init_same_as(
                &_element, const_cast<backend::element_s*>(e.c_element()));
            _type = e._type;
            _pairing = e._pairing;
        }

        ElementType type() const { return _type; }
        PairingPtr pairing() const { return _pairing; }
        const backend::element_s* c_element() const { return &_element; }
        std::string to_str(int base = 10) const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            std::stringstream buf;
            streamopen::stream_fd_ptr sfd = streamopen::streamopen(buf);
            backend::element_out_str(
                sfd->fd, base, const_cast<backend::element_s*>(c_element()));
            streamopen::streamclose(sfd);
            return buf.str();
        }

        Element& operator=(Element&& e)
        {
            _element = e._element;
            _type = e._type;
            _pairing = e._pairing;
            e._type = ElementType::NotInitialized;
            return *this;
        }
        Element& operator=(const Element& e)
        {
            if (_type != ElementType::NotInitialized && _type != e._type) {
                backend::element_clear(&_element);
                _type = ElementType::NotInitialized;
            }
            if (_type == ElementType::NotInitialized &&
                e._type != ElementType::NotInitialized)
                backend::element_init_same_as(
                    &_element, const_cast<backend::element_s*>(e.c_element()));
            if (e._type != ElementType::NotInitialized)
                backend::element_set(
                    &_element, const_cast<backend::element_s*>(e.c_element()));
            _type = e._type;
            _pairing = e._pairing;
            return *this;
        }
        Element& operator=(int i)
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_set_si(&_element, i);
            return *this;
        }
        Element& random()
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_random(&_element);
            return *this;
        }
        Element& from_str(const std::string& s, int base = 10)
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_set_str(&_element, s.c_str(), base);
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
                           const_cast<backend::element_s*>(c_element()),
                           const_cast<backend::element_s*>(e.c_element())) == 0;
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
            backend::element_add(
                &out._element, const_cast<backend::element_s*>(c_element()),
                const_cast<backend::element_s*>(e.c_element()));
            return out;
        }
        Element& operator+=(const Element& e)
        {
            return operator=(operator+(e));
        }
        Element operator-(const Element& e) const
        {
            if (_type == ElementType::NotInitialized ||
                e._type == ElementType::NotInitialized)
                throw NotInitializedError();
            if (_type != e._type) throw ElementTypeError();
            Element out;
            out.init_same_as(*this);
            backend::element_sub(
                &out._element, const_cast<backend::element_s*>(c_element()),
                const_cast<backend::element_s*>(e.c_element()));
            return out;
        }
        Element& operator-=(const Element& e)
        {
            return operator=(operator-(e));
        }
        Element operator*(int i) const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            Element out;
            out.init_same_as(*this);
            backend::element_mul_si(
                &out._element, const_cast<backend::element_s*>(c_element()), i);
            return out;
        }
        Element operator*(const Element& e) const
        {
            if (_type == ElementType::NotInitialized ||
                e._type == ElementType::NotInitialized)
                throw NotInitializedError();
            Element out;
            out.init_same_as(*this);
            backend::element_mul(
                &out._element, const_cast<backend::element_s*>(c_element()),
                const_cast<backend::element_s*>(e.c_element()));
            return out;
        }
        Element& operator*=(int i) { return operator=(operator*(i)); }
        Element& operator*=(const Element& e)
        {
            return operator=(operator*(e));
        }
        Element operator/(const Element& e) const
        {
            if (_type == ElementType::NotInitialized ||
                e._type == ElementType::NotInitialized)
                throw NotInitializedError();
            if (_type != e._type) throw ElementTypeError();
            Element out;
            out.init_same_as(*this);
            backend::element_div(
                &out._element, const_cast<backend::element_s*>(c_element()),
                const_cast<backend::element_s*>(e.c_element()));
            return out;
        }
        Element& operator/=(const Element& e)
        {
            return operator=(operator/(e));
        }
        Element operator-() const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            Element out;
            out.init_same_as(*this);
            backend::element_neg(&out._element,
                                 const_cast<backend::element_s*>(c_element()));
            return out;
        }

        Element pow(const Element& e) const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            if (e._type != ElementType::Zr) throw ElementTypeError();
            Element out;
            out.init_same_as(*this);
            backend::element_pow_zn(
                &out._element, const_cast<backend::element_s*>(c_element()),
                const_cast<backend::element_s*>(e.c_element()));
            return out;
        }

        Element inverse() const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            Element out;
            out.init_same_as(*this);
            backend::element_invert(
                &out._element, const_cast<backend::element_s*>(c_element()));
            return out;
        }

        friend Element e(const Element&, const Element&);
        template <typename Container>
        friend typename std::enable_if<
            std::is_same<typename Container::value_type, Element>::value,
            Element>::type
        e_prod(const Container&, const Container&);
        friend FixedG1Pairing;
        friend FixedBasePower;

    private:
        backend::element_s _element;
        ElementType _type;
        PairingPtr _pairing;
    };

    std::ostream& operator<<(std::ostream& o, const Element& e)
    {
        o << e.to_str();
        return o;
    }
};
