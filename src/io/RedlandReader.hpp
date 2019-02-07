#ifndef OWLAPI_IO_REDLAND_READER_HPP
#define OWLAPI_IO_REDLAND_READER_HPP

#include <stdexcept>
#include "OWLReader.hpp"
#include <redland.h>

namespace owlapi {
namespace io {

class ParsingFailed : std::runtime_error
{
public:
    ParsingFailed(const std::string& message = "");
};

class RedlandReader : public OWLReader
{
public:
    RedlandReader();

    virtual ~RedlandReader();

    std::vector<std::string> getSupportedFormats() const;

    std::vector<std::string> getSupportedQueryFormats() const;

    /**
     * \throw ParsingFailed when filename cannot be parsed
     */
    void read(const std::string& filename) const;
    void read(const std::string& filename, const std::string& format) const;

    librdf_world* getWorld() { return mpWorld; }
    librdf_model* getModel() { return mpModel; }

private:
    librdf_world* mpWorld;
    librdf_storage *mpStorage;
    librdf_model* mpModel;
};

} // end namespace io
} // end namespace owlapi

#endif // OWLAPI_IO_REDLAND_READER_HPP
