#ifndef OWLAPI_MODEL_OWL_DATA_MIN_CARDINALITY_HPP
#define OWLAPI_MODEL_OWL_DATA_MIN_CARDINALITY_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "OWLMinCardinalityRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLDataMinCardinality : public OWLMinCardinalityRestriction
{
public:
    OWLDataMinCardinality(OWLDataPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLMinCardinalityRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), cardinality, qualification)
    {}

    virtual ~OWLDataMinCardinality() {}

    ClassExpressionType getClassExpressionType() const { return DATA_MIN_CARDINALITY; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_MIN_CARDINALITY_HPP
