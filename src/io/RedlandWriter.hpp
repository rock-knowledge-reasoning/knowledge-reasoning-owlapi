#ifndef OWLAPI_IO_REDLAND_WRITER_HPP
#define OWLAPI_IO_REDLAND_WRITER_HPP

#include <string>
#include <vector>
#include <owlapi/io/OWLWriter.hpp>

struct raptor_world_s;
struct raptor_serializer_s;

namespace owlapi {
namespace io {

class RedlandWriter : public OWLWriter
{
public:
    RedlandWriter();
    virtual ~RedlandWriter();

    std::vector<std::string> getSupportedFormats() const;

    virtual void write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology) const; 

    void setFormat(const std::string& format) { mFormat = format; }

private:
    void writeTriple(const owlapi::model::IRI& subject,
        const owlapi::model::IRI& predicate,
        const owlapi::model::IRI& object) const;

    raptor_world_s* mWorld;
    mutable raptor_serializer_s* mSerializer;
    std::string mFormat;
};

} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_IO_RDFXML_WRITER_HPP
