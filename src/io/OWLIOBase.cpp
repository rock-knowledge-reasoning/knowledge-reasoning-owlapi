#include "OWLIOBase.hpp"
#include <algorithm>

namespace owlapi {
namespace io {

OWLIOBase::OWLIOBase(const std::string& defaultFormat)
    : mFormat(defaultFormat)
{}

void OWLIOBase::setFormat(const std::string& format)
{
    if(!supportsFormat(format))
    {
        throw std::invalid_argument("owlapi::io::OWLIOBase::setFormat: format '"
                + format + "' is not supported");
    }
    mFormat = format;
}

bool OWLIOBase::supportsFormat(const std::string& format) const
{
    std::vector<std::string> supportedFormats = getSupportedFormats();
    return supportedFormats.end() != std::find(supportedFormats.begin(), supportedFormats.end(), format);
}

} // end namespace io
} // end namespace owlapi
