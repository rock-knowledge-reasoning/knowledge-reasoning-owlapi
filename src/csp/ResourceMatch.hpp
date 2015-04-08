#ifndef OWLAPI_CSP_RESOURCE_MATCH_HPP
#define OWLAPI_CSP_RESOURCE_MATCH_HPP

#include <gecode/set.hh>
#include <gecode/int.hh>
#include <gecode/search.hh>

#include <owlapi/model/OWLOntology.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>
#include <base/Logging.hpp>

namespace owlapi {
namespace csp {

typedef std::map<owlapi::model::IRI, std::vector<int> > TypeInstanceMap;
typedef std::map<owlapi::model::IRI, owlapi::model::IRIList > AllowedTypesMap;
typedef owlapi::model::IRIList InstanceList;
typedef owlapi::model::IRIList TypeList;

/**
 * ResourceMatch allows to search for a valid solution to a CSP problem.
 * The CSP problem is defined by a query that searches to fulfill a set of cardinality
 * restrictions. Available resources are implicitly defined -- also by a list of
 * cardinality restrictions. This originates from the fact that these
 * restrictions describe a model that a certain instance ('robot') fulfills and thus
 * defines which resource have to exist for this robot.
 *
 * While in an actual instance resources might be missing solving the csp allows for model
 * checking in the first place.
 *
 \verbatim

 using namespace owlapi;

 OWLOntology::Ptr ontology = reader.fromFile("organization-model.owl");
 ...
 IRI sherpa = owlapi::vocabulary::OM::resolve("Sherpa");
 IRI move_to = owlapi::vocabulary::OM::resolve("MoveTo");

 std::vector<OWLCardinalityRestriction::Ptr> r_sherpa = ask.getCardinalityRestrictions(sherpa);
 std::vector<OWLCardinalityRestriction::Ptr> r_move_to = ask.getCardinalityRestrictions(move_to);

 try {
     // Check whether 'move_to' provides a subset of restriction existing for 'sherpa'
     csp::ResourceMatch* fulfillment = cs::ResourceMatch::solve(r_move_to, r_sherpa, ontology);
     std::cout << fulfillment->toString() << std::endl;
     ...
     delete fulfillment;
 } catch(const std::runtime_error& e)
 {
     std::cout << "No solution found" << std::endl;
 }

 // or in compact form
 ResourceMatch::Ptr fulfillment = csp::ResourceMatch::fulfills(sherpa, move_to, ontology);
 if(fulfillment)
 {
    std::cout << fulfillment->toString() << std::endl;
 }
 \endverbatim
 */
class ResourceMatch : public Gecode::Space
{
    std::vector<owlapi::model::OWLCardinalityRestriction::Ptr> mQueryRestrictions;
    std::vector<owlapi::model::OWLCardinalityRestriction::Ptr> mResourcePoolRestrictions;
    std::map<owlapi::model::OWLCardinalityRestriction::Ptr, InstanceList> mSolution;
    InstanceList mResourcePool;

    /**
     * Assignments of query resources to pool resources. This is what has to be solved.
     */
    Gecode::SetVarArray mSetAssignment;

    ResourceMatch* solve();

protected:
    ResourceMatch(const std::vector<owlapi::model::OWLCardinalityRestriction::Ptr>& queryRestrictions,
            const InstanceList& resourcePoolRestrictions, owlapi::model::OWLOntology::Ptr ontology);

    /**
     * Search support
     * This copy constructor is required for the search engine
     * and it has to provide a deep copy
     */
    ResourceMatch(bool share, ResourceMatch& s);

    /**
     * This method is called by the search engine
     */
    virtual Gecode::Space* copy(bool share);

    /**
     * Convert restrictions to type instance map, where a qualification is
     * mapped to a list of instances (identified by an id)
     * \return Type to instances map
     */
    static TypeInstanceMap toTypeInstanceMap(const std::vector<owlapi::model::OWLCardinalityRestriction::Ptr>& restrictions);

    /**
     * Get the list of types that are given by the restrictions
     * \return List of types
     */
    static TypeList getTypeList(const std::vector<owlapi::model::OWLCardinalityRestriction::Ptr>& restrictions);

    /**
     * Identify the allowed types, i.e., what items in the pool can fulfill the
     * items in the query.
     * Either items of the same class or subclasses are allowed
     */
    static AllowedTypesMap getAllowedTypes(const TypeInstanceMap& query, const TypeInstanceMap& pool,
            owlapi::model::OWLOntology::Ptr ontology);

    /**
     * Compute the allowed domains for Gecode.
     * This the qualification item to the actual instances that can 'fulfill'
     * this qualification item.
     */
    static std::vector<int> getAllowedDomain(const owlapi::model::IRI& qualificationItem,
            const AllowedTypesMap& allowedTypes, const TypeInstanceMap& typeInstanceMap);

    /**
     * Compute the number of all instances in a TypeInstanceMap
     * \return Number of instances
     */
    static uint32_t getInstanceCount(const TypeInstanceMap& map);

    /**
     * Map solution from internally used integer list to input format, i.e.
     * cardinality restrictions
     */
    void remapSolution();

public:
    typedef boost::shared_ptr<ResourceMatch> Ptr;

    void print(std::ostream& os) const;

    /**
     * Convert restriction list to a list of instances, e.g., a restriction of min 4 items will be
     * converted to a list of items (qualification of the restriction) where the
     * list has size 4
     * \return List of (model) instances
     */
    static InstanceList getInstanceList(const std::vector<owlapi::model::OWLCardinalityRestriction::Ptr>& restrictions);

    /*
     * constrain function for best solution search. the
     * currently best solution _b is passed and we have to constraint that this solution can only
     * be better than b, for it to be excluded if it isn't
     */
    //virtual void constrain(const Gecode::Space& _b);

    /**
     * Construct a solution with an initial situation to search
     * \throw std::runtime_error if a solution could not be found
     * \return Solution to the constrained satisfaction problem as ResourceMatch
     * object, receiver takes over ownership, i.e. object needs to be deleted
     * \throws std::runtime_error if no solution could be found
     */
    static ResourceMatch* solve(const std::vector<owlapi::model::OWLCardinalityRestriction::Ptr>& queryRestrictions,
            const std::vector<owlapi::model::OWLCardinalityRestriction::Ptr>& resourcePoolRestrictions, owlapi::model::OWLOntology::Ptr ontology);

    /**
     * Solve the given constraint satisfaction problem
     * \param queryRestrictions The restrictions to be fulfilled
     * \param resourcePool The items available to fulfill the restriction
     * \param ontology Ontology to check whether an item in the resource pool is a valid replacement for an item in the query
     * \return Solution to the constrained satisfaction problem as ResourceMatch
     * object, receiver takes over ownership, i.e. object needs to be deleted
     * \throws std::runtime_error if no solution could be found
     */
    static ResourceMatch* solve(const std::vector<owlapi::model::OWLCardinalityRestriction::Ptr>& queryRestrictions,
            const InstanceList& resourcePool, owlapi::model::OWLOntology::Ptr ontology);

    /**
     * Create a string representation of this object
     * \return Stringified ResourceMatch object
     */
    std::string toString() const;

    /**
     * Retrieve assignment of a solution for a given restriction
     * \return InstanceList that has been assigned from the resourcePool to
     * fulfill this restriction
     */
    InstanceList getAssignedResources(owlapi::model::OWLCardinalityRestriction::Ptr restriction) const;

    /**
     * Retrieve the list of resources that remain unassigned
     * \return InstanceList representing unassigned resources
     */
    InstanceList getUnassignedResources() const;

    /**
     * Check if the serviceModel is supported by the providerModel
     * \param providerModel
     * \param serviceModel
     * \param resourceMatch
     * \return Pointer to resource match, which is NULL if there is no solution
     */
    static ResourceMatch::Ptr isSupporting(const owlapi::model::IRI& providerModel, const owlapi::model::IRI& serviceModel,
            owlapi::model::OWLOntology::Ptr ontology);
};

} // end namespace cps
} // end namespace owlapi
#endif // OWLAPI_CSP_RESOURCE_MATCH_HPP
