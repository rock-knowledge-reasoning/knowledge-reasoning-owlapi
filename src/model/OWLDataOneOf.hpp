#ifndef OWLAPI_MODEL_OWLDATAONEOF_HPP
#define OWLAPI_MODEL_OWLDATAONEOF_HPP

#include "OWLDataRange.hpp"
#include "OWLLiteral.hpp"

namespace owlapi {
namespace model {

class OWLDataOneOf : public OWLDataRange
{
public:
    typedef shared_ptr<OWLDataOneOf> Ptr;

    OWLDataOneOf(const IRIList& iris);

    OWLDataOneOf(const OWLLiteral::PtrList& literals =
            OWLLiteral::PtrList());

    const OWLLiteral::PtrList& getLiterals() const { return mLiterals; }

    OWLDataRange::Type getDataRangeType() const { return DATA_ONE_OF; }


private:
    OWLLiteral::PtrList mLiterals;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWLDATAONEOF_HPP
