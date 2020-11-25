#ifndef OWLAPI_MODEL_OWL_ANNOTATION_PROPERTIY_RANGE_AXIOM_HPP
#define OWLAPI_MODEL_OWL_ANNOTATION_PROPERTIY_RANGE_AXIOM_HPP

#include "IRI.hpp"
#include "OWLUnaryPropertyAxiom.hpp"
#include "OWLAnnotationAxiom.hpp"
#include "OWLPropertyRangeAxiom.hpp"

namespace owlapi {
namespace model {

/**
 * Represents an <a href="http://www.w3.org/TR/owl2-syntax/#Annotation_Property_Range" >
 * AnnotationPropertyRange</a> axiom in the OWL 2 specification.
 *
 * An annotation property range axiom AnnotationPropertyRange( AP U ) states that the range of the annotation property AP is the IRI U.
 */
class OWLAnnotationPropertyRangeAxiom : public OWLUnaryPropertyAxiom<OWLAnnotationAxiom>, public OWLPropertyRangeAxiom<IRI>
{
public:
    typedef shared_ptr<OWLAnnotationPropertyRangeAxiom> Ptr;

    OWLAnnotationPropertyRangeAxiom(const OWLAnnotationProperty::Ptr& property,
            const IRI& range,
            const OWLAnnotationList& annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLAnnotationAxiom>(property, AxiomType::AnnotationPropertyRange, annotations)
        , OWLPropertyRangeAxiom<IRI>(range)
    {}

    virtual ~OWLAnnotationPropertyRangeAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANNOTATION_PROPERTY_RANGE_AXIOM_HPP
