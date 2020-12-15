#ifndef OWLAPI_MODEL_OWL_DATA_SOME_VALUES_FROM_HPP
#define OWLAPI_MODEL_OWL_DATA_SOME_VALUES_FROM_HPP

#include "OWLClassExpression.hpp"
#include "OWLDataPropertyExpression.hpp"
#include "../Vocabulary.hpp"
#include "OWLQuantifiedDataRestriction.hpp"
namespace owlapi {
namespace model {

class OWLDataSomeValuesFrom : public OWLQuantifiedDataRestriction
{
public:
    typedef shared_ptr<OWLDataSomeValuesFrom> Ptr;

    OWLDataSomeValuesFrom(const OWLDataPropertyExpression::Ptr& property,
            const OWLDataRange::Ptr& dataRange)
        : OWLQuantifiedDataRestriction(property, dataRange)
    {
        if(!property)
        {
            throw std::invalid_argument("owlapi::model::OWLDataSomeValuesFrom:"
                    " property cannot be null");
        }
    }

    virtual ~OWLDataSomeValuesFrom() = default;

    ClassExpressionType getClassExpressionType() const override { return DATA_SOME_VALUES_FROM; }

    bool isClassExpressionLiteral() const override { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_SOME_VALUES_FROM_HPP
