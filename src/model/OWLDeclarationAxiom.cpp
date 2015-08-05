#include "OWLDeclarationAxiom.hpp"
#include "OWLAxiomVisitor.hpp"

namespace owlapi {
namespace model {

std::string OWLDeclarationAxiom::toString() const
{
    return "OWLDeclarationAxiom: " + mEntity->toString();
}

} // end namespace model
} // end namespace owlapi
