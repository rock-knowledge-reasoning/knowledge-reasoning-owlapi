#include "OWLFacetRestriction.hpp"
#include <regex>
#include <base-logging/Logging.hpp>
#include <sstream>

namespace owlapi {
namespace model {

OWLFacetRestriction::OWLFacetRestriction()
    : mFacet()
    , mFacetValue()
{}

OWLFacetRestriction::OWLFacetRestriction(const OWLFacet& facet,
            const OWLLiteral::Ptr& facetValue)
        : mFacet(facet)
        , mFacetValue(facetValue)
    {}

bool OWLFacetRestriction::isCompliant(const std::string& valueTxt) const
{
    double restrictingValue;
    std::string plainString;

    double value;
    try {
        std::stringstream ss(valueTxt);
        ss >> value;
    } catch(...)
    {}

    if(mFacetValue->isInteger())
    {
        restrictingValue = mFacetValue->getInteger();
    } else if(mFacetValue->isDouble() || mFacetValue->isFloat())
    {
        restrictingValue = mFacetValue->getDouble();
    } else if(mFacetValue->isPlainLiteral())
    {
        plainString = mFacetValue->getValue();
    }

    switch(mFacet.getFacetType())
    {
        case OWLFacet::LENGTH:
            return valueTxt.size() == restrictingValue;
        case OWLFacet::MIN_LENGTH:
            return valueTxt.size() >= restrictingValue;
        case OWLFacet::MAX_LENGTH:
            return valueTxt.size() <= restrictingValue;
        case OWLFacet::PATTERN:
            {
                std::regex pattern(plainString);
                return std::regex_match(valueTxt, pattern);
            }
        case OWLFacet::MIN_INCLUSIVE:
            return value >= restrictingValue;
        case OWLFacet::MIN_EXCLUSIVE:
            return value > restrictingValue;
        case OWLFacet::MAX_INCLUSIVE:
            return value <= restrictingValue;
        case OWLFacet::MAX_EXCLUSIVE:
            return value < restrictingValue;
        /// Specifies the exact number of digits allowed. Must be greater than zero
        case OWLFacet::TOTAL_DIGITS:
            break;
        /// Specifies the maximum number of decimal places allowed. Must be equal to or greater than zero
        case OWLFacet::FRACTION_DIGITS:
            break;
        case OWLFacet::LANG_RANGE:
            break;
        default:
            LOG_WARN_S << "Checking compliance of type: " << mFacet.getFacetType()
                << " is currently no supported";
            return true;
    }
    return true;
}

} // end namespace model
} // end namespace owlapi
