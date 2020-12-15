#ifndef OWLAPI_MODEL_OWL_QUANTIFIED_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_QUANTIFIED_RESTRICTION_HPP

#include "HasFiller.hpp"
#include "OWLRestriction.hpp"
#include "OWLPropertyExpression.hpp"

namespace owlapi {
namespace model {

template<typename T>
class OWLQuantifiedRestriction : public OWLRestriction, public HasFiller<T>
{
public:
    typedef shared_ptr< OWLQuantifiedRestriction<T> > Ptr;
    OWLQuantifiedRestriction()
        : OWLRestriction()
        , HasFiller<T>()
    {}

    OWLQuantifiedRestriction(const OWLPropertyExpression::Ptr& property,
            const typename HasFiller<T>::filler_t& filler)
        : OWLRestriction(property)
        , HasFiller<T>(filler)
    {}

    virtual ~OWLQuantifiedRestriction() = default;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_QUANTIFIED_RESTRICTION_HPP
