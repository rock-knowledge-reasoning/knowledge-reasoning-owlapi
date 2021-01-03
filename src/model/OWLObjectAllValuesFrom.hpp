#ifndef OWLAPI_MODEL_OWL_OBJECT_ALL_VALUES_FROM_HPP
#define OWLAPI_MODEL_OWL_OBJECT_ALL_VALUES_FROM_HPP

#include "OWLClassExpression.hpp"
#include "OWLObjectPropertyExpression.hpp"
#include "OWLAllValuesFromRestriction.hpp"
#include "OWLObjectRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLObjectAllValuesFrom : public OWLQuantifiedObjectRestriction
{
public:
    typedef shared_ptr<OWLObjectAllValuesFrom> Ptr;

    OWLObjectAllValuesFrom(
            const OWLObjectPropertyExpression::Ptr& property,
            const OWLClassExpression::Ptr& classExpression
    )
        : OWLQuantifiedObjectRestriction(property, classExpression)
    {}

    virtual ~OWLObjectAllValuesFrom() = default;

    ClassExpressionType getClassExpressionType() const { return OBJECT_ALL_VALUES_FROM; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_ALL_VALUES_FROM_HPP
