#ifndef OWLAPI_MODEL_OWL_OBJECT_SOME_VALUES_FROM_HPP
#define OWLAPI_MODEL_OWL_OBJECT_SOME_VALUES_FROM_HPP

#include <owlapi/model/OWLClassExpression.hpp>
#include <owlapi/model/OWLObjectPropertyExpression.hpp>
#include <owlapi/model/OWLSomeValuesFromRestriction.hpp>
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

class OWLObjectSomeValuesFrom : public OWLSomeValuesFromRestriction
{
public:
    OWLObjectSomeValuesFrom(OWLObjectPropertyExpression::Ptr property, const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLSomeValuesFromRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), qualification)
    {}

    virtual ~OWLObjectSomeValuesFrom() {}

    ClassExpressionType getClassExpressionType() const { return OBJECT_SOME_VALUES_FROM; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_SOME_VALUES_FROM_HPP
