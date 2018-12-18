#ifndef OWLAPI_DB_FILE_BACKEND_HPP
#define OWLAPI_DB_FILE_BACKEND_HPP

#include <owlapi/db/rdf/SparqlInterface.hpp>

namespace owlapi {
namespace db {

/**
 * A Sparql-based query interface for a file-based ontology
 */
class FileBackend : public query::SparqlInterface
{
public:
    FileBackend(const std::string& filename, const std::string& baseUri);

    virtual ~FileBackend() {}

    /**
     * Query the filebackend
     * \param query The query string, sparql
     * \param bindings The bindings for which results can be later retrieved
     * \return Results of the query
     */
    virtual query::Results query(const std::string& query, const query::Bindings& bindings) const { throw std::runtime_error("owlapi::FileBackend: query interface not implemented"); }

protected:
    std::string mFilename;
    std::string mBaseUri;
};

} // end namespace db
} // end namespace owlapi
#endif // OWLAPI_DB_FILE_BACKEND_HPP
