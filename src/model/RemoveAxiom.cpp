#include "RemoveAxiom.hpp"
#include "OWLOntologyChangeVisitor.hpp"

namespace owlapi {
namespace model {

RemoveAxiom::RemoveAxiom(const OWLOntology::Ptr& ontology, const OWLAxiom::Ptr& axiom)
    : OWLAxiomChange(ontology, axiom)
{
}

ChangeApplied RemoveAxiom::accept(const OWLOntologyChangeVisitor::Ptr& visitor)
{
    return visitor->visit(*this);
}

} // end namespace model
} // end namespace owlapi
