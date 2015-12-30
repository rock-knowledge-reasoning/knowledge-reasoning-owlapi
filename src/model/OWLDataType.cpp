#include "OWLDataType.hpp"
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

OWLDataType::OWLDataType(const IRI& iri, OWLDataRange::Type type)
    : OWLDataRange(type)
    , OWLLogicalEntity(iri, OWLEntity::DATATYPE)
{}

OWLDataType OWLDataType::fromRange(const OWLDataRange::Ptr& range)
{
    OWLDataType::Ptr ptr = dynamic_pointer_cast<OWLDataType>(range);
    if(ptr)
    {
        return *ptr.get();
    } else {
        throw std::invalid_argument("owlapi::model::OWLDataType::fromRange: "
                " range cannot be casted to an OWLDataType object");
    }
}

bool OWLDataType::isBoolean() const
{
    return getIRI() == vocabulary::XSD::boolean();
}

bool OWLDataType::isDouble() const
{
    return getIRI() == vocabulary::XSD::resolve("double");
}

bool OWLDataType::isFloat() const
{
    return getIRI() == vocabulary::XSD::resolve("float");
}

bool OWLDataType::isRDFPlainLiteral() const
{
    return getIRI() == vocabulary::RDF::PlainLiteral();
}

bool OWLDataType::isInteger() const
{
    IRI iri = getIRI();
    if(iri == vocabulary::XSD::integer())
    {
        return true;
    } else if(iri == vocabulary::XSD::nonNegativeInteger())
    {
        return true;
    } else if(iri == vocabulary::XSD::resolve("int"))
    {
        return true;
    }

    return false;
}

} // end namespace model
} // end namespace owlapi
