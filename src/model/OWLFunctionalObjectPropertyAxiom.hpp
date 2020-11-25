#ifndef OWLAPI_MODEL_OWL_FUNCTIONAL_OBJECT_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_FUNCTIONAL_OBJECT_PROPERTY_AXIOM_HPP

#include "OWLUnaryPropertyAxiom.hpp"
#include "OWLObjectPropertyAxiom.hpp"

namespace owlapi {
namespace model {

class OWLFunctionalObjectPropertyAxiom : public OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>
{
public:
    OWLFunctionalObjectPropertyAxiom(const OWLObjectPropertyExpression::Ptr& property, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>(property, AxiomType::FunctionalObjectProperty, annotations)
    {}

    virtual ~OWLFunctionalObjectPropertyAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_FUNCTIONAL_OBJECT_PROPERTY_AXIOM_HPP
