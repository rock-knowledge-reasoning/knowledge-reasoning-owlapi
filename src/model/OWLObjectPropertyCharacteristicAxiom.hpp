#ifndef OWLAPI_MODEL_OWL_OBJECT_PROPERTY_CHARACTERISTIC_AXIOM_HPP
#define OWLAPI_MODEL_OWL_OBJECT_PROPERTY_CHARACTERISTIC_AXIOM_HPP

#include <owlapi/model/OWLAxiom.hpp>

namespace owlapi {
namespace model {

class OWLObjectPropertyCharacteristicAxiom : public OWLAxiom
{
public:
    typedef shared_ptr<OWLObjectPropertyCharacteristicAxiom> Ptr;

    virtual ~OWLObjectPropertyCharacteristicAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_PROPERTY_CHARACTERISTIC_AXIOM_HPP
