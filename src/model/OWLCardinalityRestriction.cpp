#include "OWLCardinalityRestriction.hpp"
#include "OWLObjectExactCardinality.hpp"
#include "OWLObjectMinCardinality.hpp"
#include "OWLObjectMaxCardinality.hpp"
#include <algorithm>
#include <sstream>
#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>

namespace owlapi {
namespace model {

std::map<OWLCardinalityRestriction::CardinalityRestrictionType, std::string> OWLCardinalityRestriction::CardinalityRestrictionTypeTxt = boost::assign::map_list_of
    (UNKNOWN, "UNKNOWN")
    (MIN, "MIN")
    (MAX, "MAX")
    (EXACT, "EXACT");

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::narrow() const
{
    if(getProperty()->isObjectPropertyExpression())
    {
        OWLObjectPropertyExpression::Ptr property = boost::dynamic_pointer_cast<OWLObjectPropertyExpression>( getProperty() );

        switch(getCardinalityRestrictionType())
        {
            case OWLCardinalityRestriction::MIN:
                return OWLCardinalityRestriction::Ptr( new OWLObjectMinCardinality(property, getCardinality(), getQualification()) ) ;
            case OWLCardinalityRestriction::MAX:
                return OWLCardinalityRestriction::Ptr(new OWLObjectMaxCardinality(property, getCardinality(), getQualification()) );
            case OWLCardinalityRestriction::EXACT:
                return OWLCardinalityRestriction::Ptr(new OWLObjectExactCardinality(property, getCardinality(), getQualification()) );
            default:
                throw std::runtime_error("OWLCardinalityRestriction::narrow: cardinality set to UNKNOWN cannot narrow");
        }
    }
    throw std::runtime_error("OWLCardinalityRestriction::narrow: has not been implemented for data property expression '" + getProperty()->toString() + "'");
}


OWLCardinalityRestriction::OWLCardinalityRestriction()
    : OWLQualifiedRestriction( OWLPropertyExpression::Ptr(), OWLQualification(""))
    , mCardinality(0)
    , mCardinalityRestrictionType(OWLCardinalityRestriction::UNKNOWN)
{}

OWLCardinalityRestriction::OWLCardinalityRestriction(OWLPropertyExpression::Ptr property,
        uint32_t cardinality,
        const OWLQualification& qualification,
        CardinalityRestrictionType restrictionType)
    : OWLQualifiedRestriction(property, qualification)
    , mCardinality(cardinality)
    , mCardinalityRestrictionType(restrictionType)
{}

std::string OWLCardinalityRestriction::toString() const
{
    std::stringstream ss;
    ss << "OWLCardinalityRestriction:" << std::endl;
    ss << "    property: " << getProperty()->toString() << std::endl;
    ss << "    cardinality: " << getCardinality() << std::endl;
    ss << "    qualification: " << getQualification().toString() << std::endl;
    ss << "    type: " << CardinalityRestrictionTypeTxt[getCardinalityRestrictionType()] << std::endl;
    return ss.str();
}


OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::getInstance(OWLPropertyExpression::Ptr property,
        uint32_t cardinality,
        const OWLQualification& qualification,
        CardinalityRestrictionType restrictionType)
{
    OWLCardinalityRestriction restriction(property, cardinality, qualification, restrictionType);
    return restriction.narrow();
}

std::map<IRI, uint32_t> OWLCardinalityRestriction::convertToExactMapping(const std::vector<OWLCardinalityRestriction::Ptr>& restrictions)
{
    std::map<IRI, uint32_t> exactMapping;
    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
    for(; cit != restrictions.end(); ++cit)
    {
        OWLCardinalityRestriction::Ptr restriction = *cit;
        // Here we assume that
        //  max 3 and max 4 -> exact 4
        //  min 3 and max 4 -> exact 4
        //  exact 1 and exact 2 -> exact 4
        uint32_t current = exactMapping[restriction->getQualification()];
        exactMapping[restriction->getQualification()] = std::max(current, restriction->getCardinality());
    }

    return exactMapping;
}

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::merge(OWLCardinalityRestriction::Ptr a,
        OWLCardinalityRestriction::Ptr b)
{
    if(areOverlapping(a,b))
    {
        CardinalityRestrictionType aType = a->getCardinalityRestrictionType();
        CardinalityRestrictionType bType = b->getCardinalityRestrictionType();

        if(aType == bType)
        {
            switch(aType)
            {
                case MIN:
                    return getInstance(a->getProperty(),
                              std::max(a->getCardinality(), b->getCardinality()),
                              a->getQualification(),
                              MIN);
                case MAX:
                    return getInstance(a->getProperty(),
                            std::min(a->getCardinality(), b->getCardinality()),
                            a->getQualification(),
                            MAX);
                case EXACT:
                    {
                        if(a->getCardinality() != b->getCardinality())
                        {
                            std::stringstream ss;
                            ss << "owlapi::model::OWLCardinalityRestriction::merge ";
                            ss << "incompatible EXACT cardinality restrictions on property ";
                            ss << "'" << a->getProperty()->toString() << "' ";
                            ss << " qualification ";
                            ss << "'" << a->getQualification() << "' ";
                            ss << " cardinality: ";
                            ss << a->getCardinality();
                            ss << " vs. ";
                            ss << b->getCardinality();

                            throw std::invalid_argument(ss.str());
                        }

                        return getInstance(a->getProperty(),
                                a->getCardinality(),
                                a->getQualification(),
                                EXACT);
                    }
                default:
                    throw std::runtime_error("owlapi::model::OWLCardinalityRestriction::merge  \
                             invalid cardinality restriction of type UNKNOWN");
            }
        } else if(aType == MIN && bType == MAX)
        {
            return mergeMinMax( boost::dynamic_pointer_cast<OWLMinCardinalityRestriction>(a),
                    boost::dynamic_pointer_cast<OWLMaxCardinalityRestriction>(b));
        } else if(aType == MAX && bType == MIN)
        {
            return mergeMinMax(boost::dynamic_pointer_cast<OWLMinCardinalityRestriction>(b),
                    boost::dynamic_pointer_cast<OWLMaxCardinalityRestriction>(a));
        }
    }

    return OWLCardinalityRestriction::Ptr();
}

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::mergeMinMax(OWLMinCardinalityRestriction::Ptr a,
    OWLMaxCardinalityRestriction::Ptr b)
{
    if(a->getCardinality() == b->getCardinality())
    {
       return getInstance(a->getProperty(),
                    a->getCardinality(),
                    a->getQualification(),
                    EXACT);
    } else if(a->getCardinality() > b->getCardinality())
    {
        std::stringstream ss;
        ss << "owlapi::model::OWLCardinalityRestriction::merge ";
        ss << "incompatible MIN/MAX cardinality restrictions on property ";
        ss << "'" << a->getProperty()->toString() << "' ";
        ss << " qualification ";
        ss << "'" << a->getQualification() << "' ";
        ss << " min cardinality: ";
        ss << a->getCardinality();
        ss << " vs. max cardinality: ";
        ss << b->getCardinality();

        throw std::invalid_argument(ss.str());
    }

    return OWLCardinalityRestriction::Ptr();
}

std::vector<OWLCardinalityRestriction::Ptr> OWLCardinalityRestriction::merge(const std::vector<OWLCardinalityRestriction::Ptr>& a,
            const std::vector<OWLCardinalityRestriction::Ptr>& _b)
{
    std::vector<OWLCardinalityRestriction::Ptr> restrictions;
    std::vector<OWLCardinalityRestriction::Ptr> b = _b;

    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator ait = a.begin();
    for(; ait != a.end(); ++ait)
    {
        LOG_DEBUG_S << "Try merging : " << (*ait)->toString();
        bool merged = false;
        std::vector<OWLCardinalityRestriction::Ptr>::iterator bit = b.begin();
        for(; bit != b.end(); ++bit)
        {
            LOG_DEBUG_S << "   -- with : " << (*bit)->toString();
            OWLCardinalityRestriction::Ptr aRestriction= *ait;
            OWLCardinalityRestriction::Ptr bRestriction= *bit;

            OWLCardinalityRestriction::Ptr restriction = merge(aRestriction, bRestriction);
            if(restriction)
            {
                // merging succeeded
                restrictions.push_back(restriction);

                // assuming a compact representation of _a and _b, i.e. not multiple
                // definitions of the same type in it we can skip checking the
                // remaining
                b.erase(bit);
                LOG_DEBUG_S << "Merging succeeded: result is " << restriction->toString();
                merged = true;
                break;
            }
        }

        if(!merged)
        {
            // seems like *ait did not get merged, so add it to results
            restrictions.push_back(*ait);
        }
    }
    // At this point the vector restrictions contains all merged ones from a,
    // but left one of b have still to be added
    restrictions.insert(restrictions.begin(), b.begin(), b.end());
    return restrictions;
}


bool OWLCardinalityRestriction::areOverlapping(OWLCardinalityRestriction::Ptr a,
            OWLCardinalityRestriction::Ptr b)
{
    if((a->isDataRestriction() && b->isDataRestriction()) ||
            (a->isObjectRestriction() && b->isObjectRestriction()))
    {
        if(a->getProperty() != b->getProperty())
        {
            return false;
        }

        if(a->getQualification() != b->getQualification())
        {
            return false;
        }
        return true;
    }

    return false;
}

} // end namespace model
} // end namespace owlapi

