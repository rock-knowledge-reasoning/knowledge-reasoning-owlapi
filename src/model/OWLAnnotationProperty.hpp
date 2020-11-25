#ifndef OWLAPI_MODEL_OWL_ANNOTATION_PROPERTY_HPP
#define OWLAPI_MODEL_OWL_ANNOTATION_PROPERTY_HPP

#include "OWLProperty.hpp"
#include "../Vocabulary.hpp"

namespace owlapi {
namespace model {

class OWLAnnotationProperty : public OWLProperty
{
public:
    typedef shared_ptr<OWLAnnotationProperty> Ptr;

    OWLAnnotationProperty(const IRI& iri)
        : OWLProperty(iri, OWLEntity::ANNOTATION_PROPERTY)
    {}

    virtual ~OWLAnnotationProperty() {}

    OWLObject::Type getObjectType() const { return OWLObject::AnnotationProperty; }

    /**
     * Determines if this annotation property has an IRI corresponding to
     * rdfs:comment
     */
    bool isComment() const { return getIRI() == vocabulary::RDFS::comment(); }


    /**
     * Determines if this annotation property has an IRI corresponding to
     * owl:deprecated
     */
    bool isDeprecated() const { return getIRI() == vocabulary::OWL::deprecated(); }

    /**
     * Determines if this annotation property has an IRI corresponding to
      rdfs:label.
     */
    bool isLabel() const { return getIRI() == vocabulary::RDFS::label(); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANNOTATION_PROPERTY_HPP
