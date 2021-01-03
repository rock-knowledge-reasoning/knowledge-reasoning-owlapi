#ifndef OWLAPI_MODEL_OWL_OBJECT_COMPLEMENT_OF_HPP
#define OWLAPI_MODEL_OWL_OBJECT_COMPLEMENT_OF_HPP

#include "OWLNaryBooleanExpression.hpp"

namespace owlapi {
namespace model {

class OWLObjectComplementOf : OWLBooleanClassExpression
{
public:
    typedef shared_ptr<OWLObjectComplementOf> Ptr;

    OWLObjectComplementOf(const OWLClassExpression::Ptr& expression)
        : OWLBooleanClassExpression()
        , mOperand(expression)
    {}

    virtual ~OWLObjectComplementOf() = default;

    const OWLClassExpression::Ptr& getOperand() const { return mOperand; }

private:
    OWLClassExpression::Ptr mOperand;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_COMPLEMENT_OF_HPP
