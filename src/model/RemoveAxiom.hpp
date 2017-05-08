#ifndef OWLAPI_MODEL_REMOVE_AXIOM_HPP
#define OWLAPI_MODEL_REMOVE_AXIOM_HPP

#include "OWLAxiomChange.hpp"

namespace owlapi {
namespace model {

class RemoveAxiom : public OWLAxiomChange
{
public:
    RemoveAxiom(const OWLOntology::Ptr& ontology, const OWLAxiom::Ptr& e);
    bool isRemoveAxiom() const { return true; }

    ChangeApplied accept(const OWLOntologyChangeVisitor::Ptr& visitor);
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_REMOVE_AXIOM_HPP
