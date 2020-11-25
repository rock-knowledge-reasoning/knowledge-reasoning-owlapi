#ifndef OWLAPI_MODEL_OWL_ANNOTATION_PROPERTIY_DOMAIN_AXIOM_HPP
#define OWLAPI_MODEL_OWL_ANNOTATION_PROPERTIY_DOMAIN_AXIOM_HPP

#include "IRI.hpp"
#include "OWLUnaryPropertyAxiom.hpp"
#include "OWLPropertyDomainAxiom.hpp"
#include "OWLAnnotationAxiom.hpp"

namespace owlapi {
namespace model {

/**
 * Represents an <a href="http://www.w3.org/TR/owl2-syntax/#Annotation_Property_Domain" >
 * AnnotationPropertyDomain</a> axiom in the OWL 2 specification.
 *
 * An annotation property range axiom AnnotationPropertyDomain( AP U ) states that the range of the annotation property AP is the IRI U.
 */
class OWLAnnotationPropertyDomainAxiom : public OWLUnaryPropertyAxiom<OWLAnnotationAxiom>, public OWLPropertyDomainAxiom<IRI>
{
public:
    typedef shared_ptr<OWLAnnotationPropertyDomainAxiom> Ptr;

    OWLAnnotationPropertyDomainAxiom(const OWLAnnotationProperty::Ptr& property,
            const IRI& range,
            const OWLAnnotationList& annotations = OWLAnnotationList())
        : OWLUnaryPropertyAxiom<OWLAnnotationAxiom>(property, AxiomType::AnnotationPropertyDomain, annotations)
        , OWLPropertyDomainAxiom<IRI>(range)
    {}

    virtual ~OWLAnnotationPropertyDomainAxiom() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANNOTATION_PROPERTY_DOMAIN_AXIOM_HPP
