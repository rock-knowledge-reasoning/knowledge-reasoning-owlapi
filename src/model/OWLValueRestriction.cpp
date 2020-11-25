#include "OWLValueRestriction.hpp"

#include "OWLObjectSomeValuesFrom.hpp"
#include "OWLObjectAllValuesFrom.hpp"
#include "OWLObjectHasValue.hpp"
#include "OWLObjectHasSelf.hpp"

#include "OWLDataPropertyExpression.hpp"
#include "OWLDataAllValuesFrom.hpp"
#include "OWLDataHasValue.hpp"
#include "OWLDataSomeValuesFrom.hpp"
#include <sstream>

namespace owlapi {
namespace model {

std::map<OWLValueRestriction::ValueRestrictionType, std::string>
    OWLValueRestriction::ValueRestrictionTypeTxt = {
        {UNKNOWN, "UNKNOWN"},
        {SOME, "SOME"},
        {ALL, "ALL"},
        {SELF, "SELF"},
        {HAS, "HAS"}
    };

OWLValueRestriction::Ptr OWLValueRestriction::narrow() const
{
    const OWLPropertyExpression::Ptr& property = getProperty();

    if(!property)
    {
        throw std::runtime_error("owlapi::model::OWLValueRestriction::narrow: narrowing failed since property is not set");
    }

    switch(property->getObjectType())
    {
        case OWLObject::ObjectPropertyExpression:
        {
            OWLObjectPropertyExpression::Ptr oProperty = dynamic_pointer_cast<OWLObjectPropertyExpression>(property);

            switch(getValueRestrictionType())
            {
                case OWLValueRestriction::SOME:
                    return make_shared<OWLObjectSomeValuesFrom>(oProperty, getQualification());
                case OWLValueRestriction::ALL:
                    return make_shared<OWLObjectAllValuesFrom>(oProperty, getQualification());
                case OWLValueRestriction::HAS:
                    return make_shared<OWLObjectHasValue>(oProperty, getQualification());
                case OWLValueRestriction::SELF:
                    return make_shared<OWLObjectHasSelf>(oProperty, getQualification());
                default:
                    throw std::runtime_error("OWLValueRestriction::narrow: value restriction set to UNKNOWN cannot narrow");
            }
        }
        case OWLObject::DataPropertyExpression:
        {
            OWLDataPropertyExpression::Ptr dProperty = dynamic_pointer_cast<OWLDataPropertyExpression>(property);

            switch(getValueRestrictionType())
            {
                case OWLValueRestriction::SOME:
                    return make_shared<OWLDataSomeValuesFrom>(dProperty, getQualification());
                case OWLValueRestriction::ALL:
                    return make_shared<OWLDataAllValuesFrom>(dProperty, getQualification());
                case OWLValueRestriction::HAS:
                    return make_shared<OWLDataHasValue>(dProperty, getQualification());
                default:
                    throw std::runtime_error("OWLValueRestriction::narrow: value restriction set to UNKNOWN cannot narrow");
            }
        }
        default:
            throw std::runtime_error("OWLValueRestriction::narrow: has not been implemented for property expression '" + property->toString() + "'");
    }
}

OWLValueRestriction::OWLValueRestriction()
    : OWLQualifiedRestriction( OWLPropertyExpression::Ptr(), OWLQualification(""))
    , mValueRestrictionType(OWLValueRestriction::UNKNOWN)
{}

OWLValueRestriction::OWLValueRestriction(OWLPropertyExpression::Ptr property,
        const OWLQualification& qualification,
        ValueRestrictionType restrictionType)
    : OWLQualifiedRestriction(property, qualification)
    , mValueRestrictionType(restrictionType)
{}

OWLValueRestriction::Ptr OWLValueRestriction::clone() const
{
    return getInstance(getProperty(), getQualification(), mValueRestrictionType);
}

std::string OWLValueRestriction::toString() const
{
    std::stringstream ss;
    ss << "OWLValueRestriction:" << std::endl;
    ss << "    property: " << getProperty()->toString() << std::endl;
    ss << "    qualification: " << getQualification().toString() << std::endl;
    ss << "    type: " << ValueRestrictionTypeTxt[getValueRestrictionType()] << std::endl;
    return ss.str();
}

std::string OWLValueRestriction::toString(const std::vector<OWLValueRestriction::Ptr>& restrictions)
{
    std::stringstream ss;
    std::vector<OWLValueRestriction::Ptr>::const_iterator cit = restrictions.begin();
    for(; cit != restrictions.end(); ++cit)
    {
        ss << (*cit)->toString();
    }
    return ss.str();
}


OWLValueRestriction::Ptr OWLValueRestriction::getInstance(OWLPropertyExpression::Ptr property,
        const OWLQualification& qualification,
        ValueRestrictionType restrictionType)
{
    OWLValueRestriction restriction(property, qualification, restrictionType);
    return restriction.narrow();
}

}
}
