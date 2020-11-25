#ifndef OWLAPI_MODEL_OWL_LITERAL_NON_NEGATIVE_INTEGER_HPP
#define OWLAPI_MODEL_OWL_LITERAL_NON_NEGATIVE_INTEGER_HPP

#include "OWLLiteralNumeric.hpp"
#include "OWLLiteral.hpp"
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

class OWLLiteralNonNegativeInteger : public OWLLiteral, OWLLiteralNumeric<uint32_t>
{
public:
    OWLLiteralNonNegativeInteger(uint32_t value)
        : OWLLiteral("", vocabulary::XSD::nonNegativeInteger().toString())
        , OWLLiteralNumeric<uint32_t>(value)
    {
        std::stringstream ss;
        ss << value;
        mValue = ss.str();
    }

    OWLLiteralNonNegativeInteger(const std::string& value)
        : OWLLiteral(value)
        , OWLLiteralNumeric<uint32_t>(0)
    {
        mNumericValue = fromString(mValue);
    }

    virtual ~OWLLiteralNonNegativeInteger() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_LITERAL_NON_NEGATIVE_INTEGER_HPP
