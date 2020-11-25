#ifndef OWLAPI_MODEL_OWL_DATA_MAX_CARDINALITY_HPP
#define OWLAPI_MODEL_OWL_DATA_MAX_CARDINALITY_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "OWLMaxCardinalityRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLDataMaxCardinality : public OWLMaxCardinalityRestriction
{
public:
    OWLDataMaxCardinality(OWLDataPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLMaxCardinalityRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), cardinality, qualification)
    {}

    virtual ~OWLDataMaxCardinality() {}

    ClassExpressionType getClassExpressionType() const { return DATA_MAX_CARDINALITY; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_MAX_CARDINALITY_HPP
