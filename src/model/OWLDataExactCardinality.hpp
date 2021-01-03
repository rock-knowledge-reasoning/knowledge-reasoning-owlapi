#ifndef OWLAPI_MODEL_OWL_DATA_EXACT_CARDINALITY_HPP
#define OWLAPI_MODEL_OWL_DATA_EXACT_CARDINALITY_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "OWLDataCardinalityRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLDataExactCardinality : public OWLDataCardinalityRestriction
{
public:
    OWLDataExactCardinality(const OWLDataPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLDataRange::Ptr& range = OWLDataRange::Ptr())
        : OWLDataCardinalityRestriction(
                dynamic_pointer_cast<OWLPropertyExpression>(property),
                cardinality,
                range,
                OWLCardinalityRestriction::EXACT
          )
    {}

    virtual ~OWLDataExactCardinality() = default;

    OWLClassExpression::ClassExpressionType getClassExpressionType() const { return DATA_EXACT_CARDINALITY; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_EXACT_CARDINALITY_HPP
