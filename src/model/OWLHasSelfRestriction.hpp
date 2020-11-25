#ifndef OWLAPI_MODEL_OWL_HAS_SELF_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_HAS_SELF_RESTRICTION_HPP

#include "OWLValueRestriction.hpp"

namespace owlapi {
namespace model {

class OWLHasSelfRestriction : public OWLValueRestriction
{
public:
    typedef shared_ptr<OWLHasSelfRestriction> Ptr;

    OWLHasSelfRestriction(OWLPropertyExpression::Ptr property, const OWLQualification& qualification)
        : OWLValueRestriction(property, qualification, OWLValueRestriction::SELF)
    {}

    virtual ~OWLHasSelfRestriction() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_HAS_SELF_RESTRICTION_HPP
