#ifndef OWLAPI_MODEL_OWL_MAX_CARDINALITY_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_MAX_CARDINALITY_RESTRICTION_HPP

#include <owlapi/model/OWLCardinalityRestriction.hpp>

namespace owlapi {
namespace model {

class OWLMaxCardinalityRestriction : public OWLCardinalityRestriction
{
public:
    typedef boost::shared_ptr<OWLMaxCardinalityRestriction> Ptr;

    OWLMaxCardinalityRestriction(OWLPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification)
        : OWLCardinalityRestriction(property, cardinality, qualification, OWLCardinalityRestriction::MAX)
    {}

    virtual ~OWLMaxCardinalityRestriction() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_MAX_CARDINALITY_RESTRICTION_HPP
