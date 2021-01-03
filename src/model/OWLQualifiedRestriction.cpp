#include "OWLQualifiedRestriction.hpp"
#include <sstream>

namespace owlapi {
namespace model {

OWLQualifiedRestriction::OWLQualifiedRestriction(const OWLPropertyExpression::Ptr& property,
        const OWLQualification& qualification)
    : OWLRestriction()
{
    if(property && qualification == IRI())
    {
        if(isDataRestriction())
        {
            mQualification = owlapi::vocabulary::RDFS::Literal();
        } else {
            mQualification = owlapi::vocabulary::OWL::Thing();
        }
    } else {
        mQualification = qualification;
    }

    mQualified = !( mQualification == owlapi::vocabulary::OWL::Thing() || mQualification == owlapi::vocabulary::RDFS::Literal() );
    setProperty(property);
}

std::string OWLQualifiedRestriction::toString() const
{
    std::stringstream ss;
    ss << "OWLQualifiedRestriction:" << std::endl;
    ss << "    property: " << getProperty()->toString() << std::endl;
    ss << "    qualification: " << getQualification().toString() << std::endl;
    return ss.str();
}

} // end namespace model
} // end namespace owlapi
