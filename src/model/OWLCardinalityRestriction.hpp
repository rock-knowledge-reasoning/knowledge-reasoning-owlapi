#ifndef OWLAPI_MODEL_CARDINALITY_RESTRICTION_HPP
#define OWLAPI_MODEL_CARDINALITY_RESTRICTION_HPP

#include <vector>
#include <stdint.h>
#include <owlapi/model/OWLQualifiedRestriction.hpp>

namespace owlapi {
namespace io {

class OWLOntologyReader;

}

namespace model {

class OWLMinCardinalityRestriction;
class OWLMaxCardinalityRestriction;
class OWLExactCardinalityRestriction;

/**
 * \class OWLCardinalityRestriction
 */
class OWLCardinalityRestriction : public OWLQualifiedRestriction
{
    friend class owlapi::io::OWLOntologyReader;

public:
    typedef shared_ptr<OWLCardinalityRestriction> Ptr;
    typedef std::pair<uint32_t, uint32_t> MinMax;

    // Allow to select the operation type for join operations
    enum OperationType { SUM_OP, MIN_OP, MAX_OP };
    enum CardinalityRestrictionType { UNKNOWN, MIN, MAX, EXACT };

    static std::map<CardinalityRestrictionType, std::string> CardinalityRestrictionTypeTxt;

    /**
     * Default constructor to allow usage of this class in a map
     */
    OWLCardinalityRestriction();

    OWLCardinalityRestriction(OWLPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification, CardinalityRestrictionType restrictionType);

    virtual ~OWLCardinalityRestriction() {}

    /**
     * Create a deep copy of this cardinality restriction
     */
    OWLCardinalityRestriction::Ptr clone() const;

    /**
     * Get the cardinality
     */
    uint32_t getCardinality() const { return mCardinality; }

    /**
     * Set cardinality -- e.g. to allow incremental construction of restrictions
     * or reuse
     */
    void setCardinality(uint32_t cardinality) { mCardinality = cardinality; }

    /**
     * Get the restriction type
     */
    virtual CardinalityRestrictionType getCardinalityRestrictionType() const { return mCardinalityRestrictionType; }

    virtual std::string toString() const;

    static std::string toString(const std::vector<OWLCardinalityRestriction::Ptr>& restrictions);

    /**
     * Convert to exact mapping
     */
    static std::map<IRI, uint32_t> convertToExactMapping(const std::vector<OWLCardinalityRestriction::Ptr>& restrictions);

    static OWLCardinalityRestriction::Ptr getInstance(OWLPropertyExpression::Ptr property,
            uint32_t cardinality,
            const OWLQualification& qualification,
            CardinalityRestrictionType restrictionType);

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
    static OWLCardinalityRestriction::Ptr intersection(OWLCardinalityRestriction::Ptr a,
            OWLCardinalityRestriction::Ptr b);

    /**
     * Create intersection of two lists of cardinality restrictions according
     * a x b using intersection operator
     * \return list of intersection results
     */
    static std::vector<OWLCardinalityRestriction::Ptr> intersection(const std::vector<OWLCardinalityRestriction::Ptr>& a,
            const std::vector<OWLCardinalityRestriction::Ptr>& b);

     /**
      * Check if restrictions have same restriction type (data/object), same property
      * and the same qualification
      * \return True, if the conditions hold, false otherwise
      */
    static bool areOverlapping(OWLCardinalityRestriction::Ptr a,
            OWLCardinalityRestriction::Ptr b);

    /**
     * Merge intersection with exact cardinality constraint
     */
    static OWLCardinalityRestriction::Ptr intersectionExact(shared_ptr<OWLExactCardinalityRestriction> a,
            OWLCardinalityRestriction::Ptr b);

    /**
     * If min and max cardinality overlap, the intersection can result
     * 1. in an exact cardinality
     * 2. in a invalid restriction, where min > max cardinality
     *
     * \return in case 1. an exact cardinality, if min max are valid, but not
     * overlapping -- return a NULL pointer
     * \throws an invalid argument exception in case 2.
     */
    static OWLCardinalityRestriction::Ptr intersectionMinMax(shared_ptr<OWLMinCardinalityRestriction> a,
            shared_ptr<OWLMaxCardinalityRestriction> b);

    /**
     * Get the join of cardinality restrictions, e.g. for overlapping
     * restrictions, so that overlapping cardinality restriction are
     * join and updated as follows
     *      MAX + MAX = 2*MAX
     *      MIN + MIN = 2*MIN
     *      EXACT + EXACT = 2*EXACT
     * \see areOverlapping
     * \param operationType SUM_OP to use the sum of cardinalities, MAX_OP to
     * use the maximum of cardinalities and MIN_OP to use the minimum of
     * cardinalities
     */
    static OWLCardinalityRestriction::Ptr join(const OWLCardinalityRestriction::Ptr& a,
        const OWLCardinalityRestriction::Ptr& b,
        OperationType operationType = SUM_OP);

    /**
     * Join overlapping restrictions of two compact (!) lists of restrictions,
     * \see areOverlapping
     * \see join
     * \param a
     * \param b
     * \param operationType One of SUM_OP, MIN_OP, or MAX_OP
     */
    static std::vector<OWLCardinalityRestriction::Ptr> join(const std::vector<OWLCardinalityRestriction::Ptr>& a,
            const std::vector<OWLCardinalityRestriction::Ptr>& b,
            OperationType operationType = SUM_OP);

    /**
     * Create a compact representation of cardinality restrictions by joining
     * the existing cardinalities with the provided
     * \parapm operationType operationType to use for joining cardinalities
     */
    static std::vector<OWLCardinalityRestriction::Ptr> compact(const std::vector<OWLCardinalityRestriction::Ptr>& a, OperationType operationType = SUM_OP);

    /**
     * Retrieve bounds for the qualifications for a set of cardinality restrictions
     * \throws invalid_argument if restrictions do not refer to the same property
     */
    static std::map<IRI, MinMax> getBounds(const std::vector<OWLCardinalityRestriction::Ptr>& e);

    /**
     * Scale the cardinality by a given factor
     */
    static std::vector<OWLCardinalityRestriction::Ptr> scale(const std::vector<OWLCardinalityRestriction::Ptr>& a, size_t factor);

protected:
    OWLCardinalityRestriction(OWLPropertyExpression::Ptr property, uint32_t minCardinality, uint32_t maxCardinality, const OWLQualification& qualification);

    void setCardinalityRestrictionType(OWLCardinalityRestriction::CardinalityRestrictionType type) { mCardinalityRestrictionType = type; }

    /**
     * Depending on the property type, narrow the internally used class to
     * either OWLObject.. or OWLData
     */
    OWLCardinalityRestriction::Ptr narrow() const;

private:
    uint32_t mCardinality;
    CardinalityRestrictionType mCardinalityRestrictionType;
};

} // end namespace model
} // end namespace owlapi

#endif // OWLAPI_MODEL_CARDINALITY_RESTRICTION_HPP
