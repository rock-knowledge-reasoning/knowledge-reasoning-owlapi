#ifndef OWLAPI_IO_CHANGES_ADD_EQUIVALENT_CLASSES_HPP
#define OWLAPI_IO_CHANGES_ADD_EQUIVALENT_CLASSES_HPP

#include "../OntologyChange.hpp"

namespace owlapi {
namespace io {
namespace changes {

class AddEquivalentClasses : public OntologyChange
{
public:
    AddEquivalentClasses() = default;

    AddEquivalentClasses(const owlapi::model::IRIList& equivalentClasses)
        : mEquivalentClasses(equivalentClasses)
    {}

    const owlapi::model::IRIList& getClasses();

    void commit(owlapi::model::OWLOntologyTell tell) const override { tell.equalClasses(mEquivalentClasses); }
    ChangeType getChangeType() const override { return OntologyChange::ADD_EQUIVALENT_CLASSES; }

private:
    owlapi::model::IRIList mEquivalentClasses;

};

} // end namepsace changes
} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_IO_CHANGES_ADD_EQUIVALENT_CLASSES_HPP
