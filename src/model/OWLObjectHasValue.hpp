#ifndef OWLAPI_MODEL_OWL_OBJECT_HAS_VALUE_HPP
#define OWLAPI_MODEL_OWL_OBJECT_HAS_VALUE_HPP

#include "OWLClassExpression.hpp"
#include "OWLObjectPropertyExpression.hpp"
#include "OWLHasValueRestriction.hpp"
#include "OWLObjectRestriction.hpp"
#include "OWLIndividual.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLObjectHasValue : public OWLHasValueRestriction<OWLIndividual>, public OWLObjectRestriction
{
public:
    typedef shared_ptr<OWLObjectHasValue> Ptr;

    OWLObjectHasValue(
            const OWLObjectPropertyExpression::Ptr& property,
            const OWLIndividual::Ptr& individual
    )
        : OWLHasValueRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property),individual)
        , OWLObjectRestriction(property)
    {}

    virtual ~OWLObjectHasValue() = default;

    ClassExpressionType getClassExpressionType() const { return OBJECT_HAS_VALUE; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_HAS_VALUE_HPP
