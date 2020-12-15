#include "OWLFacet.hpp"
#include "../Vocabulary.hpp"

#include <algorithm>

namespace owlapi {
namespace model {

std::map<OWLFacet::FacetType, IRI> OWLFacet::msFacetIRIs =
{
    { LENGTH,           vocabulary::XSD::length() },
    { MIN_LENGTH,       vocabulary::XSD::minLength() },
    { MAX_LENGTH,       vocabulary::XSD::maxLength() },
    { PATTERN,          vocabulary::XSD::pattern() },
    { MIN_INCLUSIVE,    vocabulary::XSD::minInclusive() },
    { MIN_EXCLUSIVE,    vocabulary::XSD::minExclusive() },
    { MAX_INCLUSIVE,    vocabulary::XSD::maxInclusive() },
    { MAX_EXCLUSIVE,    vocabulary::XSD::maxExclusive() },
    { TOTAL_DIGITS,     vocabulary::XSD::totalDigits() },
    { FRACTION_DIGITS,  vocabulary::XSD::fractionDigits() },
    { LANG_RANGE,       vocabulary::XSD::langRange() }
};

std::map<OWLFacet::FacetType, std::string> OWLFacet::msFacetSymbols =
{
    { LENGTH,           "length" },
    { MIN_LENGTH,       "minLength" },
    { MAX_LENGTH,       "maxLength" },
    { PATTERN,          "pattern" },
    { MIN_INCLUSIVE,    ">=" },
    { MIN_EXCLUSIVE,    ">" },
    { MAX_INCLUSIVE,    "<=" },
    { MAX_EXCLUSIVE,    "<" },
    { TOTAL_DIGITS,     "totalDigits" },
    { FRACTION_DIGITS,  "fractionDigits" },
    { LANG_RANGE,       "langRange" }
};

OWLFacet::OWLFacet()
    : mFacetType(UNKNOWN)
{}

OWLFacet::OWLFacet(const FacetType& type)
    : mFacetType(type)
{
    std::map<FacetType, IRI>::const_iterator cit = msFacetIRIs.find(type);
    if(cit == msFacetIRIs.end())
    {
        throw std::invalid_argument("owlapi::model::OWLFacet: cannot create "
                "facet of given type - no matching iri found");
    }
    mIRI = cit->second;
}

std::string OWLFacet::getSymbol() const
{
    return msFacetSymbols.find(mFacetType)->second;
}

OWLFacet OWLFacet::getFacet(const IRI& iri)
{
    std::map<FacetType, IRI>::const_iterator cit = std::find_if(msFacetIRIs.begin(), msFacetIRIs.end(),
            [&iri](const std::pair<FacetType,IRI>& other)
            {
                return other.second == iri;
            });
    if(cit != msFacetIRIs.end())
    {
        return OWLFacet(cit->first);
    }

    throw std::invalid_argument("owlapi::model::OWLFacet::getFacet:"
            "no facet know for '" + iri.toString() + "'");
}

bool OWLFacet::isFacet(const IRI& iri)
{
    std::map<FacetType, IRI>::const_iterator cit = std::find_if(msFacetIRIs.begin(), msFacetIRIs.end(),
            [&iri](const std::pair<FacetType,IRI>& other)
            {
                return other.second == iri;
            });

    return cit != msFacetIRIs.end();
}

} // end namespace model
} // end namespace owlapi
