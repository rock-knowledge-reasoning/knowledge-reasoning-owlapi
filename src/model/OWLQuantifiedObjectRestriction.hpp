#ifndef OWLAPI_MODEL_OWL_QUANTIFIED_OBJECT_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_QUANTIFIED_OBJECT_RESTRICTION_HPP

#include "OWLQuantifiedRestriction.hpp"
#include "OWLClassExpression.hpp"
#include "OWLObjectRestriction.hpp"

namespace owlapi {
namespace model {

class OWLQuantifiedObjectRestriction : public OWLQuantifiedRestriction<OWLClassExpression>, public OWLObjectRestriction
{
public:
    typedef shared_ptr<OWLQuantifiedObjectRestriction> Ptr;

    OWLQuantifiedObjectRestriction()
        : OWLQuantifiedRestriction<OWLClassExpression>()
        , OWLObjectRestriction()
    {}

    OWLQuantifiedObjectRestriction(
            const OWLObjectPropertyExpression::Ptr& property,
            const OWLClassExpression::Ptr& classExpression
    )
        : OWLQuantifiedRestriction<OWLClassExpression>(dynamic_pointer_cast<OWLPropertyExpression>(property), classExpression)
        , OWLObjectRestriction(property)
    {

        if(!property)
        {
            throw std::invalid_argument("owlapi::model::OWLQuantifiedObjectRestriction"
                    " cannot construct without property");
        }

    }

    virtual ~OWLQuantifiedObjectRestriction() = default;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_QUANTIFIED_OBJECT_RESTRICTION_HPP
