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
    OWLEquivalentClassesAxiom(OWLClassExpression::PtrList classExpressions,
            OWLAnnotationList annotations = OWLAnnotationList())
        : OWLClassAxiom(EquivalentClasses, annotations)
        , mClassExpressions(classExpressions)
    {}

    virtual ~OWLEquivalentClassesAxiom() {}

    bool containsNamedEquivalentClass() const { throw std::runtime_error("OWLEquivalentClassesAxiom:containsOWLNothing: not implemented"); }

    bool containsOWLNothing() const { throw std::runtime_error("OWLEquivalentClassesAxiom:containsOWLNothing: not implemented"); }

    bool containsOWLThing() const { throw std::runtime_error("OWLEquivalentClassesAxiom:containsOWLThing: not implemented"); }

    std::vector<OWLClass> getNamedClasses() { throw std::runtime_error("OWLEquivalentClassesAxiom:getNamedClasses: not implemented"); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_EQUIVALENT_CLASSES_AXIOM_HPP
