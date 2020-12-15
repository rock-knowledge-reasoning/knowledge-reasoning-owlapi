#ifndef OWLAPI_MODEL_OWL_ANONYMOUS_CLASS_EXPRESSION_HPP
#define OWLAPI_MODEL_OWL_ANONYMOUS_CLASS_EXPRESSION_HPP

#include "OWLClassExpression.hpp"

namespace owlapi {
namespace model {

class OWLAnonymousClassExpression : public OWLClassExpression
{
public:
    typedef shared_ptr<OWLAnonymousClassExpression> Ptr;

    bool isAnonymous() const override { return true; }

    ClassExpressionType getClassExpressionType() const override { return OWLClassExpression::ANONYMOUS; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANONYMOUS_CLASS_EXPRESSION_HPP
