#ifndef OWLAPI_MODEL_OWL_EQUIVALENT_OBJECT_PROPERTIES_AXIOM_HPP
#define OWLAPI_MODEL_OWL_EQUIVALENT_OBJECT_PROPERTIES_AXIOM_HPP

#include "OWLObjectPropertyAxiom.hpp"

namespace owlapi {
namespace model {

class OWLEquivalentObjectPropertiesAxiom : public OWLObjectPropertyAxiom
{
    OWLObjectPropertyExpression::PtrList mProperties;

public:
    OWLEquivalentObjectPropertiesAxiom(
            const OWLObjectPropertyExpression::PtrList& properties,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLObjectPropertyAxiom(AxiomType::EquivalentObjectProperties, annotations)
        , mProperties(properties)
    {}

    virtual ~OWLEquivalentObjectPropertiesAxiom() {}

    OWLObjectPropertyExpression::PtrList getProperties() const { return mProperties; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_EQUIVALENT_OBJECT_PROPERTIES_AXIOM_HPP
