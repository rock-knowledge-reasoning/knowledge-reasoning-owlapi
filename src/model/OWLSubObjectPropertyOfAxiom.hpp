#ifndef OWLAPI_MODEL_OWL_SUB_OBJECT_PROPERTY_OF_AXIOM_HPP
#define OWLAPI_MODEL_OWL_SUB_OBJECT_PROPERTY_OF_AXIOM_HPP

#include <owlapi/model/OWLSubPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLSubObjectPropertyOfAxiom : public OWLSubPropertyAxiom
{
public:
        OWLSubObjectPropertyOfAxiom(const OWLPropertyExpression::Ptr subProperty,
                const OWLPropertyExpression::Ptr superProperty,
                OWLAnnotationList annotations = OWLAnnotationList())
            : OWLSubPropertyAxiom(subProperty, superProperty, OWLAxiom::SubObjectPropertyOf, annotations)
        {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_SUB_OBJECT_PROPERTY_OF_AXIOM_HPP
