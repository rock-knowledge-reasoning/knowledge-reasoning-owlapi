#ifndef OWLAPI_MODEL_OBJECT_CARDINALITY_RESTRICTION_HPP
#define OWLAPI_MODEL_OBJECT_CARDINALITY_RESTRICTION_HPP

#include "OWLCardinalityRestriction.hpp"
#include "OWLQuantifiedObjectRestriction.hpp"

namespace owlapi {
namespace model {

class OWLObjectCardinalityRestriction : public OWLCardinalityRestriction,
    public OWLQuantifiedObjectRestriction
{
public:
    friend class OWLCardinalityRestrictionOps;

    typedef shared_ptr<OWLObjectCardinalityRestriction> Ptr;

    OWLObjectCardinalityRestriction(
            const OWLPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLClassExpression::Ptr& qualification,
            OWLCardinalityRestriction::CardinalityRestrictionType restrictionType
    );

    virtual ~OWLObjectCardinalityRestriction() = default;

    static OWLCardinalityRestriction::Ptr createInstance(
            const OWLObjectPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLClassExpression::Ptr& qualification,
            OWLCardinalityRestriction::CardinalityRestrictionType type
    );

    /**
     * If min and max cardinality overlap, the intersection can result
     * 1. in an exact cardinality
     * 2. in a invalid restriction, where min > max cardinality
     *
     * \return in case 1. an exact cardinality, if min max are valid, but not
     * overlapping -- return a NULL pointer
     * \throws an invalid argument exception in case 2.
     */
    static OWLObjectCardinalityRestriction::Ptr intersectionMinMax(
            const OWLObjectCardinalityRestriction& a,
            const OWLObjectCardinalityRestriction& b
    );

    /**
     * Merge intersection with exact cardinality constraint
     */
    static OWLObjectCardinalityRestriction::Ptr intersectionExact(
            const OWLObjectCardinalityRestriction& a,
            const OWLObjectCardinalityRestriction& b
    );

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
    OWLCardinalityRestriction::Ptr intersection(const OWLCardinalityRestriction::Ptr& a) override;

     /**
      * Check if restrictions have same restriction type (data/object), same property
      * and the same qualification
      * \return True, if the conditions hold, false otherwise
      */
    bool isOverlapping(const OWLCardinalityRestriction::Ptr& other) override;

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
    OWLCardinalityRestriction::Ptr join(const OWLCardinalityRestriction::Ptr& other,
        OWLCardinalityRestrictionOps::OperationType operationType = OWLCardinalityRestrictionOps::SUM_OP) override;

    /**
     * Retrieve bounds for the qualifications for a set of cardinality restrictions
     * \throws invalid_argument if restrictions do not refer to the same property
     */
    static std::map<IRI, MinMax> getBounds(const std::vector<OWLCardinalityRestriction::Ptr>& e);

    /**
     * Check if cardinality restriction is qualified
     */
    bool isQualified() const  { return getFiller() != nullptr; }

    /**
     * Return class qualification if available
     */
    IRI getQualification() const;

    virtual std::string toString() const override { return toString(0); }

    virtual std::string toString(size_t indent) const override;

protected:
    OWLCardinalityRestriction::Ptr doClone() const;

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OBJECT_CARDINALITY_RESTRICTION_HPP
