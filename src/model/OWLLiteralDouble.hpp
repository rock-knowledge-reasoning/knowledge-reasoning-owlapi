#ifndef OWLAPI_MODEL_OWL_LITERAL_DOUBLE_HPP
#define OWLAPI_MODEL_OWL_LITERAL_DOUBLE_HPP

#include "OWLLiteralNumeric.hpp"
#include "OWLLiteral.hpp"

namespace owlapi {
namespace model {

class OWLLiteralDouble : public OWLLiteral, OWLLiteralNumeric<double>
{
public:
    OWLLiteralDouble(double value);

    OWLLiteralDouble(const std::string& value);

    virtual ~OWLLiteralDouble() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_LITERAL_DOUBLE_HPP
