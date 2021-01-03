#ifndef OWLAPI_MODEL_OWL_OBJECT_INTERSECTION_OF_HPP
#define OWLAPI_MODEL_OWL_OBJECT_INTERSECTION_OF_HPP

#include "OWLNaryBooleanExpression.hpp"

namespace owlapi {
namespace model {

class OWLObjectIntersectionOf : public OWLNaryBooleanExpression
{
public:
    OWLObjectIntersectionOf(const OWLClassExpression::PtrList& expressions)
        : OWLNaryBooleanExpression(expressions)
    {}

    virtual ~OWLObjectIntersectionOf() = default;

    virtual ClassExpressionType getClassExpressionType() const override { return OWLClassExpression::OBJECT_INTERSECTION_OF; };

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_INTERSECTION_OF_HPP

