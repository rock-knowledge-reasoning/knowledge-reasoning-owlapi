#ifndef OWLAPI_MODEL_OWL_OBJECT_MIN_CARDINALITY_HPP
#define OWLAPI_MODEL_OWL_OBJECT_MIN_CARDINALITY_HPP

#include "OWLClassExpression.hpp"
#include "OWLObjectPropertyExpression.hpp"
#include "OWLObjectCardinalityRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLObjectMinCardinality : public OWLObjectCardinalityRestriction
{
public:
    OWLObjectMinCardinality(const OWLObjectPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLClassExpression::Ptr& qualification = OWLClassExpression::Ptr())
        : OWLObjectCardinalityRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), cardinality, qualification, OWLCardinalityRestriction::MIN )
    {}

    virtual ~OWLObjectMinCardinality() = default;

    ClassExpressionType getClassExpressionType() const { return OBJECT_MIN_CARDINALITY; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_MIN_CARDINALITY_HPP
