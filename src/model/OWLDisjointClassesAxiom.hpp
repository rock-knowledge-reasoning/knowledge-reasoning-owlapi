#ifndef OWLAPI_MODEL_OWL_DISJOINT_CLASSES_AXIOM_HPP
#define OWLAPI_MODEL_OWL_DISJOINT_CLASSES_AXIOM_HPP

#include <owlapi/model/OWLNaryClassAxiom.hpp>

namespace owlapi {
namespace model {

class OWLDisjointClassesAxiom : public OWLNaryClassAxiom
{
public:
    typedef shared_ptr<OWLDisjointClassesAxiom> Ptr;

    OWLDisjointClassesAxiom(OWLClassExpression::PtrList classExpressions,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLNaryClassAxiom(classExpressions, DisjointClasses, annotations)
    {}

    virtual ~OWLDisjointClassesAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DISJOINT_CLASSES_AXIOM_HPP
