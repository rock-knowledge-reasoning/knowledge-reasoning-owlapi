#ifndef OWLAPI_IO_OWL_WRITER_HPP
#define OWLAPI_IO_OWL_WRITER_HPP

#include <stdexcept>
#include "OWLIOBase.hpp"

namespace owlapi {
namespace io {

class OWLWriter : public OWLIOBase
{
public:
    OWLWriter(const std::string& defaultFormat = "");
    virtual ~OWLWriter();

    /**
     * Write the ontology to a given file
     * \param filename Name of the file
     * \param ontology Ontology to serialize
     * \param includeImports If set to true, write also the imports to the same file -- by default set to false
     */
    virtual void write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology, bool includeImports = false) const { throw std::runtime_error("owlapi::io::OWLWriter: write not implemented"); }
};

} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_IO_OWL_WRITER_HPP
