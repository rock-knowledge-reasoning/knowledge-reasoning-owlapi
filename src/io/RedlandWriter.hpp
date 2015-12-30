#ifndef OWLAPI_IO_REDLAND_WRITER_HPP
#define OWLAPI_IO_REDLAND_WRITER_HPP

#include <string>
#include <vector>
#include <owlapi/io/OWLWriter.hpp>
#include <owlapi/model/OWLAxiomVisitor.hpp>
#include <owlapi/model/OWLCardinalityRestriction.hpp>
#include <raptor2/raptor2.h>

namespace owlapi {
namespace io {

/**
 * \class RedlandVisitor
 * \brief This visitor renders triple statements according to the language mapping
 * from OWL2 to RDF
 * \details This implementation handles declarations, cardinality restrictions,
 * properties assignments -- however it is still incomplete mainly with respect
 * to complex class expressions
 * The visitor is apply to the list of axioms returned by owlapi::model::OWLOntology::getAxioms
 * \see http://www.w3.org/TR/owl2-mapping-to-rdf/
 *
 */
class RedlandVisitor : public owlapi::model::OWLAxiomVisitor
{
public:
    RedlandVisitor(raptor_world* world, raptor_serializer* serializer);

    ~RedlandVisitor();

    void visit(const owlapi::model::OWLDeclarationAxiom& axiom);
    void visit(const owlapi::model::OWLSubClassOfAxiom& axiom);
    void visit(const owlapi::model::OWLSubObjectPropertyOfAxiom& axiom);
    void visit(const owlapi::model::OWLSubDataPropertyOfAxiom& axiom);

    void visit(const owlapi::model::OWLObjectPropertyDomainAxiom& axiom);
    void visit(const owlapi::model::OWLObjectPropertyRangeAxiom& axiom);
    void visit(const owlapi::model::OWLDataPropertyDomainAxiom& axiom);
    void visit(const owlapi::model::OWLDataPropertyRangeAxiom& axiom);

    void visit(const owlapi::model::OWLInverseObjectPropertiesAxiom& axiom);
    void visit(const owlapi::model::OWLFunctionalObjectPropertyAxiom& axiom);
    void visit(const owlapi::model::OWLInverseFunctionalObjectPropertyAxiom& axiom);
    void visit(const owlapi::model::OWLReflexiveObjectPropertyAxiom& axiom);
    void visit(const owlapi::model::OWLIrreflexiveObjectPropertyAxiom& axiom);
    void visit(const owlapi::model::OWLSymmetricObjectPropertyAxiom& axiom);
    void visit(const owlapi::model::OWLAsymmetricObjectPropertyAxiom& axiom);
    void visit(const owlapi::model::OWLTransitiveObjectPropertyAxiom& axiom);
    void visit(const owlapi::model::OWLFunctionalDataPropertyAxiom& axiom);

    void visit(const owlapi::model::OWLObjectPropertyAssertionAxiom& axiom);
    void visit(const owlapi::model::OWLDataPropertyAssertionAxiom& axiom);
    void visit(const owlapi::model::OWLClassAssertionAxiom& axiom);

    template<typename T>
    void writeObjectProperty(const T& axiom, const owlapi::model::IRI& propertyType)
    {
        using namespace owlapi::model;
        IRI iri = dynamic_pointer_cast<OWLObjectProperty>(axiom.getProperty())->getIRI();
        writeTriple(iri, vocabulary::RDF::type(), propertyType);
    }

    raptor_term* writeSequence(const owlapi::model::IRIList& list);

    /**
     * Write a triple using literal as last term
     */
    void writeTriple(const owlapi::model::IRI& subject,
            const owlapi::model::IRI& predicate,
            const owlapi::model::OWLLiteral::Ptr& literal) const;

    void writeTriple(const owlapi::model::IRI& subject,
        const owlapi::model::IRI& predicate,
        const owlapi::model::IRI& object) const;

    raptor_term* writeAnonymous(raptor_term* anonymous, const owlapi::model::IRI& predicate,
        const owlapi::model::IRI& object) const;

    raptor_term* writeAnonymous(const owlapi::model::IRI& subject, const owlapi::model::IRI& predicate,
        raptor_term* anonymous) const;

    void writeAnonymous(raptor_term* anonymous, const owlapi::model::IRI& predicate,
        raptor_term* anonymousObject) const;

    void writeAnonymousLiteral(raptor_term* anonymous, const owlapi::model::IRI& predicate, const owlapi::model::OWLLiteral::Ptr& literal) const;

    void writeTripleWithLiteral(raptor_term* subject, const owlapi::model::IRI& predicate, const owlapi::model::OWLLiteral::Ptr& literal) const;

    raptor_term* writeRestriction(owlapi::model::OWLRestriction::Ptr restriction, const owlapi::model::IRI& type);

    raptor_term* termFromIRI(const owlapi::model::IRI& iri) const;
private:
    raptor_world* mWorld;
    raptor_serializer* mSerializer;

    typedef std::map< std::pair<owlapi::model::OWLRestriction::Ptr, owlapi::model::IRI> , raptor_term*> Restriction2Term;
    Restriction2Term mRestrictions;
};

/**
 * Serialization based on raptor and per default serialization to RDFXML
 *
 * \see http://www.w3.org/TR/REC-rdf-syntax/#section-Syntax-datatyped-literals
 * \see http://librdf.org/raptor/
 */
class RedlandWriter : public OWLWriter
{
public:
    RedlandWriter();
    virtual ~RedlandWriter();

    /**
     * Get supported formats as returned from the underlying writer
     */
    virtual std::vector<std::string> getSupportedFormats() const;

    virtual void write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology, bool includeImports = false) const;
private:
    raptor_world* mWorld;
    mutable raptor_serializer* mSerializer;
};

} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_IO_RDFXML_WRITER_HPP
