#ifndef OWLAPI_MODEL_OWL_INVERSE_OBJECT_PROPERTY_HPP
#define OWLAPI_MODEL_OWL_INVERSE_OBJECT_PROPERTY_HPP

#include "OWLObjectPropertyExpression.hpp"
#include <base-logging/Logging.hpp>

namespace owlapi {
namespace model {

/**
 * \details The inverse of a given OWLObjectProperty
 */
class OWLInverseObjectProperty : public OWLObjectPropertyExpression
{
    OWLObjectPropertyExpression::Ptr mObjectPropertyExpression;

public:
    /**
     */
    OWLInverseObjectProperty(OWLObjectPropertyExpression::Ptr inverse)
        : OWLObjectPropertyExpression()
        , mObjectPropertyExpression(inverse)
    {}

    virtual ~OWLInverseObjectProperty() {}

    /**
     * Get inverse of this property expression
     * \return inverse object property expression
     */
    OWLObjectPropertyExpression::Ptr getInverse() const { return mObjectPropertyExpression; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_INVERSE_OBJECT_PROPERTY_HPP
