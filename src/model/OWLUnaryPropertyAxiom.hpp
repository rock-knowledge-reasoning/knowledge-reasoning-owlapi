#ifndef OWLAPI_MODEL_OWL_UNARY_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_UNARY_PROPERTY_AXIOM_HPP

#include "OWLAxiom.hpp"
#include "OWLPropertyAxiom.hpp"

namespace owlapi {
namespace model {

// Typename: OWLObjectPropertyAxiom or OWLDataPropertyAxiom
template<typename T>
class OWLUnaryPropertyAxiom : public T
{
public:
    // e.g. for the ObjectProperties this should refer to the
    // ObjectPropertyExpression::Ptr
    typedef shared_ptr< typename T::property_t > PropertyPtr;

    OWLUnaryPropertyAxiom(const PropertyPtr& propertyExpression,
            OWLAxiom::AxiomType type,
            const OWLAnnotationList& annotations)
        : T(type, annotations)
        , mProperty(propertyExpression)
    {}

    virtual ~OWLUnaryPropertyAxiom() {}

    PropertyPtr getProperty() const { return mProperty; }

private:
    PropertyPtr mProperty;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_UNARY_PROPERTY_AXIOM_HPP
