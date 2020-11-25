#ifndef OWLAPI_MODEL_OWL_ANNOTATION_AXIOM_HPP
#define OWLAPI_MODEL_OWL_ANNOTATION_AXIOM_HPP

#include "OWLAxiom.hpp"
#include "OWLAnnotationObject.hpp"
#include "OWLAnnotationProperty.hpp"

namespace owlapi {
namespace model {

/**
 *  A high level interface which marks annotation property axioms (e.g.
 * annotation property domain etc.)
 */
class OWLAnnotationAxiom : public OWLAxiom, public OWLAnnotationObject
{
public:
    typedef shared_ptr<OWLAnnotationAxiom> Ptr;
    typedef OWLAnnotationProperty property_t;

    OWLAnnotationAxiom(AxiomType type,
            const OWLAnnotationList& annotations = OWLAnnotationList())
        : OWLAxiom(type, annotations)
    {}

    virtual ~OWLAnnotationAxiom() {}

    bool isAnnotationAxiom() const { return true; }

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANNOTATION_AXIOM_HPP
