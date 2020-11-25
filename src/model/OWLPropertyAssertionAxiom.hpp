#ifndef OWLAPI_MODEL_PROPERTY_ASSERTION_AXIOM_HPP
#define OWLAPI_MODEL_PROPERTY_ASSERTION_AXIOM_HPP

#include "OWLIndividual.hpp"
#include "OWLIndividualAxiom.hpp"
#include "OWLPropertyExpression.hpp"
#include "OWLPropertyAssertionObject.hpp"

namespace owlapi {
namespace model {

/**
 * \class OWLPropertyAssertionAxiom
 * \brief Represents a triple, to relate a property value to a given subject
 * \see http://www.w3.org/TR/owl2-syntax/#Assertions
 */
class OWLPropertyAssertionAxiom : public OWLIndividualAxiom // OWLSubclassOfAxiomShortCut
{
    OWLIndividual::Ptr mSubject;
    OWLPropertyExpression::Ptr mProperty;
    OWLPropertyAssertionObject::Ptr mObject;
public:
    typedef shared_ptr<OWLPropertyAssertionAxiom> Ptr;

    OWLPropertyAssertionAxiom(const OWLIndividual::Ptr& subject,
            const OWLPropertyExpression::Ptr& property,
            const OWLPropertyAssertionObject::Ptr& object,
            AxiomType type,
            const OWLAnnotationList& annotations)
        : OWLIndividualAxiom(type, annotations)
        , mSubject(subject)
        , mProperty(property)
        , mObject(object)

    {}

    virtual ~OWLPropertyAssertionAxiom() {}

    /**
     * Gets the individual that is the subject of this assertion.
     */
    OWLIndividual::Ptr getSubject() const { return mSubject; }

    /**
     * Get the property that related individual (subject) and value (object)
     * \return Property Expression
     */
    OWLPropertyExpression::Ptr getProperty() const { return mProperty; }

    /**
     * Get the assigned value for this property
     */
    OWLPropertyAssertionObject::Ptr getObject() const { return mObject; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_PROPERTY_ASSERTION_AXIOM_HPP
