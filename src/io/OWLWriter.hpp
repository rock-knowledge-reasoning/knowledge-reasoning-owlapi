#ifndef OWLAPI_IO_OWL_WRITER_HPP
#define OWLAPI_IO_OWL_WRITER_HPP

#include <stdexcept>
#include <owlapi/model/OWLOntology.hpp>

namespace owlapi {
namespace io {

class OWLWriter
{
public:
    virtual ~OWLWriter() {}

    virtual void write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology) const { throw std::runtime_error("owlapi::io::OWLWriter: write not implemented"); }
};

} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_IO_OWL_WRITER_HPP
