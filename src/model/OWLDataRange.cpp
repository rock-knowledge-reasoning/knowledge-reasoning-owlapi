#include "OWLDataRange.hpp"

namespace owlapi {
namespace model {

OWLDataRange::OWLDataRange(Type type = DATATYPE)
    : OWLPropertyRange()
    , mDataRangeType(type)
    , mTop(false)
{}

OWLDataRange::~OWLDataRange()
{}

} // end namespace model
} // end namespace owlapi
