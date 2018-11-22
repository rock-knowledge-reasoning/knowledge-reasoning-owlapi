#ifndef OWLAPI_MODEL_OWL_DISJOINT_OBJECT_PROPERTIES_AXIOM_HPP
#define OWLAPI_MODEL_OWL_DISJOINT_OBJECT_PROPERTIES_AXIOM_HPP

#include "OWLObjectPropertyAxiom.hpp"

namespace owlapi {
namespace model {

class OWLDisjointObjectPropertiesAxiom : public OWLObjectPropertyAxiom
{
    OWLObjectPropertyExpression::PtrList mProperties;

public:
    OWLDisjointObjectPropertiesAxiom(
            const OWLObjectPropertyExpression::PtrList& properties,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLObjectPropertyAxiom(AxiomType::DisjointObjectProperties, annotations)
        , mProperties(properties)
    {}

    virtual ~OWLDisjointObjectPropertiesAxiom() {}

    OWLObjectPropertyExpression::PtrList getProperties() const { return mProperties; }

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DISJOINT_OBJECT_PROPERTIES_AXIOM_HPP
