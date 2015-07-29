#include "OWLDataProperty.hpp"
#include <stdexcept>

namespace owlapi {
namespace model {

OWLDataProperty::OWLDataProperty(const IRI& iri)
    : OWLDataPropertyExpression()
    , OWLProperty(iri)
{}

OWLDataProperty::~OWLDataProperty()
{}

void OWLDataProperty::addRange(const OWLPropertyRange::Ptr& range)
{
   if(range->isDataRange())
   {
       mRanges.push_back(range);
   } else {
       throw std::invalid_argument("owlapi::model::OWLDataProperty::addRange: trying to add a non-datarange to data property");
   }
}

OWLDataRange::PtrList OWLDataProperty::getDataRanges() const
{
    OWLDataRange::PtrList ranges;
    OWLPropertyRange::PtrList::const_iterator cit = mRanges.begin();

    for(; cit != mRanges.end(); ++cit)
    {
        OWLDataRange::Ptr range = boost::dynamic_pointer_cast<OWLDataRange>(*cit);
        assert(range);
        ranges.push_back( range );
    }
    return ranges;
}

} // end namespace model
} // end namespace owlapi
