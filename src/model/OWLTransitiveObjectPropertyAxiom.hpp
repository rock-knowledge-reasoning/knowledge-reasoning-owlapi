#ifndef OWLAPI_MODEL_OWL_TRANSITIVE_OBJECT_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_TRANSITIVE_OBJECT_PROPERTY_AXIOM_HPP

#include <owlapi/model/OWLUnaryPropertyAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLTransitiveObjectPropertyAxiom : public OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>
{
public:
    OWLTransitiveObjectPropertyAxiom(const OWLObjectPropertyExpression::Ptr& property, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>(property, AxiomType::TransitiveObjectProperty, annotations)
    {}

    virtual ~OWLTransitiveObjectPropertyAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_TRANSITIVE_OBJECT_PROPERTY_AXIOM_HPP
