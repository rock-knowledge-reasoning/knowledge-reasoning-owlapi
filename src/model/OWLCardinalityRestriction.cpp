#include "OWLCardinalityRestriction.hpp"
#include "OWLObjectExactCardinality.hpp"
#include "OWLObjectMinCardinality.hpp"
#include "OWLObjectMaxCardinality.hpp"
#include <algorithm>
#include <limits>
#include <math.h>
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

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::clone() const
{
    return getInstance(getProperty(), mCardinality, getQualification(), mCardinalityRestrictionType);
}

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

std::string OWLCardinalityRestriction::toString(const std::vector<OWLCardinalityRestriction::Ptr>& restrictions)
{
    std::stringstream ss;
    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
    for(; cit != restrictions.end(); ++cit)
    {
        ss << (*cit)->toString();
    }
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

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::intersection(OWLCardinalityRestriction::Ptr a,
        OWLCardinalityRestriction::Ptr b)
{
    if(areOverlapping(a,b))
    {
        CardinalityRestrictionType aType = a->getCardinalityRestrictionType();
        CardinalityRestrictionType bType = b->getCardinalityRestrictionType();

        // Same cardinality type
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
                            ss << "owlapi::model::OWLCardinalityRestriction::intersection ";
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
                    throw std::runtime_error("owlapi::model::OWLCardinalityRestriction::intersection  \
                             invalid cardinality restriction of type UNKNOWN");
            }
        } else if(aType == MIN && bType == MAX)
        {
            return intersectionMinMax( boost::dynamic_pointer_cast<OWLMinCardinalityRestriction>(a),
                    boost::dynamic_pointer_cast<OWLMaxCardinalityRestriction>(b));
        } else if(aType == MAX && bType == MIN)
        {
            return intersectionMinMax(boost::dynamic_pointer_cast<OWLMinCardinalityRestriction>(b),
                    boost::dynamic_pointer_cast<OWLMaxCardinalityRestriction>(a));
        } else if(aType == EXACT)
        {
            return intersectionExact(boost::dynamic_pointer_cast<OWLExactCardinalityRestriction>(a),
                    b);
        } else if(bType == EXACT)
        {
            return intersectionExact(boost::dynamic_pointer_cast<OWLExactCardinalityRestriction>(b),
                    a);
        }
    }

    return OWLCardinalityRestriction::Ptr();
}

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::intersectionExact(OWLExactCardinalityRestriction::Ptr exact,
        OWLCardinalityRestriction::Ptr b)
{
    switch(b->getCardinalityRestrictionType())
    {
        case MIN:
            if(exact->getCardinality() >= b->getCardinality())
            {
                return exact->clone();
            }
            throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionExact: incompatible \
                    cardinality restriction of exact and min : " + exact->toString() + " vs. " + b->toString());
        case MAX:
            if(exact->getCardinality() <= b->getCardinality())
            {
                return exact->clone();
            }
            throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionExact: incompatible \
                    cardinality restriction of exact and max : " + exact->toString() + " vs. " + b->toString());
        case EXACT:
            if(exact->getCardinality() == b->getCardinality())
            {
                return exact->clone();
            }
            throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionExact: incompatible \
                    cardinality restriction of exact and exact: " + exact->toString() + " vs. " + b->toString());
        default:
            throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionExact: \
                    cannot handle UNKNOWN type");
    }
}

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::intersectionMinMax(OWLMinCardinalityRestriction::Ptr a,
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
        ss << "owlapi::model::OWLCardinalityRestriction::intersection ";
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

std::vector<OWLCardinalityRestriction::Ptr> OWLCardinalityRestriction::intersection(const std::vector<OWLCardinalityRestriction::Ptr>& a,
            const std::vector<OWLCardinalityRestriction::Ptr>& _b)
{
    std::vector<OWLCardinalityRestriction::Ptr> restrictions;
    std::vector<OWLCardinalityRestriction::Ptr> b = _b;

    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator ait = a.begin();
    for(; ait != a.end(); ++ait)
    {
        LOG_DEBUG_S << "Try merging : " << (*ait)->toString();
        bool intersectiond = false;
        std::vector<OWLCardinalityRestriction::Ptr>::iterator bit = b.begin();
        for(; bit != b.end(); ++bit)
        {
            LOG_DEBUG_S << "   -- with : " << (*bit)->toString();
            OWLCardinalityRestriction::Ptr aRestriction= *ait;
            OWLCardinalityRestriction::Ptr bRestriction= *bit;

            OWLCardinalityRestriction::Ptr restriction = intersection(aRestriction, bRestriction);
            if(restriction)
            {
                // merging succeeded
                restrictions.push_back(restriction);

                // assuming a compact representation of _a and _b, i.e. not multiple
                // definitions of the same type in it we can skip checking the
                // remaining
                b.erase(bit);
                LOG_DEBUG_S << "Merging succeeded: result is " << restriction->toString();
                intersectiond = true;
                break;
            }
        }

        if(!intersectiond)
        {
            // seems like *ait did not get intersectiond, so add it to results
            restrictions.push_back((*ait)->clone());
        }
    }
    // At this point the vector restrictions contains all intersectiond ones from a,
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

OWLCardinalityRestriction::Ptr OWLCardinalityRestriction::join(OWLCardinalityRestriction::Ptr a,
        OWLCardinalityRestriction::Ptr b)
{
    if(areOverlapping(a,b))
    {
        if(a->getCardinalityRestrictionType() == b->getCardinalityRestrictionType())
        {
            return getInstance(a->getProperty(),
                            a->getCardinality() + b->getCardinality(),
                            a->getQualification(),
                            a->getCardinalityRestrictionType());
        }
    }
    return OWLCardinalityRestriction::Ptr();
}

std::vector<OWLCardinalityRestriction::Ptr> OWLCardinalityRestriction::join(const std::vector<OWLCardinalityRestriction::Ptr>& a,
        const std::vector<OWLCardinalityRestriction::Ptr>& _b)
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
            OWLCardinalityRestriction::Ptr aRestriction= *ait;
            OWLCardinalityRestriction::Ptr bRestriction= *bit;

            OWLCardinalityRestriction::Ptr restriction = join(aRestriction, bRestriction);
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
    restrictions.insert(restrictions.begin(), b.begin(), b.end());
    return restrictions;
}

std::vector<OWLCardinalityRestriction::Ptr> OWLCardinalityRestriction::compact(const std::vector<OWLCardinalityRestriction::Ptr>& restrictions)
{
    std::vector<OWLCardinalityRestriction::Ptr> compactRestrictions;
    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
    for(; cit != restrictions.end(); ++cit)
    {
        std::vector<OWLCardinalityRestriction::Ptr> singleRestriction;
        singleRestriction.push_back(*cit);

        std::vector<OWLCardinalityRestriction::Ptr> merged = join(compactRestrictions, singleRestriction);
        compactRestrictions = merged;
    }

    return compactRestrictions;
}

std::map<IRI, OWLCardinalityRestriction::MinMax> OWLCardinalityRestriction::getBounds(const std::vector<OWLCardinalityRestriction::Ptr>& restrictions)
{
    owlapi::model::OWLPropertyExpression::Ptr property;
    std::map<IRI, OWLCardinalityRestriction::MinMax> modelCount;

    // We assume a compact list of the query restrictions, but
    // have to generate an intermediate bounded representation for each 
    // possible resource model
    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
    for(; cit != restrictions.end(); ++cit)
    {
        OWLCardinalityRestriction::Ptr restriction = *cit;

        // Check if the same property is used for all cardinality restrictions
        // Note: might to too restrictive, but need to verify lateron
        if(!property)
        {
            property = restriction->getProperty();
        } else {
            if(property->toString() != restriction->getProperty()->toString())
            {
                throw std::invalid_argument("owlapi::model::getBounds cardinality restrictions are inconsistent, i.e. apply to different properties: " + \
                        property->toString() + " vs. " + restriction->getProperty()->toString());
            }
        }

        uint32_t cardinality = restriction->getCardinality();
        owlapi::model::IRI qualification = restriction->getQualification();
        std::pair<uint32_t,uint32_t>& minMax = modelCount[qualification];

        if(restriction->getCardinalityRestrictionType() == owlapi::model::OWLCardinalityRestriction::MAX)
        {
            minMax.first = std::max(static_cast<uint32_t>(0), minMax.first);
            if(minMax.second == 0) // has not been initialized yet
            {
                minMax.second = cardinality;
            } else {
                minMax.second = std::min(cardinality, minMax.second);
            }
        } else if(restriction->getCardinalityRestrictionType() == owlapi::model::OWLCardinalityRestriction::MIN)
        {
            minMax.first = std::max(cardinality, minMax.first);
            minMax.second = std::numeric_limits<uint32_t>::max();
        } else if(restriction->getCardinalityRestrictionType() == owlapi::model::OWLCardinalityRestriction::EXACT)
        {
            minMax.first = cardinality;
            minMax.second = cardinality;
        }
    }

    return modelCount;
}

} // end namespace model
} // end namespace owlapi

