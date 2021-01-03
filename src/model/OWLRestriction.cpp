#include "OWLRestriction.hpp"
#include <stdexcept>
#include <sstream>
#include <ostream>
#include <base-logging/Logging.hpp>

namespace owlapi {
namespace model {

OWLRestriction::OWLRestriction()
    : OWLAnonymousClassExpression()
    , mpProperty()
{}

OWLRestriction::OWLRestriction(const OWLPropertyExpression::Ptr& property)
    : OWLAnonymousClassExpression()
    , mpProperty(property)
{
    if(!property)
    {
        throw std::invalid_argument("owlapi::model::OWLRestriction"
                " cannot construct object without property");
    }
}

const OWLPropertyExpression::Ptr& OWLRestriction::getProperty() const
{
    return mpProperty;
}

std::string OWLRestriction::toString() const
{
    std::stringstream ss;
    ss << "OWLRestriction";
    if(isDataRestriction())
    {
        ss << " (Data):" << std::endl;
    } else if(isObjectRestriction())
    {
        ss << " (Object):" << std::endl;
    } else {
        throw std::runtime_error("OWLRestriction: neither data nor object restriction -- internal error");
    }
    ss << "    property: " << getProperty()->toString() << std::endl;
    return ss.str();
}

bool OWLRestriction::isDataRestriction() const
{
    if(!mpProperty)
    {
        throw std::runtime_error("owlapi::model::OWLRestriction::isDataRestriction: property has not been set");
    }
    return mpProperty->getObjectType() == OWLObject::DataPropertyExpression;
}

bool OWLRestriction::isObjectRestriction() const
{
    if(!mpProperty)
    {
        throw std::runtime_error("owlapi::model::OWLRestriction::isObjectRestriction: property has not been set");
    }

    return mpProperty->getObjectType() == OWLObject::ObjectPropertyExpression;
}

} // end namespace model
} // end namespace owlapi

std::ostream& operator<<(std::ostream& os, const owlapi::model::OWLRestrictionPtrList& list)
{
    owlapi::model::OWLRestrictionPtrList::const_iterator cit = list.begin();
    for(; cit != list.end(); ++cit)
    {
        if(*cit)
        {
            os << (*cit)->toString() << std::endl;
        }
    }
    return os;
}
