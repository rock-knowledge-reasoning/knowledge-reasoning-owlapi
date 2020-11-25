#ifndef OWLAPI_MODEL_OWL_DATA_SOME_VALUES_FROM_HPP
#define OWLAPI_MODEL_OWL_DATA_SOME_VALUES_FROM_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "OWLSomeValuesFromRestriction.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLDataSomeValuesFrom : public OWLSomeValuesFromRestriction
{
public:
    OWLDataSomeValuesFrom(OWLDataPropertyExpression::Ptr property, const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLSomeValuesFromRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), qualification)
    {}

    virtual ~OWLDataSomeValuesFrom() {}

    ClassExpressionType getClassExpressionType() const { return DATA_SOME_VALUES_FROM; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_SOME_VALUES_FROM_HPP
