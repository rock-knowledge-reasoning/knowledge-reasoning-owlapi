#ifndef OWLAPI_MODEL_OWL_OBJECT_PROPERTIY_RANGE_AXIOM_HPP
#define OWLAPI_MODEL_OWL_OBJECT_PROPERTIY_RANGE_AXIOM_HPP

#include "OWLClassExpression.hpp"
#include "OWLUnaryPropertyAxiom.hpp"
#include "OWLPropertyRangeAxiom.hpp"

namespace owlapi {
namespace model {

class OWLObjectPropertyRangeAxiom : public OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>, public OWLPropertyRangeAxiom<OWLClassExpression::Ptr> 
{
public:
    typedef shared_ptr<OWLObjectPropertyRangeAxiom> Ptr;

    OWLObjectPropertyRangeAxiom(const OWLObjectPropertyExpression::Ptr& property,
            OWLClassExpression::Ptr range,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>(property, AxiomType::ObjectPropertyRange, annotations)
        , OWLPropertyRangeAxiom<OWLClassExpression::Ptr>(range)
    {}

    virtual ~OWLObjectPropertyRangeAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_PROPERTY_RANGE_AXIOM_HPP
