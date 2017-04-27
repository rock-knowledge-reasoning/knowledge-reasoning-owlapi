#ifndef OWLAPI_MODEL_OWL_OBJECT_HAS_VALUE_HPP
#define OWLAPI_MODEL_OWL_OBJECT_HAS_VALUE_HPP

#include <owlapi/model/OWLClassExpression.hpp>
#include <owlapi/model/OWLObjectPropertyExpression.hpp>
#include <owlapi/model/OWLHasValueRestriction.hpp>
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

class OWLObjectHasValue : public OWLHasValueRestriction
{
public:
    OWLObjectHasValue(OWLObjectPropertyExpression::Ptr property, const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLHasValueRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), qualification)
    {}

    virtual ~OWLObjectHasValue() {}

    ClassExpressionType getClassExpressionType() const { return OBJECT_HAS_VALUE; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_HAS_VALUE_HPP
