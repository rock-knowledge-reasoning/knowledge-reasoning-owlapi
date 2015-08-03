#include "RedlandWriter.hpp"
#include <base/Logging.hpp>
#include <raptor2/raptor2.h>
#include <owlapi/OWLApi.hpp>

using namespace owlapi::model;

namespace owlapi {
namespace io {

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

void RedlandWriter::writeTriple(const owlapi::model::IRI& subject,
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

void RedlandWriter::write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology) const
{
    const owlapi::model::OWLAxiom::PtrList& axioms = ontology->getAxioms();

    // rdfxml
    const char *serializer_syntax_name = mFormat.c_str();
    unsigned char* uri_string = raptor_uri_filename_to_uri_string(filename.c_str());
    raptor_uri* base_uri = raptor_new_uri(mWorld, uri_string);

    mSerializer = raptor_new_serializer(mWorld, serializer_syntax_name);

    raptor_serializer_start_to_filename(mSerializer, filename.c_str());

    OWLAxiom::PtrList::const_iterator cit = axioms.begin();
    for(; cit != axioms.end(); ++cit)
    {
        const OWLAxiom::Ptr& axiom = *cit;

        IRI subject, predicate, object;
        switch(axiom->getAxiomType())
        {
            case OWLAxiom::Declaration:
            {
                OWLEntity::Ptr entity = boost::dynamic_pointer_cast<OWLDeclarationAxiom>(axiom)->getEntity();
                IRI iri = entity->getIRI();
                switch(entity->getEntityType())
                {
                    case OWLEntity::Class:
                    {
                        if(iri == vocabulary::OWL::Class() || iri == vocabulary::OWL::Thing())
                        {
                            break;
                        }
                        writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::Class());
                        break;
                    }
                    case OWLEntity::ObjectProperty:
                        writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::ObjectProperty());
                        break;
                    case OWLEntity::DataProperty:
                        writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::DatatypeProperty());
                        break;
                    case OWLEntity::AnnotationProperty:
                        writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::AnnotationProperty());
                        break;
                    case OWLEntity::Datatype:
                        writeTriple(iri, vocabulary::RDF::type(), vocabulary::RDFS::Datatype());
                        break;
                    case OWLEntity::NamedIndividual:
                        writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::NamedIndividual());
                        break;
                    default:
                        break;
                }
                break;
            }
            case OWLAxiom::ClassAxiom:
                break;
            case OWLAxiom::ObjectPropertyAxiom:
                
                break;
            case OWLAxiom::DataPropertyAxiom:
                break;
            case OWLAxiom::DatatypeDefinition:
                break;
            case OWLAxiom::ClassAssertion:
                break;
            case OWLAxiom::SubClassOf:
            {
                OWLSubClassOfAxiom::Ptr e_axiom = boost::dynamic_pointer_cast<OWLSubClassOfAxiom>(axiom);
                
                OWLClassExpression::Ptr sub = e_axiom->getSubClass();
                OWLClassExpression::Ptr super = e_axiom->getSuperClass();

                if(sub->getClassExpressionType() == OWLClassExpression::OWL_CLASS
                    && super->getClassExpressionType() == OWLClassExpression::OWL_CLASS)
                {
                    IRI subIRI = boost::dynamic_pointer_cast<OWLClass>(sub)->getIRI();
                    IRI superIRI = boost::dynamic_pointer_cast<OWLClass>(super)->getIRI();
                    if(superIRI != vocabulary::OWL::Thing())
                    {
                        writeTriple(subIRI, vocabulary::RDFS::subClassOf(), superIRI);
                    }
                }
                break;
            }
            case OWLAxiom::SubObjectPropertyOf:
            {
                OWLSubObjectPropertyOfAxiom::Ptr e_axiom = boost::dynamic_pointer_cast<OWLSubObjectPropertyOfAxiom>(axiom);
                
                IRI subIRI = boost::dynamic_pointer_cast<OWLObjectProperty>(e_axiom->getSubProperty())->getIRI();
                IRI superIRI = boost::dynamic_pointer_cast<OWLObjectProperty>(e_axiom->getSuperProperty())->getIRI();

                writeTriple(subIRI, vocabulary::RDFS::subPropertyOf(), superIRI);
                break;
            }
            case OWLAxiom::SubDataPropertyOf:
            {
                OWLSubDataPropertyOfAxiom::Ptr e_axiom = boost::dynamic_pointer_cast<OWLSubDataPropertyOfAxiom>(axiom);
                
                IRI subIRI = boost::dynamic_pointer_cast<OWLDataProperty>(e_axiom->getSubProperty())->getIRI();
                IRI superIRI = boost::dynamic_pointer_cast<OWLDataProperty>(e_axiom->getSuperProperty())->getIRI();

                writeTriple(subIRI, vocabulary::RDFS::subPropertyOf(), superIRI);
                break;
            }
            default:
                break;
        }

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
