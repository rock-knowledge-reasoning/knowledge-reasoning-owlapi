#ifndef OWLAPI_MODEL_VALUE_RESTRICTION_HPP
#define OWLAPI_MODEL_VALUE_RESTRICTION_HPP

#include <vector>
#include <stdint.h>
#include <owlapi/model/OWLQualifiedRestriction.hpp>

namespace owlapi {
namespace io {

class OWLOntologyReader;

}

namespace model {

class OWLSomeValuesFromRestriction;
class OWLAllValuesFromRestriction;
class OWLHasValueRestriction;
class OWLHasSelfRestriction;

/**
 * \class OWLValueRestriction
 */
class OWLValueRestriction : public OWLQualifiedRestriction
{
    friend class owlapi::io::OWLOntologyReader;

public:
    typedef shared_ptr<OWLValueRestriction> Ptr;

    enum ValueRestrictionType { UNKNOWN, SOME, ALL, HAS, SELF };
    static std::map<ValueRestrictionType, std::string> ValueRestrictionTypeTxt;

    /**
     * Default constructor to allow usage of this class in a map
     */
    OWLValueRestriction();

    OWLValueRestriction(OWLPropertyExpression::Ptr property, const OWLQualification& qualification, ValueRestrictionType restrictionType);

    virtual ~OWLValueRestriction() {}

    /**
     * Create a deep copy of this value restriction
     */
    OWLValueRestriction::Ptr clone() const;

    /**
     * Get the restriction type
     */
    virtual ValueRestrictionType getValueRestrictionType() const { return mValueRestrictionType; }

    virtual std::string toString() const;

    static std::string toString(const std::vector<OWLValueRestriction::Ptr>& restrictions);

    static OWLValueRestriction::Ptr getInstance(OWLPropertyExpression::Ptr property,
            const OWLQualification& qualification,
            ValueRestrictionType restrictionType);

protected:
    OWLValueRestriction(OWLPropertyExpression::Ptr property, const OWLQualification& qualification);

    void setValueRestrictionType(OWLValueRestriction::ValueRestrictionType type) { mValueRestrictionType = type; }

    /**
     * Depending on the property type, narrow the internally used class to
     * either OWLObject.. or OWLData
     */
    OWLValueRestriction::Ptr narrow() const;

private:
    ValueRestrictionType mValueRestrictionType;
};

} // end namespace model
} // end namespace owlapi

#endif // OWLAPI_MODEL_VALUE_RESTRICTION_HPP

