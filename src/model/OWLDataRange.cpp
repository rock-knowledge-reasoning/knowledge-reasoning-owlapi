#include "OWLDataRange.hpp"

namespace owlapi {
namespace model {

std::map<OWLDataRange::Type, std::string> OWLDataRange::TypeTxt = {
    { OWLDataRange::COMPLEMENT_OF,        "COMPLEMENT_OF" },
    { OWLDataRange::INTERSECTION_OF,      "INTERSECTION_OF" },
    { OWLDataRange::ONE_OF,               "ONE_OF" },
    { OWLDataRange::UNION_OF,             "UNION_OF" },
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
