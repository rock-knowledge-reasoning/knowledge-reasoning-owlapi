#ifndef OWLAPI_MODEL_OWL_FACET_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_FACET_RESTRICTION_HPP

#include <vector>
#include "OWLObject.hpp"
#include "OWLFacet.hpp"
#include "OWLLiteral.hpp"

namespace owlapi {
namespace model {

class OWLFacetRestriction : public OWLObject
{
public:
    OWLFacetRestriction();

    typedef std::vector<OWLFacetRestriction> List;

    OWLFacetRestriction(const OWLFacet& facet,
            const OWLLiteral::Ptr& facetValue);

    virtual ~OWLFacetRestriction() = default;

    OWLFacet getFacet() const { return mFacet; }
    OWLLiteral::Ptr getFacetValue() const { return mFacetValue; }

    /**
     * Check if the given value complies with this
     * facet restriction
     */
    bool isCompliant(const std::string& value) const;

private:
    OWLFacet mFacet;
    OWLLiteral::Ptr mFacetValue;

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_FACET_RESTRICTION_HPP
