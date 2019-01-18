#include "OWLOntologyReader.hpp"

#include <base-logging/Logging.hpp>
#include <utilmm/configfile/pkgconfig.hh>
#include <boost/filesystem.hpp>
#include <fstream>

#include "../model/OWLOntologyTell.hpp"
#include "../db/rdf/SopranoDB.hpp"
#include "../db/rdf/Sparql.hpp"
#include "../Vocabulary.hpp"

using namespace owlapi::db::query;
using namespace owlapi::model;

namespace owlapi {
namespace io {

OWLOntologyReader::OWLOntologyReader()
    : mSparqlInterface(0)
{}

OWLOntology::Ptr OWLOntologyReader::open(const std::string& filename)
{
    OWLOntology::Ptr ontology(new OWLOntology());
    // check first if file is empty
    std::ifstream file(filename);
    if(file.peek() == std::ifstream::traits_type::eof())
    {
        throw std::invalid_argument("owlapi::io::OWLOntologyReader::open: file '"
                + filename + "' is empty. Please check the iri/file you tried to"
                " load and remove the current file.");
    }

    mSparqlInterface = new db::SopranoDB(filename);

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
    LOG_DEBUG_S << "Results: " << results.toString();
    return results;
}

void OWLOntologyReader::load(OWLOntology::Ptr& ontology)
{
    loadDeclarationsAndImports(ontology, true);
    loadAxioms(ontology);
}

void OWLOntologyReader::loadDeclarationsAndImports(OWLOntology::Ptr& ontology, bool directImport)
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
                            LOG_DEBUG_S << "Skipping NamedIndividual " << classType;
                        }
                    }
                } else if(object == vocabulary::RDF::Property())
                {
                    tell.subClassOf(subject, vocabulary::RDF::Property());
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
                    LOG_DEBUG_S << "Annotation property '" << subject << "' ignored for reasoning";
                } else if( object == vocabulary::OWL::Restriction() )
                {
                    // delayed handling
                    mRestrictions.push_back(subject);
                } else if( object == vocabulary::OWL::Ontology() )
                {
                    tell.ontology(subject);
                }
            } else if(predicate == vocabulary::OWL::imports())
            {
                if(directImport)
                {
                    tell.directlyImports(object);
                } else {
                    tell.imports(object);
                }
            }
        } // end while
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
                    // cache the restrictions
                    mAnonymousRestrictions[object].push_back(e_subject);

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
                // raise, else
            } else if(predicate == vocabulary::OWL::oneOf())
            {
                // object is a node representing a list of named individuals
                // if l is not a list (of named individuals) raise
            } else if(predicate == vocabulary::OWL::intersectionOf())
            {
                // add the axiom Class(x complete lt1 lt2 .. ltn)
                // where lt1 ... ltn ard the translated descriptions in the list
                // l
                // if l is not a list (of class descriptions) raise an error

            } else if(predicate == vocabulary::OWL::unionOf())
            {
                // add the axiom Class(x complete unionOf(lt1 lt2 ..ltn)
                // otherwise same as intersectionOf
            } else if(predicate == vocabulary::OWL::complementOf())
            {
                // add the axiom Class(x complete complementOf(nt))
                // where nt is the translation of object, if nt is not a class
                // description raise
            } else if(predicate == vocabulary::OWL::equivalentClass())
            {
                if(!ask.isOWLClass(object))
                {
                    throw std::runtime_error("owlapi::io::OWLOntologyReader::loadAxioms:"
                            " equivalentClass: object '" + object.toString() + "'"
                            " is not a known class");
                }
                if(!ask.isOWLClass(subject))
                {
                    throw std::runtime_error("owlapi::io::OWLOntologyReader::loadAxioms:"
                            " equivalentClass: subject '" + subject.toString() + "'"
                            " is not a known class");
                }

                tell.equalClasses({ subject, object});

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

    loadRestrictions(ontology);

    loadAnonymousLists(ontology);

    loadObjectProperties(ontology);
    loadDataProperties(ontology);


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
                // validate that subject and object have the same property type
                // add axiom to assert superproperty
                tell.subPropertyOf(subject, object);
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
    Results results = findAll(Subject(), Predicate(), Object());
    ResultsIterator it(results);

    // Maps to hold generic stuff to be added to the child classes
    std::map<owlapi::model::IRI, owlapi::model::OWLRestriction> restrictionMap;
    std::map<owlapi::model::IRI, owlapi::model::OWLQualifiedRestriction> qualifiedRestrictionMap;
    while(it.next())
    {
        IRI restriction = it[Subject()];
        if( mRestrictions.end() == std::find(mRestrictions.begin(), mRestrictions.end(), restriction))
        {
            continue;
        }

        IRI predicate = it[Predicate()];
        if(predicate == vocabulary::OWL::onProperty())
        {
            OWLObjectProperty::Ptr oProperty = ask.getOWLObjectProperty( it[Object()] );

            OWLRestriction* r = &restrictionMap[restriction];
            if (r->getProperty())
            {
                std::stringstream ss;
                ss << "Restriction '" << restriction << "' applies to more than one property, but requires to be exactly one";
                throw std::invalid_argument("owlapi::Ontology: " + ss.str() );
            }
            r->setProperty(dynamic_pointer_cast<OWLPropertyExpression>(oProperty));
            continue;
        }
        else if(predicate == vocabulary::OWL::minCardinality() || predicate == vocabulary::OWL::minQualifiedCardinality())
        {
            OWLCardinalityRestriction* cardinalityRestrictionPtr = &mCardinalityRestrictions[restriction];

            uint32_t cardinality = OWLLiteral::create( it[Object()].toString() )->getInteger();
            cardinalityRestrictionPtr->setCardinality(cardinality);
            cardinalityRestrictionPtr->setCardinalityRestrictionType(OWLCardinalityRestriction::MIN);
            continue;
        } else if(predicate == vocabulary::OWL::maxCardinality() || predicate == vocabulary::OWL::maxQualifiedCardinality())
        {
            OWLCardinalityRestriction* cardinalityRestrictionPtr = &mCardinalityRestrictions[restriction];

            uint32_t cardinality = OWLLiteral::create( it[Object()].toString() )->getInteger();
            cardinalityRestrictionPtr->setCardinality(cardinality);
            cardinalityRestrictionPtr->setCardinalityRestrictionType(OWLCardinalityRestriction::MAX);
            continue;
        } else if(predicate == vocabulary::OWL::cardinality() || predicate == vocabulary::OWL::qualifiedCardinality())
        {
            OWLCardinalityRestriction* cardinalityRestrictionPtr = &mCardinalityRestrictions[restriction];

            uint32_t cardinality = OWLLiteral::create( it[Object()].toString() )->getInteger();
            cardinalityRestrictionPtr->setCardinality(cardinality);
            cardinalityRestrictionPtr->setCardinalityRestrictionType(OWLCardinalityRestriction::EXACT);
            continue;
        } else if(predicate == vocabulary::OWL::someValuesFrom())
        {
            OWLValueRestriction* valueRestrictionPtr = &mValueRestrictions[restriction];

            valueRestrictionPtr->setQualification(it[Object()]);
            valueRestrictionPtr->setValueRestrictionType(OWLValueRestriction::SOME);
            continue;
        } else if(predicate == vocabulary::OWL::allValuesFrom())
        {
            OWLValueRestriction* valueRestrictionPtr = &mValueRestrictions[restriction];

            valueRestrictionPtr->setQualification(it[Object()]);
            valueRestrictionPtr->setValueRestrictionType(OWLValueRestriction::ALL);
            continue;
        } else if(predicate == vocabulary::OWL::hasSelf())
        {
            // FIXME: hasSelf is not a qualified restriction but a value restriction?!?
            OWLValueRestriction* valueRestrictionPtr = &mValueRestrictions[restriction];

            valueRestrictionPtr->setValueRestrictionType(OWLValueRestriction::ALL);
            continue;
        } else if(predicate == vocabulary::OWL::hasValue())
        {
            OWLValueRestriction* valueRestrictionPtr = &mValueRestrictions[restriction];

            // FIXME: the object is a rdfs::resource!!! -> setQualification?
            valueRestrictionPtr->setQualification(it[Object()]);
            valueRestrictionPtr->setValueRestrictionType(OWLValueRestriction::HAS);
            continue;
        } else if(predicate == vocabulary::OWL::onClass())
        {
            // NOTE: This is only needed and valid if we have a cardinality restriction
            IRI qualification = it[Object()];
            OWLQualifiedRestriction* qr = &qualifiedRestrictionMap[restriction];

            qr->setQualification(qualification);
            if (!qr->isQualified())
            {
                std::stringstream ss;
                ss << "Restriction '" << restriction << "' could not be qualified";
                throw std::invalid_argument("owlapi::Ontology: " + ss.str() );
            }
            continue;
        }
    }  // while(it.next())


    // Second pass: Join generic, qualified and value or cardinality restrictions!!!

    // For each value restriction:
    //  find generic and update otherwise throw
    //  NOTE: qualification has already been set!
    // Then tell the ontology that we have an anonymous superclass
    {
        LOG_DEBUG_S << "Value Restrictions";
        std::map<IRI, OWLValueRestriction>::iterator cit = mValueRestrictions.begin();
        for(; cit != mValueRestrictions.end(); ++cit)
        {
            OWLValueRestriction& valueRestriction = cit->second;

            // Found value restriction
            try {

                // Set stuff of associated generic and qualified restriction classes
                std::map<IRI, OWLRestriction>::const_iterator rcit = restrictionMap.find(cit->first);
                if (rcit != restrictionMap.end())
                {
                    valueRestriction.setProperty(rcit->second.getProperty());
                } else {
                    throw std::invalid_argument("owl::onProperty missing for value restriction");
                }

                // narrow only after property is assigned
                OWLValueRestriction::Ptr valueRestrictionPtr = cit->second.narrow();

                // Get anonymous node this restriction is responsible for
                std::vector<OWLClass::Ptr> subclasses = mAnonymousRestrictions[cit->first];
                std::vector<OWLClass::Ptr>::const_iterator sit = subclasses.begin();
                for(; sit != subclasses.end(); ++sit)
                {
                    tell.subClassOf(*sit, valueRestrictionPtr);
                }
            } catch(const std::runtime_error& e)
            {
                LOG_ERROR_S << "Error handling value restriction: '" << cit->first << "' -- " << e.what();
            }
        }
    }

    // For each cardinality restriction:
    //  find generic and update otherwise throw
    //  find qualified and update (if not, continue)
    // Then tell the ontology that we have an anonymous superclass
    {
        LOG_DEBUG_S << "Cardinality Restrictions";
        std::map<IRI, OWLCardinalityRestriction>::iterator cit = mCardinalityRestrictions.begin();
        for(; cit != mCardinalityRestrictions.end(); ++cit)
        {
            OWLCardinalityRestriction& cardinalityRestriction = cit->second;

            // Found cardinality restriction
            try {
                // Set stuff of associated generic and qualified restriction classes
                std::map<IRI, OWLRestriction>::const_iterator rcit = restrictionMap.find(cit->first);
                if (rcit != restrictionMap.end())
                {
                    cardinalityRestriction.setProperty(rcit->second.getProperty());
                } else {
                    throw std::invalid_argument("owl::onProperty missing for cardinality restriction");
                }
                std::map<IRI, OWLQualifiedRestriction>::const_iterator qrcit = qualifiedRestrictionMap.find(cit->first);
                if (qrcit != qualifiedRestrictionMap.end())
                {
                    cardinalityRestriction.setQualification(qrcit->second.getQualification());
                }

                OWLCardinalityRestriction::Ptr cardinalityRestrictionPtr = cit->second.narrow();


                // Get anonymous node this restriction is responsible for
                std::vector<OWLClass::Ptr> subclasses = mAnonymousRestrictions[cit->first];
                std::vector<OWLClass::Ptr>::const_iterator sit = subclasses.begin();
                for(; sit != subclasses.end(); ++sit)
                {
                    tell.subClassOf(*sit, cardinalityRestrictionPtr);
                }
            } catch(const std::runtime_error& e)
            {
                LOG_ERROR_S << "Error handling cardinality restriction: '" << cit->first << "' -- " << e.what();
            }
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

    {
        db::query::Results results = findAll(Subject(), Predicate(), Object());
        ResultsIterator it(results);
        while(it.next())
        {
            IRI subject = it[Subject()];
            IRI predicate = it[Predicate()];
            IRI object = it[Object()];

            if(predicate == vocabulary::OWL::oneOf())
            {
                // object is a node representing a list of named individuals
                // if l is not a list (of named individuals) raise
                if( ask.isSubClassOf(subject, vocabulary::RDFS::Datatype() ))
                {
                    owlapi::model::IRIList list = getList(object);
                    tell.dataOneOf(subject, list);

                }
            } else if(predicate == vocabulary::OWL::intersectionOf())
            {
                // add the axiom Class(x complete lt1 lt2 .. ltn)
                // where lt1 ... ltn ard the translated descriptions in the list
                // l
                // if l is not a list (of class descriptions) raise an error

            } else if(predicate == vocabulary::OWL::unionOf())
            {
                // add the axiom Class(x complete unionOf(lt1 lt2 ..ltn)
                // otherwise same as intersectionOf
            } else if(predicate == vocabulary::OWL::complementOf())
            {
                // add the axiom Class(x complete complementOf(nt))
                // where nt is the translation of object, if nt is not a class
                // description raise
            }
        }
    }
}

owlapi::model::IRIList OWLOntologyReader::getList(const owlapi::model::IRI& anonymousId)
{
    owlapi::model::IRIList list;
    IRI id = anonymousId;
    while(true)
    {
        std::map<IRI, HeadTail>::const_iterator cit = mAnonymousLists.find(id);

        if(cit == mAnonymousLists.end())
        {
            throw std::runtime_error("owlapi::model::OWLOntologyReader::getList"
                    " failed to identify list for id '" + anonymousId.toString()
                    + "'");
        }
        HeadTail headTail = cit->second;
        if(headTail.second == vocabulary::RDF::nil())
        {
            return list;
        } else {
            list.push_back(headTail.first);
            id = headTail.second;
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
        {
            Results domain = findAll(relation, vocabulary::RDFS::domain(), Object());
            if(!domain.empty())
            {
                ResultsIterator domainIt(domain);
                while(domainIt.next())
                {
                    IRI classType = domainIt[Object()];
                    LOG_DEBUG_S << "tell: " << relation << " rdfs:domain " << classType;
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
            LOG_DEBUG_S << subject << " " << relation << " " << object;
            try {
                // TDB: Plaing data type
                OWLDataType dataType = ask.getDataType(relation, object.toString());
                OWLLiteral::Ptr literal = OWLLiteral::create(object.toString(), dataType);
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
