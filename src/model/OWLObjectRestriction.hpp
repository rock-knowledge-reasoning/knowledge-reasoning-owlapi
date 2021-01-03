#ifndef OWLAPI_MODEL_OWL_OBJECT_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_OBJECT_RESTRICTION_HPP

#include "OWLRestriction.hpp"
#include "OWLObjectPropertyExpression.hpp"

namespace owlapi {
namespace model {

class OWLObjectRestriction : virtual public OWLRestriction
{
public:
    typedef shared_ptr<OWLObjectRestriction> Ptr;

    OWLObjectRestriction() = default;

    OWLObjectRestriction(const OWLObjectPropertyExpression::Ptr& property)
        : OWLRestriction()
    {
        if(!property)
        {
            throw std::invalid_argument("owlapi::model::OWLObjectRestriction"
                    " property cannot be null");
        }

        if(!dynamic_pointer_cast<OWLPropertyExpression>(property))
        {
            throw std::invalid_argument("owlapi::model::OWLObjectRestriction:"
                    " property is not an OWLPropertyExpression");
        }
        setProperty(dynamic_pointer_cast<OWLPropertyExpression>(property));
    }

    virtual ~OWLObjectRestriction() = default;

    bool isObjectRestriction() const override { return true; }

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_RESTRICTION_HPP
