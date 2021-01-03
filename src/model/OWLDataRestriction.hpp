#ifndef OWLAPI_MODEL_OWL_DATA_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_DATA_RESTRICTION_HPP

#include "OWLRestriction.hpp"
#include "OWLDataPropertyExpression.hpp"

namespace owlapi {
namespace model {

class OWLDataRestriction : public OWLRestriction
{
public:
    typedef shared_ptr<OWLDataRestriction> Ptr;

    OWLDataRestriction() = default;

    OWLDataRestriction(const OWLDataPropertyExpression::Ptr& property)
        : OWLRestriction()
    {
        if(!property)
        {
            throw std::invalid_argument("owlapi::model::OWLDataRestriction"
                    " property cannot be null");
        }

        if(!dynamic_pointer_cast<OWLPropertyExpression>(property))
        {
            throw std::invalid_argument("owlapi::model::OWLDataRestriction:"
                    " property is not an OWLPropertyExpression");
        }
        setProperty(dynamic_pointer_cast<OWLPropertyExpression>(property));
    }

    virtual ~OWLDataRestriction() = default;

    bool isDataRestriction() const override { return true; }

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_RESTRICTION_HPP
