#include "OWLOntologyReader.hpp"

#include <base-logging/Logging.hpp>
#include <utilmm/configfile/pkgconfig.hh>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

#include "../model/OWLOntologyTell.hpp"
#include "../db/rdf/Redland.hpp"
#include "../db/rdf/Sparql.hpp"
#include "../Vocabulary.hpp"
#include "changes/AddEquivalentClasses.hpp"
#include "changes/AddSubClassOfAnonymous.hpp"
#include "OWLOntologyIO.hpp"
#include "../model/OWLDataSomeValuesFrom.hpp"
#include "../model/OWLDataAllValuesFrom.hpp"
#include "../model/OWLDataRange.hpp"
#include "../model/OWLDataCardinalityRestriction.hpp"
#include "../model/OWLDataMinCardinality.hpp"
#include "../model/OWLDataMaxCardinality.hpp"
#include "../model/OWLDataExactCardinality.hpp"

#include "../model/OWLObjectCardinalityRestriction.hpp"
#include "../model/OWLObjectMinCardinality.hpp"
#include "../model/OWLObjectMaxCardinality.hpp"
#include "../model/OWLObjectExactCardinality.hpp"

#include "../model/OWLObjectSomeValuesFrom.hpp"
#include "../model/OWLObjectAllValuesFrom.hpp"
#include "../model/OWLObjectIntersectionOf.hpp"
#include "../model/OWLObjectUnionOf.hpp"
#include "../model/OWLObjectOneOf.hpp"
#include "../model/OWLObjectComplementOf.hpp"

using namespace owlapi::db::query;
using namespace owlapi::model;

namespace owlapi {
namespace io {

OWLOntologyReader::OWLOntologyReader()
    : mSparqlInterface(0)
{}

OWLOntology::Ptr OWLOntologyReader::open(const std::string& filename)
{
    OWLOntology::Ptr ontology = make_shared<OWLOntology>();
    // check first if file is empty
    std::ifstream file(filename);
    if(file.peek() == std::ifstream::traits_type::eof())
    {
        throw OWLOntologyNotFound("owlapi::io::OWLOntologyReader::open: file '"
                + filename + "' is empty. Please check the iri/file you tried to"
                " load and remove the current file.");
    }

    mSparqlInterface = new db::Redland(filename);

    mAbsolutePath = boost::filesystem::absolute(filename).string();
    ontology->setAbsolutePath(mAbsolutePath);
    return ontology;
}

OWLOntology::Ptr OWLOntologyReader::fromFile(const std::string& filename)
{
    OWLOntology::Ptr ontology = open(filename);
    load(ontology);

    return ontology;
}

OWLOntologyReader::~OWLOntologyReader()
{
    delete mSparqlInterface;
}

db::query::Results OWLOntologyReader::findAll(const db::query::Variable& subject, const db::query::Variable& predicate, const db::query::Variable& object) const
{
    db::query::Results results = mSparqlInterface->findAll(subject, predicate, object);
    return results;
}

void OWLOntologyReader::load(OWLOntology::Ptr& ontology)
{
    loadImports(ontology, true);
    loadDeclarations(ontology, true);
    loadAxioms(ontology);
}

void OWLOntologyReader::loadImports(OWLOntology::Ptr& ontology, bool directImport)
{
    Results results = findAll(Subject(), vocabulary::RDF::type(), vocabulary::OWL::Ontology());
    ResultsIterator it(results);
    while(it.next())
    {
        IRI subject = it[Subject()];
        LOG_DEBUG_S << "Ontology identified: '" << subject << "' loading from: " << mAbsolutePath;

        if(directImport)
        {
            ontology->setIRI(subject);
        }
    }

    OWLOntologyTell tell(ontology);
    OWLOntologyAsk ask(ontology);

    tell.initializeDefaultClasses();

    {
        db::query::Results results = findAll(Subject(), vocabulary::OWL::imports(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI object = it[Object()];

            if(directImport)
            {
                tell.directlyImports(object);
            } else {
                tell.imports(object);
            }
        } // end while
    }
}

void OWLOntologyReader::loadDeclarations(OWLOntology::Ptr& ontology, bool directImport)
{
    Results results = findAll(Subject(), vocabulary::RDF::type(), vocabulary::OWL::Ontology());
    ResultsIterator it(results);
    while(it.next())
    {
        IRI subject = it[Subject()];
        LOG_DEBUG_S << "Ontology identified: '" << subject << "' loading from: " << mAbsolutePath;

        if(directImport)
        {
            ontology->setIRI(subject);
        }
    }

    OWLOntologyTell tell(ontology);
    OWLOntologyAsk ask(ontology);

    tell.initializeDefaultClasses();

    {
        db::query::Results results = findAll(Subject(),vocabulary::RDF::type(),vocabulary::OWL::Class());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            tell.subClassOf(subject, vocabulary::OWL::Thing());
        }
    }

    // Identify restrictions -- will contain the id, should be anonymous
    // Since SPARQL cannot query directly for anonymous restrictions, we do
    // an incremental construction after querying all triple and filtering for
    // the one that are related to the restriction
    //
    // TODO: introduce rdf triple parser
    // http://www.w3.org/TR/owl-parsing/#subsec-streaming
    {
        db::query::Results results = findAll(Subject(), Predicate(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI predicate = it[Predicate()];
            IRI object = it[Object()];

            // Check for all type triples
            if(predicate == vocabulary::RDF::type())
            {
                // Creating classes
                if(object == vocabulary::OWL::Class())
                {
                    // already registered
                } else if( object == vocabulary::OWL::NamedIndividual())
                {
                    // search for class types, but exclude NamedIndividual 'class'
                    Results objects = findAll(subject, vocabulary::RDF::type(), Object());
                    ResultsIterator objectsIt(objects);
                    while(objectsIt.next())
                    {
                        IRI classType = objectsIt[Object()];
                        if(classType != vocabulary::OWL::NamedIndividual())
                        {
                            tell.instanceOf(subject, classType);
                        } else {
                            LOG_INFO_S << "NamedIndividual: " << subject << " with no class type given, using Thing()";
                            tell.instanceOf(subject, vocabulary::OWL::Thing());
                        }
                    }
                } else if(object == vocabulary::RDF::Property())
                {
                    // Have to guess the actual type for OWL handling
                    // object, data, annotation, ontologyProperty
                    tell.rdfProperty(subject);
                } else if(object == vocabulary::OWL::DatatypeProperty())
                {
                    tell.dataProperty(subject);
                } else if (object == vocabulary::RDFS::Datatype())
                {
                    // introduces a new datatype
                    tell.datatype(subject);
                } else if( object == vocabulary::OWL::ObjectProperty())
                {
                    tell.objectProperty(subject);

                } else if( object == vocabulary::OWL::FunctionalProperty())
                {
                    // delayed handling
                } else if( object == vocabulary::OWL::InverseFunctionalProperty())
                {
                    tell.objectProperty(subject);
                    // delayed handling
                } else if(object == vocabulary::OWL::AsymmetricProperty())
                {
                    tell.objectProperty(subject);
                    // delayed handling
                } else if(object == vocabulary::OWL::SymmetricProperty())
                {
                    tell.objectProperty(subject);
                    // delayed handling
                } else if(object == vocabulary::OWL::ReflexiveProperty())
                {
                    tell.objectProperty(subject);
                    // delayed handling
                } else if(object == vocabulary::OWL::IrreflexiveProperty())
                {
                    tell.objectProperty(subject);
                    // delayed handling
                } else if(object == vocabulary::OWL::TransitiveProperty())
                {
                    tell.objectProperty(subject);
                    // delayed handling
                } else if( object == vocabulary::OWL::AnnotationProperty())
                {
                    tell.annotationProperty(subject);
                } else if( object == vocabulary::OWL::Restriction() )
                {
                    // delayed handling
                    mRestrictions.push_back(subject);
                } else if( object == vocabulary::OWL::Ontology() )
                {
                    tell.ontology(subject);
                } else {
                    if(!ask.isOWLClass(object))
                    {
                        // Forward declaration encountered
                        tell.klass(object);
                    }
                    tell.instanceOf(subject, object);
                }
            } else if(predicate == vocabulary::OWL::versionIRI())
            {
                tell.ontology(subject);
            } else if( OWLFacet::isFacet(predicate) )
            {
                mFacetRestrictions[subject] = OWLFacetRestriction(OWLFacet::getFacet(predicate),
                        OWLLiteral::create(object.toString()));
            }

        } // end while

        for(const IRI& rdfProperty : ask.allRDFProperties())
        {
            db::query::Results results = findAll(rdfProperty,vocabulary::RDFS::range(),Object());
            ResultsIterator it(results);
            while(it.next())
            {
                IRI object = it[Object()];
                if(ask.isDatatype(object))
                {
                    tell.dataProperty(rdfProperty);
                } else if(ask.isOWLIndividual(object))
                {
                    tell.objectProperty(rdfProperty);
                } else {
                    tell.annotationProperty(rdfProperty);
                }
            }
        }
    }
}

void OWLOntologyReader::loadAxioms(OWLOntology::Ptr& ontology)
{
    LOG_DEBUG_S << "Loading axioms from path: " << mAbsolutePath << " ontology:"
        " iri: " << ontology->getIRI();

    OWLOntologyTell tell(ontology);
    OWLOntologyAsk ask(ontology);

    {
        db::query::Results results = findAll(Subject(),vocabulary::RDF::type(),vocabulary::OWL::Class());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            tell.subClassOf(subject, vocabulary::OWL::Thing());
        }
    }

    // Identify restrictions -- will contain the id, should be anonymous
    // Since SPARQL cannot query directly for anonymous restrictions, we do
    // an incremental construction after querying all triple and filtering for
    // the one that are related to the restriction
    //
    // TODO: introduce rdf triple parser
    // http://www.w3.org/TR/owl-parsing/#subsec-streaming
    {
        db::query::Results results = findAll(Subject(), Predicate(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI predicate = it[Predicate()];
            IRI object = it[Object()];

            if(predicate == vocabulary::RDFS::subClassOf())
            {
                // add a new axiom SubClassOf(subject-translation
                // object-translation)
                // where subject-translation is the translation to a class description
                // and object-translation correspondingly
                // If subject is a named class, then use partial definition
                // Class(subject partial object-translation)
                // If this axiom already exists of the form Class(subject
                // partial e1 e2 ... en) -> add axiom so that Class(subject
                // partial e1 e2 ... en object-translation)
                // subclassOf(subject, object);

                // Check if this is truely a class (or an AnonymousIndividual)
                if( ask.isOWLClass(object) )
                {
                    // This is a class
                    tell.subClassOf(subject, object);
                } else {
                    // We have to delay the mapping until the anonymous node has
                    // been fully resolved to a restriction or similar

                    OWLClass::Ptr e_subject = tell.klass(subject);
                    OWLAnonymousClassExpression::Ptr e_object = tell.anonymousClass(object);

                    mAnonymousOntologyChanges[object].push_back(make_shared<changes::AddSubClassOfAnonymous>(subject, object));
                    LOG_DEBUG_S << "Add anonymous " << object << " from s: " << subject << ", p: " << predicate << ", o: " << object;
                }
            } else if(predicate == vocabulary::RDFS::domain())
            {
                // domain of a property
                // add object to a class description and add this class to the
                // domains of the given property (if this property exists)
                //
                // will be handled in either loadObjectProperties or
                // loadDataProperties
            } else if(predicate == vocabulary::RDFS::range())
            {
                // range of a property
                // add object to a class description if this is an object
                // property and convert to data range if this is a data property
                // and add this class the range of the given property
                //
                // will be handled in either loadObjectProperties or
                // loadDataProperties
            } else if(predicate == vocabulary::RDFS::subPropertyOf())
            {
                // validate that subject and object have the same property type
                // add axiom to assert superproperty

                // treated in loadProperties
            } else if(predicate == vocabulary::OWL::equivalentProperty())
            {
                if(ask.isObjectProperty(subject) &&
                        ask.isObjectProperty(object))
                {
                    tell.equalObjectProperties({subject, object});
                }
                if(ask.isDataProperty(subject) &&
                        ask.isDataProperty(object))
                {
                    tell.equalDataProperties({subject, object});
                }
                // validate that subject and object have the same property type
                // add axiom to assert superproperty
            } else if(predicate == vocabulary::OWL::inverseOf())
            {
                // check that subject and object are object properties, if not
                // handled in loadObjectProperties
            } else if(predicate == vocabulary::OWL::oneOf())
            {
                // object is a node representing a list of named individuals
                // if l is not a list (of named individuals) raise

                // handled partially in loadAnonymousLists
            } else if(predicate == vocabulary::OWL::intersectionOf())
            {
                // add the axiom Class(x complete lt1 lt2 .. ltn)
                // where lt1 ... ltn ard the translated descriptions in the list
                // l
                // if l is not a list (of class descriptions) raise an error
                // TODO: handle in loadAnonymousLists
            } else if(predicate == vocabulary::OWL::unionOf())
            {
                // add the axiom Class(x complete unionOf(lt1 lt2 ..ltn)
                // otherwise same as intersectionOf
                // TODO: handle in loadAnonymousLists
            } else if(predicate == vocabulary::OWL::complementOf())
            {
                // add the axiom Class(x complete complementOf(nt))
                // where nt is the translation of object, if nt is not a class
                // description raise
                // TODO: handle in loadAnonymousLists
            } else if(predicate == vocabulary::OWL::equivalentClass())
            {
                bool anonymousObject = false;
                bool anonymousSubject = false;

                if(object.toString().rfind("genid",0) == 0)
                {
                    anonymousObject = true;
                    tell.anonymousClass(object);
                    LOG_INFO_S << "owlapi::io::OWLOntologyReader::loadAxioms:"
                            << " equivalentClass: object '" << object << "'"
                            " is not a known class" <<
                            " -- " << ontology->getIRI()
                            << " registering as anonymous";
                } else if(!ask.isOWLClass(object))
                {
                    tell.klass(object);
                }

                if(subject.toString().rfind("genid",0) == 0)
                {
                    anonymousObject = true;
                    tell.anonymousClass(object);
                    LOG_INFO_S << "owlapi::io::OWLOntologyReader::loadAxioms:"
                            << " equivalentClass: subject '" << subject << "'"
                            " is not a known class" <<
                            " -- " << ontology->getIRI()
                            << " registering as anonymous";

                } else if(!ask.isOWLClass(subject))
                {
                    tell.klass(subject);
                }

                if(anonymousObject)
                {
                    IRIList klasses = {subject, object};
                    mAnonymousOntologyChanges[object].push_back(make_shared<changes::AddEquivalentClasses>(klasses));
                } else {
                    tell.equalClasses({ subject, object});
                }

            } else if(predicate == vocabulary::OWL::disjointWith())
            {
                if(!ask.isOWLClass(object))
                {
                    throw std::runtime_error("owlapi::io::OWLOntologyReader::loadAxioms:"
                            " disjointWith: object '" + object.toString() + "'"
                            " is not a known class");
                }
                if(!ask.isOWLClass(subject))
                {
                    throw std::runtime_error("owlapi::io::OWLOntologyReader::loadAxioms:"
                            " disjointWith: subject '" + subject.toString() + "'"
                            " is not a known class");
                }

                tell.disjointClasses({ subject, object});
            }
        }
    }

    loadProperties(ontology);
} // end loadAxioms()

void OWLOntologyReader::loadProperties(OWLOntology::Ptr& ontology)
{
    LOG_DEBUG_S << "Loading properties from path: " << mAbsolutePath;

    OWLOntologyTell tell(ontology);
    OWLOntologyAsk ask(ontology);

    // Properties
    // http://www.w3.org/TR/owl-ref/
    //
    // An object property is defined as an instance of the built-in OWL class
    // owl:ObjectProperty.
    //
    // A datatype property is defined as an instance of the built-in OWL class
    // owl:DatatypeProperty.
    //
    // Both owl:ObjectProperty and owl:DatatypeProperty
    // are subclasses of the RDF class rdf:Property (see Appendix B).
    //
    // Delayed execution since we need to know whether we deal with an object or datatype property
    // TODO Property check
    {
        Results results = findAll(Subject(), vocabulary::RDF::type(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI object = it[Object()];
            if(object == vocabulary::OWL::FunctionalProperty())
            {
                // delayed handling to deal with subproperties
            } else if( object == vocabulary::OWL::InverseFunctionalProperty())
            {
                tell.inverseFunctionalProperty(subject);
            } else if(object == vocabulary::OWL::ReflexiveProperty())
            {
                tell.reflexiveProperty(subject);
            } else if(object == vocabulary::OWL::IrreflexiveProperty())
            {
                tell.irreflexiveProperty(subject);
            } else if(object == vocabulary::OWL::SymmetricProperty())
            {
                tell.symmetricProperty(subject);
            } else if(object == vocabulary::OWL::AsymmetricProperty())
            {
                tell.asymmetricProperty(subject);
            } else if(object == vocabulary::OWL::TransitiveProperty())
            {
                tell.transitiveProperty(subject);
            }
        }
    }

    // First handle restriction on (false) annotation properties
    loadRestrictions(ontology);

    loadAnonymousLists(ontology);
    // Ensure that all facet and restrictions are loaded for anonymous
    // datatypes and added to the ontology+reasoner as restricted
    // although anonymous datatypes
    loadDataTypeRestrictions(ontology);
    loadObjectRestrictions(ontology);

    // Get anonymous node this restriction is responsible for
    for(const auto& p : mAnonymousOntologyChanges)
    {
        for(OntologyChange::Ptr change : p.second)
        {
            switch(change->getChangeType())
            {
                case OntologyChange::ADD_SUBCLASSOF_ANONYMOUS:
                case OntologyChange::ADD_EQUIVALENT_CLASSES:
                    change->commit(tell);
                    break;
                default:
                    break;
            }
        }
    }

    loadObjectProperties(ontology);
    loadDataProperties(ontology);
    loadAnnotationProperties(ontology);

    {
        db::query::Results results = findAll(Subject(), Predicate(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI predicate = it[Predicate()];
            IRI object = it[Object()];

            if(predicate == vocabulary::RDFS::subPropertyOf())
            {
                if(!ask.isOWLIndividual(object))
                {
                    tell.instanceOf(object, vocabulary::RDF::Property());
                }

                // validate/enforce that subject and object have the same property type
                // add axiom to assert superproperty
                try {
                    tell.subPropertyOf(subject, object);
                } catch(const std::exception& e)
                {
                    tell.annotationProperty(object);
                    tell.subPropertyOf(subject, object);
                }

            } else if(predicate == vocabulary::OWL::equivalentProperty())
            {
                // validate that subject and object have the same property type
                // add axiom to assert superproperty
                // treated in loadAxioms
            } else if(predicate == vocabulary::OWL::inverseOf())
            {
                // check that subject and object are object properties, if not
                // raise, else
                tell.inverseOf(subject, object);
            }
        }
    }

    {
        db::query::Results results = findAll(Subject(), Predicate(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI predicate = it[Predicate()];
            IRI object = it[Object()];

            if(object == vocabulary::OWL::FunctionalProperty())
            {
                if( ask.isObjectProperty(subject))
                {
                    tell.functionalObjectProperty(subject);
                } else if ( ask.isDataProperty(subject) )
                {
                    tell.functionalDataProperty(subject);
                } else {
                    throw
                        std::invalid_argument("owlapi::io::OWLOntologyReader::loadProperties:"
                                " property '" + subject.toString() + "' is not a"
                                " known object or data property -- cannot define"
                                " functional property");
                }
            }
        }
    }
}

void OWLOntologyReader::loadRestrictions(OWLOntology::Ptr& ontology)
{
    OWLOntologyTell tell(ontology);
    OWLOntologyAsk ask(ontology);

    // Example: Cardinality Restrictions
    // RDF/XML Syntax
    // <owl:Class rdf:about="Person">
    //   <rdfs:subClassOf>
    //     <owl:Restriction>
    //       <owl:onProperty rdf:resource="hasParent"/>
    //       <owl:qualifiedCardinality rdf:datatype="&xsd;nonNegativeInteger">
    //         1
    //       </owl:qualifiedCardinality>
    //       <owl:onClass rdf:resource="Male">
    //     </owl:Restriction>
    //   </rdfs:subClassOf>
    // </owl:Class>

    // Iterate through all restrictions
    // _n rdf:type Restriction <- exactly one
    // _n owl:onProperty p  <- exactly one for _n, otherwise throw
    // _n owl:minCardinality 1
    // _n owl:maxCardinality 2
    db::query::Variable propertyVar("?property");
    db::rdf::sparql::Query customQuery;
    customQuery.select(db::query::Subject())
            .select(propertyVar)
           .beginWhere() \
           .triple(db::query::Subject(),vocabulary::RDF::type(), vocabulary::OWL::Restriction())
           .triple(db::query::Subject(), vocabulary::OWL::onProperty(), propertyVar)
           .endWhere();

    std::string queryTxt = customQuery.toString();
    Results results = mSparqlInterface->query(customQuery.toString(),customQuery.getBindings());
    ResultsIterator it(results);

    while(it.next())
    {
        IRI restriction = it[Subject()];
        IRI property = it[propertyVar];

        if( mRestrictions.end() == std::find(mRestrictions.begin(), mRestrictions.end(), restriction))
        {
            continue;
        }

        if(ask.isObjectProperty(property))
        {
        } else if(ask.isDataProperty(property))
        {
        } else if(ask.isAnnotationProperty(property))
        {
            if(ask.isRDFProperty(property))
            {
                tell.dataProperty(property);
            }
        } else
        {
            tell.annotationProperty(property);
        }

    }
}

void OWLOntologyReader::loadAnonymousLists(OWLOntology::Ptr& ontology)
{
    OWLOntologyAsk ask(ontology);
    OWLOntologyTell tell(ontology);

    {
        Results results = findAll(Subject(), vocabulary::RDF::type(), vocabulary::RDF::List());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            mAnonymousLists[subject] = HeadTail();
        }
    }
    // _:genid1 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/2000/01/rdf-schema#Datatype> .
    // _:genid2 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/1999/02/22-rdf-syntax-ns#List> .
    // _:genid2 <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> "DC_External" .
    // _:genid3 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/1999/02/22-rdf-syntax-ns#List> .
    // _:genid3 <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> "Li-NMC" .
    // _:genid4 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/1999/02/22-rdf-syntax-ns#List> .
    // _:genid4 <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> "Li-ion" .
    // _:genid5 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/1999/02/22-rdf-syntax-ns#List> .
    // _:genid5 <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> "LiFePO4" .
    // _:genid6 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/1999/02/22-rdf-syntax-ns#List> .
    // _:genid6 <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> "Ni-MH" .
    // _:genid7 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/1999/02/22-rdf-syntax-ns#List> .
    // _:genid7 <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> "NiMH" .
    // _:genid7 <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> <http://www.w3.org/1999/02/22-rdf-syntax-ns#nil> .
    // _:genid6 <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> _:genid7 .
    // _:genid5 <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> _:genid6 .
    // _:genid4 <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> _:genid5 .
    // _:genid3 <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> _:genid4 .
    // _:genid2 <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> _:genid3 .
    // _:genid1 <http://www.w3.org/2002/07/owl#oneOf> _:genid2 .
    {
        Results results = findAll(Subject(), vocabulary::RDF::first(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            mAnonymousLists[ subject ].first = it[Object()];
        }
    }

    {
        Results results = findAll(Subject(), vocabulary::RDF::rest(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            mAnonymousLists[ subject ].second = it[Object()];
        }
    }
}

void OWLOntologyReader::loadDataTypeRestrictions(OWLOntology::Ptr& ontology)
{
    OWLOntologyAsk ask(ontology);
    OWLOntologyTell tell(ontology);

    {
        // genid11 http://www.w3.org/2002/07/owl#withRestrictions genid10
        // genid11 http://www.w3.org/1999/02/22-rdf-syntax-ns#type http://www.w3.org/2000/01/rdf-schema#Datatype
        // genid11 http://www.w3.org/2002/07/owl#onDatatype http://www.w3.org/2001/XMLSchema#double

        // Find declarations of anonymous lists resulting from restriction definition

        db::query::Variable propertyVar("?property");
        db::rdf::sparql::Query customQuery;
        customQuery.select(db::query::Subject())
                .select(propertyVar)
                .select(db::query::Object())
               .beginWhere() \
               .triple(db::query::Subject(),vocabulary::OWL::withRestrictions(),
                       propertyVar)
               .triple(db::query::Subject(), vocabulary::OWL::onDatatype(), Object())
               .endWhere();

        //Results results = findAll(Subject(), vocabulary::OWL::withRestrictions(), Object());
        Results results = mSparqlInterface->query(customQuery.toString(),customQuery.getBindings());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            if(!ask.isDatatype(subject))
            {
                LOG_DEBUG_S << subject << " is not a datatype" << std::endl;
                continue;
            }

            const IRI& anonymousId = it[propertyVar];
            const IRI& object = it[Object()];

            // Find declarations for data restrictions
            OWLDataTypeRestriction::Ptr dataTypeRestriction = make_shared<OWLDataTypeRestriction>(make_shared<OWLDataType>(object));
            mAnonymousDataTypeRestrictions[subject] = dataTypeRestriction;

            OWLFacetRestriction::List facetRestrictions;
            owlapi::model::IRIList iris = getList(anonymousId, mAnonymousLists);
            for(const IRI& iri : iris)
            {
                std::map<IRI, OWLFacetRestriction>::const_iterator cit =  mFacetRestrictions.find(iri);
                if(cit == mFacetRestrictions.end())
                {
                    throw std::runtime_error("owlapi::io::OWLOntologyReader::loadFacetRestrictionsLists:"
                            " failed to identify anonymous id '" + iri.toString() +
                            "' as facet restriction");
                } else {
                    facetRestrictions.push_back(cit->second);
                }
            }

            if(!facetRestrictions.empty())
            {
                if(dataTypeRestriction)
                {
                    dataTypeRestriction->setFacetRestrictions(facetRestrictions);
                } else {
                    throw std::runtime_error("owlapi::io::OWLOntologyReader::loadFacetRestrictionsLists:"
                            " could not find datatype restriction for facet restriction list '" +
                            anonymousId.toString() + "'");
                }
            }
        }
    }

    {
        IRIList predicates = {
            vocabulary::OWL::oneOf(),
            vocabulary::OWL::unionOf(),
            vocabulary::OWL::datatypeComplementOf(),
            vocabulary::OWL::intersectionOf()
        };

        for(const IRI& predicate : predicates)
        {
            db::rdf::sparql::Query customQuery;
            customQuery.select(db::query::Subject())
                    .select(db::query::Object())
                   .beginWhere() \
                   .triple(db::query::Subject(),vocabulary::RDF::type(),
                           vocabulary::RDFS::Datatype())
                   .triple(db::query::Subject(), predicate, db::query::Object())
                   .endWhere();

            Results results = mSparqlInterface->query(customQuery.toString(),customQuery.getBindings());
            ResultsIterator it(results);
            while(it.next())
            {
                IRI subject = it[Subject()];
                IRI object = it[Object()];
                // TODO: 2maz
            }
        }
    }

    IRIList predicates = {
        vocabulary::OWL::allValuesFrom(),
        vocabulary::OWL::someValuesFrom()
    };

    for(const IRI& predicate : predicates)
    {
        db::query::Variable propertyVar("?property");
        db::rdf::sparql::Query customQuery;
        customQuery.select(db::query::Subject())
                .select(propertyVar)
                .select(db::query::Object())
               .beginWhere() \
               .triple(db::query::Subject(),vocabulary::OWL::onProperty(), propertyVar)
               .triple(propertyVar, vocabulary::RDF::type(), vocabulary::OWL::DatatypeProperty())
               .triple(db::query::Subject(), predicate, db::query::Object())
               .endWhere();

        //Results results = findAll(Subject(), vocabulary::OWL::withRestrictions(), Object());
        Results results = mSparqlInterface->query(customQuery.toString(),customQuery.getBindings());
        //Results results = findAll(Subject(), vocabulary::OWL::onProperty(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI object = it[Object()];
            IRI property = it[propertyVar];

            if(!ask.isDataProperty(property))
            {
                continue;
            }

            OWLDataProperty::Ptr restrictedProperty = ask.getOWLDataProperty(property);
            if(!restrictedProperty)
            {
                throw
                    std::invalid_argument("owlapi::io::OWLOntologyReader:"
                            "failed to identify property: " +
                            property.toString());
            }

            OWLDataRange::Ptr dataRange;

            std::map<IRI,
                OWLDataTypeRestriction::Ptr>::const_iterator cit = mAnonymousDataTypeRestrictions.find(object);
            if(cit != mAnonymousDataTypeRestrictions.end())
            {
                 dataRange = dynamic_pointer_cast<OWLDataRange>(cit->second);
            } else
            {
                dataRange = dynamic_pointer_cast<OWLDataRange>( tell.datatype(object) );
            }

            OWLDataRestriction::Ptr dataRestriction;
            if(predicate == vocabulary::OWL::someValuesFrom())
            {
                OWLDataSomeValuesFrom::Ptr someValuesFrom =
                    make_shared<OWLDataSomeValuesFrom>(
                            restrictedProperty,
                            dataRange);
                if(!dynamic_pointer_cast<OWLDataRestriction>(someValuesFrom))
                {
                    throw
                        std::invalid_argument("owlapi::io::OWLOntologyReader:"
                                "failed to identify OWLDataRestriction");
                }

                dataRestriction =
                    dynamic_pointer_cast<OWLDataRestriction>(someValuesFrom);
            } else if(predicate == vocabulary::OWL::allValuesFrom())
            {
                OWLDataAllValuesFrom::Ptr allValuesFrom = make_shared<OWLDataAllValuesFrom>(restrictedProperty,
                        dataRange
                );
                if(!dynamic_pointer_cast<OWLDataRestriction>(allValuesFrom))
                {
                    throw
                        std::invalid_argument("owlapi::io::OWLOntologyReader:"
                                "failed to identify OWLDataRestriction");
                }
                dataRestriction =
                    dynamic_pointer_cast<OWLDataRestriction>(allValuesFrom);

            } else if(predicate == vocabulary::OWL::sameAs())
            {
            }

            if(dataRestriction)
            {
                tell.dataPropertyRestriction(subject, dataRestriction);
            }
        }
    }

    {
        IRIList predicates = {
            vocabulary::OWL::cardinality(),
            vocabulary::OWL::minCardinality(),
            vocabulary::OWL::maxCardinality(),
            vocabulary::OWL::qualifiedCardinality(),
            vocabulary::OWL::minQualifiedCardinality(),
            vocabulary::OWL::maxQualifiedCardinality(),
        };

        for(const IRI& predicate : predicates)
        {
            db::query::Variable propertyVar("?property");
            db::query::Variable cardinalityVar("?card");
            db::query::Variable dataRangeVar("?range");
            db::rdf::sparql::Query customQuery;

            bool qualified =
                owlapi::vocabulary::OWL::isQualifiedCardinality(predicate);

            customQuery.select(db::query::Subject())
                    .select(propertyVar)
                    .select(cardinalityVar);

            if(qualified)
            {
                customQuery.select(dataRangeVar);
            }

            db::rdf::sparql::WhereClause& whereClause = customQuery.beginWhere()
                   .triple(db::query::Subject(),vocabulary::OWL::onProperty(), propertyVar)
                   // this does not work in a modular context, since
                   // property might be declared in another files
                   //.triple(propertyVar, vocabulary::RDF::type(), vocabulary::OWL::DatatypeProperty())
                   .triple(db::query::Subject(), predicate, cardinalityVar);

            if(qualified)
            {
                whereClause.triple(db::query::Subject(),
                        vocabulary::OWL::onDatatype(), dataRangeVar);
            }
            whereClause.endWhere();

            Results results = mSparqlInterface->query(customQuery.toString(),customQuery.getBindings());
            ResultsIterator it(results);
            while(it.next())
            {
                IRI subject = it[Subject()];
                IRI property = it[propertyVar];
                IRI cardinalityIRI = it[cardinalityVar];

                OWLLiteral::Ptr literal = OWLLiteral::create(cardinalityIRI.toString());
                size_t cardinality = literal->getNonNegativeInteger();

                if(!ask.isDataProperty(property))
                {
                    if(qualified)
                    {
                        throw
                            std::runtime_error("owlapi::io::OWLOntologyReader::loadDataTypeRestrictions:"
                                    + property.toString() + "' is not defined as"
                                    "DatatypeProperty, but used used with 'onDatatype'");
                    }
                    continue;
                }

                OWLDataProperty::Ptr restrictedProperty = ask.getOWLDataProperty(property);
                if(!restrictedProperty)
                {
                    throw
                        std::invalid_argument("owlapi::io::OWLOntologyReader:"
                                "failed to identify property: " +
                                property.toString());
                }

                OWLDataRange::Ptr dataRange;
                if(qualified)
                {
                    IRI range = it[dataRangeVar];
                    dataRange = make_shared<OWLDataType>(range);
                }
                OWLDataRestriction::Ptr dataRestriction;
                if(predicate == vocabulary::OWL::cardinality()
                        || predicate == vocabulary::OWL::qualifiedCardinality())
                {
                    OWLDataExactCardinality::Ptr exact =
                        make_shared<OWLDataExactCardinality>(restrictedProperty,
                                cardinality,
                                dataRange
                        );
                    dataRestriction =
                        dynamic_pointer_cast<OWLDataRestriction>(exact);
                } else if(predicate == vocabulary::OWL::minCardinality()
                        || predicate == vocabulary::OWL::minQualifiedCardinality())
                {
                    OWLDataMinCardinality::Ptr min =
                        make_shared<OWLDataMinCardinality>(restrictedProperty,
                                cardinality,
                                dataRange
                        );
                    dataRestriction =
                        dynamic_pointer_cast<OWLDataRestriction>(min);
                } else if(predicate == vocabulary::OWL::maxCardinality()
                        || predicate == vocabulary::OWL::maxQualifiedCardinality())
                {

                    OWLDataMaxCardinality::Ptr max =
                        make_shared<OWLDataMaxCardinality>(restrictedProperty,
                                cardinality,
                                dataRange
                        );
                    dataRestriction =
                        dynamic_pointer_cast<OWLDataRestriction>(max);
                }

                if(dataRestriction)
                {
                    tell.dataPropertyRestriction(subject,
                            dataRestriction
                            );
                } else
                {
                    throw
                        std::invalid_argument("owlapi::io::OWLOntologyReader::loadDataTypeRestrictions:"
                                " failed to identify restrictions for:" +
                                predicate.toString());
                }
            }
        } // for all predicates
    } // cardinalities
}

void OWLOntologyReader::loadObjectRestrictions(OWLOntology::Ptr& ontology)
{
    OWLOntologyAsk ask(ontology);
    OWLOntologyTell tell(ontology);

    // cardinalities
    {
        IRIList predicates = {
            vocabulary::OWL::cardinality(),
            vocabulary::OWL::minCardinality(),
            vocabulary::OWL::maxCardinality(),
            vocabulary::OWL::qualifiedCardinality(),
            vocabulary::OWL::minQualifiedCardinality(),
            vocabulary::OWL::maxQualifiedCardinality(),
        };

        for(const IRI& predicate : predicates)
        {
            //_:genid5 <http://www.w3.org/2002/07/owl#oneOf> _:genid4 .
            //_:genid6 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/2002/07/owl#Restriction> .
            //_:genid6 <http://www.w3.org/2002/07/owl#onProperty> <http://www.rock-robotics.org/test/turtle/restrictions#hasComponent> .
            //_:genid6 <http://www.w3.org/2002/07/owl#allValuesFrom> _:genid5 .
            //<http://www.rock-robotics.org/test/turtle/restrictions#Item> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/2002/07/owl#Class> .
            //<http://www.rock-robotics.org/test/turtle/restrictions#Item> <http://www.w3.org/2000/01/rdf-schema#subClassOf> _:genid6 .
            // Find declarations of anonymous lists resulting from restriction definition
            db::query::Variable propertyVar("?property");
            db::query::Variable cardinalityVar("?card");
            db::query::Variable classVar("?class");
            db::rdf::sparql::Query customQuery;

            bool qualified =
                owlapi::vocabulary::OWL::isQualifiedCardinality(predicate);

            customQuery.select(db::query::Subject())
                    .select(propertyVar)
                    .select(cardinalityVar);

            if(qualified)
            {
                customQuery.select(classVar);
            }

            db::rdf::sparql::WhereClause& whereClause = customQuery.beginWhere()
                   .triple(db::query::Subject(), vocabulary::RDF::type(), vocabulary::OWL::Restriction())
                   .triple(db::query::Subject(),vocabulary::OWL::onProperty(), propertyVar)
                   // this does not work in a modular context, since
                   // property might be declared in another files
                   //.triple(propertyVar, vocabulary::RDF::type(), vocabulary::OWL::ObjectProperty())
                   .triple(db::query::Subject(), predicate, cardinalityVar);

            if(qualified)
            {
                whereClause.triple(db::query::Subject(),
                        vocabulary::OWL::onClass(), classVar);
            }
            whereClause.endWhere();

            Results results = mSparqlInterface->query(customQuery.toString(),customQuery.getBindings());
            ResultsIterator it(results);
            while(it.next())
            {
                IRI subject = it[Subject()];
                IRI property = it[propertyVar];
                IRI cardinalityIRI = it[cardinalityVar];

                OWLLiteral::Ptr literal = OWLLiteral::create(cardinalityIRI.toString());
                size_t cardinality = literal->getNonNegativeInteger();

                if(!ask.isObjectProperty(property))
                {
                    if(qualified)
                    {
                        throw
                            std::runtime_error("owlapi::io::OWLOntologyReader::loadObjectRestrictions:"
                                    + property.toString() + "' is not defined as"
                                    "ObjectProperty, but used used with 'onClass'");
                    }
                    continue;
                }

                OWLObjectProperty::Ptr restrictedProperty = ask.getOWLObjectProperty(property);
                if(!restrictedProperty)
                {
                    throw
                        std::invalid_argument("owlapi::io::OWLOntologyReader:"
                                "failed to identify property: " +
                                property.toString());
                }

                OWLClassExpression::Ptr klass;
                if(qualified)
                {
                    klass = ask.getOWLClassExpression(it[classVar]);
                }
                OWLObjectRestriction::Ptr objectRestriction;

                if(predicate == vocabulary::OWL::cardinality() ||
                        predicate == vocabulary::OWL::qualifiedCardinality())
                {
                    OWLObjectExactCardinality::Ptr exact =
                        make_shared<OWLObjectExactCardinality>(restrictedProperty,
                                cardinality,
                                klass
                        );
                    objectRestriction =
                        dynamic_pointer_cast<OWLObjectRestriction>(exact);
                } else if(predicate == vocabulary::OWL::minCardinality() ||
                        predicate == vocabulary::OWL::minQualifiedCardinality())
                {
                    OWLObjectMinCardinality::Ptr min =
                        make_shared<OWLObjectMinCardinality>(restrictedProperty,
                                cardinality,
                                klass
                        );
                    objectRestriction =
                        dynamic_pointer_cast<OWLObjectRestriction>(min);
                } else if(predicate == vocabulary::OWL::maxCardinality() ||
                        predicate == vocabulary::OWL::maxQualifiedCardinality())
                {

                    OWLObjectMaxCardinality::Ptr max =
                        make_shared<OWLObjectMaxCardinality>(restrictedProperty,
                                cardinality,
                                klass
                        );
                    objectRestriction =
                        dynamic_pointer_cast<OWLObjectRestriction>(max);
                }

                if(objectRestriction)
                {
                    tell.objectPropertyRestriction(subject,
                            objectRestriction
                            );
                } else
                {
                    throw
                        std::invalid_argument("owlapi::io::OWLOntologyReader::loadObjectRestrictions:"
                                " failed to identify restrictions for:" +
                                predicate.toString());
                }
            }
        } // for all predicates
    } // cardinalities

    {
        IRIList predicates = {
            vocabulary::OWL::oneOf(),
            vocabulary::OWL::unionOf(),
            vocabulary::OWL::complementOf(),
            vocabulary::OWL::intersectionOf()
        };

        for(const IRI& predicate : predicates)
        {
            db::rdf::sparql::Query customQuery;
            // This query pattern is unique for object properties,
            customQuery.select(db::query::Subject())
                    .select(db::query::Object())
                   .beginWhere() \
                   .triple(db::query::Subject(),vocabulary::RDF::type(), vocabulary::OWL::Class())
                   .triple(db::query::Subject(), predicate, db::query::Object())
                   .endWhere();

            Results results = mSparqlInterface->query(customQuery.toString(),customQuery.getBindings());
            ResultsIterator it(results);
            while(it.next())
            {
                IRI subject = it[Subject()];
                IRI object = it[Object()];

                if(predicate == vocabulary::OWL::oneOf())
                {
                    IRIList list = getList(object, mAnonymousLists);
                    tell.objectOneOf(subject, list);
                } else if(predicate == vocabulary::OWL::intersectionOf())
                {
                    // add the axiom Class(x complete lt1 lt2 .. ltn)
                    // where lt1 ... ltn ard the translated descriptions in the list
                    // l
                    // if l is not a list (of class descriptions) raise an error
                    // TODO: register via tell
                    // need to make it via OntologyChange, same as for the
                    // others in order to ensure
                    // that dependencies can be resolved
                    IRIList list = getList(object, mAnonymousLists);
                    tell.objectIntersectionOf(subject, list);
                } else if(predicate == vocabulary::OWL::unionOf())
                {
                    IRIList list = getList(object, mAnonymousLists);
                    tell.objectUnionOf(subject, list);
                } else if(predicate == vocabulary::OWL::complementOf())
                {
                    // add the axiom Class(x complete complementOf(nt))
                    // where nt is the translation of object, if nt is not a class
                    // description raise
                    tell.objectComplementOf(subject, object);
                }
            } // end while
        } // end for
    } // end scope

    {
        IRIList predicates = {
            vocabulary::OWL::allValuesFrom(),
            vocabulary::OWL::someValuesFrom()
        };

        for(const IRI& predicate : predicates)
        {
            //_:genid5 <http://www.w3.org/2002/07/owl#oneOf> _:genid4 .
            //_:genid6 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/2002/07/owl#Restriction> .
            //_:genid6 <http://www.w3.org/2002/07/owl#onProperty> <http://www.rock-robotics.org/test/turtle/restrictions#hasComponent> .
            //_:genid6 <http://www.w3.org/2002/07/owl#allValuesFrom> _:genid5 .
            //<http://www.rock-robotics.org/test/turtle/restrictions#Item> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/2002/07/owl#Class> .
            //<http://www.rock-robotics.org/test/turtle/restrictions#Item> <http://www.w3.org/2000/01/rdf-schema#subClassOf> _:genid6 .
            // Find declarations of anonymous lists resulting from restriction definition
            db::query::Variable propertyVar("?property");
            db::rdf::sparql::Query customQuery;
            customQuery.select(db::query::Subject())
                    .select(propertyVar)
                    .select(db::query::Object())
                   .beginWhere() \
                   .triple(db::query::Subject(),vocabulary::OWL::onProperty(), propertyVar)
                   .triple(propertyVar, vocabulary::RDF::type(), vocabulary::OWL::ObjectProperty())
                   .triple(db::query::Subject(), predicate, db::query::Object())
                   .endWhere();

            std::string queryTxt = customQuery.toString();
            LOG_DEBUG_S << queryTxt;

            Results results = mSparqlInterface->query(customQuery.toString(),customQuery.getBindings());
            //Results results = findAll(Subject(), vocabulary::OWL::onProperty(), Object());
            ResultsIterator it(results);
            while(it.next())
            {
                IRI subject = it[Subject()];
                IRI property = it[propertyVar];
                IRI object = it[Object()];

                if(!ask.isObjectProperty(property))
                {
                    continue;
                }

                OWLObjectProperty::Ptr restrictedProperty = ask.getOWLObjectProperty(property);
                if(!restrictedProperty)
                {
                    throw
                        std::invalid_argument("owlapi::io::OWLOntologyReader:"
                                "failed to identify property: " +
                                property.toString());
                }

                OWLClassExpression::Ptr klass = ask.getOWLClassExpression(object);
                OWLObjectRestriction::Ptr objectRestriction;
                if(predicate == vocabulary::OWL::allValuesFrom())
                {
                    OWLObjectAllValuesFrom::Ptr allValuesFrom =
                        make_shared<OWLObjectAllValuesFrom>(restrictedProperty, klass);
                    if(!dynamic_pointer_cast<OWLObjectRestriction>(allValuesFrom))
                    {
                        throw
                            std::invalid_argument("owlapi::io::OWLOntologyReader:"
                                    "failed to identify OWLObjectRestriction from "
                                    "someValuesFrom");
                    }
                    objectRestriction = dynamic_pointer_cast<OWLObjectRestriction>(allValuesFrom);
                } else if(predicate == vocabulary::OWL::someValuesFrom())
                {
                    OWLObjectSomeValuesFrom::Ptr someValuesFrom =
                        make_shared<OWLObjectSomeValuesFrom>(restrictedProperty, klass);
                    if(!dynamic_pointer_cast<OWLObjectRestriction>(someValuesFrom))
                    {
                        throw
                            std::invalid_argument("owlapi::io::OWLOntologyReader:"
                                    "failed to identify OWLObjectRestriction from "
                                    "someValuesFrom");
                    }
                    objectRestriction = dynamic_pointer_cast<OWLObjectRestriction>(someValuesFrom);
                }

                if(objectRestriction)
                {
                    tell.objectPropertyRestriction(subject, objectRestriction);
                }
            }
        }
    }

}

owlapi::model::IRIList OWLOntologyReader::getList(const owlapi::model::IRI& anonymousId,
        const std::map<IRI, HeadTail>& anonymousLists)
{
    owlapi::model::IRIList list;
    IRI id = anonymousId;
    while(true)
    {
        std::map<IRI, HeadTail>::const_iterator cit = anonymousLists.find(id);
        if(cit == anonymousLists.end())
        {
            throw std::runtime_error("owlapi::model::OWLOntologyReader::getList"
                    " failed to identify list for id '" + anonymousId.toString()
                    + "'");
        }

        const HeadTail& headTail = cit->second;
        list.push_back(headTail.first);

        if(headTail.second != vocabulary::RDF::nil())
        {
            id = headTail.second;
        } else {
            return list;
        }
    }

    throw std::runtime_error("owlapi::model::OWLOntologyReader::getList"
            " failed to generate list for id '" + anonymousId.toString()
            + "'");
}


void OWLOntologyReader::loadDataProperties(OWLOntology::Ptr& ontology)
{
    OWLOntologyTell tell(ontology);
    OWLOntologyAsk ask(ontology);

    using namespace db::query;

    IRIList dataProperties = ask.allDataProperties();
    IRIList::const_iterator cit = dataProperties.begin();
    for(; cit != dataProperties.end(); ++cit)
    {
        IRI relation = *cit;
        Results results = findAll(Subject(), relation, Object());
        ResultsIterator it(results);

        // Setting domain of property
        // currently limited to simple classes, i.e. no expressions permitted
        {
            Results domain = findAll(relation, vocabulary::RDFS::domain(), Object());
            if(!domain.empty())
            {
                ResultsIterator domainIt(domain);
                while(domainIt.next())
                {
                    IRI classType = domainIt[Object()];
                    LOG_DEBUG_S << "tell: " << relation << " rdfs:domain " << classType;
                    if(!ask.isOWLClass(classType))
                    {
                        // Forward declaration of class type
                        tell.klass(classType);
                    }
                    tell.dataPropertyDomainOf(relation, classType);
                }
            }
        }
        // Setting range of property
        {
            Results range = findAll(relation, vocabulary::RDFS::range(), Object());
            if(!range.empty())
            {
                ResultsIterator rangeIt(range);
                while(rangeIt.next())
                {
                    IRI classType = rangeIt[Object()];
                    tell.dataPropertyRangeOf(relation, classType);
                }
            }
        }
    }

    cit = dataProperties.begin();
    for(; cit != dataProperties.end(); ++cit)
    {
        IRI relation = *cit;
        Results results = findAll(Subject(), relation, Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI object = it[Object()];

            // Setting of DataPropertyAssertions
            try {
                std::string value = object.toString();
                if(value.empty())
                {
                    LOG_WARN_S << "Encountered empty property value for subject: " << subject
                        << " and relation: " << relation;
                    continue;
                }

                OWLLiteral::Ptr literal = OWLLiteral::create(value);
                if(!literal->isTyped())
                {
                    // check if range type is known
                    OWLDataType dataType = ask.getDataType(relation, value);
                    literal = OWLLiteral::create(object.toString(), dataType);
                }
                if(!ask.isOWLIndividual(subject))
                {
                    tell.anonymousIndividual(subject);
                }
                tell.valueOf(subject, relation, literal);
            } catch(const std::invalid_argument& e)
            {
                std::string msg = "owlapi::model::OWLOntologyReader::fromFile: "
                        " '" + mAbsolutePath + "' "
                        " cannot set data property: " + relation.toString() + " on"
                        " '" + subject.toString() + "' since data range is not "
                        "specified or unsupported"
                        " for this property -- " + e.what();
                LOG_WARN_S << msg;
            }
        }
    }
}

void OWLOntologyReader::loadObjectProperties(OWLOntology::Ptr& ontology)
{
    OWLOntologyTell tell(ontology);
    OWLOntologyAsk ask(ontology);

    using namespace db::query;

    IRIList objectProperties = ask.allObjectProperties();
    IRIList::const_iterator cit = objectProperties.begin();
    for(; cit != objectProperties.end(); ++cit)
    {
        IRI relation = *cit;
        Results results = findAll(Subject(), relation, Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI object = it[Object()];

            LOG_DEBUG_S << subject << " " << relation << " " << object;
            tell.relatedTo(subject, relation, object);
        }

        // Setting domain of property
        {
            Results domain = findAll(relation, vocabulary::RDFS::domain(), Object());
            if(!domain.empty())
            {
                ResultsIterator domainIt(domain);
                while(domainIt.next())
                {
                    IRI classType = domainIt[Object()];
                    tell.objectPropertyDomainOf(relation, classType);
                }
            }
        }
        // Setting range of property
        {
            Results range = findAll(relation, vocabulary::RDFS::range(), Object());
            if(!range.empty())
            {
                ResultsIterator rangeIt(range);
                while(rangeIt.next())
                {
                    IRI classType = rangeIt[Object()];
                    tell.objectPropertyRangeOf(relation, classType);
                }
            }
        }
        // Setting inverse property
        {
            Results inverses = findAll(relation, vocabulary::OWL::inverseOf(), Object());
            if(!inverses.empty())
            {
                ResultsIterator inversesIt(inverses);
                while(inversesIt.next())
                {
                    IRI inverseType = inversesIt[Object()];
                    tell.inverseOf(relation, inverseType);
                }
            }
        }
    }
}

void OWLOntologyReader::loadAnnotationProperties(OWLOntology::Ptr& ontology)
{
    OWLOntologyTell tell(ontology);
    OWLOntologyAsk ask(ontology);

    using namespace db::query;

    IRISet removedAnnotationProperties;
    for(const IRI& relation : ask.allAnnotationProperties())
    {
        Results results = findAll(Subject(), relation, Object());
        ResultsIterator it(results);

        // Setting domain of property
        // currently limited to simple classes, i.e. no expressions permitte
        {
            //Results domain = findAll(relation, vocabulary::RDFS::domain(), Object());
            //if(!domain.empty())
            //{
            //    ResultsIterator domainIt(domain);
            //    while(domainIt.next())
            //    {
            //        IRI classType = domainIt[Object()];
            //        LOG_DEBUG_S << "tell: " << relation << " rdfs:domain " << classType;
            //        if(!ask.isOWLClass(classType))
            //        {
            //            // Forward declaration of class type
            //            tell.klass(classType);
            //        }
            //        tell.annotationPropertyDomainOf(relation, classType);
            //    }
            //}
        }
        // Setting range of property
        {
            Results range = findAll(relation, vocabulary::RDFS::range(), Object());
            if(!range.empty())
            {
                ResultsIterator rangeIt(range);
                while(rangeIt.next())
                {
                    IRI classType = rangeIt[Object()];
                    if(ask.isRDFProperty(relation))
                    {
                        if(ask.isDatatype(classType))
                        {
                            tell.dataProperty(relation);
                            tell.dataPropertyRangeOf(relation, classType);
                            removedAnnotationProperties.insert(relation);
                        } else if(ask.isOWLClass(classType))
                        {
                            tell.objectProperty(relation);
                            tell.objectPropertyRangeOf(relation, classType);
                            removedAnnotationProperties.insert(relation);
                        } else
                        {
                            tell.annotationPropertyRangeOf(relation, classType);
                        }
                    } else {
                        tell.annotationPropertyRangeOf(relation, classType);
                    }
                }
            }
        }
    }

    for(const IRI& property : removedAnnotationProperties)
    {
        tell.removeAnnotationProperty(property);
    }

    for(const IRI& relation : ask.allAnnotationProperties())
    {
        Results results = findAll(Subject(), relation, Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            try {
                tell.annotationOf(subject, relation, it[Object()]);
            } catch(const std::exception& e)
            {
                std::string msg = "owlapi::model::OWLOntologyReader::fromFile: "
                        " '" + mAbsolutePath + "' "
                        " cannot set annotation property: " + relation.toString() + " on"
                        " '" + subject.toString() + "' since data range is not "
                        "specified or unsupported"
                        " for this property -- " + e.what();
                LOG_WARN_S << msg;
            }
        }
    }
}

//std::string Ontology::toString() const
//{
//    std::stringstream txt;
//    txt << "Ontology:" << std::endl;
//    txt << "Classes (TBox statements):" << std::endl;
//    {
//        IRIList classes = allClasses();
//        IRIList::const_iterator cit = classes.begin();
//        for(; cit != classes.end(); ++cit)
//        {
//            txt << *cit << std::endl;
//        }
//        txt << std::endl << std::endl;
//    }
//    txt << "- - - - - - - - - - - - - - - -" << std::endl;
//    txt << "Instances (ABox statements):" << std::endl;
//    {
//        IRIList instances = allInstances();
//        IRIList::const_iterator cit = instances.begin();
//        for(; cit != instances.end(); ++cit)
//        {
//            txt << *cit << std::endl;
//        }
//    }
//    txt << "- - - - - - - - - - - - - - - -" << std::endl;
//    txt << "ObjectProperties:" << std::endl;
//    {
//        IRIList properties = allObjectProperties();
//        IRIList::const_iterator cit = properties.begin();
//        for(; cit != properties.end(); ++cit)
//        {
//            txt << *cit << std::endl;
//        }
//    }
//    txt << "- - - - - - - - - - - - - - - -" << std::endl;
//    txt << "DataProperties:" << std::endl;
//    {
//        IRIList properties = allDataProperties();
//        IRIList::const_iterator cit = properties.begin();
//        for(; cit != properties.end(); ++cit)
//        {
//            txt << *cit << std::endl;
//        }
//    }
//
//    txt << "- - - - - - - - - - - - - - - -" << std::endl;
//    txt << "LISP Based Representation:" << std::endl;
//    txt << KnowledgeBase::toString();
//
//    return txt.str();
//}

} // end namespace io
} // end namespace owlapi
