#ifndef OWLAPI_MODEL_OWL_OBJECT_HPP
#define OWLAPI_MODEL_OWL_OBJECT_HPP

#include <owlapi/Exceptions.hpp>
#include <boost/shared_ptr.hpp>

namespace owlapi {
namespace model {

class OWLAxiomVisitor;

class OWLObject
{
public:
    typedef boost::shared_ptr<OWLObject> Ptr;

    virtual ~OWLObject() {}

    virtual bool isBottomEntity();
    virtual bool isTopEntity();
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_HPP
