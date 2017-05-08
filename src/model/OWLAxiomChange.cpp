#include "OWLAxiomChange.hpp"

namespace owlapi {
namespace model {

OWLAxiomChange::OWLAxiomChange(const OWLOntology::Ptr& ontology, const OWLAxiom::Ptr& axiom)
    : OWLOntologyChange(ontology)
    , mpAxiom(axiom)
{}

} // end namespace model
} // end namespace owlapi
