#ifndef OWLAPI_MODEL_OWL_HAS_VALUE_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_HAS_VALUE_RESTRICTION_HPP

#include <owlapi/model/OWLValueRestriction.hpp>

namespace owlapi {
namespace model {

class OWLHasValueRestriction : public OWLValueRestriction
{
public:
    typedef shared_ptr<OWLHasValueRestriction> Ptr;

    OWLHasValueRestriction(OWLPropertyExpression::Ptr property, const OWLQualification& qualification)
        : OWLValueRestriction(property, qualification, OWLValueRestriction::HAS)
    {}

    virtual ~OWLHasValueRestriction() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_HAS_VALUE_RESTRICTION_HPP

