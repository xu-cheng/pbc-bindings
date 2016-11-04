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

    template <typename Container>
    typename std::enable_if<
        std::is_same<typename Container::value_type, Element>::value,
        Element>::type
    e_prod(const Container& in1, const Container& in2)
    {
        size_t length = in1.size();
        if (length == 0)
            throw std::invalid_argument("in1.size() == 0");
        else if (length != in2.size())
            throw std::invalid_argument("in1.size() != in2.size()");

        backend::element_t *_in1, *_in2;
        _in1 = new backend::element_t[length];
        _in2 = new backend::element_t[length];

        typename Container::const_iterator _p1, _p2;
        _p1 = in1.cbegin();
        _p2 = in2.cbegin();

        for (size_t i = 0; i < length; ++i, ++_p1, ++_p2) {
            if (_p1->type() == ElementType::NotInitialized ||
                _p2->type() == ElementType::NotInitialized) {
                delete[] _in1;
                delete[] _in2;
                throw NotInitializedError();
            }
            _in1[i][0] = *_p1->c_element();
            _in2[i][0] = *_p2->c_element();
        }

        Element out;
        out.init_gt(in1.cbegin()->pairing());
        backend::element_prod_pairing(&out._element, _in1, _in2, length);

        delete[] _in1;
        delete[] _in2;
        return out;
    }
};
