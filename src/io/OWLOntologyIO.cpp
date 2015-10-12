#include "OWLOntologyIO.hpp"
#include "RedlandWriter.hpp"
#include "OWLOntologyReader.hpp"

namespace owlapi {
namespace io {

void OWLOntologyIO::write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology, Format format)
{
    switch(format)
    {
        case RDFXML:
        {
            RedlandWriter redlandWriter;
            redlandWriter.write(filename, ontology);
            break;
        }
        case UNKNOWN:
        case END_FORMAT:
        default:
            throw std::invalid_argument("owlapi::io::OWLWriter::write: unsupported format");

    }
}

owlapi::model::OWLOntology::Ptr OWLOntologyIO::fromFile(const std::string& filename)
{
    OWLOntologyReader reader;
    return reader.fromFile(filename);
}

} // end namespace io
} // end namespace owlapi
