#ifndef OWLAPI_MODEL_DATA_PROPERTY_ASSERTION_AXIOM_HPP
#define OWLAPI_MODEL_DATA_PROPERTY_ASSERTION_AXIOM_HPP

#include "OWLPropertyAssertionAxiom.hpp"
#include "OWLLiteral.hpp"

namespace owlapi {
namespace model {

/**
 * Represents a triple to describe the associated property value for a given subject, e.g.
 * subject: Peter
 * property: hasLastName
 * object: "Pan"
 * \see http://www.w3.org/TR/owl2-syntax/#Assertions
 */
class OWLDataPropertyAssertionAxiom : public OWLPropertyAssertionAxiom
{
public:

    typedef shared_ptr<OWLDataPropertyAssertionAxiom> Ptr;

    OWLDataPropertyAssertionAxiom(const OWLIndividual::Ptr& subject,
            const OWLPropertyExpression::Ptr& property,
            const OWLLiteral::Ptr& object,
            const OWLAnnotationList& annotations = OWLAnnotationList())
            :  OWLPropertyAssertionAxiom(subject, property, object, DataPropertyAssertion, annotations)
    {}

    virtual ~OWLDataPropertyAssertionAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_DATA_PROPERTY_ASSERTION_AXIOM_HPP
