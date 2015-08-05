#ifndef OWLAPI_MODEL_OWL_ASYMMETRIC_OBJECT_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_ASYMMETRIC_OBJECT_PROPERTY_AXIOM_HPP

#include <owlapi/model/OWLUnaryPropertyAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLAsymmetricObjectPropertyAxiom : public OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>
{
public:
    OWLAsymmetricObjectPropertyAxiom(const OWLObjectPropertyExpression::Ptr& property, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>(property, AxiomType::AsymmetricObjectProperty, annotations)
    {}

    virtual ~OWLAsymmetricObjectPropertyAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ASYMMETRIC_OBJECT_PROPERTY_AXIOM_HPP
