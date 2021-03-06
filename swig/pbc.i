%module pbc
%{
#include <cstddef>
#include "pbc/pbc.hpp"
using namespace pbc;
%}

%nodefaultctor;
%feature("autodoc", "3");

%include <exception.i>
%include <std_except.i>
%include <std_shared_ptr.i>
%include <std_string.i>
%include <std_vector.i>

%exception {
    try {
        $action
    } catch (const std::runtime_error& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    } catch (const std::invalid_argument& e) {
        SWIG_exception(SWIG_ValueError, e.what());
    } catch (...) {
        SWIG_exception(SWIG_RuntimeError, "unknown exception");
    }
}

namespace std{
    %template(ElementVector) vector<pbc::Element>;
}

%ignore pbc::Element::Element(Element &&);
%ignore pbc::Element::operator=;

%shared_ptr(pbc::Pairing)
%shared_ptr(pbc::PairingParam)
%shared_ptr(pbc::FixedG1Pairing)
%shared_ptr(pbc::FixedBasePower)

/* the order is sensitive */
%include "pbc/detail/pairing_param.hpp"
%include "pbc/detail/pairing.hpp"
%include "pbc/detail/element_type.hpp"
%include "pbc/detail/element.hpp"
%include "pbc/detail/element_pairing.hpp"
%include "pbc/detail/element_power.hpp"

%inline %{
    inline pbc::Element e_prod(const std::vector<pbc::Element>& in1,
                               const std::vector<pbc::Element>& in2)
    {
        return pbc::e_prod(in1, in2);
    }
%}

%extend pbc::Element {
    pbc::Element __pow__(const pbc::Element& e) const
    {
        return $self->pow(e);
    }
    pbc::Element __pow__(int value) const
    {
        return $self->pow(value);
    }
    std::string __str__() const
    {
        return $self->to_str();
    }
};

%extend pbc::FixedBasePower {
    pbc::Element __pow__(const pbc::Element& e) const
    {
        return $self->apply(e);
    }
    pbc::Element __pow__(int value) const
    {
        return $self->apply(value);
    }
}
