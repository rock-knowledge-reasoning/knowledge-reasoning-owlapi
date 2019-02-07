#include "Redland.hpp"
#include "../../io/RedlandReader.hpp"
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>

namespace owlapi {
namespace db {

Redland::Redland(const std::string& filename,
        const std::string& baseUri)
    : FileBackend(filename, baseUri)
    , mpReader(new io::RedlandReader())
{
    mpReader->read(filename);
}

Redland::~Redland()
{
    delete mpReader;
}

std::vector<std::string> Redland::getSupportedQueryFormats() const
{
    std::vector<std::string> formats;
    int i = 0;
    while(true)
    {
        const raptor_syntax_description* sd = librdf_query_language_get_description(mpReader->getWorld(), i++);
        if(sd == NULL)
        {
            break;
        }
        formats.push_back(std::string(sd->names[0]));
    }
    return formats;
}

query::Results Redland::query(const std::string& query, const query::Bindings& bindings) const
{
    query::Results results;

    const unsigned char* query_string = (const unsigned char*) query.c_str();
    librdf_query* rdfQuery = librdf_new_query(mpReader->getWorld(), "sparql", NULL, query_string,
            NULL);


    librdf_query_results* rdfResults = librdf_model_query_execute(mpReader->getModel(), rdfQuery);
    while( !librdf_query_results_finished(rdfResults))
    {
        const char** names = NULL;
        librdf_node* values[10];
        if(librdf_query_results_get_bindings(rdfResults, &names, values))
        {
            break;
        }
        if(names)
        {
            query::Row row;
            for(int i = 0; names[i]; ++i)
            {
                query::Variable variable(names[i], false);
                librdf_node* node = values[i];
                if(node == NULL)
                {
                    continue;
                }
                switch( librdf_node_get_type(node) )
                {
                    case LIBRDF_NODE_TYPE_UNKNOWN:
                        break;
                    case LIBRDF_NODE_TYPE_RESOURCE:
                    {
                        //LOG_WARN_S << "RESOURCE";
                        librdf_uri* uri = librdf_node_get_uri(node);
                        unsigned char* uriStr = librdf_uri_to_string(uri);

                        owlapi::model::IRI valueIri((const char*) uriStr);
                        row[variable] = valueIri;

                        librdf_free_memory(uriStr);
                        break;
                    }
                    case LIBRDF_NODE_TYPE_LITERAL:
                    {
                        //LOG_WARN_S << "LITERAL";
                        unsigned char* valueStr = librdf_node_get_literal_value(node);
                        if(valueStr == NULL || strlen((const char*) valueStr) == 0)
                        {
                            row[variable] = owlapi::model::IRI();
                            continue;
                        }
                        std::string literalValue((const char*) valueStr);
                        librdf_uri* uri = librdf_node_get_literal_value_datatype_uri(node);
                        if(uri)
                        {
                            unsigned char* uriStr = librdf_uri_to_string(uri);
                            literalValue.append("^^");
                            literalValue.append((const char*) uriStr);
                            librdf_free_memory(uriStr);
                        }

                        owlapi::model::IRI valueIri(literalValue);
                        row[variable] = valueIri;

                        break;
                    }
                    case LIBRDF_NODE_TYPE_BLANK:
                    {
                        //LOG_WARN_S << "BLANK";
                        unsigned char* valueStr = librdf_node_get_blank_identifier(node);
                        owlapi::model::IRI valueIri((const char*) valueStr);
                        row[variable] = valueIri;
                        break;
                    }
                    default:
                        break;
                }

                //unsigned char* valueStr = NULL;
                //librdf_node* node = values[i];
                //raptor_iostream* iostr = raptor_new_iostream_to_string(node->world,
                //        (void**) &valueStr,
                //        NULL,
                //        malloc);

                //if(iostr)
                //{
                //    int rc = librdf_node_write(values[i],
                //            iostr);
                //    raptor_free_iostream(iostr);

                //    if(!rc)
                //    {
                //        owlapi::model::IRI value((const char*) valueStr);
                //        row[variable] = value;
                //    }
                //    raptor_free_memory(valueStr);
                //    valueStr = NULL;
                //}
                librdf_free_node(values[i]);
            }
            results.rows.push_back(row);
        }

        librdf_query_results_next(rdfResults);
    }

    librdf_free_query_results(rdfResults);
    librdf_free_query(rdfQuery);
    return results;
}

} // end namespace db
} // end namespace owlapi
