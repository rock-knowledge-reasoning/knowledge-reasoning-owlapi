#ifndef OWLAPI_MODEL_HAS_FILLER
#define OWLAPI_MODEL_HAS_FILLER

#include "../SharedPtr.hpp"

namespace owlapi {
namespace model {

template<typename T>
class HasFiller
{
public:
    typedef shared_ptr<T> filler_t;

    HasFiller() = default;

    HasFiller(const filler_t& filler)
        : mFiller(filler)
    {}

    const filler_t& getFiller() const { return mFiller; }

    void setFiller(filler_t filler) { mFiller = filler; }

protected:
    filler_t mFiller;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_HAS_FILLER
