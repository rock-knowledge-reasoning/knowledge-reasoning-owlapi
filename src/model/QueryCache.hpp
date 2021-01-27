#ifndef OWLAPI_MODEL_QUERY_CACHE_HPP
#define OWLAPI_MODEL_QUERY_CACHE_HPP

#include <unordered_map>
#include <functional>
#include <boost/functional/hash.hpp>
#include <tuple>
#include "IRI.hpp"
#include "OWLCardinalityRestriction.hpp"

namespace std {
using namespace owlapi::model;
template<>
struct hash< tuple<IRIList, IRI, OWLCardinalityRestrictionOps::OperationType> >
{
    size_t operator()(const tuple<IRIList, IRI, OWLCardinalityRestrictionOps::OperationType>& tpl) const
    {
        size_t seed = 0;
        for(const owlapi::model::IRI& iri : get<0>(tpl) )
        {
            boost::hash_combine(seed, iri.toString());
        }
        boost::hash_combine(seed, get<1>(tpl).toString());
        boost::hash_combine(seed, get<2>(tpl));
        return seed;
    }
};
} // end namespace std

namespace owlapi {
namespace model {

class QueryCache
{
public:
    /**
     * Clear the query cache
     */
    void clear();

    std::pair<bool,bool> isSubClassOf(const IRI& iri, const IRI& otherIri) const;

    /**
     * Retrieve a cached call to identify cardinality restrictions
     * \return a pair of actual result and success of the retrieval, if success
     * value is false, then the query result is not available
     */
    std::pair<OWLCardinalityRestriction::PtrList, bool> getCardinalityRestrictions(const IRI& iri,
            const IRI& objectProperty) const;

    /**
     * Retrieve a cached call to identify cardinality restrictions
     * \return a pair of actual result and success of the retrieval, if success
     * value is false, then the query result is not available
     */
    std::pair<OWLCardinalityRestriction::PtrList, bool>
        getCardinalityRestrictions(const IRIList& iri,
            const IRI& objectProperty,
            OWLCardinalityRestrictionOps::OperationType operationType) const;

    /**
     * Cache result of call to identify cardinality restrictions
     */
    void cacheCardinalityRestrictions(const IRI& iri, const IRI& objectProperty,
            const OWLCardinalityRestriction::PtrList& restrictions);

    void cacheCardinalityRestrictions(const IRIList& iris,
            const IRI& objectProperty,
            OWLCardinalityRestrictionOps::OperationType operationType,
            const OWLCardinalityRestriction::PtrList& restrictions);

    /**
     * Cache subclass relationship testing
     */
    void cacheIsSubClassOf(const IRI& iri, const IRI& otherIri, bool isSubclass);

private:
    std::unordered_map< std::pair<IRI, IRI>, OWLCardinalityRestriction::PtrList> mCardinalityRestrictions;

    std::unordered_map< std::tuple<IRIList, IRI,
        OWLCardinalityRestrictionOps::OperationType>, OWLCardinalityRestriction::PtrList>
        mCardinalityRestrictionsOps;

    std::unordered_map< std::pair<IRI, IRI>, bool> mSubClassing;

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_QUERY_CACHE_HPP
