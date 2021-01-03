#include "OWLDataRange.hpp"

namespace owlapi {
namespace model {

std::map<OWLDataRange::Type, std::string> OWLDataRange::TypeTxt = {
    { OWLDataRange::DATA_COMPLEMENT_OF,   "DATA_COMPLEMENT_OF" },
    { OWLDataRange::DATA_INTERSECTION_OF, "DATA_INTERSECTION_OF" },
    { OWLDataRange::DATA_ONE_OF,          "DATA_ONE_OF" },
    { OWLDataRange::DATA_UNION_OF,        "DATA_UNION_OF" },
    { OWLDataRange::DATATYPE,             "DATATYPE" },
    { OWLDataRange::DATATYPE_RESTRICTION, "DATATYPE_RESTRICTION" }
};

OWLDataRange::OWLDataRange(Type type)
    : OWLPropertyRange()
    , mDataRangeType(type)
    , mTop(false)
{}

OWLDataRange::~OWLDataRange()
{}

} // end namespace model
} // end namespace owlapi
