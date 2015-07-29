#include "OWLPropertyExpression.hpp"

namespace owlapi {
namespace model {

OWLPropertyExpression::~OWLPropertyExpression()
{}

bool OWLPropertyExpression::isDataPropertyExpression() const
{
    throw std::runtime_error("OWLPropertyExpression::isDataPropertyExpression: not implemented");
}

bool OWLPropertyExpression::isObjectPropertyExpression() const
{
    throw std::runtime_error("OWLPropertyExpression::isObjectPropertyExpression: not implemented");
}

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
