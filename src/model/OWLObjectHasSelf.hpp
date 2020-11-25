#ifndef OWLAPI_MODEL_OWL_OBJECT_HAS_SELF_HPP
#define OWLAPI_MODEL_OWL_OBJECT_HAS_SELF_HPP

#include "OWLClassExpression.hpp"
#include "OWLObjectPropertyExpression.hpp"
#include "OWLHasSelfRestriction.hpp"
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

class OWLObjectHasSelf : public OWLHasSelfRestriction
{
public:
    OWLObjectHasSelf(OWLObjectPropertyExpression::Ptr property, const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLHasSelfRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), qualification)
    {}

    virtual ~OWLObjectHasSelf() {}

    ClassExpressionType getClassExpressionType() const { return OBJECT_HAS_SELF; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_HAS_SELF_HPP
