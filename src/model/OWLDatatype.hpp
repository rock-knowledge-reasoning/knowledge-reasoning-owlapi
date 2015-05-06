#ifndef OWLAPI_MODEL_OWL_DATATYPE_HPP
#define OWLAPI_MODEL_OWL_DATATYPE_HPP

#include <owlapi/model/OWLEntity.hpp>
#include <owlapi/model/OWLDataRange>
#include <owlapi/model/entities/OWLLogicalEntity.hpp>

namespace owlapi {
namespace model {

class OWLDatatype : public OWLDataRange, public OWLLogicalEntity, public OWLNamedObject
{
public:
    virtual ~OWLDatatype() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATATYPE_HPP
