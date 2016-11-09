/*
  The MIT License (MIT)
  Copyright (c) 2016 Xu Cheng
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
        FixedBasePower(const FixedBasePower&) = delete;
        ~FixedBasePower() { backend::element_pp_clear(&_element_pp); }
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

    private:
        Element _base;
        backend::element_pp_s _element_pp;
    };
};
