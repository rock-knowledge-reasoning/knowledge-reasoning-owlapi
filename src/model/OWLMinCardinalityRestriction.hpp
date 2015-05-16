#ifndef OWLAPI_MODEL_OWL_MIN_CARDINALITY_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_MIN_CARDINALITY_RESTRICTION_HPP

#include <owlapi/model/OWLCardinalityRestriction.hpp>

namespace owlapi {
namespace model {

class OWLMinCardinalityRestriction : public OWLCardinalityRestriction
{
public:
    typedef boost::shared_ptr<OWLMinCardinalityRestriction> Ptr;

    OWLMinCardinalityRestriction(OWLPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification)
        : OWLCardinalityRestriction(property, cardinality, qualification, OWLCardinalityRestriction::MIN)
    {}

    virtual ~OWLMinCardinalityRestriction() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_MIN_CARDINALITY_RESTRICTION_HPP