#ifndef OWLAPI_MODEL_OWL_AXIOM_CHANGE_HPP
#define OWLAPI_MODEL_OWL_AXIOM_CHANGE_HPP

#include "OWLOntologyChange.hpp"

namespace owlapi {
namespace model {

class OWLAxiomChange : public OWLOntologyChange
{
public:
    OWLAxiomChange(const OWLOntology::Ptr& ontology, const OWLAxiom::Ptr& axiom);

    bool isAxiomChange() const { return true; }

    OWLAxiom::Ptr getAxiom() const { return mpAxiom; }

private:
    OWLAxiom::Ptr mpAxiom;

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_AXIOM_CHANGE_HPP
