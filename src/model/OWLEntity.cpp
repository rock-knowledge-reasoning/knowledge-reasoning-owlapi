#include "OWLEntity.hpp"

#include <owlapi/model/OWLClass.hpp>
#include <owlapi/model/OWLAnnotationProperty.hpp>
#include <owlapi/model/OWLDataProperty.hpp>
#include <owlapi/model/OWLDataType.hpp>
#include <owlapi/model/OWLObjectProperty.hpp>
#include <owlapi/model/OWLNamedIndividual.hpp>

#include <boost/assign/list_of.hpp>

namespace owlapi {
namespace model {

std::map<OWLEntity::EntityType, std::string> OWLEntity::TypeTxt = boost::assign::map_list_of
    (UNKNOWN, "Unknown")
    (ANNOTATION_PROPERTY, "AnnotationProperty")
    (CLASS, "Class")
    (DATATYPE, "Datatype")
    (DATA_PROPERTY, "DataProperty")
    (NAMED_INDIVIDUAL, "NamedIndividual")
    (OBJECT_PROPERTY, "ObjectProperty")
    ;

OWLEntity::OWLEntity( const IRI& iri, EntityType type)
    : OWLNamedObject(iri)
    , mEntityType(type)
{}

OWLEntity::Ptr OWLEntity::annotationProperty(const IRI& iri)
{
    return Ptr(new OWLAnnotationProperty(iri));
}

OWLEntity::Ptr OWLEntity::klass(const IRI& iri)
{
    return Ptr(new OWLClass(iri));
}

OWLEntity::Ptr OWLEntity::datatype(const IRI& iri)
{
    return Ptr(new OWLDataType(iri));
}

OWLEntity::Ptr OWLEntity::dataProperty(const IRI& iri)
{
    return Ptr(new OWLDataProperty(iri));
}

OWLEntity::Ptr OWLEntity::objectProperty(const IRI& iri)
{
    return Ptr(new OWLObjectProperty(iri));
}

OWLEntity::Ptr OWLEntity::namedIndividual(const IRI& iri)
{
    return Ptr(new OWLNamedIndividual(iri));
}

std::string OWLEntity::toString() const
{
    return "OWLEntity: " + TypeTxt[mEntityType] + ": " + getIRI().toString();
}

} // end namespace model
} // end namespace owlapi
