#ifndef OWLAPI_IO_OWL_READER_HPP
#define OWLAPI_IO_OWL_READER_HPP

#include <stdexcept>
#include "OWLIOBase.hpp"

namespace owlapi {
namespace io {

class OWLReader : public OWLIOBase
{
public:
    OWLReader(const std::string& defaultFormat = "");
    virtual ~OWLReader();

    /**
     * Write the ontology to a given file
     * \param filename Name of the file
     */
    virtual void read(const std::string& filename) const { throw std::runtime_error("owlapi::io::OWLReader: write not implemented"); }
};

} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_IO_OWL_READER_HPP
