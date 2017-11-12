#include "OWLAxiomAddVisitor.hpp"

namespace owlapi {
namespace model {

OWLAxiomAddVisitor::OWLAxiomAddVisitor(const OWLOntology::Ptr& ontology)
    : mpOntology(ontology)
{}


void OWLAxiomAddVisitor::visit(const OWLDeclarationAxiom& axiom)
{
}

} // end namespace mmodel
} // end namespace owlapi
