#ifndef OWLAPI_MODEL_OWL_DATA_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_DATA_RESTRICTION_HPP

#include "OWLRestriction.hpp"

namespace owlapi {
namespace model {

class OWLDataRestriction : public OWLRestriction
{
public:
    typedef shared_ptr<OWLDataRestriction> Ptr;

    OWLDataRestriction() = default;

    OWLDataRestriction(const OWLDataPropertyExpression::Ptr& property)
        : OWLRestriction(dynamic_pointer_cast<OWLPropertyExpression>(property))
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
    }

    virtual ~OWLDataRestriction() = default;

    bool isDataRestriction() const override { return true; }

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_RESTRICTION_HPP
