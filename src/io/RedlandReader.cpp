#include "RedlandReader.hpp"
#include "OWLOntologyIO.hpp"
#include <raptor2.h>
#include <cstdlib>

namespace owlapi {
namespace io {

unsigned char* blankNodeHandler(void *user_data, unsigned char* user_bnodeid)
{
  //raptor_world *world = (raptor_world*)user_data;
  int id;
  unsigned char *buffer;

  if(user_bnodeid)
    return user_bnodeid;

  id = RedlandReader::consumeBlankNodeId();
  std::stringstream ss;
  ss << "genid" << id;
  std::string blankNodeId = ss.str();

  buffer = static_cast<unsigned char*>( calloc(sizeof(unsigned char*), blankNodeId.size() + 1) );
  memcpy(buffer, blankNodeId.c_str(), blankNodeId.size());
  return buffer;
}

int RedlandReader::msBlankNodeId = 0;

ParsingFailed::ParsingFailed(const std::string& message)
    : std::runtime_error(message)
{}

RedlandReader::RedlandReader()
    : mpWorld(librdf_new_world())
    , mpStorage(0)
    , mpModel(0)
{
    librdf_world_open(mpWorld);

    // see http://librdf.org/docs/api/redland-storage-module-hashes.html
    mpStorage = librdf_new_storage(mpWorld, "hashes", NULL,
            "hash-type='memory'");
    mpModel = librdf_new_model(mpWorld, mpStorage, NULL );
}

RedlandReader::~RedlandReader()
{
    librdf_free_model(mpModel);
    librdf_free_storage(mpStorage);
    librdf_free_world(mpWorld);
}

std::vector<std::string> RedlandReader::getSupportedFormats() const
{
    std::vector<std::string> formats;
    int i = 0;
    while(true)
    {
        const raptor_syntax_description* sd = librdf_parser_get_description(mpWorld, i++);
        if(sd == NULL)
        {
            break;
        }
        formats.push_back(std::string(sd->names[0]));
    }
    return formats;
}

void RedlandReader::read(const std::string& filename) const
{
    Format guessedFormat = OWLOntologyIO::guessFormat(filename);
    try {
        read(filename, FormatTxt[ guessedFormat ]);
        return;
    } catch(const io::ParsingFailed& pf)
    {
        LOG_WARN_S << "Parsing '" << filename << " with guessed format '"
            << FormatTxt[ guessedFormat ] << "' failed";
    }

    // Fallback to trying all supported formats
    for(const std::string& format : getSupportedFormats())
    {
        try {
            read(filename, format);
            return;
        } catch(const io::ParsingFailed& pf)
        {
            // try all formats
            LOG_DEBUG_S << "Parsing '" << filename << " with " << format
                << "failed";
        }
    }
    throw io::ParsingFailed("owlapi::io::RedlandReader::read: could not read '"
            + filename + "' tried all formats");
}

void RedlandReader::read(const std::string& filename, const std::string& format) const
{
    unsigned char* uri_string = raptor_uri_filename_to_uri_string(filename.c_str());
    librdf_uri* base_uri = librdf_new_uri(mpWorld, uri_string);
    raptor_world* raptorWorld = librdf_world_get_raptor(mpWorld);
    raptor_world_set_generate_bnodeid_handler(raptorWorld, NULL, blankNodeHandler);

    std::string parserName = format;
    if(format.empty())
    {
        parserName = raptor_world_guess_parser_name(raptorWorld, NULL, NULL, NULL, 0,
                uri_string);
        LOG_INFO_S << "Guessed parser name: " << parserName;
    } else {
        LOG_DEBUG_S << "Using format: " << format;
    }

    librdf_parser* p = librdf_new_parser(mpWorld, parserName.c_str(), NULL, NULL);
    if(!p)
    {
        throw ParsingFailed("owlapi::io::RedlandReader::read: reading '"
                + filename + "' failed. Parser for format '" + mFormat + "'"
                " could not be created");
    }

    if(librdf_parser_parse_into_model(p, base_uri, NULL, mpModel) != 0)
    {
        throw ParsingFailed("owlapi::io::RedlandReader::read: reading '"
                + filename + "' failed for format: " + mFormat);
    }

    //raptor_iostream* iostr = raptor_new_iostream_to_file_handle(raptorWorld, stdout);
    //librdf_model_write(mpModel, iostr);
    //raptor_free_iostream(iostr);

    librdf_free_parser(p);
    librdf_free_uri(base_uri);
}

} // end namespace io
} // end namespace owlapi
