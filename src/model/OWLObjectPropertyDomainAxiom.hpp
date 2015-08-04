#ifndef OWLAPI_MODEL_OWL_OBJECT_PROPERTIY_DOMAIN_AXIOM_HPP
#define OWLAPI_MODEL_OWL_OBJECT_PROPERTIY_DOMAIN_AXIOM_HPP

namespace owlapi {
namespace model {

class OWLObjectPropertyDomainAxiom : public OWLObjectPropertyAxiom
{
public:
    OWLObjectPropertyDomainAxiom(OWLAnnotationList annotations = OWLAnnotationList())
        : OWLObjectPropertyAxiom(AxiomType::ObjectPropertyDomain, annotations)
    {}

    virtual ~OWLObjectPropertyDomainAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_PROPERTY_DOMAIN_AXIOM_HPP
