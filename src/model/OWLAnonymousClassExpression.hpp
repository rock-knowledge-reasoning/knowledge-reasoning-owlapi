#ifndef OWLAPI_MODEL_OWL_ANONYMOUS_CLASS_EXPRESSION_HPP
#define OWLAPI_MODEL_OWL_ANONYMOUS_CLASS_EXPRESSION_HPP

#include <owlapi/model/OWLClassExpression.hpp>

namespace owlapi {
namespace model {

class OWLAnonymousClassExpression : public OWLClassExpression
{
public:
    bool isAnonymous() const { return true; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANONYMOUS_CLASS_EXPRESSION_HPP
