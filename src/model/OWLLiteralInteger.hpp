#ifndef OWLAPI_MODEL_OWL_LITERAL_INTEGER_HPP
#define OWLAPI_MODEL_OWL_LITERAL_INTEGER_HPP

#include <owlapi/model/OWLLiteralNumeric.hpp>
#include <owlapi/model/OWLLiteral.hpp>
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

class OWLLiteralInteger : public OWLLiteral, OWLLiteralNumeric<int>
{
public:
    OWLLiteralInteger(int32_t value)
        : OWLLiteral( "", vocabulary::XSD::integer().toString())
        , OWLLiteralNumeric<int>(value)
    {
        std::stringstream ss;
        ss << value;
        mValue = ss.str();
    }

    OWLLiteralInteger(const std::string& value)
        : OWLLiteral(value)
        , OWLLiteralNumeric<int>(0)
    {
        mNumericValue = fromString(mValue);
    }

    virtual ~OWLLiteralInteger() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_LITERAL_INTEGER_HPP
