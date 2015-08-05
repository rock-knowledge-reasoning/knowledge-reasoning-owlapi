#ifndef OWLAPI_MODEL_OWL_IRREFLEXIVE_OBJECT_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_IRREFLEXIVE_OBJECT_PROPERTY_AXIOM_HPP

#include <owlapi/model/OWLUnaryPropertyAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLIrreflexiveObjectPropertyAxiom : public OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>
{
public:
    OWLIrreflexiveObjectPropertyAxiom(const OWLObjectPropertyExpression::Ptr& property, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>(property, AxiomType::IrreflexiveObjectProperty, annotations)
    {}

    virtual ~OWLIrreflexiveObjectPropertyAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_IRREFLEXIVE_OBJECT_PROPERTY_AXIOM_HPP
