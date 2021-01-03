#ifndef OWLAPI_MODEL_OWL_DATA_HAS_VALUE_HPP
#define OWLAPI_MODEL_OWL_DATA_HAS_VALUE_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "OWLHasValueRestriction.hpp"
#include "OWLDataRestriction.hpp"
#include "OWLLiteral.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLDataHasValue : public OWLHasValueRestriction<OWLLiteral>, public OWLDataRestriction
{
public:
    typedef shared_ptr<OWLDataHasValue> Ptr;

    OWLDataHasValue(const OWLDataPropertyExpression::Ptr& property,
            const OWLLiteral::Ptr& literal)
        : OWLHasValueRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), literal)
        , OWLDataRestriction(property)
    {}

    virtual ~OWLDataHasValue() = default;

    ClassExpressionType getClassExpressionType() const { return DATA_HAS_VALUE; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_HAS_VALUE_HPP
