#include "OWLReader.hpp"
#include <algorithm>

namespace owlapi {
namespace io {

OWLReader::OWLReader(const std::string& defaultFormat)
    : OWLIOBase(defaultFormat)
{}

OWLReader::~OWLReader()
{}

} // end namespace io
} // end namespace owlapi
