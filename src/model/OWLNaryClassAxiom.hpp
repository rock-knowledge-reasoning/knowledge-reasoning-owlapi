#ifndef OWLAPI_MODEL_OWL_NARY_CLASS_AXIOM_HPP
#define OWLAPI_MODEL_OWL_NARY_CLASS_AXIOM_HPP

#include "OWLNaryAxiom.hpp"
#include "OWLClassAxiom.hpp"
#include "OWLClassExpression.hpp"

namespace owlapi {
namespace model {

class OWLNaryClassAxiom : public OWLClassAxiom, public OWLNaryAxiom
{
    OWLClassExpression::PtrList mClassExpressions;

public:
    OWLNaryClassAxiom(OWLClassExpression::PtrList classExpressions, AxiomType type, OWLAnnotationList annotations)
        : OWLClassAxiom(type, annotations)
        , OWLNaryAxiom()
    {}

    virtual ~OWLNaryClassAxiom() {}

    bool contains(OWLClassExpression::Ptr ce) { throw std::runtime_error("OWLNaryClassAxiom::contains not implemented"); }
    OWLClassExpression::PtrList getClassExpressions() { throw std::runtime_error("OWLNaryClassAxiom::getClassExpressions not implemented"); }
    OWLClassExpression::PtrList getClassExpressionsMinus(OWLClassExpression::PtrList classExpressions) { throw std::runtime_error("OWLNaryClassAxiom::getClassExpressionsMinus not implemented"); }

    std::vector<OWLAxiom::Ptr> asPairwiseAxioms() { // Compute combinations of the ClassExpressions 
        throw std::runtime_error("OWLNaryClassAxiom::asPairwiseAxioms not implemented"); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_NARY_CLASS_AXIOM_HPP
