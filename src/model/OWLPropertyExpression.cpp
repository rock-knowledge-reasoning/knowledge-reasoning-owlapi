#include "OWLPropertyExpression.hpp"

namespace owlapi {
namespace model {

OWLPropertyExpression::~OWLPropertyExpression()
{}

void OWLPropertyExpression::setRanges(const std::vector<OWLPropertyRange::Ptr>& ranges)
{
    mRanges.clear();

    // Allow potential validation in addRange to take effect, e.g. check if it
    // contains a valid data domain for data properties
    std::vector<OWLPropertyRange::Ptr>::const_iterator cit = ranges.begin();
    for(; cit != ranges.end(); ++cit)
    {
        addRange(*cit);
    }
}

} // end namespace model
} // end namespace owlapi
