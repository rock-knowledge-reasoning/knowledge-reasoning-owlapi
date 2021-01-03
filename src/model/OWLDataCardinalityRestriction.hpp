#ifndef OWLAPI_MODEL_OWL_DATA_CARDINALITY_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_DATA_CARDINALITY_RESTRICTION_HPP

#include "OWLCardinalityRestriction.hpp"
#include "OWLDataRange.hpp"
#include "OWLQuantifiedDataRestriction.hpp"
#include "OWLDataPropertyExpression.hpp"

namespace owlapi {
namespace model {

class OWLDataCardinalityRestriction : public OWLCardinalityRestriction, public OWLQuantifiedDataRestriction
{
public:
    friend class OWLCardinalityRestrictionOps;

    typedef shared_ptr<OWLDataCardinalityRestriction> Ptr;

    OWLDataCardinalityRestriction(const OWLPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLDataRange::Ptr& range,
            OWLCardinalityRestriction::CardinalityRestrictionType restrictionType);

    virtual ~OWLDataCardinalityRestriction() = default;

    static OWLCardinalityRestriction::Ptr createInstance(const OWLDataPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLDataRange::Ptr& range,
            OWLCardinalityRestriction::CardinalityRestrictionType type);


     /**
      * Check if restrictions have same restriction type (data/object), same property
      * and the same qualification
      * \return True, if the conditions hold, false otherwise
      */
    bool isOverlapping(const OWLCardinalityRestriction::Ptr& other) override;

protected:
    OWLCardinalityRestriction::Ptr doClone() const;

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_CARDINALITY_RESTRICTION_HPP
