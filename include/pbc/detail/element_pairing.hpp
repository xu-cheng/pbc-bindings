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
#include <vector>
#include "./backend.hpp"
#include "./element.hpp"
#include "./exceptions.hpp"
#include "./pairing.hpp"

namespace pbc
{
    inline Element e(const Element& g1, const Element& g2)
    {
        if (g1.type() == ElementType::NotInitialized ||
            g2.type() == ElementType::NotInitialized)
            throw NotInitializedError();

        Element out;
        out.init_gt(g1.pairing());
        backend::element_pairing(
            &out._element, const_cast<backend::element_s*>(g1.c_element()),
            const_cast<backend::element_s*>(g2.c_element()));
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

        std::vector<backend::element_s> _in1, _in2;
        _in1.reserve(length);
        _in2.reserve(length);

        typename Container::const_iterator _p1, _p2;
        _p1 = in1.cbegin();
        _p2 = in2.cbegin();

        for (; _p1 != in1.cend() && _p2 != in2.cend(); ++_p1, ++_p2) {
            if (_p1->type() == ElementType::NotInitialized ||
                _p2->type() == ElementType::NotInitialized) {
                throw NotInitializedError();
            }

            // The C backend of pbc has a bug that if any input is identity
            // element, `element_prod_pairing` will always return identity
            // element. Here, we workaround this issue.
            if (_p1->is_one() || _p2->is_one()) continue;

            _in1.push_back(*_p1->c_element());
            _in2.push_back(*_p2->c_element());
        }

        Element out;
        out.init_gt(in1.cbegin()->pairing());
        if (_in1.size() > 0) {
            backend::element_prod_pairing(
                &out._element,
                reinterpret_cast<backend::element_t*>(_in1.data()),
                reinterpret_cast<backend::element_t*>(_in2.data()),
                _in1.size());
        } else {
            out.set_one();
        }

        return out;
    }

    class FixedG1Pairing;

    typedef std::shared_ptr<FixedG1Pairing> FixedG1PairingPtr;

    class FixedG1Pairing
    {
    public:
        FixedG1Pairing(const Element& g1) : _g1(g1)
        {
            if (g1.type() == ElementType::NotInitialized)
                throw NotInitializedError();
            else if (g1.type() != ElementType::G1)
                throw ElementTypeError();
            auto pairing = g1.pairing();
            backend::pairing_pp_init(
                &_pairing_pp, const_cast<backend::element_s*>(g1.c_element()),
                const_cast<backend::pairing_s*>(pairing->c_pairing()));
        }
        FixedG1Pairing(const Element& g1,
                       const backend::pairing_pp_s& pairing_pp)
            : _g1(g1), _pairing_pp(pairing_pp)
        {
        }
        FixedG1Pairing(const FixedG1Pairing&) = delete;
        ~FixedG1Pairing() { backend::pairing_pp_clear(&_pairing_pp); }
        Element get_g1() const { return _g1; }
        Element apply(const Element& g2) const
        {
            if (g2.type() == ElementType::NotInitialized)
                throw NotInitializedError();
            Element out;
            out.init_gt(_g1.pairing());
            backend::pairing_pp_apply(
                &out._element, const_cast<backend::element_s*>(g2.c_element()),
                const_cast<backend::pairing_pp_s*>(&_pairing_pp));
            return out;
        }
        backend::pairing_pp_s* c_pairing_pp() { return &_pairing_pp; }
        const backend::pairing_pp_s* c_pairing_pp() const
        {
            return &_pairing_pp;
        }

    private:
        Element _g1;
        backend::pairing_pp_s _pairing_pp;
    };
}  // namespace pbc
