/*
  The MIT License (MIT)
  Copyright (c) 2016-2018 Xu Cheng
  See licensing at:
    https://opensource.org/licenses/MIT
*/

#pragma once

#include <ostream>

namespace pbc
{
    enum class ElementType { G1, G2, GT, Zr, NotInitialized };

    inline std::ostream& operator<<(std::ostream& o, ElementType type)
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
}  // namespace pbc
