#ifndef OWLAPI_MODEL_ANNOTATION_ASSERTION_AXIOM_HPP
#define OWLAPI_MODEL_ANNOTATION_ASSERTION_AXIOM_HPP

#include "OWLLiteral.hpp"
#include "OWLAnnotationAxiom.hpp"
#include "OWLAnnotationProperty.hpp"
#include "HasSubject.hpp"
#include "HasProperty.hpp"
#include "HasAnnotationValue.hpp"

namespace owlapi {
namespace model {

/**
 * Represents a triple to describe the associated property value for a given subject, e.g.
 * subject: Peter
 * property: hasLastName
 * object: "Pan"
 * \see http://www.w3.org/TR/owl2-syntax/#Assertions
 * \see https://www.w3.org/TR/owl2-syntax/#Annotation_Assertion
 */
class OWLAnnotationAssertionAxiom : public OWLAnnotationAxiom, public HasSubject<OWLAnnotationSubject>, public HasProperty<OWLAnnotationProperty>, public HasAnnotationValue
{
public:
    typedef shared_ptr<OWLAnnotationAssertionAxiom> Ptr;

    OWLAnnotationAssertionAxiom(const OWLAnnotationSubject::Ptr& subject,
            OWLAnnotationProperty::Ptr property,
            const OWLAnnotationValue::Ptr& object,
            const OWLAnnotationList& annotations = OWLAnnotationList())
            : OWLAnnotationAxiom(OWLAxiom::AnnotationAssertion, annotations)
            , HasSubject<OWLAnnotationSubject>(subject)
            , HasProperty<OWLAnnotationProperty>(property)
            , HasAnnotationValue(object)
    {}

    virtual ~OWLAnnotationAssertionAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_ANNOTATION_ASSERTION_AXIOM_HPP
