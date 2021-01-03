#ifndef OWLAPI_MODEL_OWL_OBJECT_UNION_OF_HPP
#define OWLAPI_MODEL_OWL_OBJECT_UNION_OF_HPP

#include "OWLNaryBooleanExpression.hpp"

namespace owlapi {
namespace model {

class OWLObjectUnionOf : public OWLNaryBooleanExpression
{
public:
    OWLObjectUnionOf(const OWLClassExpression::PtrList& expressions)
        : OWLNaryBooleanExpression(expressions)
    {}

    virtual ~OWLObjectUnionOf() = default;

    virtual ClassExpressionType getClassExpressionType() const override { return OWLClassExpression::OBJECT_UNION_OF; };

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_UNION_OF_HPP

