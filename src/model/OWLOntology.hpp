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

OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestriction::Ptr( new OWLMaxCardinalityRestriction(oProperty, 10, cameraKlass.getIRI()));


// Retrieve information from ontology
OWLOntologyAsk ask(ontology);
IRI robot("http:://www.rock-robotics.org/2014/01/om-schema#Sherpa")
std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = ask.getCardinalityRestrictions(robot);
 
 \endverbatim
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
    // Mapping of iri to types
    std::map<IRI, OWLClass::Ptr> mClasses;
    std::map<IRI, OWLNamedIndividual::Ptr> mNamedIndividuals;
    std::map<IRI, OWLAnonymousIndividual::Ptr> mAnonymousIndividuals;
    std::map<IRI, OWLObjectProperty::Ptr> mObjectProperties;
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
