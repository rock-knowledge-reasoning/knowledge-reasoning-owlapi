#ifndef OWLAPI_MODEL_OWL_OBJECT_HAS_SELF_HPP
#define OWLAPI_MODEL_OWL_OBJECT_HAS_SELF_HPP

#include "OWLClassExpression.hpp"
#include "OWLObjectPropertyExpression.hpp"
#include "OWLHasSelfRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLObjectHasSelf : public OWLObjectRestriction
{
public:
    OWLObjectHasSelf(const OWLObjectPropertyExpression::Ptr property)
        : OWLObjectRestriction(property)
    {}

    virtual ~OWLObjectHasSelf() = default;

    ClassExpressionType getClassExpressionType() const { return OBJECT_HAS_SELF; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_HAS_SELF_HPP
