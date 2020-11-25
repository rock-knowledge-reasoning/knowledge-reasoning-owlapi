#ifndef OWLAPI_MODEL_ANONYMOUS_INDIVIDUAL_HPP
#define OWLAPI_MODEL_ANONYMOUS_INDIVIDUAL_HPP

#include "OWLObject.hpp"
#include "NodeID.hpp"
#include "OWLAnnotationValue.hpp"
#include "OWLAnnotationSubject.hpp"

namespace owlapi {
namespace model {

/**
 * \class OWLAnonymousIndividual
 * \brief Represents Anonymous Individuals in the OWL 2 Specification
 * \see http://www.w3.org/TR/2009/REC-owl2-syntax-20091027/#Anonymous_Individuals
 */
class OWLAnonymousIndividual : public OWLIndividual, public OWLAnnotationValue, public OWLAnnotationSubject
{
    NodeID mNodeID;
public:
    typedef shared_ptr<OWLAnonymousIndividual> Ptr;

    OWLAnonymousIndividual();
    OWLAnonymousIndividual(const NodeID& nodeId)
        : mNodeID()
    {}

    virtual ~OWLAnonymousIndividual() {}

    OWLObject::Type getObjectType() const { return OWLObject::AnonymousIndividual; }

    /**
     * Retrieve associated NodeID of this anonymous individual
     * \return NodeID
     */
    NodeID getNodeID() const { return mNodeID; }

    bool isAnonymous() const { return true; }

    IRI getReferenceID() const { return mNodeID.getIRIFromNodeID(); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_ANONYMOUS_INDIVIDUAL_HPP
