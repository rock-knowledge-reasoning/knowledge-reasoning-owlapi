#ifndef OWLAPI_MODEL_CARDINALITY_RESTRICTION_HPP
#define OWLAPI_MODEL_CARDINALITY_RESTRICTION_HPP

#include <vector>
#include <stdint.h>
#include <owlapi/model/OWLQualifiedRestriction.hpp>

namespace owlapi {
namespace model {

class OWLOntologyReader;
class OWLMinCardinalityRestriction;
class OWLMaxCardinalityRestriction;
class OWLExactCardinalityRestriction;

/**
 * \class OWLCardinalityRestriction
 */
class OWLCardinalityRestriction : public OWLQualifiedRestriction
{
    friend class OWLOntologyReader;

public:
    typedef boost::shared_ptr<OWLCardinalityRestriction> Ptr;

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
    static OWLCardinalityRestriction::Ptr intersectionExact(boost::shared_ptr<OWLExactCardinalityRestriction> a,
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
    static OWLCardinalityRestriction::Ptr intersectionMinMax(boost::shared_ptr<OWLMinCardinalityRestriction> a,
            boost::shared_ptr<OWLMaxCardinalityRestriction> b);

    /**
     * Get the join of cardinality restrictions, e.g. for overlapping
     * restrictions, so that overlapping cardinality restriction are
     * join and updated as follows
     *      MAX + MAX = 2*MAX
     *      MIN + MIN = 2*MIN
     *      EXACT + EXACT = 2*EXACT
     * \see areOverlapping
     */
    static OWLCardinalityRestriction::Ptr join(OWLCardinalityRestriction::Ptr a,
            OWLCardinalityRestriction::Ptr b);

    /**
     * Join overlapping restrictions of two compact (!) lists of restrictions,
     * \see areOverlapping
     */
    static std::vector<OWLCardinalityRestriction::Ptr> join(const std::vector<OWLCardinalityRestriction::Ptr>& a,
            const std::vector<OWLCardinalityRestriction::Ptr>& b);

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
