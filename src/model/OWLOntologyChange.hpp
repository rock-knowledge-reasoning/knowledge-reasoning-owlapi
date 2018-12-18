#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_HPP

#include "OWLOntology.hpp"
#include "OWLOntologyChangeVisitor.hpp"

namespace owlapi {
namespace model {

class OWLOntologyChange
{
private:
    OWLOntology::Ptr mpOntology;

public:
    typedef shared_ptr<OWLOntologyChange> Ptr;
    typedef std::vector< Ptr > PtrList;

    OWLOntologyChange(const OWLOntology::Ptr& ontology);
    virtual ~OWLOntologyChange();

    virtual bool isAxiomChange() const = 0;
    virtual bool isAddAxiom() const = 0;
    virtual bool isRemoveAxiom() const { return isAxiomChange() && !isAddAxiom(); }
    virtual OWLAxiom::Ptr getAxiom() const = 0;
    //bool isImportChange() const = 0;

    OWLOntology::Ptr getOntology() const { return mpOntology; }

    virtual ChangeApplied accept(const OWLOntologyChangeVisitor::Ptr& visitor) = 0;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_HPP
