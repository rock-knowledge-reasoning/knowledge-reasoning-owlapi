#ifndef OWLAPI_MODEL_OWL_OBJECT_SOME_VALUES_FROM_HPP
#define OWLAPI_MODEL_OWL_OBJECT_SOME_VALUES_FROM_HPP

#include "OWLClassExpression.hpp"
#include "OWLObjectPropertyExpression.hpp"
#include "OWLQuantifiedObjectRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLObjectSomeValuesFrom : public OWLQuantifiedObjectRestriction
{
public:
    OWLObjectSomeValuesFrom(const OWLObjectPropertyExpression::Ptr& property,
            const OWLClassExpression::Ptr& classExpression)
        : OWLQuantifiedObjectRestriction(property, classExpression)
    {}

    virtual ~OWLObjectSomeValuesFrom() = default;

    OWLClassExpression::ClassExpressionType getClassExpressionType() const { return OBJECT_SOME_VALUES_FROM; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_SOME_VALUES_FROM_HPP
