#ifndef OWLAPI_MODEL_OWL_OBJECT_EXACT_CARDINALITY_HPP
#define OWLAPI_MODEL_OWL_OBJECT_EXACT_CARDINALITY_HPP

#include "OWLClassExpression.hpp"
#include "OWLObjectPropertyExpression.hpp"
#include "OWLObjectCardinalityRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLObjectExactCardinality : public OWLObjectCardinalityRestriction
{
public:
    OWLObjectExactCardinality(const OWLObjectPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLClassExpression::Ptr& qualification = OWLClassExpression::Ptr())
        : OWLObjectCardinalityRestriction(
                dynamic_pointer_cast<OWLPropertyExpression>(property),
                cardinality,
                qualification,
                OWLCardinalityRestriction::EXACT )
    {
        if(!property)
        {
            throw std::invalid_argument("owlapi::model::OWLObjectExactCardinalityRestriction"
                    " cannot construct without property");
        }
    }

    virtual ~OWLObjectExactCardinality() = default;

    ClassExpressionType getClassExpressionType() const { return OBJECT_EXACT_CARDINALITY; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_EXACT_CARDINALITY_HPP
