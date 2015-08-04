#ifndef OWLAPI_MODEL_OWL_DATATYPE_HPP
#define OWLAPI_MODEL_OWL_DATATYPE_HPP

#include <owlapi/model/OWLDataRange.hpp>
#include <owlapi/model/OWLLogicalEntity.hpp>

namespace owlapi {
namespace model {

class OWLDataType : public OWLDataRange, public OWLLogicalEntity
{
public:
    typedef boost::shared_ptr<OWLDataType> Ptr;

    OWLDataType(const IRI& iri, OWLDataRange::Type type = OWLDataRange::DATATYPE);
    virtual ~OWLDataType() {}

    static OWLDataType fromRange(const OWLDataRange::Ptr& range);

    // OWL2DataType getBuiltInDataType() const;

    bool isBoolean() const;
    bool isDouble() const;
    bool isFloat() const;
    bool isInteger() const;
    bool isRDFPlainLiteral() const;
    bool isString() const;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATATYPE_HPP
