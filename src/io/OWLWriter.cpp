#include "OWLWriter.hpp"

namespace owlapi {
namespace io {

OWLWriter::OWLWriter(const std::string& defaultFormat)
    : mFormat(defaultFormat)
{}

void OWLWriter::setFormat(const std::string& format)
{
    mFormat = format;
}

bool OWLWriter::supportsFormat(const std::string& format) const
{
    std::vector<std::string> supportedFormats = getSupportedFormats();
    return supportedFormats.end() != std::find(supportedFormats.begin(), supportedFormats.end(), format);
}


} // end namespace io
} // end namespace owlapi
