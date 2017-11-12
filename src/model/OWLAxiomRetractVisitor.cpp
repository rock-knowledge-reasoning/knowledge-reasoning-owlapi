#include "OWLAxiomRetractVisitor.hpp"
#include "OWLOntology.hpp"

namespace owlapi {
namespace model {

OWLAxiomRetractVisitor::OWLAxiomRetractVisitor(OWLOntology* ontology)
    : mpOntology(ontology)
{}


void OWLAxiomRetractVisitor::visit(const OWLDeclarationAxiom& axiom)
{
}

void OWLAxiomRetractVisitor::visit(const OWLClassAssertionAxiom& axiom)
{
    mpOntology->removeAxiomFromMap(mpOntology->mClassAssertionAxiomsByClass[axiom.getClassExpression()],
            &axiom);
    mpOntology->removeAxiomFromMap(mpOntology->mClassAssertionAxiomsByIndividual[axiom.getIndividual()],
            &axiom);

    OWLNamedIndividual::Ptr namedIndividual = dynamic_pointer_cast<OWLNamedIndividual>(axiom.getIndividual());
    if(namedIndividual)
    {
        mpOntology->removeAxiomFromMap(mpOntology->mNamedIndividualAxioms[namedIndividual],
                &axiom);
        mpOntology->mNamedIndividuals.erase(namedIndividual->getIRI());
    } else {
        OWLAnonymousIndividual::Ptr anonymousIndividual = dynamic_pointer_cast<OWLAnonymousIndividual>(axiom.getIndividual());

        mpOntology->removeAxiomFromMap(mpOntology->mAnonymousIndividualAxioms[anonymousIndividual],
                &axiom);
        mpOntology->mAnonymousIndividuals.erase(anonymousIndividual->getReferenceID());

        mpOntology->mAnonymousIndividualAxioms.erase(anonymousIndividual);

    }

    axiom.markForRetraction();
    mpOntology->cleanup();
}

} // end namespace mmodel
} // end namespace owlapi
