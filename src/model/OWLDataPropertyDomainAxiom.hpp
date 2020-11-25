#ifndef OWLAPI_MODEL_OWL_DATA_PROPERTIY_DOMAIN_AXIOM_HPP
#define OWLAPI_MODEL_OWL_DATA_PROPERTIY_DOMAIN_AXIOM_HPP

#include "OWLClassExpression.hpp"
#include "OWLUnaryPropertyAxiom.hpp"
#include "OWLPropertyDomainAxiom.hpp"

namespace owlapi {
namespace model {

class OWLDataPropertyDomainAxiom : public OWLUnaryPropertyAxiom<OWLDataPropertyAxiom>, public OWLPropertyDomainAxiom<OWLClassExpression::Ptr>
{
public:
    typedef shared_ptr<OWLDataPropertyDomainAxiom> Ptr;

    OWLDataPropertyDomainAxiom(OWLDataPropertyExpression::Ptr property, 
            OWLClassExpression::Ptr domain,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLDataPropertyAxiom>(property, AxiomType::DataPropertyDomain, annotations)
        , OWLPropertyDomainAxiom<OWLClassExpression::Ptr>(domain)
    {}

    virtual ~OWLDataPropertyDomainAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_PROPERTY_DOMAIN_AXIOM_HPP
