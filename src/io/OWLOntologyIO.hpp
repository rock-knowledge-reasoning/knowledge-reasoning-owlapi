#ifndef OWLAPI_IO_OWL_ONTOLOGY_IO_HPP
#define OWLAPI_IO_OWL_ONTOLOGY_IO_HPP

#include <string>
#include <owlapi/model/OWLOntology.hpp>

namespace owlapi {
namespace io {

enum Format { UNKNOWN = 0,
        RDFXML,
        END_FORMAT
        };

class OWLOntologyIO
{
public:
    static void write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology, Format format = RDFXML);

    static owlapi::model::OWLOntology::Ptr fromFile(const std::string& filename);

};


} // end namespace io
} // end namespace owlapi
#endif // end OWLAPI_IO_OWL_ONTOLOGY_IO_HPP
