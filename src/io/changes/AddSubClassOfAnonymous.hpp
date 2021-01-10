#ifndef OWLAPI_IO_CHANGES_ADD_SUBCLASSOF_ANONYMOUS_HPP
#define OWLAPI_IO_CHANGES_ADD_SUBCLASSOF_ANONYMOUS_HPP

#include "../OntologyChange.hpp"

namespace owlapi {
namespace io {
namespace changes {

class AddSubClassOfAnonymous : public OntologyChange
{
public:
    AddSubClassOfAnonymous() = default;

    AddSubClassOfAnonymous(const owlapi::model::IRI& subclass, const owlapi::model::IRI& anonymous)
        : mSubClass(subclass)
        , mAnonymous(anonymous)
    {}

    const owlapi::model::IRI& getSubClass() const { return mSubClass; }
    const owlapi::model::IRI& getAnonymous() const { return mAnonymous; }

    void commit(owlapi::model::OWLOntologyTell tell) const override { tell.subClassOf(mSubClass, mAnonymous); };
    ChangeType getChangeType() const override { return OntologyChange::ADD_SUBCLASSOF_ANONYMOUS; }

private:
    owlapi::model::IRI mSubClass;
    owlapi::model::IRI mAnonymous;

};

} // end namepsace changes
} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_IO_CHANGES_ADD_SUBCLASSOF_ANONYMOUS_HPP
