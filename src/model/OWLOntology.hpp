#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_HPP

#include <map>
#include <owlapi/model/OWLClass.hpp>
#include <owlapi/model/OWLNamedIndividual.hpp>
#include <owlapi/model/OWLAnonymousIndividual.hpp>
#include <owlapi/model/OWLObjectProperty.hpp>
#include <owlapi/model/OWLDataProperty.hpp>
#include <owlapi/model/OWLAxiom.hpp>
#include <owlapi/model/OWLClassAssertionAxiom.hpp>
#include <owlapi/model/OWLDeclarationAxiom.hpp>
#include <owlapi/model/OWLSubClassOfAxiom.hpp>

/**
 * \mainpage OWL API in C++
 * \mainsection Managing Web Ontology Language (OWL) based information in C++
 *
 * In general we recommend the usage of the OWL editor protége: http://protege.stanford.edu
 * in case you want to edit/create OWL model files.
 * Make sure you export as RDF/XML -- this is currently the only supported
 * format for import.
 *
 * Currently this library supports a limit modelling capability, i.e., the main
 * limitations arises from the fact that modelling from user level is done
 * mainly through NamedInvididuals.
 * Thus, anonymous classes and complex constructs such as Collections, OneOf,
 * UnionOf classes is currently now supported.
 *
 * \section Examples
 * The following examples shall help you to understand the user interface and
 * applicability.
 *
 * \verbatim
#include <owlapi/model/OWLOntology.hpp>

using namespace owlapi::model;

OWLOntologyReader reader;
OWLOntology::Ptr ontology = reader.fromFile("om-schema-v0.6.owl");

// Add information to ontology
OWLOntologyTell tell(ontology);
OWLClass::Ptr robotKlass = tell->klass("http:://www.rock-robotics.org/2014/01/om-schema#Sherpa")
OWLClass::Ptr cameraKlass = tell->klass("http:://www.rock-robotics.org/2014/01/om-schema#Camera")
OWLObjectPropery::Ptr oProperty = tell->objectProperty("http://www.rock-robotics.org/2014/01/om-schema#has");

// either
{
    OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestriction::Ptr( new OWLMaxCardinalityRestriction(oProperty, 10, cameraKlass.getIRI()));
    tell->restrictClass(robotKlass, restriction); // alternatively: tell->subClassOf(robotKlass, restriction);
}
// or
{
    OWLCardinalityRestriction::Ptr restriction = tell->cardinalityRestriction(oProperty, 10, cameraKlass.getIRI(), OWLCardinalityRestriction::MAX);
    tell->restrictClass(robotKlass, restriction); // alternatively: tell->subClassOf(robotKlass, restriction);
}


// Retrieve information from ontology
OWLOntologyAsk ask(ontology);
IRI robot("http:://www.rock-robotics.org/2014/01/om-schema#Sherpa")
std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = ask.getCardinalityRestrictions(robot);
 \endverbatim

 * \subsection Vocabularies
 *
 * To facilitate the handling of OWL statements, you can use inbuilt
 * vocabularies or define your own -- see owlapi/vocabularies/OWL.hpp
 * for a starting example including some macros that facilitate the definition
 * of new vocabularies.
 *
 * Define a custom vocabulary
 * \verbatim
 #include <owlapi/Vocabulary.hpp>

 owlapi::vocabulary::Custom custom("http://base-url/");
 owlapi::model::IRI iri = custom.resolve("my-instance");
 \endverbatim
 *
 * Use an existing vocabulary. Note that the iri for types which collide
 * with C++ inbuilt types/keyword have to rely on the 'resolve' function, e.g.,
 * double, float, int
 *
 * \verbatim
 #include <owlapi/Vocabulary.hpp>

 owlapi::model::IRI iriThing = owlapi::vocabulary::OWL::Thing();
 owlapi::model::IRI iriDouble = owlapi::vocabulary::OWL::resolve("double");
 \endverbatim
 *
 *
 * \subsection DataProperties Handling of DataProperties
 *
 * Retrieving values of data properties, e.g.,
 * when the datatype for the property is known as 'double'
 *
 * \verbatim

 IRI instance = owlapi::vocabulary::OM::resolve("MyRobot");
 IRI property = owlapi::vocabulary::OM::resolve("mass");

 OWLLiteral::Ptr value = ask.getDataValue(instance, property);
 double robotMass = value->getDouble();

 \endverbatim
 *
 * Setting values of data properties, e.g.,
 * when the datatype for the property is known as 'double'
 * \verbatim

 IRI instance = owlapi::vocabulary::OM::resolve("MyRobot");
 IRI property = owlapi::vocabulary::OM::resolve("mass");

 OWLLiteral::Ptr value = OWLLiteral::create("0.5", owlapi::vocabulary::XSD::resolve("double"));
 tell.valueOf(instance, property, literal);

 \endverbatim
 *
 * \section Architecture
 *
 * This library is a C++-Clone of the JAVA-based owlapi: http://owlapi.sourceforge.net.
 * The motivation for implementation is to allow a consistent application of
 * C/C++-based programs on robotic systems -- especially since we are intending
 * to target small/embedded devices.
 *
 * Furthermore, the embedded reasoner FACT++ (Reasoner for the SROIQ(D) Description Logic v1.6.2) is actually written in C++
 * and thus can be access almost directly.
 * This implementation is not as complete as the orignal JAVA-based
 * one, but provides core features to handle ontologies and supports also some more complex
 * modelling using qualified cardinality restrictions.
 *
 * The user can load the model from file, and manipulate the resulting ontology
 * using two separate accessor classes: the Tell and the Ask interface --
 * inspiration has been taken from the DIG Interface
 * (http://dl.kr.org/dig/interface.html) to use this split.
 *
 */
namespace owlapi {

class KnowledgeBase;

namespace model {

class OWLOntologyTell;
class OWLOntologyAsk;

/**
 * Ontology
 */
class OWLOntology
{
    friend class OWLOntologyTell;
    friend class OWLOntologyAsk;

protected:
    /// Mapping of iri to types
    /// All existing classes
    std::map<IRI, OWLClass::Ptr> mClasses;
    /// All named individuals
    std::map<IRI, OWLNamedIndividual::Ptr> mNamedIndividuals;
    /// All anonymous individuals
    std::map<IRI, OWLAnonymousIndividual::Ptr> mAnonymousIndividuals;
    /// All object properties
    std::map<IRI, OWLObjectProperty::Ptr> mObjectProperties;
    /// All data properties
    std::map<IRI, OWLDataProperty::Ptr> mDataProperties;

    /// General axiom map
    std::map<OWLAxiom::AxiomType, std::vector<OWLAxiom::Ptr> > mAxiomsByType;

    std::map<OWLClassExpression::Ptr, std::vector<OWLClassAssertionAxiom::Ptr> > mClassAssertionAxiomsByClass;

    std::map<OWLIndividual::Ptr, std::vector<OWLClassAssertionAxiom::Ptr> > mClassAssertionAxiomsByIndividual;

    std::map<OWLDataProperty::Ptr, std::vector<OWLAxiom::Ptr> > mDataPropertyAxioms;
    std::map<OWLObjectProperty::Ptr, std::vector<OWLAxiom::Ptr> > mObjectPropertyAxioms;
    std::map<OWLNamedIndividual::Ptr, std::vector<OWLAxiom::Ptr> > mNamedIndividualAxioms;
    /// Map of anonymous individual to all axioms the individual is involved into
    std::map<OWLAnonymousIndividual::Ptr, std::vector<OWLAxiom::Ptr> > mAnonymousIndividualAxioms;
    std::map<OWLEntity::Ptr, std::vector<OWLDeclarationAxiom::Ptr> > mDeclarationsByEntity;


    /// Map to access subclass axiom by a given subclass
    std::map<OWLClassExpression::Ptr, std::vector<OWLSubClassOfAxiom::Ptr> > mSubClassAxiomBySubPosition;
    /// Map to access subclass axiom by a given superclass
    std::map<OWLClassExpression::Ptr, std::vector<OWLSubClassOfAxiom::Ptr> > mSubClassAxiomBySuperPosition;

    KnowledgeBase* mpKnowledgeBase;

    KnowledgeBase* kb() { return mpKnowledgeBase; }

protected:

    OWLClass::Ptr getClass(const IRI& iri) const;
    OWLDataProperty::Ptr getDataProperty(const IRI& iri) const;
    OWLObjectProperty::Ptr getObjectProperty(const IRI& iri) const;
public:
    typedef boost::shared_ptr<OWLOntology> Ptr;

    void refresh();

    /**
     * Default constructor
     */
    OWLOntology();

    ~OWLOntology();
};

} // ane namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_HPP
