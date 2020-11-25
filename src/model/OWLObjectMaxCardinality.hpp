#ifndef OWLAPI_MODEL_OWL_OBJECT_MAX_CARDINALITY_HPP
#define OWLAPI_MODEL_OWL_OBJECT_MAX_CARDINALITY_HPP

#include "OWLClassExpression.hpp"
#include "OWLObjectPropertyExpression.hpp"
#include "OWLMaxCardinalityRestriction.hpp"
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

class OWLObjectMaxCardinality : public OWLMaxCardinalityRestriction
{
public:
    OWLObjectMaxCardinality(OWLObjectPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLMaxCardinalityRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), cardinality, qualification)
    {}

    virtual ~OWLObjectMaxCardinality() {}

    ClassExpressionType getClassExpressionType() const { return OBJECT_MAX_CARDINALITY; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_MAX_CARDINALITY_HPP
