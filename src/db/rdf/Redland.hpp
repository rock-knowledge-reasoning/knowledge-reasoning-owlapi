#ifndef OWLAPI_DB_REDLAND_HPP
#define OWLAPI_DB_REDLAND_HPP

#include "../FileBackend.hpp"

namespace owlapi {
namespace io {
class RedlandReader;
}

namespace db {

class Redland : public FileBackend
{
public:
    Redland(const std::string& filename,
            const std::string& baseUri = "");

    virtual ~Redland();

    /**
     * Send a sparql query to the underlying database using the given bindings
     * \param query The SPARQL conform query
     * \param bindings The set of bindings used
     */
    query::Results query(const std::string& query, const query::Bindings& bindings) const;

    std::vector<std::string> getSupportedQueryFormats() const;

private:
    io::RedlandReader* mpReader;
};

} // end namespace db
} // end namespace owlapi
#endif // OWLAPI_DB_REDLAND_HPP
