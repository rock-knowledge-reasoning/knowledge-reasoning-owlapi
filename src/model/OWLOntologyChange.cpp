#include "OWLOntologyChange.hpp"

namespace owlapi {
namespace model {

OWLOntologyChange::OWLOntologyChange(const OWLOntology::Ptr& ontology)
    : mpOntology(ontology)
{}

OWLOntologyChange::~OWLOntologyChange()
{}

} // end namespace model
} // end namespace owlapi
