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

// public abstract OWLOntologyChangeData getChangeData();
//
//    /**
//     * Gets a {@link OWLOntologyChangeRecord} that is derived from this
//     * {@code OWLOntologyChange}'s {@link OWLOntologyID} and it's
//     * {@link OWLOntologyChangeData}.
//     *
//     * @return An {@link OWLOntologyChangeRecord} containing an
//     *         {@link OWLOntologyID} equal to the {@link OWLOntologyID} of this
//     *         {@code OWLOntologyChange}'s {@link OWLOntology}. Not {@code null}
//     *         .
//     */
//    @Nonnull
//    public OWLOntologyChangeRecord getChangeRecord() {
//        return new OWLOntologyChangeRecord(ont.getOntologyID(), getChangeData());
//    }
//
//    /**
//     * Gets the signature of this ontology change. That is, the set of entities
//     * appearing in objects in this change.
//     *
//     * @return A set of entities that correspond to the signature of this
//     *         object. The set is a copy, changes are not reflected back.
//     */
//    @Override
//    @Nonnull
//    public abstract Set<OWLEntity> getSignature();

    virtual ChangeApplied accept(const OWLOntologyChangeVisitor::Ptr& visitor) = 0;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_HPP
