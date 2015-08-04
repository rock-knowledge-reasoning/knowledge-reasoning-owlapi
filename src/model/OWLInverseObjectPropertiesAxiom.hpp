#ifndef OWLAPI_MODEL_OWL_INVERSE_OBJECT_PROPERTIES_AXIOM_HPP
#define OWLAPI_MODEL_OWL_INVERSE_OBJECT_PROPERTIES_AXIOM_HPP

#include <owlapi/model/OWLObjectProperty.hpp>
#include <owlapi/model/OWLObjectPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLInverseObjectPropertiesAxiom : public OWLObjectPropertyAxiom
{
public:
    OWLInverseObjectPropertiesAxiom(
            OWLObjectPropertyExpression::Ptr first,
            OWLObjectPropertyExpression::Ptr second,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLObjectPropertyAxiom(AxiomType::InverseObjectProperties, annotations)
        , mFirst(first)
        , mSecond(second)
    {}

    virtual ~OWLInverseObjectPropertiesAxiom() {}

    OWLObjectPropertyExpression::Ptr getFirstProperty() const { return mFirst; }
    OWLObjectPropertyExpression::Ptr getSecondProperty() const { return mSecond; }

private:
    OWLObjectPropertyExpression::Ptr mFirst;
    OWLObjectPropertyExpression::Ptr mSecond;

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_INVERSE_OBJECT_PROPERTIES_AXIOM_HPP
