#include "OWLDataRange.hpp"

namespace owlapi {
namespace model {

OWLDataRange::OWLDataRange(Type type)
    : OWLPropertyRange()
    , mDataRangeType(type)
    , mTop(false)
{}

OWLDataRange::~OWLDataRange()
{}

} // end namespace model
} // end namespace owlapi
