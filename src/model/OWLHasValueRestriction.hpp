#ifndef OWLAPI_MODEL_OWL_HAS_VALUE_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_HAS_VALUE_RESTRICTION_HPP

#include "OWLRestriction.hpp"
#include "HasFiller.hpp"

namespace owlapi {
namespace model {

template<typename T>
class OWLHasValueRestriction : public OWLRestriction, public HasFiller<T>
{
public:
    typedef shared_ptr<OWLHasValueRestriction> Ptr;

    OWLHasValueRestriction(const OWLPropertyExpression::Ptr& property,
            const typename HasFiller<T>::filler_t& value)
        : OWLRestriction(property)
        , HasFiller<T>(value)
    {}

    virtual ~OWLHasValueRestriction() = default;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_HAS_VALUE_RESTRICTION_HPP

