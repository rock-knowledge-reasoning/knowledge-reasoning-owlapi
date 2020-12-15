#ifndef OWLAPI_MODEL_OWL_SOME_VALUES_FROM_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_SOME_VALUES_FROM_RESTRICTION_HPP

#include "OWLValueRestriction.hpp"

namespace owlapi {
namespace model {

class OWLSomeValuesFromRestriction : public OWLValueRestriction
{
public:
    typedef shared_ptr<OWLSomeValuesFromRestriction> Ptr;

    OWLSomeValuesFromRestriction(const OWLPropertyExpression::Ptr& property,
            const OWLQualification& qualification)
        : OWLValueRestriction(property, qualification, OWLValueRestriction::SOME)
    {}

    virtual ~OWLSomeValuesFromRestriction() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_SOME_VALUES_FROM_RESTRICTION_HPP

