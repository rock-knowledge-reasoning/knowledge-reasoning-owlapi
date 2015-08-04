#include "RedlandWriter.hpp"
#include <base/Logging.hpp>
#include <owlapi/OWLApi.hpp>

using namespace owlapi::model;

namespace owlapi {
namespace io {

RedlandVisitor::RedlandVisitor(raptor_world* world, raptor_serializer* serializer)
    : OWLAxiomVisitor()
    , mWorld(world)
    , mSerializer(serializer)
{}

void RedlandVisitor::writeTriple(const owlapi::model::IRI& subject,
        const owlapi::model::IRI& predicate,
        const owlapi::model::IRI& object) const
{
    raptor_statement* triple = raptor_new_statement(mWorld);
    triple->subject = raptor_new_term_from_uri_string(mWorld, (const unsigned char*) subject.toString().c_str());
    triple->predicate = raptor_new_term_from_uri_string(mWorld, (const unsigned char*) predicate.toString().c_str());
    triple->object = raptor_new_term_from_uri_string(mWorld, (const unsigned char*) object.toString().c_str());

    raptor_serializer_serialize_statement(mSerializer, triple);
    raptor_free_statement(triple);
}

void RedlandVisitor::visit(const OWLDeclarationAxiom& axiom)
{
    OWLEntity::Ptr entity = axiom.getEntity();

    IRI iri = entity->getIRI();
    switch(entity->getEntityType())
    {
        case OWLEntity::CLASS:
        {
            if(iri == vocabulary::OWL::Class() || iri == vocabulary::OWL::Thing())
            {
                break;
            }
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::Class());
            break;
        }
        case OWLEntity::OBJECT_PROPERTY:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::ObjectProperty());
            break;
        case OWLEntity::DATA_PROPERTY:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::DatatypeProperty());
            break;
        case OWLEntity::ANNOTATION_PROPERTY:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::AnnotationProperty());
            break;
        case OWLEntity::DATATYPE:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::RDFS::Datatype());
            break;
        case OWLEntity::NAMED_INDIVIDUAL:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::NamedIndividual());
            break;
        default:
            break;
    }
}

raptor_term* RedlandVisitor::writeAnonymous(raptor_term* anonymous,
        const owlapi::model::IRI& predicate,
        const owlapi::model::IRI& object) const
{
    LOG_DEBUG_S << "Write anonymous: " << predicate << " " << object << " anon: " << anonymous;
    if(!anonymous)
    {
        anonymous = raptor_new_term_from_blank(mWorld, NULL);
    }

    raptor_statement* triple = raptor_new_statement(mWorld);
    triple->subject = raptor_term_copy(anonymous);
    triple->predicate = raptor_new_term_from_uri_string(mWorld, (const unsigned char*) predicate.toString().c_str());
    triple->object = raptor_new_term_from_uri_string(mWorld, (const unsigned char*) object.toString().c_str());

    raptor_serializer_serialize_statement(mSerializer, triple);
    raptor_free_statement(triple);

    return anonymous;
}

raptor_term* RedlandVisitor::writeAnonymous(const owlapi::model::IRI& subject,
        const owlapi::model::IRI& predicate,
        raptor_term* anonymous) const
{
    LOG_DEBUG_S << "Write anonymous: " << subject << " " << predicate;
    if(!anonymous)
    {
        anonymous = raptor_new_term_from_blank(mWorld, NULL);
    }

    raptor_statement* triple = raptor_new_statement(mWorld);
    triple->subject = raptor_new_term_from_uri_string(mWorld, (const unsigned char*) subject.toString().c_str());
    triple->predicate = raptor_new_term_from_uri_string(mWorld, (const unsigned char*) predicate.toString().c_str());
    triple->object = raptor_term_copy(anonymous);

    raptor_serializer_serialize_statement(mSerializer, triple);
    raptor_free_statement(triple);
    return anonymous;
}

void RedlandVisitor::writeAnonymousLiteral(raptor_term* anonymous,
        const owlapi::model::IRI& predicate,
        const OWLLiteral::Ptr& literal)
{
    raptor_statement* triple = raptor_new_statement(mWorld);
    triple->subject = raptor_term_copy(anonymous);
    triple->predicate = raptor_new_term_from_uri_string(mWorld, (const unsigned char*) predicate.toString().c_str());

    raptor_uri* datatype = raptor_new_uri(mWorld, (const unsigned char*) literal->getType().c_str());

    triple->object = raptor_new_term_from_literal(mWorld,
            (const unsigned char*) literal->getValue().c_str(),
            datatype,
            NULL);

    raptor_serializer_serialize_statement(mSerializer, triple);
    raptor_free_statement(triple);
}

raptor_term* RedlandVisitor::writeRestriction(OWLRestriction::Ptr restriction, const owlapi::model::IRI& restrictionType)
{
    std::pair<OWLRestriction::Ptr, IRI> key(restriction, restrictionType);
    std::map< std::pair<OWLRestriction::Ptr, IRI>, raptor_term*>::iterator it = mRestrictions.find(key);
    if(it != mRestrictions.end())
    {
        return it->second;
    }

    raptor_term* anonymous = writeAnonymous(NULL, vocabulary::RDF::type(), vocabulary::OWL::Restriction());
    OWLNamedObject::Ptr namedObject = boost::dynamic_pointer_cast<OWLNamedObject>( restriction->getProperty() );
    if(!namedObject)
    {
        throw std::runtime_error("owlapi::io::RedlandVisitor::writeRestriction: no support for "
                " complex property expression in restriction");
    } else {
        writeAnonymous(anonymous, vocabulary::OWL::onProperty(), namedObject->getIRI());
    }

    OWLQualifiedRestriction::Ptr qualifiedRestriction = boost::dynamic_pointer_cast<OWLQualifiedRestriction>(restriction);
    if(qualifiedRestriction)
    {
        writeAnonymous(anonymous, vocabulary::OWL::onClass(), qualifiedRestriction->getQualification());
    }

    OWLCardinalityRestriction::Ptr cardinalityRestriction = boost::dynamic_pointer_cast<OWLCardinalityRestriction>(restriction);
    if(cardinalityRestriction)
    {
        OWLLiteral::Ptr literal = OWLLiteral::nonNegativeInteger( cardinalityRestriction->getCardinality() );
        writeAnonymousLiteral(anonymous, restrictionType, literal);
    }

    mRestrictions[key] = anonymous;
    return anonymous;
}

void RedlandVisitor::visit(const OWLSubClassOfAxiom& axiom)
{
    OWLClassExpression::Ptr sub = axiom.getSubClass();
    OWLClassExpression::Ptr super = axiom.getSuperClass();

    if(sub->getClassExpressionType() == OWLClassExpression::OWL_CLASS
        && super->getClassExpressionType() == OWLClassExpression::OWL_CLASS)
    {
        IRI subIRI = boost::dynamic_pointer_cast<OWLClass>(sub)->getIRI();
        IRI superIRI = boost::dynamic_pointer_cast<OWLClass>(super)->getIRI();
        if(superIRI != vocabulary::OWL::Thing())
        {
            writeTriple(subIRI, vocabulary::RDFS::subClassOf(), superIRI);
        }

        return;
    }

    std::vector<raptor_term*> restrictionTerms;

    owlapi::model::OWLRestriction::Ptr restriction = boost::dynamic_pointer_cast<OWLRestriction>(super);

    switch(super->getClassExpressionType())
    {
        case OWLClassExpression::OWL_CLASS:
            throw std::runtime_error("owlapi::io::RedlandWriter: no support for complex subclass expression");

        case OWLClassExpression::DATA_ALL_VALUES_FROM:
        case OWLClassExpression::DATA_EXACT_CARDINALITY:
        case OWLClassExpression::DATA_HAS_VALUE:
        case OWLClassExpression::DATA_MAX_CARDINALITY:
        case OWLClassExpression::DATA_MIN_CARDINALITY:
        case OWLClassExpression::DATA_SOME_VALUES_FROM:
        case OWLClassExpression::OBJECT_ALL_VALUES_FROM:
        case OWLClassExpression::OBJECT_COMPLEMENT_OF:
        case OWLClassExpression::OBJECT_HAS_SELF:
        case OWLClassExpression::OBJECT_HAS_VALUE:
        case OWLClassExpression::OBJECT_INTERSECTION_OF:
        case OWLClassExpression::OBJECT_ONE_OF:
        case OWLClassExpression::OBJECT_SOME_VALUES_FROM:
        case OWLClassExpression::OBJECT_UNION_OF:
            throw std::runtime_error("owlapi::io::RedlandWriter: no support for expression: "
                    + OWLClassExpression::TypeTxt[ super->getClassExpressionType() ] );

        case OWLClassExpression::OBJECT_EXACT_CARDINALITY:
        {
            raptor_term* minRestriction = writeRestriction(restriction, vocabulary::OWL::minQualifiedCardinality());
            raptor_term* maxRestriction = writeRestriction(restriction, vocabulary::OWL::maxQualifiedCardinality());

            restrictionTerms.push_back(minRestriction);
            restrictionTerms.push_back(maxRestriction);
            break;
        }
        case OWLClassExpression::OBJECT_MAX_CARDINALITY:
        {
            raptor_term* maxRestriction = writeRestriction(restriction, vocabulary::OWL::maxQualifiedCardinality());
            restrictionTerms.push_back(maxRestriction);
            break;
        }
        case OWLClassExpression::OBJECT_MIN_CARDINALITY:
        {
            raptor_term* minRestriction = writeRestriction(restriction, vocabulary::OWL::minQualifiedCardinality());
            restrictionTerms.push_back(minRestriction);
            break;
        }
        default:
            throw std::runtime_error("owlapi::io::RedlandWriter: no support for expression: "
                    + OWLClassExpression::TypeTxt[ super->getClassExpressionType() ] );
    }

    switch(sub->getClassExpressionType())
    {
        case OWLClassExpression::OWL_CLASS:
        {
            IRI subIRI = boost::dynamic_pointer_cast<OWLClass>(sub)->getIRI();
            std::vector<raptor_term*>::const_iterator cit = restrictionTerms.begin();
            for(; cit != restrictionTerms.end(); ++cit)
            {
                writeAnonymous(subIRI, vocabulary::RDFS::subClassOf(), *cit);
            }
            break;
        }
        default:
            throw std::runtime_error("owlapi::io::RedlandWriter: no support for complex subclass expression");
    }
}

void RedlandVisitor::visit(const OWLSubObjectPropertyOfAxiom& axiom)
{
    IRI subIRI = boost::dynamic_pointer_cast<OWLObjectProperty>(axiom.getSubProperty())->getIRI();
    IRI superIRI = boost::dynamic_pointer_cast<OWLObjectProperty>(axiom.getSuperProperty())->getIRI();

    writeTriple(subIRI, vocabulary::RDFS::subPropertyOf(), superIRI);
}

void RedlandVisitor::visit(const OWLSubDataPropertyOfAxiom& axiom)
{
    IRI subIRI = boost::dynamic_pointer_cast<OWLDataProperty>(axiom.getSubProperty())->getIRI();
    IRI superIRI = boost::dynamic_pointer_cast<OWLDataProperty>(axiom.getSuperProperty())->getIRI();

    writeTriple(subIRI, vocabulary::RDFS::subPropertyOf(), superIRI);
}


void RedlandVisitor::visit(const owlapi::model::OWLInverseObjectPropertiesAxiom& axiom)
{
    IRI first = boost::dynamic_pointer_cast<OWLObjectProperty>(axiom.getFirstProperty())->getIRI();
    IRI second = boost::dynamic_pointer_cast<OWLObjectProperty>(axiom.getSecondProperty())->getIRI();

    writeTriple(first, vocabulary::OWL::inverseOf(), second);
}

void RedlandVisitor::visit(const owlapi::model::OWLFunctionalObjectPropertyAxiom& axiom)
{
    IRI iri = boost::dynamic_pointer_cast<OWLObjectProperty>(axiom.getProperty())->getIRI();
    writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::FunctionalProperty());
}

void RedlandVisitor::visit(const owlapi::model::OWLFunctionalDataPropertyAxiom& axiom)
{
    IRI iri = boost::dynamic_pointer_cast<OWLDataProperty>(axiom.getProperty())->getIRI();
    writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::FunctionalProperty());
}


RedlandWriter::RedlandWriter()
    : OWLWriter()
    , mWorld(raptor_new_world())
    , mSerializer(NULL)
    , mFormat("rdfxml")
{
}

RedlandWriter::~RedlandWriter()
{
    raptor_free_world(mWorld);
}

std::vector<std::string> RedlandWriter::getSupportedFormats() const
{
    std::vector<std::string> formats;
    int i = 0;
    while(true)
    {
        const raptor_syntax_description* sd = raptor_world_get_serializer_description(mWorld, i++);
        if(sd == NULL)
        {
            break;
        }
        LOG_DEBUG_S << "format: " << sd->names[0] << " , description: " <<  sd->label;
        formats.push_back(std::string(sd->names[0]));
    }
    return formats;
}


void RedlandWriter::write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology) const
{
    const owlapi::model::OWLAxiom::PtrList& axioms = ontology->getAxioms();

    // rdfxml
    const char *serializer_syntax_name = mFormat.c_str();
    unsigned char* uri_string = raptor_uri_filename_to_uri_string(filename.c_str());
    raptor_uri* base_uri = raptor_new_uri(mWorld, uri_string);

    mSerializer = raptor_new_serializer(mWorld, serializer_syntax_name);

    raptor_serializer_start_to_filename(mSerializer, filename.c_str());

    RedlandVisitor visitor(mWorld, mSerializer);

    OWLAxiom::PtrList::const_iterator cit = axioms.begin();
    for(; cit != axioms.end(); ++cit)
    {
        const OWLAxiom::Ptr& axiom = *cit;
        axiom->accept(&visitor);
    }
    raptor_serializer_serialize_end(mSerializer);
    raptor_free_serializer(mSerializer);

    raptor_free_uri(base_uri);
    raptor_free_memory(uri_string);

    //raptor_serialize(statement_
//    TripleStore triples = TripleStore::fromOntology(ontology);
//
//    TripleStore::const_iterator cit = triples.begin();
//    for(; cit != triples.end(); ++cit)
//    {
//        // librdf_world* world = librdf_new_world();
//        // librdf_free_world(world);
//        //
//        // librdf_world_open(world);
//        //
//        // allowed values:
//        //
//        //  const raptor_syntax_description* sd;
//        // raptor_world_get_serializer_description(world, i)
//        // printf("    %-14s  %s", sd->names[0], sd->label);
//        //
//        // serializer = raptor_new_serializer(world, serializer_syntax_name);
//        //
//        //  const unsigned char *output_base_uri_string = NULL;
//        //
//        // -- raptor_parser_set_statement_handler(rdf_parser, rdf_parser,
//        // print_triples);
//        //
//        // void print_triples(void *user_data, raptor_statement *triple)
//        //     raptor_parser* rdf_parser = (raptor_parser*)user_data;
//        //     raptor_serializer_serialize_statement(serializer, triple);
//        //
//        //     raptor_serializer_set_namespace_from_namespace(rdf_serializer,
//        //     nspace);
//
//// librdf_node *       librdf_node_decode                  (librdf_world *world,
//                                                         //size_t *size_p,
//                                                         //unsigned char *buffer,
//                                                         //size_t length);
////  librdf_new_statement_from_nodes     (librdf_world *world,
//                                                         //librdf_node *subject,
//                                                         //librdf_node *predicate,
//                                                         //librdf_node *object);
//
// // librdf_storage_add_statement        (librdf_storage *storage,
//   //                                                      librdf_statement *statement);
//        //
//        //   !!!     librdf_serializer_serialize_model_to_file(
//        //    (librdf_serializer *serializer,
//        //                                                 const char *name,
//        //                                                 librdf_uri *base_uri,
//        //                                                 librdf_model *model);
//    }
}

} // end namespace io
} // end namespace owlapi

//  raptor_world *world = NULL;
//  raptor_serializer* rdf_serializer = NULL;
//  unsigned char *uri_string;
//  raptor_uri *base_uri;
//  raptor_statement* triple;
//
//  world = raptor_new_world();
//  
//  uri_string = raptor_uri_filename_to_uri_string(argv[1]);
//  base_uri = raptor_new_uri(world, uri_string);
//
//  rdf_serializer = raptor_new_serializer(world, "rdfxml-abbrev");
//  raptor_serializer_start_to_file_handle(rdf_serializer, base_uri, stdout);
//  
//   Make a triple with URI subject, URI predicate, literal object 
//  triple = raptor_new_statement(world);
//  triple->subject = raptor_new_term_from_uri_string(world, (const unsigned char*)"http://example.org/subject");
//  triple->predicate = raptor_new_term_from_uri_string(world, (const unsigned char*)"http://example.org/predicate");
//  triple->object = raptor_new_term_from_literal(world,
//                                                (const unsigned char*)"An example literal",
//                                                NULL,
//                                                (const unsigned char*)"en");
//
//  raptor_serializer_serialize_statement(rdf_serializer, triple);
//
//  raptor_free_statement(triple);
//
//  raptor_serializer_serialize_end(rdf_serializer);
//  raptor_free_serializer(rdf_serializer);
//  
//  raptor_free_uri(base_uri);
//  raptor_free_memory(uri_string);
//
//  raptor_free_world(world);
