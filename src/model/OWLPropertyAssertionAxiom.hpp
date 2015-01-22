#ifndef OWLAPI_MODEL_PROPERTY_ASSERTION_AXIOM_HPP
#define OWLAPI_MODEL_PROPERTY_ASSERTION_AXIOM_HPP

#include <owlapi/model/OWLIndividual.hpp>
#include <owlapi/model/OWLIndividualAxiom.hpp>
#include <owlapi/model/OWLPropertyExpression.hpp>
#include <owlapi/model/OWLPropertyAssertionObject.hpp>

namespace owlapi {
namespace model {

class OWLPropertyAssertionAxiom : public OWLIndividualAxiom // OWLSubclassOfAxiomShortCut
{
    OWLIndividual::Ptr mSubject;
    OWLPropertyExpression::Ptr mProperty;
    OWLPropertyAssertionObject::Ptr mObject;
public:
    OWLPropertyAssertionAxiom(OWLIndividual::Ptr subject, OWLPropertyExpression::Ptr property, OWLPropertyAssertionObject::Ptr object, AxiomType type, OWLAnnotationList annotations)
        : OWLIndividualAxiom(type, annotations)
        , mSubject(subject)
        , mProperty(property)
        , mObject(object)

    {}
    /**
     * Gets the individual that is the subject of this assertion.
     */
    OWLIndividual::Ptr getSubject() const { return mSubject; }

    OWLPropertyExpression::Ptr getProperty() const { return mProperty; }

    OWLPropertyAssertionObject::Ptr getObject() const { return mObject; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_PROPERTY_ASSERTION_AXIOM_HPP
