#ifndef OWLAPI_MODEL_OWL_INVERSE_FUNCTIONAL_OBJECT_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_INVERSE_FUNCTIONAL_OBJECT_PROPERTY_AXIOM_HPP

#include <owlapi/model/OWLUnaryPropertyAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLInverseFunctionalObjectPropertyAxiom : public OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>
{
public:
    OWLInverseFunctionalObjectPropertyAxiom(const OWLObjectPropertyExpression::Ptr& property, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>(property, AxiomType::InverseFunctionalObjectProperty, annotations)
    {}

    virtual ~OWLInverseFunctionalObjectPropertyAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_INVERSE_FUNCTIONAL_OBJECT_PROPERTY_AXIOM_HPP
