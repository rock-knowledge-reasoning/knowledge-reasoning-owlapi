#include "QueryCache.hpp"

namespace owlapi {
namespace model {


void QueryCache::clear()
{
    mCardinalityRestrictions.clear();
    mCardinalityRestrictionsOps.clear();
}

std::pair<bool,bool> QueryCache::isSubClassOf(const IRI& iri, const IRI& otherIri) const
{
    std::pair<bool, bool> result;
    std::unordered_map< std::pair<IRI, IRI>, bool>::const_iterator cit = mSubClassing.find( std::pair<IRI,IRI>(iri, otherIri) );
    if(cit != mSubClassing.end())
    {
        result.first = cit->second;
        result.second = true;
    } else {
        result.second = false;
    }
    return result;
}

std::pair<OWLCardinalityRestriction::PtrList, bool> QueryCache::getCardinalityRestrictions(const IRI& iri,
        const IRI& objectProperty) const
{
    std::pair<OWLCardinalityRestriction::PtrList, bool> result;
    std::unordered_map< std::pair<IRI, IRI>, OWLCardinalityRestriction::PtrList >::const_iterator cit =
    mCardinalityRestrictions.find( std::pair<IRI,IRI>(iri, objectProperty) );
    if(cit != mCardinalityRestrictions.end())
    {
        result.first = cit->second;
        result.second = true;
    } else {
        result.second = false;
    }
    return result;
}

void QueryCache::cacheCardinalityRestrictions(const IRI& iri, const IRI& objectProperty,
            const OWLCardinalityRestriction::PtrList& restrictions)
{
    mCardinalityRestrictions.emplace(std::pair<IRI, IRI>(iri, objectProperty), restrictions);

}

std::pair<OWLCardinalityRestriction::PtrList, bool> QueryCache::getCardinalityRestrictions(const IRIList& iris,
        const IRI& objectProperty,
        OWLCardinalityRestrictionOps::OperationType operationType) const
{
    std::pair<OWLCardinalityRestriction::PtrList, bool> result;
    std::unordered_map< std::tuple<IRIList, IRI, OWLCardinalityRestrictionOps::OperationType>, OWLCardinalityRestriction::PtrList >::const_iterator cit;

    std::tuple<IRIList,IRI,OWLCardinalityRestrictionOps::OperationType> tpl = std::make_tuple(iris, objectProperty, operationType);
    cit = mCardinalityRestrictionsOps.find(tpl);

    if(cit != mCardinalityRestrictionsOps.end())
    {
        result.first = cit->second;
        result.second = true;
    } else {
        result.second = false;
    }
    return result;
}

void QueryCache::cacheCardinalityRestrictions(const IRIList& iris,
        const IRI& objectProperty,
        OWLCardinalityRestrictionOps::OperationType operationType,
        const OWLCardinalityRestriction::PtrList& restrictions)
{
    std::tuple<IRIList,IRI,OWLCardinalityRestrictionOps::OperationType> tpl = std::make_tuple(iris, objectProperty, operationType);
    mCardinalityRestrictionsOps.emplace(tpl,restrictions);

}

void QueryCache::cacheIsSubClassOf(const IRI& iri, const IRI& otherIri, bool isSubclass)
{
    mSubClassing.emplace( std::pair<IRI, IRI>(iri, otherIri), isSubclass);
}

} // end namespace model
} // end namespace owlapi
