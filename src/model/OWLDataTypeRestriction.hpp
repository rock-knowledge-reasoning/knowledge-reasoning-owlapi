#ifndef OWLAPI_MODEL_OWL_DATA_TYPE_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_DATA_TYPE_RESTRICTION_HPP

#include "OWLFacetRestriction.hpp"

namespace owlapi {
namespace model {

class DataRange;

/**
 * A facet restriction allows to constrain the value range of a datatype
 * \see OWLFacetRestriction
 * \verbatim
   OWLFacetRestriction facetRestriction(OWLFacet::getFacet(vocabulary::XSD::minInclusive()),
        OWLLiteral::create("100.0^^<http://www.w3.org/2001/XMLSchema#double>"));
    \endverbatim
 *
 */
class OWLDataTypeRestriction : public OWLDataRange
{
public:
    typedef shared_ptr<OWLDataTypeRestriction> Ptr;

    OWLDataTypeRestriction(const OWLDataType::Ptr& datatype,
            const OWLFacetRestriction::List& facetRestrictions = OWLFacetRestriction::List());

    virtual ~OWLDataTypeRestriction() = default;

    OWLDataType getDataType() const { return *mDataType.get(); }

    const OWLFacetRestriction::List& getFacetRestrictions() const { return mFacetRestrictions; }
    void setFacetRestrictions(const OWLFacetRestriction::List& restrictions) { mFacetRestrictions = restrictions; }

private:
    OWLDataType::Ptr mDataType;
    OWLFacetRestriction::List mFacetRestrictions;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_TYPE_RESTRICTION_HPP
