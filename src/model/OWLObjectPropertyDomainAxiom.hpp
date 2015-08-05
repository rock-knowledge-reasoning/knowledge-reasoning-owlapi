#ifndef OWLAPI_MODEL_OWL_OBJECT_PROPERTIY_DOMAIN_AXIOM_HPP
#define OWLAPI_MODEL_OWL_OBJECT_PROPERTIY_DOMAIN_AXIOM_HPP

#include <owlapi/model/OWLClassExpression.hpp>
#include <owlapi/model/OWLUnaryPropertyAxiom.hpp>
#include <owlapi/model/OWLPropertyDomainAxiom.hpp>

namespace owlapi {
namespace model {

class OWLObjectPropertyDomainAxiom : public OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>, public OWLPropertyDomainAxiom<OWLClassExpression::Ptr>
{
public:
    typedef boost::shared_ptr<OWLObjectPropertyDomainAxiom> Ptr;

    OWLObjectPropertyDomainAxiom(OWLObjectPropertyExpression::Ptr property, 
            OWLClassExpression::Ptr domain,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLObjectPropertyAxiom>(property, AxiomType::ObjectPropertyDomain, annotations)
        , OWLPropertyDomainAxiom<OWLClassExpression::Ptr>(domain)
    {}

    virtual ~OWLObjectPropertyDomainAxiom() {}

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_PROPERTY_DOMAIN_AXIOM_HPP
