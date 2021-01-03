#ifndef OWLAPI_MODEL_CARDINALITY_RESTRICTION_HPP
#define OWLAPI_MODEL_CARDINALITY_RESTRICTION_HPP

#include <vector>
#include <stdint.h>
#include "OWLQualifiedRestriction.hpp"
#include "OWLCardinalityRestrictionOps.hpp"
#include "OWLDataRange.hpp"
#include "OWLClassExpression.hpp"

namespace owlapi {
namespace io {

class OWLOntologyReader;

}

namespace model {

/**
 * \class OWLCardinalityRestriction
 */
class OWLCardinalityRestriction : virtual public OWLRestriction, public OWLCardinalityRestrictionOps
{
    friend class owlapi::io::OWLOntologyReader;

public:
    typedef shared_ptr< OWLCardinalityRestriction > Ptr;
    typedef std::vector<Ptr> PtrList;
    typedef std::pair<uint32_t, uint32_t> MinMax;

    enum CardinalityRestrictionType { UNKNOWN, MIN, MAX, EXACT };
    static std::map<CardinalityRestrictionType, std::string> CardinalityRestrictionTypeTxt;

    /**
     * Default constructor to allow usage of this class in a map
     */
    OWLCardinalityRestriction();

    OWLCardinalityRestriction(const OWLPropertyExpression::Ptr& property,
            uint32_t cardinality,
            CardinalityRestrictionType restrictionType);

    virtual ~OWLCardinalityRestriction() = default;

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

    std::string toString(size_t indent) const;

    /**
     * Stringify list of restriction
     * \param indent number of space for indentation
     * \return stringified list
     */
    static std::string toString(const std::vector<OWLCardinalityRestriction::Ptr>& restrictions,
            size_t indent = 0);

    /**
     * Create cardinality restrictions for object properties
     */
    static OWLCardinalityRestriction::Ptr getInstance(const OWLPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLClassExpression::Ptr& qualification,
            CardinalityRestrictionType restrictionType);

    /**
     * Create cardinality restrictions for data properties
     */
    static OWLCardinalityRestriction::Ptr getInstance(const OWLPropertyExpression::Ptr& property,
            uint32_t cardinality,
            const OWLDataRange::Ptr& range,
            CardinalityRestrictionType restrictionType);

    void setCardinalityRestrictionType(OWLCardinalityRestriction::CardinalityRestrictionType type) { mCardinalityRestrictionType = type; }

    static std::vector<OWLCardinalityRestriction::Ptr> join(
            const std::vector<OWLCardinalityRestriction::Ptr>& a,
            const std::vector<OWLCardinalityRestriction::Ptr>& _b,
            OperationType operationType
    );

    /**
     * Depending on the property type, narrow the internally used class to
     * either OWLObject.. or OWLData
     */
    OWLCardinalityRestriction::Ptr narrow() const;

    virtual bool isOverlapping(const shared_ptr<OWLCardinalityRestriction>& other)
    {
        throw NotImplemented("owlapi::model::OWLCardinalityRestrictionOps::isOverlapping");
    }

    virtual shared_ptr<OWLCardinalityRestriction> intersection(
            const shared_ptr<OWLCardinalityRestriction>& other
    )
    {
        throw NotImplemented("owlapi::model::OWLCardinalityRestrictionOps::intersection");
    }

    virtual shared_ptr<OWLCardinalityRestriction> join(
            const shared_ptr<OWLCardinalityRestriction>& other,
            OWLCardinalityRestrictionOps::OperationType operationType = OWLCardinalityRestrictionOps::SUM_OP
    )
    {
        throw NotImplemented("owlapi::model::OWLCardinalityRestrictionOps::join");
    }

protected:
    virtual OWLCardinalityRestriction::Ptr doClone() const { throw std::runtime_error("owlapi::model::OWLCardinalityRestriction::doClone has not been implemented"); };

private:
    uint32_t mCardinality;
    CardinalityRestrictionType mCardinalityRestrictionType;
};

} // end namespace model
} // end namespace owlapi

#endif // OWLAPI_MODEL_CARDINALITY_RESTRICTION_HPP
