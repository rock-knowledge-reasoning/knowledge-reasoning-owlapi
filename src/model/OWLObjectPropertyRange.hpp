#ifndef OWLAPI_MODEL_OWL_OBJECT_PROPERTIY_RANGE_AXIOM_HPP
#define OWLAPI_MODEL_OWL_OBJECT_PROPERTIY_RANGE_AXIOM_HPP

namespace owlapi {
namespace model {

class OWLObjectPropertyRangeAxiom : public OWLObjectPropertyAxiom
{
public:
    OWLObjectPropertyRangeAxiom(OWLAnnotationList annotations = OWLAnnotationList())
        : OWLObjectPropertyAxiom(AxiomType::ObjectPropertyRange, annotations)
    {}

    virtual ~OWLObjectPropertyRangeAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_PROPERTY_RANGE_AXIOM_HPP
