#ifndef OWLAPI_MODEL_OWL_OBJECT_PROPERTY_EXPRESSION_HPP
#define OWLAPI_MODEL_OWL_OBJECT_PROPERTY_EXPRESSION_HPP

#include <stdexcept>
#include <vector>
#include "OWLPropertyExpression.hpp"

namespace owlapi {
namespace model {

/**
 * \details Object properties can by used in OWL 2 to form object property expressions, which represent relationships between pairs of individuals. They are represented in the structural specification of OWL 2 by ObjectPropertyExpression
 */
class OWLObjectPropertyExpression : public virtual OWLPropertyExpression
{
public:
    typedef shared_ptr< OWLObjectPropertyExpression > Ptr;
    typedef std::vector<Ptr> PtrList;

    virtual ~OWLObjectPropertyExpression() {}

    /**
     * Get the named property used in this property expression
     */
    OWLObjectPropertyExpression::Ptr getNamedProperty() const { throw std::runtime_error("not implemented"); }

    /**
     * Return property in its simplified form
     */
    OWLObjectPropertyExpression::Ptr getSimplified() const { throw std::runtime_error("not implemented"); }

    OWLObject::Type getObjectType() const { return OWLObject::ObjectPropertyExpression; }

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_PROPERTY_EXPRESSION_HPP
