#ifndef OWLAPI_MODEL_OWL_PROPERTY_RANGE_HPP
#define OWLAPI_MODEL_OWL_PROPERTY_RANGE_HPP

#include <vector>
#include <owlapi/model/OWLObject.hpp>

namespace owlapi {
namespace model {

/**
 * \class OWLPropertyRange
 * \brief Marker class for objects that can be the ranges of properties
 * \details Multiple range restrictions are interpreted as stating that the range of the
 * property is the intersection of all ranges
 * \see http://www.w3.org/TR/2003/WD-owl-ref-20030221/#range-def
 */
class OWLPropertyRange : public OWLObject//, SWRLPredicate
{
public:
    typedef shared_ptr<OWLPropertyRange> Ptr;
    typedef std::vector<Ptr> PtrList;

    virtual ~OWLPropertyRange() {}

    virtual bool isDataRange() const { return false; }
    virtual bool isObjectRange() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_PROPERTY_RANGE_HPP
