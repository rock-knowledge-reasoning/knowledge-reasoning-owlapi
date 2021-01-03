#ifndef OWLAPI_MODEL_OWL_BOOLEAN_CLASS_EXPRESSION_HPP
#define OWLAPI_MODEL_OWL_BOOLEAN_CLASS_EXPRESSION_HPP

namespace owlapi {
namespace model {

// A marker interface for Boolean Class Expressions, i.e.
// intersectionOf, unionOf, complementOf, oneOf
class OWLBooleanClassExpression : public OWLAnonymousClassExpression
{
public:
    virtual ~OWLBooleanClassExpression() = default;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_BOOLEAN_CLASS_EXPRESSION_HPP
