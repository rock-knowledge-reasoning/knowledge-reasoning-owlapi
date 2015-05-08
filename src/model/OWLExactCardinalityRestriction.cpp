#include "OWLExactCardinalityRestriction.hpp"

namespace owlapi {
namespace model {

OWLExactCardinalityRestriction::OWLExactCardinalityRestriction(OWLPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification)
    : OWLCardinalityRestriction(property, cardinality, qualification, OWLCardinalityRestriction::EXACT)
{}

OWLExactCardinalityRestriction::~OWLExactCardinalityRestriction() {}

} // end namespace model
} // end namespace owlapi
