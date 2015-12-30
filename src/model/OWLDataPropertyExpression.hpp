#ifndef OWLAPI_MODEL_OWL_DATA_PROPERTY_EXPRESSION_HPP
#define OWLAPI_MODEL_OWL_DATA_PROPERTY_EXPRESSION_HPP

#include <owlapi/model/OWLPropertyExpression.hpp>

namespace owlapi {
namespace model {

class OWLDataPropertyExpression : public OWLPropertyExpression // SWRLPredicate
{
public:
    typedef shared_ptr<OWLDataPropertyExpression> Ptr;

    virtual ~OWLDataPropertyExpression() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_PROPERTY_EXPRESSION_HPP
