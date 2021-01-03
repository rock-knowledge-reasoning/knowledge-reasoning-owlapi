#include "OWLDataCardinalityRestriction.hpp"
#include "OWLDataMinCardinality.hpp"
#include "OWLDataMaxCardinality.hpp"
#include "OWLDataExactCardinality.hpp"
#include <base-logging/Logging.hpp>

namespace owlapi {
namespace model {

OWLDataCardinalityRestriction::OWLDataCardinalityRestriction(
        const OWLPropertyExpression::Ptr& property,
        uint32_t cardinality,
        const OWLDataRange::Ptr& range,
        OWLCardinalityRestriction::CardinalityRestrictionType restrictionType)
    : OWLCardinalityRestriction(property, cardinality, restrictionType)
    , OWLQuantifiedDataRestriction(dynamic_pointer_cast<OWLDataPropertyExpression>(property), range)
{}

OWLCardinalityRestriction::Ptr OWLDataCardinalityRestriction::createInstance(
        const OWLDataPropertyExpression::Ptr& property,
        uint32_t cardinality,
        const OWLDataRange::Ptr& range,
        OWLCardinalityRestriction::CardinalityRestrictionType type)
{
    switch(type)
    {
        case OWLCardinalityRestriction::MIN:
            return make_shared<OWLDataMinCardinality>(property, cardinality, range);
        case OWLCardinalityRestriction::MAX:
            return make_shared<OWLDataMaxCardinality>(property, cardinality, range);
        case OWLCardinalityRestriction::EXACT:
            return make_shared<OWLDataExactCardinality>(property, cardinality, range);
        default:
            break;
    }
    throw std::invalid_argument("owlapi::model::OWLDataCardinalityRestriction::createInstance"
            " creation of UNKNOWN cardinality type requested");
}

OWLCardinalityRestriction::Ptr OWLDataCardinalityRestriction::doClone() const
{
    return createInstance(dynamic_pointer_cast<OWLDataPropertyExpression>(OWLDataRestriction::getProperty()), getCardinality(), getFiller(), getCardinalityRestrictionType());
}

bool OWLDataCardinalityRestriction::isOverlapping(
            const OWLCardinalityRestriction::Ptr& other
)
{
    if(other->isDataRestriction())
    {
        if(OWLDataRestriction::getProperty()->toString() != other->getProperty()->toString() )
        {
            LOG_DEBUG_S << "Different property";
            return false;
        }

        OWLDataCardinalityRestriction::Ptr bRestriction = dynamic_pointer_cast<OWLDataCardinalityRestriction>(other);

        if(getFiller() != bRestriction->getFiller())
        {
            LOG_WARN_S << "Testing for a partial overlap of dataRanges is not supported - only checking for equality of data ranges";
            return false;
        }
        return true;
    }

    LOG_DEBUG_S << "Data/Data Restrictions are incompatible";
    return false;
}



} // end namespace model
} // end namespace owlapi
