#ifndef OWLAPI_MODEL_OWL_DATA_ALL_VALUES_FROM_HPP
#define OWLAPI_MODEL_OWL_DATA_ALL_VALUES_FROM_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "../Vocabulary.hpp"
#include "OWLQuantifiedDataRestriction.hpp"

namespace owlapi {
namespace model {

class OWLDataAllValuesFrom : public OWLQuantifiedDataRestriction
{
public:
    OWLDataAllValuesFrom(const OWLDataPropertyExpression::Ptr& property,
            const OWLDataRange::Ptr& dataRange)
        : OWLQuantifiedDataRestriction(property, dataRange)
    {
        if(!property)
        {
            throw std::invalid_argument("owlapi::model::OWLDataAllValuesFrom:"
                    " property cannot be null");
        }
    }

    virtual ~OWLDataAllValuesFrom() = default;

    ClassExpressionType getClassExpressionType() const { return DATA_ALL_VALUES_FROM; }

    bool isClassExpressionLiteral() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_ALL_VALUES_FROM_HPP
