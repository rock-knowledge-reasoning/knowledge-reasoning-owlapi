#ifndef OWLAPI_MODEL_OWL_SUB_DATA_PROPERTY_OF_AXIOM_HPP
#define OWLAPI_MODEL_OWL_SUB_DATA_PROPERTY_OF_AXIOM_HPP

#include <owlapi/model/OWLSubPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLSubDataPropertyOfAxiom : public OWLSubPropertyAxiom
{
public:
        OWLSubDataPropertyOfAxiom(const OWLPropertyExpression::Ptr subProperty,
                const OWLPropertyExpression::Ptr superProperty,
                OWLAnnotationList annotations = OWLAnnotationList())
            : OWLSubPropertyAxiom(subProperty, superProperty, OWLAxiom::SubDataPropertyOf, annotations)
        {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_SUB_DATA_PROPERTY_OF_AXIOM_HPP
