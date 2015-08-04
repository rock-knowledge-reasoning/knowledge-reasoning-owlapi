#ifndef OWLAPI_MODEL_OWL_DISJOINT_OBJECT_PROPERTIES_AXIOM_HPP
#define OWLAPI_MODEL_OWL_DISJOINT_OBJECT_PROPERTIES_AXIOM_HPP

namespace owlapi {
namespace model {

class OWLDisjointObjectPropertiesAxiom : public OWLObjectPropertyAxiom
{
public:
    OWLDisjointObjectPropertiesAxiom(OWLAnnotationList annotations = OWLAnnotationList())
        : OWLObjectPropertyAxiom(AxiomType::DisjointObjectProperties, annotations)
    {}

    virtual ~OWLDisjointObjectPropertiesAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DISJOINT_OBJECT_PROPERTIES_AXIOM_HPP
