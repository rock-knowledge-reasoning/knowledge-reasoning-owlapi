#ifndef OWLAPI_MODEL_OWL_REFLEXIVE_OBJECT_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_REFLEXIVE_OBJECT_PROPERTY_AXIOM_HPP

#include <owlapi/model/OWLUnaryPropertyAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLReflexiveObjectPropertyAxiom : public OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>
{
public:
    OWLReflexiveObjectPropertyAxiom(const OWLObjectPropertyExpression::Ptr& property, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>(property, AxiomType::ReflexiveObjectProperty, annotations)
    {}

    virtual ~OWLReflexiveObjectPropertyAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_REFLEXIVE_OBJECT_PROPERTY_AXIOM_HPP
