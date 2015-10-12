#ifndef OWLAPI_IO_OWL_WRITER_HPP
#define OWLAPI_IO_OWL_WRITER_HPP

#include <stdexcept>
#include <owlapi/model/OWLOntology.hpp>

namespace owlapi {
namespace io {

class OWLWriter
{
public:
    OWLWriter(const std::string& defaultFormat = "");
    virtual ~OWLWriter() {}

    /**
     * Retrieve the supported formats that this writer supports
     */
    virtual std::vector<std::string> getSupportedFormats() const { return std::vector<std::string>(); }

    /**
     * Set the format to use for serialization (if multiple formats are supported by this write)
     */
    virtual void setFormat(const std::string& format);

    /**
     * Write the ontology to a given file
     * \param filename Name of the file
     * \param ontology Ontology to serialize
     */
    virtual void write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology) const { throw std::runtime_error("owlapi::io::OWLWriter: write not implemented"); }

    /**
     * Check if the writer supports the given format
     * \return True if the writer supports the format, false otherwise
     */
    bool supportsFormat(const std::string& format) const;

protected:
    std::string mFormat;
};

} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_IO_OWL_WRITER_HPP
