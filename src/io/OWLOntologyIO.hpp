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
 * \class OWLOntologyIO
 * \brief Loading a single ontology
 * \details Retrieval will only work after installation of the owlapi package, which
 * installs a pkg-config file that allows to identify the 'sharedir' variable.
 * Ontologies will be stored in a subfolder 'ontologies'
 *
 * Additionally the package 'owlapi' will install a cmake file, which includes a
 * macro to install ontologies, e.g. to install a file in test/data
 * \verbatim
 find_package(owlapi)
 owlapi_install_ontology("http://www.rock-robotics.org/2014/01/om-schema"
    "test/data/om-schema-v0.15.owl")
 \endverbatim
 *
 * \see OWLOntologyIO::getOntologyPath
 * \todo "Check Canonical Parsing of OWL2 Ontologies"
 * \see http://www.w3.org/TR/2012/REC-owl2-syntax-20121211/#Ontology_Documents
 */
class OWLOntologyIO
{
public:
    static void write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology, Format format = RDFXML);

    /**
     * Load an ontology from file
     * \param filename File to load from
     * \return OWLOntology::Ptr
     */
    static owlapi::model::OWLOntology::Ptr fromFile(const std::string& filename);

    /**
     * Load an ontology by a given iri -- imported ontologies will be loaded as
     * well into the single ontology object
     * If the given ontology is not present as a file on the hard drive, the
     * ontology will be retrieved (by the given ontology's IRI)
     *
     * Retrieval will only work after installation of the owlapi package, which
     * installs a pkg-config file that allows to identify the 'sharedir' variable.
     * Ontologies will be stored in a subfolder 'ontologies'
     * \return Pointer to the OWLOntology instance
     */
    static owlapi::model::OWLOntology::Ptr load(const owlapi::model::IRI& ontologyIRI);

    /**
     * Load an new ontology, which is only defined through its imported documents
     * \param ontology Ontology object to load and populate
     * \param ontologyIRI ontology iri that should be assigned to the document if no iri
     * is given in the document
     * \throws if this ontology has already been loaded (checking on any existing
     * axioms)
     * \return Pointer to the OWLOntology instance
     */
    static owlapi::model::OWLOntology::Ptr loadNew(owlapi::model::OWLOntology::Ptr&
            ontology,
            const owlapi::model::IRI& ontologyIRI = "");

    /**
     * Load an ontology which has only been defined through imported documents
     * \param ontology Ontology object to load and populate
     * \param ontologyIRI ontology iri that should be assigned to the document if no iri
     * is given in the document
     * \param isEmptyTopOntology Mark the top ontology as being empty
     * \throws if this ontology has already been loaded (checking on any existing
     * axioms)
     * \return Pointer to the OWLOntology instance
     */
    static owlapi::model::OWLOntology::Ptr load(owlapi::model::OWLOntology::Ptr&
            ontology,
            const owlapi::model::IRI& ontologyIRI = "",
            bool isEmptyTopOntology = false);

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
