#ifndef OWLAPI_MODEL_OWL_CLASS_ASSERTION_AXIOM_HPP
#define OWLAPI_MODEL_OWL_CLASS_ASSERTION_AXIOM_HPP

#include "OWLIndividual.hpp"
#include "OWLIndividualAxiom.hpp"
#include "OWLClassExpression.hpp"

namespace owlapi {
namespace model {

class OWLClassAssertionAxiom : public OWLIndividualAxiom
{
    OWLIndividual::Ptr mpIndividual;
    OWLClassExpression::Ptr mpClassExpression;
public:
    typedef shared_ptr<OWLClassAssertionAxiom> Ptr;

    virtual ~OWLClassAssertionAxiom() = default;

    OWLClassAssertionAxiom(
        const OWLIndividual::Ptr& individual,
        const OWLClassExpression::Ptr& classExpression,
        const OWLAnnotationList& annotations = OWLAnnotationList())
        : OWLIndividualAxiom(OWLAxiom::ClassAssertion, annotations)
        , mpIndividual(individual)
        , mpClassExpression(classExpression)
    {}

    OWLClassExpression::Ptr getClassExpression() const { return mpClassExpression; }
    OWLIndividual::Ptr getIndividual() const { return mpIndividual; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_CLASS_ASSERTION_AXIOM_HPP
