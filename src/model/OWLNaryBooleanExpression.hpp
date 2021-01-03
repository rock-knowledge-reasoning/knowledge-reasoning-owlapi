#ifndef OWLAPI_MODEL_OWL_NARY_BOOLEAN_EXPRESSION_HPP
#define OWLAPI_MODEL_OWL_NARY_BOOLEAN_EXPRESSION_HPP

#include "OWLBooleanClassExpression.hpp"
#include "OWLAnonymousClassExpression.hpp"
#include "HasOperands.hpp"

namespace owlapi {
namespace model {

class OWLNaryBooleanExpression : public OWLBooleanClassExpression, public HasOperands<OWLClassExpression>
{
public:
    OWLNaryBooleanExpression(const OWLClassExpression::PtrList& expressions)
        : OWLBooleanClassExpression()
        , HasOperands<OWLClassExpression>(expressions)
    {}

    virtual ~OWLNaryBooleanExpression() = default;

    virtual ClassExpressionType getClassExpressionType() const override { return OWLClassExpression::ANONYMOUS; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_NARY_BOOLEAN_EXPRESSION_HPP
