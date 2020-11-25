#ifndef OWLAPI_MODEL_HAS_SUBJECT_HPP
#define OWLAPI_MODEL_HAS_SUBJECT_HPP

#include "../SharedPtr.hpp"

namespace owlapi {
namespace model {

template<typename T>
class HasSubject
{
public:
    typedef shared_ptr<T> subject_t;

    HasSubject(const subject_t& subject)
        : mSubject(subject)
    {}

    virtual ~HasSubject() {}
    subject_t getSubject() const { return mSubject; }

private:
    subject_t mSubject;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_HAS_SUBJECT_HPP
