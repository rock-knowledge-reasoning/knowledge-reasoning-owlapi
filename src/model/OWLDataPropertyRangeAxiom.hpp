#ifndef OWLAPI_MODEL_OWL_DATA_PROPERTIY_RANGE_AXIOM_HPP
#define OWLAPI_MODEL_OWL_DATA_PROPERTIY_RANGE_AXIOM_HPP

#include "OWLDataRange.hpp"
#include "OWLUnaryPropertyAxiom.hpp"
#include "OWLPropertyRangeAxiom.hpp"

namespace owlapi {
namespace model {

class OWLDataPropertyRangeAxiom : public OWLUnaryPropertyAxiom<OWLDataPropertyAxiom>, public OWLPropertyRangeAxiom<OWLDataRange::Ptr>
{
public:
    typedef shared_ptr<OWLDataPropertyRangeAxiom> Ptr;

    OWLDataPropertyRangeAxiom(OWLDataPropertyExpression::Ptr property,
            OWLDataRange::Ptr range,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLDataPropertyAxiom>(property, AxiomType::DataPropertyRange, annotations)
        , OWLPropertyRangeAxiom<OWLDataRange::Ptr>(range)
    {}

    virtual ~OWLDataPropertyRangeAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_PROPERTY_RANGE_AXIOM_HPP
