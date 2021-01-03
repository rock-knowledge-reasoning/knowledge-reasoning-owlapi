#ifndef OWLAPI_MODEL_OWL_CARDINALITY_RESTRICTION_OPS_HPP
#define OWLAPI_MODEL_OWL_CARDINALITY_RESTRICTION_OPS_HPP

#include "../SharedPtr.hpp"
#include "../Exceptions.hpp"
#include <map>
#include <vector>
#include <string>

namespace owlapi {
namespace model {

class OWLCardinalityRestriction;

class OWLCardinalityRestrictionOps
{
public:
    virtual ~OWLCardinalityRestrictionOps() = default;

    // Allow to select the operation type for join operations
    enum OperationType { SUM_OP, MIN_OP, MAX_OP };
    static std::map<OperationType, std::string> OperationTypeTxt;

    /**
     * Create intersection of cardinality restrictions which overlap, i.e.
     * data or object restrictions for the very same qualification
     *
     *  examples:
     *   intersection of:
     *   a -- min 1 cam
     *          with
     *          b -- min 1 specialcam, where specialcam subsumes cam
     *          -->  min 1 cam, min 1 special cam
     *  intersection of:
     *  a -- min 1 cam
     *          with
     *          b -- min 1 specialcam, where specialcam subsumes cam
     *          --> min 2 cam + min 1 special cam
     *
     * \return NULL, when candidates are compatible, but cannot be merged
     * since they are do not overlap, otherwise the intersection of both
     * restrictions
     * \throws invalid_argument if restriction are incompatible
     */
    static shared_ptr<OWLCardinalityRestriction> intersection(
            const shared_ptr<OWLCardinalityRestriction>& a,
            const shared_ptr<OWLCardinalityRestriction>& b
    );

    /**
     * Create intersection of two lists of cardinality restrictions according
     * a x b using intersection operator
     * \return list of intersection results
     */
    static std::vector<shared_ptr<OWLCardinalityRestriction>> intersection(
            const std::vector<shared_ptr<OWLCardinalityRestriction> >& a,
            const std::vector<shared_ptr<OWLCardinalityRestriction> >& b
    );

     /**
      * Check if restrictions have same restriction type (data/object), same property
      * and the same qualification
      * \return True, if the conditions hold, false otherwise
      */
    static bool areOverlapping(
            const shared_ptr<OWLCardinalityRestriction>& a,
            const shared_ptr<OWLCardinalityRestriction>& b
    );

    /**
     * Get the join of cardinality restrictions, e.g. for overlapping
     * restrictions, so that overlapping cardinality restriction are
     * join and updated as follows
     *      MAX + MAX = 2*MAX
     *      MIN + MIN = 2*MIN
     *      EXACT + EXACT = 2*EXACT
     * \see areOverlapping
     * \param a List of cardinality restrictions
     * \param b Cardinality restriction to be joined with a
     * \param operationType SUM_OP to use the sum of cardinalities, MAX_OP to
     * use the maximum of cardinalities and MIN_OP to use the minimum of
     * cardinalities
     */
    static shared_ptr<OWLCardinalityRestriction> join(
            const shared_ptr<OWLCardinalityRestriction>& a,
            const shared_ptr<OWLCardinalityRestriction>& b,
            OperationType operationType = SUM_OP
    );

    /**
     * Join overlapping restrictions of two compact (!) lists of restrictions,
     * \see areOverlapping
     * \see join
     * \param a List of cardinality restrictions
     * \param b List of cardinality restrictions to be joined with a
     * \param operationType One of SUM_OP, MIN_OP, or MAX_OP
     */
    static std::vector<shared_ptr<OWLCardinalityRestriction>> join(
            const std::vector<shared_ptr<OWLCardinalityRestriction> >& a,
            const std::vector<shared_ptr<OWLCardinalityRestriction> >& b,
            OperationType operationType = SUM_OP
    );

    /**
     * Create a compact representation of cardinality restrictions by joining
     * the existing cardinalities with the provided
     * \param a List of cardinality restrictions
     * \param operationType operationType to use for joining cardinalities
     */
    static std::vector<shared_ptr<OWLCardinalityRestriction>> compact(
            const std::vector<shared_ptr<OWLCardinalityRestriction> >& a,
            OperationType operationType = SUM_OP
    );

    /**
     * Scale the cardinality by a given factor
     */
    static std::vector<shared_ptr<OWLCardinalityRestriction>> scale(
            const std::vector< shared_ptr<OWLCardinalityRestriction> >& a,
            size_t factor
    );

protected:
    virtual bool isOverlapping(const shared_ptr<OWLCardinalityRestriction>& other) = 0;

    virtual shared_ptr<OWLCardinalityRestriction> intersection(
            const shared_ptr<OWLCardinalityRestriction>& other
    ) = 0;

    virtual shared_ptr<OWLCardinalityRestriction> join(
            const shared_ptr<OWLCardinalityRestriction>& other,
            OperationType operationType = SUM_OP
    ) = 0;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_CARDINALITY_RESTRICTION_OPS_HPP
