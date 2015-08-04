#ifndef OWLAPI_MODEL_OWL_EQUIVALENT_OBJECT_PROPERTIES_AXIOM_HPP
#define OWLAPI_MODEL_OWL_EQUIVALENT_OBJECT_PROPERTIES_AXIOM_HPP

namespace owlapi {
namespace model {

class OWLEquivalentObjectPropertiesAxiom : public OWLObjectPropertyAxiom
{
public:
    OWLEquivalentObjectPropertiesAxiom(OWLAnnotationList annotations = OWLAnnotationList())
        : OWLObjectPropertyAxiom(AxiomType::EquivalentObjectProperties, annotations)
    {}

    virtual ~OWLEquivalentObjectPropertiesAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_EQUIVALENT_OBJECT_PROPERTIES_AXIOM_HPP
