#ifndef OWLAPI_MODEL_OWL_ALL_VALUES_FROM_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_ALL_VALUES_FROM_RESTRICTION_HPP

#include <owlapi/model/OWLValueRestriction.hpp>

namespace owlapi {
namespace model {

class OWLAllValuesFromRestriction : public OWLValueRestriction
{
public:
    typedef shared_ptr<OWLAllValuesFromRestriction> Ptr;

    OWLAllValuesFromRestriction(OWLPropertyExpression::Ptr property, const OWLQualification& qualification)
        : OWLValueRestriction(property, qualification, OWLValueRestriction::ALL)
    {}

    virtual ~OWLAllValuesFromRestriction() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ALL_VALUES_FROM_RESTRICTION_HPP
