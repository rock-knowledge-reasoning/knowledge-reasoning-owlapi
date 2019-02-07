#ifndef OWLAPI_IO_OWL_IO_BASE_HPP
#define OWLAPI_IO_OWL_IO_BASE_HPP

#include <stdexcept>
#include <owlapi/model/OWLOntology.hpp>

namespace owlapi {
namespace io {

class OWLIOBase
{
public:
    OWLIOBase(const std::string& defaultFormat = "");
    virtual ~OWLIOBase() {}

    /**
     * Retrieve the supported formats that this writer supports
     */
    virtual std::vector<std::string> getSupportedFormats() const { return std::vector<std::string>(); }

    /**
     * Set the format to use for serialization (if multiple formats are supported by this write)
     */
    virtual void setFormat(const std::string& format);

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
#endif // OWLAPI_IO_OWL_IO_BASE_HPP
