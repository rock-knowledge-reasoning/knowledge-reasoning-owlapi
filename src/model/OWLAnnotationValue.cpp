#include "OWLAnnotationValue.hpp"
#include "OWLLiteral.hpp"

namespace owlapi {
namespace model {

OWLLiteral::Ptr OWLAnnotationValue::asLiteral() const
{
    return OWLLiteral::Ptr();
}

IRI OWLAnnotationValue::asIRI() const
{
    return IRI();
}

} // model
} // owlapi
