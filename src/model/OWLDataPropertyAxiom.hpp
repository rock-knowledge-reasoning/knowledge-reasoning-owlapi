#ifndef OWLAPI_MODEL_OWL_DATA_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_DATA_PROPERTY_AXIOM_HPP

#include "OWLPropertyAxiom.hpp"
#include "OWLDataPropertyExpression.hpp"

namespace owlapi {
namespace model {

/**
 *  A high level interface which marks data property axioms (e.g. Functional
 *  data property, data property domain etc.)
 */
class OWLDataPropertyAxiom : public OWLPropertyAxiom
{
public:
    typedef shared_ptr<OWLDataPropertyAxiom> Ptr;
    typedef OWLDataPropertyExpression property_t;

    OWLDataPropertyAxiom(AxiomType type, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLPropertyAxiom(type, annotations)
    {}

    virtual ~OWLDataPropertyAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_PROPERTY_AXIOM_HPP
