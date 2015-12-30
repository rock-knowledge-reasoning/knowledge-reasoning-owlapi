#ifndef OWLAPI_MODEL_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_PROPERTY_AXIOM_HPP

#include <owlapi/model/OWLLogicalAxiom.hpp>

namespace owlapi {
namespace model {

class OWLPropertyAxiom : public OWLLogicalAxiom
{
public:
    typedef shared_ptr<OWLPropertyAxiom> Ptr;

    OWLPropertyAxiom(AxiomType type, OWLAnnotationList annotations)
        : OWLLogicalAxiom(type, annotations)
    {}

    virtual ~OWLPropertyAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_PROPERTY_AXIOM_HPP
