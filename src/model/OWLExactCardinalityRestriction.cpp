#include "OWLExactCardinalityRestriction.hpp"

namespace owlapi {
namespace model {

OWLExactCardinalityRestriction::OWLExactCardinalityRestriction(
        const OWLPropertyExpression::Ptr& property,
        uint32_t cardinality)
    : OWLCardinalityRestriction(property, cardinality, qualification, OWLCardinalityRestriction::EXACT)
{}

OWLExactCardinalityRestriction::~OWLExactCardinalityRestriction() {}

} // end namespace model
} // end namespace owlapi
