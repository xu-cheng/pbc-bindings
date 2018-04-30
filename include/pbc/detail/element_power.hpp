/*
  The MIT License (MIT)
  Copyright (c) 2016-2018 Xu Cheng
  See licensing at:
    https://opensource.org/licenses/MIT
*/

#pragma once

#include <memory>
#include <ostream>
#include <string>
#include "./backend.hpp"
#include "./element.hpp"
#include "./exceptions.hpp"
#include "./pairing.hpp"

namespace pbc
{
    class FixedBasePower;

    typedef std::shared_ptr<FixedBasePower> FixedBasePowerPtr;

    class FixedBasePower
    {
    public:
        FixedBasePower(const Element& base) : _base(base)
        {
            if (base.type() == ElementType::NotInitialized)
                throw NotInitializedError();
            backend::element_pp_init(
                &_element_pp,
                const_cast<backend::element_s*>(base.c_element()));
        }
        FixedBasePower(const Element& base,
                       const backend::element_pp_s& element_pp)
            : _base(base), _element_pp(element_pp)
        {
        }
        FixedBasePower(const FixedBasePower&) = delete;
        ~FixedBasePower() { backend::element_pp_clear(&_element_pp); }
        Element get_base() const { return _base; }
        Element apply(const Element& pow) const
        {
            if (pow.type() == ElementType::NotInitialized)
                throw NotInitializedError();
            else if (pow.type() != ElementType::Zr)
                throw ElementTypeError();
            Element out;
            out.init_same_as(_base);
            backend::element_pp_pow_zn(
                &out._element, const_cast<backend::element_s*>(pow.c_element()),
                const_cast<backend::element_pp_s*>(&_element_pp));
            return out;
        }
        Element apply(int value) const
        {
            return apply(Element::from_integer(_base.pairing(), value));
        }
        template <class... Args>
        inline Element pow(Args&&... args) const
        {
            return apply(std::forward<Args>(args)...);
        }
        backend::element_pp_s* c_element_pp() { return &_element_pp; }
        const backend::element_pp_s* c_element_pp() const
        {
            return &_element_pp;
        }

    private:
        Element _base;
        backend::element_pp_s _element_pp;
    };

    // return a1^n1 * a2^n2
    inline Element pow2(const Element& a1, const Element& n1, const Element& a2,
                 const Element& n2)
    {
        if (a1.type() == ElementType::NotInitialized ||
            a2.type() == ElementType::NotInitialized)
            throw NotInitializedError();
        else if (n1.type() != ElementType::Zr || n2.type() != ElementType::Zr)
            throw ElementTypeError();

        Element out;
        out.init_same_as(a1);
        backend::element_pow2_zn(
            &out._element, const_cast<backend::element_s*>(a1.c_element()),
            const_cast<backend::element_s*>(n1.c_element()),
            const_cast<backend::element_s*>(a2.c_element()),
            const_cast<backend::element_s*>(n2.c_element()));
        return out;
    }

    // return a1^n1 * a2^n2 * a3^n3
    inline Element pow3(const Element& a1, const Element& n1, const Element& a2,
                 const Element& n2, const Element& a3, const Element& n3)
    {
        if (a1.type() == ElementType::NotInitialized ||
            a2.type() == ElementType::NotInitialized ||
            a3.type() == ElementType::NotInitialized)
            throw NotInitializedError();
        else if (n1.type() != ElementType::Zr || n2.type() != ElementType::Zr ||
                 n3.type() != ElementType::Zr)
            throw ElementTypeError();

        Element out;
        out.init_same_as(a1);
        backend::element_pow3_zn(
            &out._element, const_cast<backend::element_s*>(a1.c_element()),
            const_cast<backend::element_s*>(n1.c_element()),
            const_cast<backend::element_s*>(a2.c_element()),
            const_cast<backend::element_s*>(n2.c_element()),
            const_cast<backend::element_s*>(a3.c_element()),
            const_cast<backend::element_s*>(n3.c_element()));
        return out;
    }
}  // namespace pbc
