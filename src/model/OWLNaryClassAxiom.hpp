#ifndef OWLAPI_MODEL_OWL_NARY_CLASS_AXIOM_HPP
#define OWLAPI_MODEL_OWL_NARY_CLASS_AXIOM_HPP

#include <owlapi/model/OWLNaryAxiom.hpp>
#include <owlapi/model/OWLClassAxiom.hpp>
#include <owlapi/model/OWLClassExpression.hpp>

namespace owlapi {
namespace model {

class OWLNaryClassAxiom : public OWLClassAxiom, public OWLNaryAxiom
{
    OWLClassExpressionPtrList mClassExpressions;

public:
    OWLNaryClassAxiom(OWLClassExpressionPtrList classExpressions, AxiomType type, OWLAnnotationList annotations)
        : OWLClassAxiom(type, annotations)
        , OWLNaryAxiom()
    {}

    virtual ~OWLNaryClassAxiom() {}

    bool contains(OWLClassExpression::Ptr ce) { throw std::runtime_error("OWLNaryClassAxiom::contains not implemented"); }
    OWLClassExpressionPtrList getClassExpressions() { throw std::runtime_error("OWLNaryClassAxiom::getClassExpressions not implemented"); }
    OWLClassExpressionPtrList getClassExpressionsMinus(OWLClassExpressionPtrList classExpressions) { throw std::runtime_error("OWLNaryClassAxiom::getClassExpressionsMinus not implemented"); }

    std::vector<OWLAxiom::Ptr> asPairwiseAxioms() { // Compute combinations of the ClassExpressions 
        throw std::runtime_error("OWLNaryClassAxiom::asPairwiseAxioms not implemented"); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_NARY_CLASS_AXIOM_HPP
