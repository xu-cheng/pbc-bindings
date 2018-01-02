/*
  The MIT License (MIT)
  Copyright (c) 2016-2018 Xu Cheng
  See licensing at:
    https://opensource.org/licenses/MIT
*/

#pragma once

#include <gmpxx.h>
#include <memory>
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
        Element(const PairingPtr& pairing, const ElementType& type)
            : _type(type), _pairing(pairing)
        {
            switch (type) {
                case ElementType::G1:
                    backend::element_init_G1(
                        &_element,
                        const_cast<backend::pairing_s*>(pairing->c_pairing()));
                    break;
                case ElementType::G2:
                    backend::element_init_G2(
                        &_element,
                        const_cast<backend::pairing_s*>(pairing->c_pairing()));

                    if (pairing->symmetric()) _type = ElementType::G1;
                    break;
                case ElementType::GT:
                    backend::element_init_GT(
                        &_element,
                        const_cast<backend::pairing_s*>(pairing->c_pairing()));
                    break;
                case ElementType::Zr:
                    backend::element_init_Zr(
                        &_element,
                        const_cast<backend::pairing_s*>(pairing->c_pairing()));
                    break;
                case ElementType::NotInitialized: break;
            }
        }
        Element(const PairingPtr& pairing, const ElementType& type,
                const backend::element_s& element)
            : _element(element), _type(type), _pairing(pairing)
        {
        }
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

        static Element from_integer(const PairingPtr& pairing, int value)
        {
            Element out(pairing, ElementType::Zr);
            backend::element_init_Zr(
                &out._element,
                const_cast<backend::pairing_s*>(pairing->c_pairing()));
            backend::element_set_si(&out._element, value);
            return out;
        }
        static Element from_mpz_class(const PairingPtr& pairing,
                                      const mpz_class& value)
        {
            Element out(pairing, ElementType::Zr);
            backend::element_init_Zr(
                &out._element,
                const_cast<backend::pairing_s*>(pairing->c_pairing()));
            backend::element_set_mpz(
                &out._element, const_cast<__mpz_struct*>(value.get_mpz_t()));
            return out;
        }
        static Element from_str(const PairingPtr& pairing,
                                const ElementType& type,
                                const std::string& data, int base = 10)
        {
            if (type == ElementType::NotInitialized) throw ElementTypeError();
            Element out(pairing, type);
            backend::element_set_str(&out._element, data.c_str(), base);
            return out;
        }
        static Element from_bytes(const PairingPtr& pairing,
                                  const ElementType& type,
                                  const std::string& data)
        {
            if (type == ElementType::NotInitialized) throw ElementTypeError();
            Element out(pairing, type);
            backend::element_from_bytes(
                &out._element,
                const_cast<unsigned char*>(
                    reinterpret_cast<const unsigned char*>(data.data())));
            return out;
        }
        static Element from_bytes_compressed(const PairingPtr& pairing,
                                             const ElementType& type,
                                             const std::string& data)
        {
            if (type == ElementType::NotInitialized) throw ElementTypeError();
            Element out(pairing, type);
            backend::element_from_bytes_compressed(
                &out._element,
                const_cast<unsigned char*>(
                    reinterpret_cast<const unsigned char*>(data.data())));
            return out;
        }

#define Element_Init_Func(type, type_lowercase)                                \
    void init_##type_lowercase(const PairingPtr& pairing)                      \
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
        void swap(Element& e)
        {
            std::swap(_type, e._type);
            std::swap(_pairing, e._pairing);
            std::swap(_element, e._element);
        }

        ElementType type() const { return _type; }
        PairingPtr pairing() const { return _pairing; }
        backend::element_s* c_element() { return &_element; }
        const backend::element_s* c_element() const { return &_element; }
        int bytes_length() const
        {
            if (_type == ElementType::NotInitialized)
                return 0;
            else
                return backend::element_length_in_bytes(
                    const_cast<backend::element_s*>(c_element()));
        }
        int compressed_bytes_length() const
        {
            if (_type == ElementType::NotInitialized)
                return 0;
            else
                return backend::element_length_in_bytes_compressed(
                    const_cast<backend::element_s*>(c_element()));
        }
        mpz_class to_mpz_class() const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            if (_type != ElementType::Zr) throw ElementTypeError();
            mpz_class out;
            backend::element_to_mpz(
                out.get_mpz_t(), const_cast<backend::element_s*>(c_element()));
            return out;
        }
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
        std::string to_bytes() const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            std::string out;
            out.resize(bytes_length());
            backend::element_to_bytes(
                reinterpret_cast<unsigned char*>(&out.front()),
                const_cast<backend::element_s*>(c_element()));
            return out;
        }
        std::string to_bytes_compressed() const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            if (_type == ElementType::Zr || _type == ElementType::GT)
                throw ElementTypeError();
            std::string out;
            out.resize(compressed_bytes_length());
            backend::element_to_bytes_compressed(
                reinterpret_cast<unsigned char*>(&out.front()),
                const_cast<backend::element_s*>(c_element()));
            return out;
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
        Element& operator=(int value)
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_set_si(&_element, value);
            return *this;
        }
        Element& random()
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_random(&_element);
            return *this;
        }
        Element& set_zero()
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_set0(&_element);
            return *this;
        }
        Element& set_one()
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_set1(&_element);
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
        bool operator==(int value) const
        {
            if (_type != ElementType::Zr) return false;
            Element e = from_integer(_pairing, value);
            return operator==(e);
        }
        bool operator==(const mpz_class& value) const
        {
            if (_type != ElementType::Zr) return false;
            Element e = from_mpz_class(_pairing, value);
            return operator==(e);
        }
        bool operator!=(const Element& e) const { return !operator==(e); }
        bool operator!=(int value) const { return !operator==(value); }
        bool is_zero() const
        {
            if (_type == ElementType::NotInitialized) return false;
            return backend::element_is0(
                const_cast<backend::element_s*>(c_element()));
        }
        bool is_one() const
        {
            if (_type == ElementType::NotInitialized) return false;
            return backend::element_is1(
                const_cast<backend::element_s*>(c_element()));
        }

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
        Element operator*(int value) const
        {
            if (_type == ElementType::NotInitialized)
                throw NotInitializedError();
            Element out;
            out.init_same_as(*this);
            backend::element_mul_si(
                &out._element, const_cast<backend::element_s*>(c_element()),
                value);
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
        Element& operator*=(int value) { return operator=(operator*(value)); }
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
        Element pow(int value) const
        {
            return pow(Element::from_integer(_pairing, value));
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
        friend Element pow2(const Element&, const Element&, const Element&,
                            const Element&);
        friend Element pow3(const Element&, const Element&, const Element&,
                            const Element&, const Element&, const Element&);
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
}  // namespace pbc
