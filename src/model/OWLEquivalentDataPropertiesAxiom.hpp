#ifndef OWLAPI_MODEL_OWL_EQUIVALENT_DATA_PROPERTIES_AXIOM_HPP
#define OWLAPI_MODEL_OWL_EQUIVALENT_DATA_PROPERTIES_AXIOM_HPP

#include "OWLDataPropertyAxiom.hpp"

namespace owlapi {
namespace model {

class OWLEquivalentDataPropertiesAxiom : public OWLDataPropertyAxiom
{
    OWLDataPropertyExpression::PtrList mProperties;

public:
    OWLEquivalentDataPropertiesAxiom(const OWLDataPropertyExpression::PtrList& properties,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLDataPropertyAxiom(AxiomType::EquivalentDataProperties, annotations)
        , mProperties(properties)
    {}

    virtual ~OWLEquivalentDataPropertiesAxiom() {}

    OWLDataPropertyExpression::PtrList getProperties() const { return mProperties; }

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_EQUIVALENT_DATA_PROPERTIES_AXIOM_HPP
