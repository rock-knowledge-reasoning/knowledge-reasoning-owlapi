#ifndef OWLAPI_UTILS_OWL_ENTITY_REMOVER_HPP
#define OWLAPI_UTILS_OWL_ENTITY_REMOVER_HPP

#include "../model/OWLEntity.hpp"
#include "../model/OWLEntityVisitor.hpp"
#include "../model/OWLOntologyChange.hpp"

namespace owlapi {
namespace utils {

/**
 * \class OWLEntityRemove
 * \brief A utility class which allows to collect all relevant axioms to remove an entity
 *
 */
class OWLEntityRemover : public owlapi::model::OWLEntityVisitor
{
public:

    OWLEntityRemover(owlapi::model::OWLOntology::PtrList& ontologies);

private:
    owlapi::model::OWLOntologyChange::PtrList mChanges;

    void generateChanges(const owlapi::model::OWLEntity* entity);

public:
    void reset() { mChanges.clear(); }

    /**
     * Get the list of collected changed to remove a given entity
     */
    owlapi::model::OWLOntologyChange::PtrList getChanges() const { return mChanges; }

    void visit(const owlapi::model::OWLClass* klass) { generateChanges(klass); }
    void visit(const owlapi::model::OWLNamedIndividual* individual) { generateChanges(individual); }
    void visit(const owlapi::model::OWLDataProperty* property) { generateChanges(property); }
    void visit(const owlapi::model::OWLObjectProperty* property) { generateChanges(property); }
    void visit(const owlapi::model::OWLAnnotationProperty* property) { generateChanges(property); }
};

} // end namespace utils
} // end namespace owlapi
#endif // OWLAPI_UTILS_OWL_ENTITY_REMOVER_HPP
