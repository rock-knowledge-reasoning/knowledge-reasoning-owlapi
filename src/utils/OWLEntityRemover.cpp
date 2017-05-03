#include "OWLEntityRemover.hpp"

using namespace owlapi::model;

namespace owlapi {
namespace model {

void OWLEntityRemover::generateChanges(const OWLEntity::Ptr& entity)
{

    OWLOntology::PtrList::const_iterator oit = mOntologies.begin();
    for(; oit != mOntologies.end(); ++oit)
    {
        const OWLOntology::Ptr& ontology = *oit;

        bool excludeImports = true;
        std::vector<OWLAxiom::Ptr> axioms = ontology->getReferencingAxioms(entity, excludeImports);
        std::vector<OWLAxiom>::const_iterator cit = axioms.begin();
        for(; cit != axioms.end(); ++cit)
        {
            mChanges.add( OWLOntologyChange::Ptr(new RemoveAxiom(ontology, ax) );
        }
    }
}

} // end namespace model
} // end namespace owlapi
