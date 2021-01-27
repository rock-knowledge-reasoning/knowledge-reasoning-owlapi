#include "OWLEquivalentClassesAxiom.hpp"
#include "OWLAxiomVisitor.hpp"
#include <algorithm>

namespace owlapi {
namespace model {
    OWLEquivalentClassesAxiom::OWLEquivalentClassesAxiom(const OWLClassExpression::PtrList& classExpressions,
           const OWLAnnotationList& annotations)
        : OWLClassAxiom(EquivalentClasses, annotations)
        , mClassExpressions(classExpressions)
    {}

    bool OWLEquivalentClassesAxiom::contains(const OWLClassExpression::Ptr& klass) const
    {
        return std::find(mClassExpressions.begin(), mClassExpressions.end(), klass) != mClassExpressions.end();
    }

} // end namespace model
} // end namespace owlapi
