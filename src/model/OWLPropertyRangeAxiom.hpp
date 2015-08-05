#ifndef OWLAPI_MODEL_OWL_PROPERTY_RANGE_AXIOM_HPP
#define OWLAPI_MODEL_OWL_PROPERTY_RANGE_AXIOM_HPP

namespace owlapi {
namespace model {

template<typename T>
class OWLPropertyRangeAxiom
{
public:
    typedef T range_t;

    OWLPropertyRangeAxiom(const T& range)
        : mRange(range)
    {}

    virtual ~OWLPropertyRangeAxiom() {}

    range_t getRange() const { return mRange; }

private:
    range_t mRange;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_PROPERTY_RANGE_AXIOM_HPP
