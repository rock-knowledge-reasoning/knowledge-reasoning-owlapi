#ifndef OWLAPI_MODEL_OWL_EXACT_CARDINALITY_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_EXACT_CARDINALITY_RESTRICTION_HPP

#include "OWLCardinalityRestriction.hpp"

namespace owlapi {
namespace model {

class OWLExactCardinalityRestriction : public OWLCardinalityRestriction
{
public:
    typedef shared_ptr<OWLExactCardinalityRestriction> Ptr;

    OWLExactCardinalityRestriction(OWLPropertyExpression::Ptr property, uint32_t cardinality);

    virtual ~OWLExactCardinalityRestriction();
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_EXACT_CARDINALITY_RESTRICTION_HPP
