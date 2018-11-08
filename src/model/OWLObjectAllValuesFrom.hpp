#ifndef OWLAPI_MODEL_OWL_OBJECT_ALL_VALUES_FROM_HPP
#define OWLAPI_MODEL_OWL_OBJECT_ALL_VALUES_FROM_HPP

#include <owlapi/model/OWLClassExpression.hpp>
#include <owlapi/model/OWLObjectPropertyExpression.hpp>
#include <owlapi/model/OWLAllValuesFromRestriction.hpp>
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

class OWLObjectAllValuesFrom : public OWLAllValuesFromRestriction
{
public:
    OWLObjectAllValuesFrom(OWLObjectPropertyExpression::Ptr property, const OWLQualification& qualification = owlapi::vocabulary::OWL::Thing())
        : OWLAllValuesFromRestriction( dynamic_pointer_cast<OWLPropertyExpression>(property), qualification)
    {}

    virtual ~OWLObjectAllValuesFrom() {}

    ClassExpressionType getClassExpressionType() const { return OBJECT_ALL_VALUES_FROM; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_ALL_VALUES_FROM_HPP