#ifndef OWLAPI_MODEL_OWL_SUB_ANNOTATION_PROPERTY_OF_AXIOM_HPP
#define OWLAPI_MODEL_OWL_SUB_ANNOTATION_PROPERTY_OF_AXIOM_HPP

#include "OWLSubPropertyAxiom.hpp"

namespace owlapi {
namespace model {

class OWLSubAnnotationPropertyOfAxiom : public OWLSubPropertyAxiom
{
public:
        OWLSubAnnotationPropertyOfAxiom(const OWLPropertyExpression::Ptr& subProperty,
                const OWLPropertyExpression::Ptr& superProperty,
                const OWLAnnotationList& annotations = OWLAnnotationList())
            : OWLSubPropertyAxiom(subProperty, superProperty, OWLAxiom::SubAnnotationPropertyOf, annotations)
        {}

        virtual ~OWLSubAnnotationPropertyOfAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_SUB_ANNOTATION_PROPERTY_OF_AXIOM_HPP
