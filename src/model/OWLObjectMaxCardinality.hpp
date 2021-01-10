#ifndef OWLAPI_MODEL_OWL_OBJECT_MAX_CARDINALITY_HPP
#define OWLAPI_MODEL_OWL_OBJECT_MAX_CARDINALITY_HPP

#include "OWLClassExpression.hpp"
#include "OWLObjectPropertyExpression.hpp"
#include "OWLObjectCardinalityRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLObjectMaxCardinality : public OWLObjectCardinalityRestriction
{
public:
    OWLObjectMaxCardinality(const OWLObjectPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLClassExpression::Ptr& qualification = OWLClassExpression::Ptr())
        : OWLObjectCardinalityRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), cardinality, qualification, OWLCardinalityRestriction::MAX)
    {}

    virtual ~OWLObjectMaxCardinality() = default;

    ClassExpressionType getClassExpressionType() const override { return OBJECT_MAX_CARDINALITY; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_MAX_CARDINALITY_HPP
