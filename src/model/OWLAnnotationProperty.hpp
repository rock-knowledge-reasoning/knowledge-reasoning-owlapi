#ifndef OWLAPI_MODEL_OWL_ANNOTATION_PROPERTY_HPP
#define OWLAPI_MODEL_OWL_ANNOTATION_PROPERTY_HPP

#include <owlapi/model/OWLEntity.hpp>
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

class OWLAnnotationProperty : public OWLEntity
{
public:
    OWLAnnotationProperty(const IRI& iri)
        : OWLEntity(iri, OWLEntity::ANNOTATION_PROPERTY)
    {}

    virtual ~OWLAnnotationProperty() {}

    /**
     * Determines if this annotation property has an IRI corresponding to
     * {@code rdfs:comment}
     */
    bool isComment() const { return getIRI() == vocabulary::RDFS::comment(); }


    /**
     * Determines if this annotation property has an IRI corresponding to
     * {@code owl:deprecated}
     */
    bool isDeprecated() const { return getIRI() == vocabulary::OWL::deprecated(); }

    /**
     * Determines if this annotation property has an IRI corresponding to
      {@code rdfs:label}.
     */
    bool isLabel() const { return getIRI() == vocabulary::RDFS::label(); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANNOTATION_PROPERTY_HPP
