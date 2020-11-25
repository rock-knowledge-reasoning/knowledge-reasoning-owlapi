#ifndef OWLAPI_MODEL_OWL_FUNCTIONAL_DATA_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_FUNCTIONAL_DATA_PROPERTY_AXIOM_HPP

#include "OWLDataPropertyAxiom.hpp"
#include "OWLUnaryPropertyAxiom.hpp"

namespace owlapi {
namespace model {

class OWLFunctionalDataPropertyAxiom : public OWLUnaryPropertyAxiom<OWLDataPropertyAxiom>
{
public:
    OWLFunctionalDataPropertyAxiom(const OWLDataPropertyExpression::Ptr& property, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLDataPropertyAxiom>(property, AxiomType::FunctionalDataProperty, annotations)
    {}

    virtual ~OWLFunctionalDataPropertyAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_FUNCTIONAL_DATA_PROPERTY_AXIOM_HPP
