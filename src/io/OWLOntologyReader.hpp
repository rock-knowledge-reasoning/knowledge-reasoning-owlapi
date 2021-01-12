#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_READER_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_READER_HPP

#include "../model/OWLOntology.hpp"
#include "../db/rdf/SparqlInterface.hpp"

namespace owlapi {
namespace io {

class OntologyChange;

}
namespace model {

class OWLOntologyTell;
class OWLOntologyAsk;
class OWLFacetRestriction;
class OWLObjectRestriction;

}

namespace io {

/**
 *
 * \see http://www.w3.org/TR/owl2-syntax/#Entity_Declarations_and_Typing
 */
class OWLOntologyReader
{
    /// Pointer to the underlying query interfaces for SPARQL
    db::query::SparqlInterface* mSparqlInterface;

    std::vector<owlapi::model::IRI> mRestrictions;
    std::map<owlapi::model::IRI, std::vector< shared_ptr<OntologyChange> > > mAnonymousOntologyChanges;
    std::map<owlapi::model::IRI, owlapi::model::OWLCardinalityRestriction> mCardinalityRestrictions;

    typedef std::pair<owlapi::model::IRI, owlapi::model::IRI> HeadTail;
    std::map<owlapi::model::IRI, HeadTail > mAnonymousLists;
    std::map<owlapi::model::IRI, owlapi::model::OWLFacetRestriction::List> mAnonymousListsOfFacetRestrictions;
    std::map<owlapi::model::IRI, owlapi::model::OWLFacetRestriction> mFacetRestrictions;
    std::map<owlapi::model::IRI, owlapi::model::OWLDataTypeRestriction::Ptr> mAnonymousDataTypeRestrictions;

    std::map<owlapi::model::IRI, owlapi::model::OWLObjectRestriction::Ptr> mAnonymousObjectRestrictions;

    /// The currently opened path
    std::string mAbsolutePath;

protected:

    db::query::Results findAll(const db::query::Variable& subject, const db::query::Variable& predicate, const db::query::Variable& object) const;

    void load(owlapi::model::OWLOntology::Ptr& ontology);


    void loadProperties(owlapi::model::OWLOntology::Ptr& ontology);
    void loadRestrictions(owlapi::model::OWLOntology::Ptr& ontology);
    void loadObjectProperties(owlapi::model::OWLOntology::Ptr& ontology);
    void loadDataProperties(owlapi::model::OWLOntology::Ptr& ontology);
    void loadAnnotationProperties(owlapi::model::OWLOntology::Ptr& ontology);
    void loadAnonymousLists(owlapi::model::OWLOntology::Ptr& ontology);
    void loadDataTypeRestrictions(owlapi::model::OWLOntology::Ptr& ontology);
    void loadObjectRestrictions(owlapi::model::OWLOntology::Ptr& ontology);
    owlapi::model::IRIList getList(const owlapi::model::IRI& anonymousId,
            const std::map<owlapi::model::IRI, HeadTail>& anonymousLists);

public:
    /**
     * Open file to load ontology in an incremental way
     */
    owlapi::model::OWLOntology::Ptr open(const std::string& filename);

    /**
     * Only identify the imports
     */
    void loadImports(owlapi::model::OWLOntology::Ptr& ontology, bool directImports);

    /**
     * Load declarations and imports for this ontology
     * \param ontology The ontology to load declarations and imports for
     * \param directImports Set to true if this a direct import false otherwise
     */
    void loadDeclarations(owlapi::model::OWLOntology::Ptr& ontology, bool directImports);

    /**
     * Load 'the rest' after declarations and import definitions
     */
    void loadAxioms(owlapi::model::OWLOntology::Ptr& ontology);

    /**
     * Default constructor for the OWLOntologyReader
     * use the SPARQL query interface by default
     */
    OWLOntologyReader();

    virtual ~OWLOntologyReader();

    /**
     * Load ontology in one go -- only possible if no imports are used
     */
    virtual owlapi::model::OWLOntology::Ptr fromFile(const std::string& filename);

    /**
     * Retrieve the absolute path this ontology has been loaded from
     * \return path
     */
    const std::string& getAbsolutePath() const { return mAbsolutePath; }
};

} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_READER_HPP
