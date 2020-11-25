#ifndef OWLAPI_MODEL_OWL_DATA_EXACT_CARDINALITY_HPP
#define OWLAPI_MODEL_OWL_DATA_EXACT_CARDINALITY_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "OWLExactCardinalityRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLDataExactCardinality : public OWLExactCardinalityRestriction
{
public:
    OWLDataExactCardinality(OWLDataPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLExactCardinalityRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), cardinality, qualification)
    {}

    virtual ~OWLDataExactCardinality() {}

    ClassExpressionType getClassExpressionType() const { return DATA_EXACT_CARDINALITY; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_EXACT_CARDINALITY_HPP
