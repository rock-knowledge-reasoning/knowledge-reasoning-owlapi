#ifndef OWLAPI_MODEL_OWL_QUANTIFIED_DATA_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_QUANTIFIED_DATA_RESTRICTION_HPP

#include "OWLQuantifiedRestriction.hpp"
#include "OWLDataRestriction.hpp"
#include "OWLDataRange.hpp"

namespace owlapi {
namespace model {

class OWLQuantifiedDataRestriction : public OWLQuantifiedRestriction<OWLDataRange>, public OWLDataRestriction
{
public:
    typedef shared_ptr<OWLQuantifiedDataRestriction> Ptr;

    OWLQuantifiedDataRestriction()
        : OWLDataRestriction()
        , OWLQuantifiedRestriction<OWLDataRange>()
    {}

    OWLQuantifiedDataRestriction(const OWLDataPropertyExpression::Ptr& property,
            const OWLDataRange::Ptr& dataRange)
        : OWLDataRestriction(property)
        , OWLQuantifiedRestriction<OWLDataRange>(property, dataRange)
    {}

    virtual ~OWLQuantifiedDataRestriction() = default;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_QUANTIFIED_DATA_RESTRICTION_HPP
