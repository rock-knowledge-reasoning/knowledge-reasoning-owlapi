#ifndef OWLAPI_MODEL_OWL_DATA_HAS_VALUE_HPP
#define OWLAPI_MODEL_OWL_DATA_HAS_VALUE_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "OWLHasValueRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLDataHasValue : public OWLHasValueRestriction
{
public:
    OWLDataHasValue(const OWLDataPropertyExpression::Ptr& property,
            const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLHasValueRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), qualification)
    {}

    virtual ~OWLDataHasValue() {}

    ClassExpressionType getClassExpressionType() const { return DATA_HAS_VALUE; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_HAS_VALUE_HPP
