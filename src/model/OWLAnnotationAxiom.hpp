#ifndef OWLAPI_MODEL_OWL_ANNOTATION_AXIOM_HPP
#define OWLAPI_MODEL_OWL_ANNOTATION_AXIOM_HPP

namespace owlapi {
namespace model {

class OWLAnnotationAxiom : public OWLAxiom
{
public:
    OWLAnnotationAxiom(AxiomType type)
        : OWLAxiom(type)
    {}

    virtual ~OWLLogicalAxiom() {}

    bool isAnnotationAxiom() const { return true; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANNOTATION_AXIOM_HPP
