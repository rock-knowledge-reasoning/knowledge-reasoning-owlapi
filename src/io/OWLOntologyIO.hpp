#ifndef OWLAPI_IO_OWL_ONTOLOGY_IO_HPP
#define OWLAPI_IO_OWL_ONTOLOGY_IO_HPP

#include <string>
#include <owlapi/model/OWLOntology.hpp>

namespace owlapi {
namespace io {

enum Format { UNKNOWN = 0,
        RDFXML,
        END_FORMAT
        };

/**
 * Loading a single ontology
 *
 * TODO: "Check Canonical Parsing of OWL2 Ontologies"
 * \see http://www.w3.org/TR/2012/REC-owl2-syntax-20121211/#Ontology_Documents
 */
class OWLOntologyIO
{
public:
    static void write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology, Format format = RDFXML);

    static owlapi::model::OWLOntology::Ptr fromFile(const std::string& filename);

    /**
     * Load an ontology which has only been defined through imported documents
     * \param ontology Ontology object to load and populate
     * \param ontologyIRI ontology iri that should be assigned to the document if no iri is given
     * \throws if this ontology has already been loaded (checking on any existing
     * axioms)
     * \return OWLOntology::Ptr
     */
    static owlapi::model::OWLOntology::Ptr load(owlapi::model::OWLOntology::Ptr& ontology, const owlapi::model::IRI& ontologyIRI = "");

    /**
     * Retrieve the Ontology document and provide path to the document
     * \return path to the document
     * \throw std::runtime_error if the document could not be retrieved
     */
    static std::string retrieve(const owlapi::model::IRI& iri);

    /**
     * Get the default path where ontologies are stored in the system
     * \return Default path for storing ontologies
     */
    static std::string getOntologyPath();

    /**
     * \brief Generate a canonized string from an IRI with the intend to generate
     * a file name from it
     * \details Replaces all characters: /:. with character _
     * \return canonized string
     */
    static std::string canonizeForOfflineUsage(const owlapi::model::IRI& iri);

    /**
     * \return the default suffix for owl files
     */
    static std::string getOWLSuffix() { return ".owl"; }

};


} // end namespace io
} // end namespace owlapi
#endif // end OWLAPI_IO_OWL_ONTOLOGY_IO_HPP
