#ifndef OWLAPI_MODEL_OWL_EQUIVALENT_CLASSES_AXIOM_HPP
#define OWLAPI_MODEL_OWL_EQUIVALENT_CLASSES_AXIOM_HPP

#include <stdexcept>
#include "OWLClassAxiom.hpp"
#include "OWLClassExpression.hpp"
#include "OWLClass.hpp"

namespace owlapi {
namespace model {

class OWLEquivalentClassesAxiom : public OWLClassAxiom
{
    OWLClassExpression::PtrList mClassExpressions;

public:
    typedef shared_ptr<OWLEquivalentClassesAxiom> Ptr;

    OWLEquivalentClassesAxiom(const OWLClassExpression::PtrList& classExpressions,
           const OWLAnnotationList& annotations = OWLAnnotationList());

    virtual ~OWLEquivalentClassesAxiom() = default;

    bool contains(const OWLClassExpression::Ptr& klass) const;

    const OWLClassExpression::PtrList& getEquivalentClasses() const { return mClassExpressions; }

    bool containsNamedEquivalentClass() const { throw std::runtime_error("OWLEquivalentClassesAxiom:containsOWLNothing: not implemented"); }

    bool containsOWLNothing() const { throw std::runtime_error("OWLEquivalentClassesAxiom:containsOWLNothing: not implemented"); }

    bool containsOWLThing() const { throw std::runtime_error("OWLEquivalentClassesAxiom:containsOWLThing: not implemented"); }

    std::vector<OWLClass> getNamedClasses() { throw std::runtime_error("OWLEquivalentClassesAxiom:getNamedClasses: not implemented"); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_EQUIVALENT_CLASSES_AXIOM_HPP
