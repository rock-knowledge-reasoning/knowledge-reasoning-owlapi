#ifndef OWLAPI_HPP
#define OWLAPI_HPP

/**
  * \namespace owlapi
  * \brief The global namespace of this library
  *
  * \namespace owlapi::db
  * \brief This namespace wraps all classes concerning the access to the
  * triplestore here refered to as database (db)
  *
  * \namespace owlapi::io
  * \brief This namespace wraps all classes for handling the Input/Output (I/O)
  * for ontologies
  *
  * \namespace owlapi::model
  * \brief This namespace wraps all classes that serve the in-memory representation of an
  * ontology - based on http://owlcs.github.io/owlapi/
  *
  * \namespace owlapi::reasoner
  * \brief This namespace wraps the reasoning functionality, which is
  * implemented in this library with FACT++
  *
  * \namespace owlapi::utils
  * \brief This namespace wraps general useful functionality to support the work with
  * ontologies in this library
  *
  * \namespace owlapi::vocabulary
  * \brief This namespace wraps existing W3C Ontologies in the form of so-called
  * vocabularies, in order to facilitate the programmatic use
  */

#include <owlapi/SharedPtr.hpp>
#include <owlapi/model/HasIRI.hpp>
#include <owlapi/model/OWLObject.hpp>
#include <owlapi/model/OWLNamedObject.hpp>
#include <owlapi/model/OWLAxiom.hpp>
#include <owlapi/model/OWLEntity.hpp>
#include <owlapi/model/OWLObjectProperty.hpp>
#include <owlapi/model/OWLInverseObjectProperty.hpp>
#include <owlapi/model/OWLClass.hpp>
#include <owlapi/model/OWLDataProperty.hpp>
#include <owlapi/model/OWLObjectProperty.hpp>
#include <owlapi/model/OWLRestriction.hpp>
#include <owlapi/model/OWLQualifiedRestriction.hpp>
#include <owlapi/model/OWLValueRestriction.hpp>
#include <owlapi/model/OWLCardinalityRestriction.hpp>
#include <owlapi/model/OWLObjectExactCardinality.hpp>
#include <owlapi/model/OWLObjectMinCardinality.hpp>
#include <owlapi/model/OWLObjectMaxCardinality.hpp>
#include <owlapi/model/OWLIndividual.hpp>
#include <owlapi/model/OWLNamedIndividual.hpp>
#include <owlapi/model/OWLAnonymousIndividual.hpp>
#include <owlapi/model/OWLLiteral.hpp>
#include <owlapi/model/OWLLiteralInteger.hpp>
#include <owlapi/model/OWLOntology.hpp>
#include <owlapi/model/OWLOntologyTell.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>

#include <owlapi/model/OWLLiteral.hpp>
#include <owlapi/model/OWLAxiomVisitor.hpp>

#endif // OWLAPI_HPP
