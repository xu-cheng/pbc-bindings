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
#include "./element.hpp"
#include "./exceptions.hpp"
#include "./pairing.hpp"

namespace pbc
{
    Element e(const Element& g, const Element& h)
    {
        if (g.type() == ElementType::NotInitialized ||
            h.type() == ElementType::NotInitialized)
            throw NotInitializedError();

        Element out;
        out.init_gt(g.pairing());
        backend::element_pairing(
            &out._element, const_cast<backend::element_s*>(g.c_element()),
            const_cast<backend::element_s*>(h.c_element()));
        return out;
    }
};
