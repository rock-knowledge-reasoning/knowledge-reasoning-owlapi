#include "OWLCardinalityRestrictionOps.hpp"
#include "OWLCardinalityRestriction.hpp"
#include "OWLObjectCardinalityRestriction.hpp"
#include "OWLDataCardinalityRestriction.hpp"

#include <base-logging/Logging.hpp>

namespace owlapi {
namespace model {

std::map<OWLCardinalityRestrictionOps::OperationType, std::string> OWLCardinalityRestrictionOps::OperationTypeTxt = {
    {SUM_OP, "SUM_OP"},
    {MIN_OP, "MIN_OP"},
    {MAX_OP, "MAX_OP"}
};

OWLCardinalityRestriction::Ptr OWLCardinalityRestrictionOps::intersection(const OWLCardinalityRestriction::Ptr& _a,
        const OWLCardinalityRestriction::Ptr& _b)
{
    OWLObjectCardinalityRestriction::Ptr oRestriction = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(_a);
    if(oRestriction)
    {
        return oRestriction->intersection(_b);
    }

    OWLDataCardinalityRestriction::Ptr dRestriction = dynamic_pointer_cast<OWLDataCardinalityRestriction>(_a);
    if(dRestriction)
    {
        return dRestriction->intersection(_b);
    }

    throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersection:"
            " can only intersect data or object restrictions");
}

std::vector<OWLCardinalityRestriction::Ptr> OWLCardinalityRestrictionOps::intersection(
        const std::vector<OWLCardinalityRestriction::Ptr>& a,
        const std::vector<OWLCardinalityRestriction::Ptr>& _b
)
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

std::vector<OWLCardinalityRestriction::Ptr> OWLCardinalityRestrictionOps::compact(
        const std::vector<OWLCardinalityRestriction::Ptr>& restrictions,
        OperationType operationType
)
{
    std::vector<OWLCardinalityRestriction::Ptr> compactRestrictions;
    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
    for(; cit != restrictions.end(); ++cit)
    {
        std::vector<OWLCardinalityRestriction::Ptr> singleRestriction;
        singleRestriction.push_back(*cit);

        std::vector<OWLCardinalityRestriction::Ptr> merged = join(compactRestrictions, singleRestriction, operationType);
        compactRestrictions = merged;
    }

    return compactRestrictions;
}

std::vector<OWLCardinalityRestriction::Ptr> OWLCardinalityRestrictionOps::scale(
        const std::vector<OWLCardinalityRestriction::Ptr>& a,
        size_t factor
)
{
    LOG_DEBUG_S << "Scale restrictions by factor: " << factor;
    std::vector<OWLCardinalityRestriction::Ptr> scaledRestrictions;

    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = a.begin();
    for(; cit != a.end(); ++cit)
    {
        OWLCardinalityRestriction::Ptr restriction = (*cit)->clone();
        restriction->setCardinality( restriction->getCardinality() * factor );
        scaledRestrictions.push_back(restriction);
    }

    return scaledRestrictions;
}


OWLCardinalityRestriction::Ptr OWLCardinalityRestrictionOps::join(const OWLCardinalityRestriction::Ptr& a,
        const OWLCardinalityRestriction::Ptr& b,
        OperationType operationType)
{
    OWLObjectCardinalityRestriction::Ptr oRestriction = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(a);
    if(oRestriction)
    {
        return oRestriction->join(b, operationType);
    }

    OWLDataCardinalityRestriction::Ptr dRestriction = dynamic_pointer_cast<OWLDataCardinalityRestriction>(a);
    if(dRestriction)
    {
        return dRestriction->join(b, operationType);
    }

    throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::join:"
            " can only join data or object restrictions");
}

std::vector<OWLCardinalityRestriction::Ptr> OWLCardinalityRestrictionOps::join(
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
            OWLCardinalityRestriction::Ptr aRestriction= *ait;
            OWLCardinalityRestriction::Ptr bRestriction= *bit;

            OWLCardinalityRestriction::Ptr restriction = join(aRestriction, bRestriction, operationType);
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

