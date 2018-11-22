#ifndef OWLAPI_MODEL_OWL_DISJOINT_DATA_PROPERTIES_AXIOM_HPP
#define OWLAPI_MODEL_OWL_DISJOINT_DATA_PROPERTIES_AXIOM_HPP

#include "OWLDataPropertyAxiom.hpp"

namespace owlapi {
namespace model {

class OWLDisjointDataPropertiesAxiom : public OWLDataPropertyAxiom
{
    OWLDataPropertyExpression::PtrList mProperties;
public:
    OWLDisjointDataPropertiesAxiom(const OWLDataPropertyExpression::PtrList& properties,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLDataPropertyAxiom(AxiomType::DisjointDataProperties, annotations)
        , mProperties(properties)
    {}

    virtual ~OWLDisjointDataPropertiesAxiom() {}

    OWLDataPropertyExpression::PtrList getProperties() const { return mProperties; }

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DISJOINT_DATA_PROPERTIES_AXIOM_HPP
