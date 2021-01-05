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

#include "Exceptions.hpp"
#include "model/HasIRI.hpp"
#include "model/OWLObject.hpp"
#include "model/OWLNamedObject.hpp"
#include "model/OWLAxiom.hpp"
#include "model/OWLEntity.hpp"
#include "model/OWLObjectProperty.hpp"
#include "model/OWLInverseObjectProperty.hpp"
#include "model/OWLClass.hpp"
#include "model/OWLDataProperty.hpp"
#include "model/OWLObjectProperty.hpp"
#include "model/OWLRestriction.hpp"
#include "model/OWLQualifiedRestriction.hpp"
#include "model/OWLValueRestriction.hpp"
#include "model/OWLCardinalityRestriction.hpp"
#include "model/OWLObjectExactCardinality.hpp"
#include "model/OWLObjectMinCardinality.hpp"
#include "model/OWLObjectMaxCardinality.hpp"
#include "model/OWLIndividual.hpp"
#include "model/OWLNamedIndividual.hpp"
#include "model/OWLAnonymousIndividual.hpp"
#include "model/OWLLiteral.hpp"
#include "model/OWLLiteralInteger.hpp"
#include "model/OWLOntology.hpp"
#include "model/OWLOntologyTell.hpp"
#include "model/OWLOntologyAsk.hpp"

#include "model/OWLLiteral.hpp"
#include "model/OWLAxiomVisitor.hpp"

#endif // OWLAPI_HPP
