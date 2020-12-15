#include "OWLDataType.hpp"
#include "../Vocabulary.hpp"
#include "OWLDataOneOf.hpp"
#include "OWLDataTypeRestriction.hpp"

namespace owlapi {
namespace model {

OWLDataType::OWLDataType(const IRI& iri, OWLDataRange::Type type)
    : OWLDataRange(type)
    , OWLLogicalEntity(iri, OWLEntity::DATATYPE)
{}

OWLDataType OWLDataType::fromRange(const OWLDataRange::Ptr& range, const std::string& value)
{
    switch(range->getDataRangeType())
    {
        case OWLDataRange::DATATYPE:
            return *dynamic_pointer_cast<OWLDataType>(range);
        case OWLDataRange::ONE_OF:
        {
            OWLDataOneOf::Ptr oneOf = dynamic_pointer_cast<OWLDataOneOf>(range);
            if(oneOf)
            {
                for(const OWLLiteral::Ptr& literal : oneOf->getLiterals())
                {
                    if(literal->getValue() == value)
                    {
                        return OWLDataType(literal->getType());
                    }
                }
                throw std::invalid_argument("owlapi::model::OWLDataType::fromRange:"
                    " failed to find literal '" + value + "' in data range");
            } else {
                throw std::invalid_argument("owlapi::model::OWLDataType::fromRange:"
                        " failed to cast range into OWLDataOneOf object");
            }

        }
        case OWLDataRange::UNION_OF:
        case OWLDataRange::COMPLEMENT_OF:
        case OWLDataRange::INTERSECTION_OF:
        case OWLDataRange::DATATYPE_RESTRICTION:
        {
            owlapi::model::OWLDataTypeRestriction::Ptr datatypeRestriction = dynamic_pointer_cast<OWLDataTypeRestriction>(range);
            if(datatypeRestriction)
            {
                bool isCompliant = true;
                for(const OWLFacetRestriction restriction : datatypeRestriction->getFacetRestrictions())
                {
                    restriction.isCompliant(value);
                }

                if(isCompliant)
                {
                    return datatypeRestriction->getDataType();
                }

                throw std::invalid_argument("owlapi::model::OWLDataType::fromRange:"
                    " failed to find literal in data range that complies with restrictions");
            } else {
                throw std::invalid_argument("owlapi::model::OWLDataType::fromRange:"
                        " failed to cast range into OWLDataTypeRestriction object");
            }
        }

            break;
    }

    throw std::invalid_argument("owlapi::model::OWLDataType::fromRange: "
                " range cannot be turned into an OWLDataType object");
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
