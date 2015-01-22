#ifndef OWLAPI_MODEL_OWL_AXIOM_VISITOR_HPP
#define OWLAPI_MODEL_OWL_AXIOM_VISITOR_HPP

#include <owlapi/model/OWLClassAssertionAxiom.hpp>
#include <owlapi/model/OWLSubClassOfAxiom.hpp>
#include <owlapi/model/OWLDeclarationAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyAssertionAxiom.hpp>
#include <owlapi/model/OWLDataPropertyAssertionAxiom.hpp>
#include <owlapi/model/OWLDisjointClassesAxiom.hpp>
#include <owlapi/model/OWLDisjointUnionAxiom.hpp>
#include <owlapi/model/OWLEquivalentClassesAxiom.hpp>

namespace owlapi {
namespace model {

/**
 * An interface for objects that can accept visits from axioms.
 *
 * Implementation based on the visitor pattern
 */
class OWLAxiomVisitor
{
public: 
    void visit(OWLClassAssertionAxiom axiom) { throw std::runtime_error("OWLAxiomVisitor:visit: not implemented"); }
    void visit(OWLSubClassOfAxiom axiom) { throw std::runtime_error("OWLAxiomVisitor:visit: not implemented"); }
    void visit(OWLDeclarationAxiom axiom) { throw std::runtime_error("OWLAxiomVisitor:visit: not implemented"); }
    void visit(OWLObjectPropertyAssertionAxiom axiom) { throw std::runtime_error("OWLAxiomVisitor:visit: not implemented"); }
    void visit(OWLDataPropertyAssertionAxiom axiom) { throw std::runtime_error("OWLAxiomVisitor:visit: not implemented"); }
    void visit(OWLDisjointClassesAxiom axiom) { throw std::runtime_error("OWLAxiomVisitor:visit: not implemented"); }
    void visit(OWLDisjointUnionAxiom axiom) { throw std::runtime_error("OWLAxiomVisitor:visit: not implemented"); }
    void visit(OWLEquivalentClassesAxiom axiom) { throw std::runtime_error("OWLAxiomVisitor:visit: not implemented"); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_AXIOM_VISITOR_HPP
