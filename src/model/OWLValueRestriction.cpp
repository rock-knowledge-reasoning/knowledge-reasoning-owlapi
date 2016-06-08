#include "OWLValueRestriction.hpp"
#include "OWLObjectSomeValuesFrom.hpp"
#include "OWLObjectAllValuesFrom.hpp"
#include "OWLObjectHasValue.hpp"
#include "OWLObjectHasSelf.hpp"
#include <sstream>
#include <boost/assign/list_of.hpp>

namespace owlapi {
namespace model {

std::map<OWLValueRestriction::ValueRestrictionType, std::string> OWLValueRestriction::ValueRestrictionTypeTxt = boost::assign::map_list_of
    (UNKNOWN, "UNKNOWN")
    (SOME, "SOME")
    (ALL, "ALL")
    (SELF, "SELF")
    (HAS, "HAS");

OWLValueRestriction::Ptr OWLValueRestriction::narrow() const
{
    if(getProperty()->isObjectPropertyExpression())
    {
        OWLObjectPropertyExpression::Ptr property = dynamic_pointer_cast<OWLObjectPropertyExpression>( getProperty() );

        switch(getValueRestrictionType())
        {
            case OWLValueRestriction::SOME:
                return OWLValueRestriction::Ptr( new OWLObjectSomeValuesFrom(property, getQualification()) ) ;
            case OWLValueRestriction::ALL:
                return OWLValueRestriction::Ptr(new OWLObjectAllValuesFrom(property, getQualification()) );
            case OWLValueRestriction::HAS:
                return OWLValueRestriction::Ptr(new OWLObjectHasValue(property, getQualification()) );
            case OWLValueRestriction::SELF:
                return OWLValueRestriction::Ptr(new OWLObjectHasSelf(property, getQualification()) );
            default:
                throw std::runtime_error("OWLValueRestriction::narrow: value restriction set to UNKNOWN cannot narrow");
        }
    }
    throw std::runtime_error("OWLValueRestriction::narrow: has not been implemented for data property expression '" + getProperty()->toString() + "'");
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
