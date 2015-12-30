#ifndef OWLAPI_MODEL_OWL_ENTITY_HPP
#define OWLAPI_MODEL_OWL_ENTITY_HPP

#include <owlapi/model/OWLNamedObject.hpp>

namespace owlapi {
namespace model {

/**
 * \class OWLEntity
 * \details Entities are the fundamental building blocks of OWL 2 ontologies, and they define the vocabulary — the named terms — of an ontology. In logic, the set of entities is usually said to constitute the signature of an ontology. Apart from entities, OWL 2 ontologies typically also contain literals, such as strings or integers.
 * \see http://www.w3.org/TR/owl2-syntax/#Entities.2C_Literals.2C_and_Anonymous_Individuals
 */
class OWLEntity : public OWLNamedObject
{
public:
    enum EntityType { UNKNOWN,
        ANNOTATION_PROPERTY,
        CLASS,
        DATATYPE,
        DATA_PROPERTY,
        NAMED_INDIVIDUAL,
        OBJECT_PROPERTY
    };

    static std::map<EntityType, std::string> TypeTxt;

    typedef shared_ptr<OWLEntity> Ptr;

    OWLEntity( const IRI& iri = IRI(), EntityType type = UNKNOWN);

    virtual ~OWLEntity() {}

    EntityType getEntityType() const { return mEntityType; }

    std::string toString() const;

    static OWLEntity::Ptr annotationProperty(const IRI& iri);
    static OWLEntity::Ptr klass(const IRI& iri);
    static OWLEntity::Ptr datatype(const IRI& iri);
    static OWLEntity::Ptr dataProperty(const IRI& iri);
    static OWLEntity::Ptr objectProperty(const IRI& iri);
    static OWLEntity::Ptr namedIndividual(const IRI& iri);

private:
    EntityType mEntityType;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ENTITY_HPP
