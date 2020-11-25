#ifndef OWLAPI_MODEL_OBJECT_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OBJECT_PROPERTY_AXIOM_HPP

#include "OWLObjectProperty.hpp"
#include "OWLPropertyAxiom.hpp"

namespace owlapi {
namespace model {

class OWLObjectPropertyAxiom : public OWLPropertyAxiom
{
public:
    typedef shared_ptr<OWLPropertyAxiom> Ptr;
    typedef OWLObjectPropertyExpression property_t;

    OWLObjectPropertyAxiom(AxiomType type, OWLAnnotationList annotations)
        : OWLPropertyAxiom(type, annotations)
    {}

    virtual ~OWLObjectPropertyAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OBJECT_PROPERTY_AXIOM_HPP
