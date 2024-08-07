#ifndef OWLAPI_MODEL_OWL_CLASS_AXIOM_HPP
#define OWLAPI_MODEL_OWL_CLASS_AXIOM_HPP

#include "OWLLogicalAxiom.hpp"

namespace owlapi {
namespace model {

class OWLClassAxiom : public OWLLogicalAxiom
{
public:
    typedef shared_ptr<OWLClassAxiom> Ptr;

    virtual ~OWLClassAxiom() {}

    OWLClassAxiom(AxiomType type, OWLAnnotationList annotations)
        : OWLLogicalAxiom(type, annotations)
    {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_CLASS_AXIOM_HPP
