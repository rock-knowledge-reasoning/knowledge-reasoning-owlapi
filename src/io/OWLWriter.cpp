#include "OWLWriter.hpp"
#include <algorithm>

namespace owlapi {
namespace io {

OWLWriter::OWLWriter(const std::string& defaultFormat)
    : mFormat(defaultFormat)
{}

void OWLWriter::setFormat(const std::string& format)
{
    if(!supportsFormat(format))
    {
        throw std::invalid_argument("owlapi::io::OWLWriter::setFormat: format '"
                + format + "' is not supported");
    }
    mFormat = format;
}

bool OWLWriter::supportsFormat(const std::string& format) const
{
    std::vector<std::string> supportedFormats = getSupportedFormats();
    return supportedFormats.end() != std::find(supportedFormats.begin(), supportedFormats.end(), format);
}


} // end namespace io
} // end namespace owlapi
