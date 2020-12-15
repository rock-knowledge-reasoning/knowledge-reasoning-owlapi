#ifndef OWLAPI_MODEL_FACET_HPP
#define OWLAPI_MODEL_FACET_HPP

#include <map>
#include <string>
#include "HasIRI.hpp"

namespace owlapi {
namespace model {

class OWLFacetRestriction;

class OWLFacet : public HasIRI
{
    friend class OWLFacetRestriction;
public:
    virtual ~OWLFacet() = default;

    enum FacetType { UNKNOWN,
            LENGTH,
            MIN_LENGTH,
            MAX_LENGTH,
            PATTERN,
            MIN_INCLUSIVE,
            MIN_EXCLUSIVE,
            MAX_INCLUSIVE,
            MAX_EXCLUSIVE,
            TOTAL_DIGITS,
            FRACTION_DIGITS,
            LANG_RANGE,
            END
    };

    static OWLFacet getFacet(const IRI& iri);

    static bool isFacet(const IRI& iri);

    std::string getSymbol() const;

    IRI getIRI() const override { return mIRI; }

    std::string getShortName() const { return mIRI.getFragment(); }

    FacetType getFacetType() const { return mFacetType; }

protected:
    FacetType mFacetType;
    owlapi::model::IRI mIRI;

    OWLFacet();

    OWLFacet(const FacetType& type);

    static std::map<FacetType, IRI> msFacetIRIs;
    static std::map<FacetType, std::string> msFacetSymbols;



};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_FACET_HPP
