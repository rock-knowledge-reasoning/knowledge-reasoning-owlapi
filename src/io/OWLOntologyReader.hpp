#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_READER_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_READER_HPP

#include <owlapi/model/OWLOntology.hpp>
#include <owlapi/db/rdf/SparqlInterface.hpp>

namespace owlapi {
namespace model { 

class OWLOntologyTell;
class OWLOntologyAsk;

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

    owlapi::model::OWLOntologyTell* mTell;
    owlapi::model::OWLOntologyAsk* mAsk;

    std::vector<owlapi::model::IRI> mRestrictions;
    std::map<owlapi::model::IRI, std::vector<owlapi::model::OWLClass::Ptr> > mAnonymousRestrictions;
    std::map<owlapi::model::IRI, owlapi::model::OWLCardinalityRestriction> mCardinalityRestrictions;

protected:

    db::query::Results findAll(const db::query::Variable& subject, const db::query::Variable& predicate, const db::query::Variable& object) const;

    void load();

    void loadDeclarationsAndImports();
    void loadAxioms();

    void loadRestrictions();
    void loadProperties();
    void loadObjectProperties();
    void loadDataProperties();

public:
    /**
     * Default constructor for the OWLOntologyReader
     * use the SPARQL query interface by default
     */
    OWLOntologyReader();

    virtual ~OWLOntologyReader();

    virtual owlapi::model::OWLOntology::Ptr fromFile(const std::string& filename);
};

} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_READER_HPP
