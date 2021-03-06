#include "OWLDataTypeRestriction.hpp"
#include "OWLDataRange.hpp"

namespace owlapi {
namespace model {

OWLDataTypeRestriction::OWLDataTypeRestriction(const OWLDataType::Ptr& datatype,
            const OWLFacetRestriction::List& facetRestrictions)
    : OWLDataRange(OWLDataTypeRestriction::DATATYPE_RESTRICTION)
    , mDataType(datatype)
    , mFacetRestrictions(facetRestrictions)
{}

} // end namespace model
} // end namespace owlapi
