#ifndef OWLAPI_MODEL_NODE_ID_HPP
#define OWLAPI_MODEL_NODE_ID_HPP

#include <stdint.h>
#include <string.h>
#include <sstream>
#include <owlapi/model/IRI.hpp>

namespace owlapi {
namespace model {

/**
 * \brief Represents the Node ID for anonymous individuals
 */
class NodeID
{
    std::string mId;
    static uint32_t msGlobalId;
    bool mAnonymous;

public:
    NodeID();

    NodeID(const std::string& id, bool anonymous);

    std::string getID() const { return mId; }

    /**
     * Returns an absolute IRI from a nodeID attribute.
     */
    static std::string getIRIFromNodeID(const std::string& nodeID) { throw std::runtime_error("NodeID::getIRIFromNodeID not implemented"); }

    /**
     * Gets a NodeID with a specific identifier string
     */
    static NodeID getNodeID(const std::string& id) { throw std::runtime_error("NodeID::getNodeID not implemented"); }

    static bool isAnonymousNodeID(const std::string iri) { throw std::runtime_error("NodeID::getNodeID not implemented"); }
    /**
     * Tests whether supplied string/IRI was generated by this parser in order to label an anonymous node.
     */
    static bool isAnonymousNodeIRI(const std::string& iri) { throw std::runtime_error("NodeID::getNodeID not implemented"); }
    /**
     * Tests whether supplied IRI was generated by this parser in order to label an anonymous node.
     */
    static bool isAnonymousNodeIRI(const IRI& iri) { throw std::runtime_error("NodeID::isAnonymousNodeIRI: not implemented"); }


    /**
     * Generates next anonymous IRI
     */
    static std::string nextAnonymousIRI();

    static std::string nodeString(uint32_t id);
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_NODE_ID_HPP