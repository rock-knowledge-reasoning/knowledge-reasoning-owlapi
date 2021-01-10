#ifndef OWLAPI_IO_ONTOLOGY_CHANGE_HPP
#define OWLAPI_IO_ONTOLOGY_CHANGE_HPP

#include "../model/OWLOntologyTell.hpp"

namespace owlapi {
namespace io {

class OntologyChange
{
public:
    typedef shared_ptr<OntologyChange> Ptr;

    virtual ~OntologyChange() {};

    enum ChangeType {
        UNKNOWN,
        ADD_EQUIVALENT_CLASSES,
        ADD_SUBCLASSOF_ANONYMOUS,
        END,
    };

    virtual void commit(owlapi::model::OWLOntologyTell tell) const = 0;

    virtual ChangeType getChangeType() const { return UNKNOWN; }
};

} // io
} // owlapi
#endif // OWLAPI_IO_ONTOLOGY_CHANGE_HPP
