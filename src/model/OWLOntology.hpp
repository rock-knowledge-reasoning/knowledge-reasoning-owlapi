#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_HPP

#include <map>
#include <algorithm>
#include "OWLClass.hpp"
#include "OWLNamedIndividual.hpp"
#include "OWLAnonymousIndividual.hpp"
#include "OWLObjectProperty.hpp"
#include "OWLDataProperty.hpp"
#include "OWLAxiom.hpp"
#include "OWLClassAssertionAxiom.hpp"
#include "OWLDeclarationAxiom.hpp"
#include "OWLSubClassOfAxiom.hpp"
#include "ChangeApplied.hpp"
#include "OWLAxiomRetractVisitor.hpp"
#include "QueryCache.hpp"

namespace owlapi {

class KnowledgeBase;

namespace io {
    class OWLOntologyIO;
    class OWLOntologyReader;
    class RedlandWriter;
}

namespace model {

class OWLOntologyTell;
class OWLOntologyAsk;
class OWLOntologyChange;

typedef std::map<OWLAxiom::AxiomType, OWLAxiom::PtrList > AxiomMap;

/**
 * Ontology
 * \see http://www.w3.org/TR/owl2-syntax
 */
class OWLOntology
{
    friend class OWLOntologyTell;
    friend class OWLOntologyAsk;
    friend class io::OWLOntologyIO;
    friend class io::OWLOntologyReader;
    friend class io::RedlandWriter;
    friend class OWLAxiomRetractVisitor;
    friend class OWLAxiomAddVisitor;

protected:
    /// Mapping of IRI to types
    /// All existing classes
    std::map<IRI, OWLClass::Ptr> mClasses;
    /// All named individuals
    std::map<IRI, OWLNamedIndividual::Ptr> mNamedIndividuals;
    /// All anonymous individuals
    std::map<IRI, OWLAnonymousIndividual::Ptr> mAnonymousIndividuals;
    /// All object properties
    std::map<IRI, OWLObjectProperty::Ptr> mObjectProperties;
    /// All data properties
    std::map<IRI, OWLDataProperty::Ptr> mDataProperties;
    /// All annotation properties
    std::map<IRI, OWLAnnotationProperty::Ptr> mAnnotationProperties;
    /// For data range list ONE_OF, UNION_OF ...
    std::map<IRI, std::vector<OWLDataRange::Ptr> > mAnonymousDataRanges;

    /// General axiom map
    AxiomMap mAxiomsByType;

    std::map<OWLClassExpression::Ptr, std::vector<OWLClassAssertionAxiom::Ptr> > mClassAssertionAxiomsByClass;

    std::map<OWLIndividual::Ptr, std::vector<OWLClassAssertionAxiom::Ptr> > mClassAssertionAxiomsByIndividual;

    std::map<OWLDataProperty::Ptr, std::vector<OWLAxiom::Ptr> > mDataPropertyAxioms;
    std::map<OWLObjectProperty::Ptr, std::vector<OWLAxiom::Ptr> > mObjectPropertyAxioms;
    std::map<OWLAnnotationProperty::Ptr, std::vector<OWLAxiom::Ptr> > mAnnotationAxioms;
    std::map<OWLNamedIndividual::Ptr, std::vector<OWLAxiom::Ptr> > mNamedIndividualAxioms;
    /// Map of anonymous individual to all axioms the individual is involved into
    std::map<OWLAnonymousIndividual::Ptr, std::vector<OWLAxiom::Ptr> > mAnonymousIndividualAxioms;
    std::map<OWLEntity::Ptr, std::vector<OWLDeclarationAxiom::Ptr> > mDeclarationsByEntity;

    /// Map to access subclass axiom by a given subclass
    std::map<OWLClassExpression::Ptr, std::vector<OWLSubClassOfAxiom::Ptr> > mSubClassAxiomBySubPosition;
    /// Map to access subclass axiom by a given superclass
    std::map<OWLClassExpression::Ptr, std::vector<OWLSubClassOfAxiom::Ptr> > mSubClassAxiomBySuperPosition;

    /// Full path this ontology has been loaded from
    std::string mAbsolutePath;

    /// The iri associated with this document, where to retrieve it
    IRI mIri;

    /// List of documents that this ontology directly imports
    IRIList mDirectImportsDocuments;

    /// List of documents that this ontology imports directly or indirectly
    IRIList mImportsDocuments;

    shared_ptr<KnowledgeBase> mpKnowledgeBase;

    shared_ptr<KnowledgeBase> kb() { return mpKnowledgeBase; }

    template<typename LIST, typename A>
    void removeAxiomFromMap(LIST& list, const A* axiom)
    {
        typename LIST::iterator it = std::find_if(list.begin(), list.end(), [axiom](const typename LIST::value_type& ptr)
                {
                    return axiom == ptr.get();
                });

        if(it != list.end())
        {
            list.erase(it);
            return;
        }
        throw std::runtime_error("owlapi::model::OWLOntology: failed to remove axiom from list");
    }

protected:
    QueryCache mQueryCache;

    OWLClass::Ptr getClass(const IRI& iri) const;
    OWLDataProperty::Ptr getDataProperty(const IRI& iri) const;
    OWLObjectProperty::Ptr getObjectProperty(const IRI& iri) const;
    OWLAnnotationProperty::Ptr getAnnotationProperty(const IRI& iri) const;
    OWLIndividual::Ptr getIndividual(const IRI& iri) const;

    void addAxiom(const OWLAxiom::Ptr& axiom);
    void removeAxiom(const OWLAxiom::Ptr& axiom);

    void retractValueOf(const OWLIndividual::Ptr& individual, const OWLDataProperty::Ptr& property);

    /**
     * Remove an existing individual
     */
    void retractIndividual(const IRI& iri);

    void setAbsolutePath(const std::string& path) { mAbsolutePath = path; }

public:
    typedef shared_ptr<OWLOntology> Ptr;
    typedef std::vector< Ptr > PtrList;

    void refresh();

    /**
     * Trigger cleanup and garbage collection of axioms that are marked for
     * retraction
     */
    void cleanup();

    /**
     * Default constructor
     * \param kb Knowledge base which can be shared across multiple instances
     */
    OWLOntology(const shared_ptr<KnowledgeBase>& kb = shared_ptr<KnowledgeBase>());

    ~OWLOntology();

    static OWLOntology::Ptr fromFile(const std::string& filename);

    /**
     * Get the absolute path this ontology has been loaded from
     */
    const std::string& getAbsolutePath() const { return mAbsolutePath; }

    void setIRI(const IRI& iri) { mIri = iri; }
    const IRI& getIRI() const { return mIri; }

    const IRIList& getDirectImportsDocuments() const { return mDirectImportsDocuments; }
    IRIList& getDirectImportsDocuments() { return mDirectImportsDocuments; }

    const IRIList& getImportsDocuments() const { return mImportsDocuments; }
    IRIList& getImportsDocuments() { return mImportsDocuments; }

    /**
     * Add a direct import
     */
    void addDirectImportsDocument(const IRI& iri);
    /**
     * Add imported document
     */
    void addImportsDocument(const IRI& iri);

    const AxiomMap& getAxiomMap() const { return mAxiomsByType; }
    OWLAxiom::PtrList getAxioms() const;

    OWLAxiom::PtrList getReferencingAxioms(const OWLEntity::Ptr& entity, bool excludeImports);

    ChangeApplied applyChange(const shared_ptr<OWLOntologyChange>& change);

    /**
      * Get acces to the query cache object
      */
    QueryCache& getQueryCache() { return mQueryCache; }
};

} // ane namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_HPP
