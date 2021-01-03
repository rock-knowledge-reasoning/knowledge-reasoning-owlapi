#include "OWLCardinalityRestriction.hpp"
#include "OWLObjectExactCardinality.hpp"
#include "OWLObjectMinCardinality.hpp"
#include "OWLObjectMaxCardinality.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "OWLDataExactCardinality.hpp"
#include "OWLDataMinCardinality.hpp"
#include "OWLDataMaxCardinality.hpp"
#include "OWLAnnotationProperty.hpp"
#include <algorithm>
#include <limits>
#include <math.h>
#include <sstream>
#include <base-logging/Logging.hpp>

namespace owlapi {
namespace model {

std::map<OWLCardinalityRestriction::CardinalityRestrictionType, std::string> OWLCardinalityRestriction::CardinalityRestrictionTypeTxt = {
    {UNKNOWN, "UNKNOWN"},
    {MIN, "MIN"},
    {MAX, "MAX"},
    {EXACT, "EXACT"}
};

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::narrow() const
{
    if(!getProperty())
    {
        throw std::runtime_error("OWLCardinalityRestriction::narrow: no property defined -- cannot narrow");
    }
    switch(getProperty()->getObjectType())
    {
        case OWLObject::ObjectPropertyExpression:
        {
            OWLObjectPropertyExpression::Ptr property = dynamic_pointer_cast<OWLObjectPropertyExpression>( getProperty() );

            switch(getCardinalityRestrictionType())
            {
                case OWLCardinalityRestriction::MIN:
                    return make_shared<OWLObjectMinCardinality>(property, getCardinality());
                case OWLCardinalityRestriction::MAX:
                    return make_shared<OWLObjectMaxCardinality>(property, getCardinality());
                case OWLCardinalityRestriction::EXACT:
                    return make_shared<OWLObjectExactCardinality>(property, getCardinality());
                default:
                    throw std::runtime_error("OWLCardinalityRestriction::narrow: cardinality set to UNKNOWN cannot narrow");
            }
        }
        case OWLObject::DataPropertyExpression:
        {
            OWLDataPropertyExpression::Ptr property =
                dynamic_pointer_cast<OWLDataPropertyExpression>( getProperty() );

            switch(getCardinalityRestrictionType())
            {
                case OWLCardinalityRestriction::MIN:
                    return make_shared<OWLDataMinCardinality>(property, getCardinality());
                case OWLCardinalityRestriction::MAX:
                    return make_shared<OWLDataMaxCardinality>(property, getCardinality());
                case OWLCardinalityRestriction::EXACT:
                    return make_shared<OWLDataExactCardinality>(property, getCardinality()); default:
                    throw std::runtime_error("OWLCardinalityRestriction::narrow: cardinality set to UNKNOWN cannot narrow");
            }
        }
        default:
            throw std::runtime_error("OWLCardinalityRestriction::narrow: has not been implemented for property expression '" + getProperty()->toString() + "'");
    }
}


OWLCardinalityRestriction::OWLCardinalityRestriction()
    : OWLRestriction()
    , OWLCardinalityRestrictionOps()
    , mCardinality(0)
    , mCardinalityRestrictionType(OWLCardinalityRestriction::UNKNOWN)
{}

OWLCardinalityRestriction::OWLCardinalityRestriction(
        const OWLPropertyExpression::Ptr& property,
        uint32_t cardinality,
        CardinalityRestrictionType restrictionType)
    : OWLRestriction()
    , OWLCardinalityRestrictionOps()
    , mCardinality(cardinality)
    , mCardinalityRestrictionType(restrictionType)
{
    if(!property)
    {
        throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction"
                " cannot construct without property");
    }
    setProperty(property);
}

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::clone() const
{
    return doClone();
}

std::string OWLCardinalityRestriction::toString() const
{
    return OWLCardinalityRestriction::toString(0);
}

std::string OWLCardinalityRestriction::toString(size_t indent) const
{
    std::string hspace(indent,' ');
    std::stringstream ss;
    ss << hspace << "OWLCardinalityRestriction:" << std::endl;
    ss << hspace << "    property: " << getProperty()->toString() << std::endl;
    ss << hspace << "    cardinality: " << getCardinality() << std::endl;
    ss << hspace << "    type: " << CardinalityRestrictionTypeTxt[getCardinalityRestrictionType()] << std::endl;
    return ss.str();
}

std::string OWLCardinalityRestriction::toString(const std::vector<OWLCardinalityRestriction::Ptr>& restrictions, size_t indent)
{
    std::stringstream ss;
    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
    for(; cit != restrictions.end(); ++cit)
    {
        ss << (*cit)->toString(indent);
    }
    return ss.str();
}


OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::getInstance(const OWLPropertyExpression::Ptr& property,
        uint32_t cardinality,
        const OWLClassExpression::Ptr& qualification,
        CardinalityRestrictionType restrictionType)
{
    return OWLObjectCardinalityRestriction::createInstance(
            dynamic_pointer_cast<OWLObjectPropertyExpression>(property),
            cardinality,
            qualification,
            restrictionType
    );
}

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::getInstance(const OWLPropertyExpression::Ptr& property,
        uint32_t cardinality,
        const OWLDataRange::Ptr& range,
        CardinalityRestrictionType restrictionType)
{
    return OWLDataCardinalityRestriction::createInstance(
            dynamic_pointer_cast<OWLDataPropertyExpression>(property),
            cardinality,
            range,
            restrictionType
    );
}

std::vector<OWLCardinalityRestriction::Ptr> OWLCardinalityRestriction::join(
        const std::vector<OWLCardinalityRestriction::Ptr>& a,
        const std::vector<OWLCardinalityRestriction::Ptr>& _b,
        OperationType operationType
)
{
    std::vector<OWLCardinalityRestriction::Ptr> restrictions;
    std::vector<OWLCardinalityRestriction::Ptr> b = _b;

    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator ait = a.begin();
    for(; ait != a.end(); ++ait)
    {
        LOG_DEBUG_S << "Try joining: " << (*ait)->toString();
        bool joined = false;
        std::vector<OWLCardinalityRestriction::Ptr>::iterator bit = b.begin();
        for(; bit != b.end(); ++bit)
        {
            LOG_DEBUG_S << "   -- with : " << (*bit)->toString();
            OWLCardinalityRestriction::Ptr restriction;
            if((*ait)->isObjectRestriction())
            {
                OWLObjectCardinalityRestriction::Ptr aRestriction = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(*ait);
                if(!aRestriction)
                {
                    throw std::runtime_error("owlapi::modell::OWLCardinalityRestriction::join:"
                            " expect Object Restriction");
                }

                OWLCardinalityRestriction::Ptr restriction = aRestriction->join(*bit, operationType);
            } else {
                throw std::runtime_error("owlapi::model::OWLCardinalityRestriction::join:"
                        " joining OWLDataCardinalityRestriction is currently not supported");
            }

            if(restriction)
            {
                // merging succeeded
                restrictions.push_back(restriction);

                // assuming a compact representation of _a and _b, i.e. not multiple
                // definitions of the same type in it we can skip checking the
                // remaining
                b.erase(bit);
                LOG_DEBUG_S << "Joining succeeded: result is " << restriction->toString();
                joined = true;
                break;
            } else {
                LOG_DEBUG_S << "Joining failed";
            }
        }

        if(!joined)
        {
            // seems like *ait did not get intersectiond, so add it to results
            restrictions.push_back((*ait)->clone());
        }
    }
    // At this point the vector restrictions contains all intersectiond ones from a,
    // but left one of b have still to be added
    OWLCardinalityRestriction::PtrList::const_iterator bit = b.begin();
    for(; bit != b.end(); ++bit)
    {
        // make sure we create an indepenant result set
        restrictions.push_back((*bit)->clone());
    }
    return restrictions;
}

} // end namespace model
} // end namespace owlapi

