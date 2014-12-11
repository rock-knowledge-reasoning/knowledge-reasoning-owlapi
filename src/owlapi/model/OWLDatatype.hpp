#ifndef OWLAPI_MODEL_OWL_DATATYPE_HPP
#define OWLAPI_MODEL_OWL_DATATYPE_HPP

#include <owl_om/owlapi/model/OWLEntity.hpp>
#include <owl_om/owlapi/model/OWLDataRange>
#include <owl_om/owlapi/model/entities/OWLLogicalEntity.hpp>

namespace owlapi {
namespace model {

class OWLDatatype : public OWLDataRange, OWLLogicalEntity, OWLNamedObject
{
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATATYPE_HPP
