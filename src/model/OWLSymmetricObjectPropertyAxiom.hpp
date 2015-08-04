#ifndef OWLAPI_MODEL_OWL_SYMMETRIC_OBJECT_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_SYMMETRIC_OBJECT_PROPERTY_AXIOM_HPP

#include <owlapi/model/OWLUnaryPropertyAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLSymmetricObjectPropertyAxiom : public OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>
{
public:
    OWLSymmetricObjectPropertyAxiom(const OWLObjectPropertyExpression::Ptr& property, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>(property, AxiomType::SymmetricObjectProperty, annotations)
    {}

    virtual ~OWLSymmetricObjectPropertyAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_SYMMETRIC_OBJECT_PROPERTY_AXIOM_HPP
