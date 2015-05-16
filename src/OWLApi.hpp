#ifndef OWL_OM_OWL_API_HPP
#define OWL_OM_OWL_API_HPP

#include <boost/shared_ptr.hpp>
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
#include <owlapi/model/OWLCardinalityRestriction.hpp>
#include <owlapi/model/OWLObjectExactCardinality.hpp>
#include <owlapi/model/OWLObjectMinCardinality.hpp>
#include <owlapi/model/OWLObjectMaxCardinality.hpp>
#include <owlapi/model/OWLIndividual.hpp>
#include <owlapi/model/OWLNamedIndividual.hpp>
#include <owlapi/model/OWLAnonymousIndividual.hpp>
#include <owlapi/model/OWLDeclarationAxiom.hpp>
#include <owlapi/model/OWLClassAxiom.hpp>
#include <owlapi/model/OWLSubClassOfAxiom.hpp>
#include <owlapi/model/OWLEquivalentClassesAxiom.hpp>
#include <owlapi/model/OWLDisjointClassesAxiom.hpp>
#include <owlapi/model/OWLDisjointUnionAxiom.hpp>
#include <owlapi/model/OWLClassAssertionAxiom.hpp>
#include <owlapi/model/OWLPropertyAssertionAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyAssertionAxiom.hpp>
#include <owlapi/model/OWLDataPropertyAssertionAxiom.hpp>
#include <owlapi/model/OWLLiteral.hpp>
#include <owlapi/model/OWLLiteralInteger.hpp>
#include <owlapi/model/OWLOntology.hpp>
#include <owlapi/model/OWLOntologyTell.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>

#endif // OWL_OM_OWL_API_HPP