#ifndef OWLAPI_MODEL_OWL_DATA_ALL_VALUES_FROM_HPP
#define OWLAPI_MODEL_OWL_DATA_ALL_VALUES_FROM_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "OWLAllValuesFromRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLDataAllValuesFrom : public OWLAllValuesFromRestriction
{
public:
    OWLDataAllValuesFrom(const OWLDataPropertyExpression::Ptr& property,
            const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLAllValuesFromRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), qualification)
    {}

    virtual ~OWLDataAllValuesFrom() {}

    ClassExpressionType getClassExpressionType() const { return DATA_ALL_VALUES_FROM; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_ALL_VALUES_FROM_HPP
