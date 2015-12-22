#include "OWLOntologyIO.hpp"
#include "RedlandWriter.hpp"
#include "OWLOntologyReader.hpp"
#include <utilmm/configfile/pkgconfig.hh>
#include <algorithm>
#include <boost/filesystem.hpp>

namespace owlapi {
namespace io {

void OWLOntologyIO::write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology, Format format)
{
    switch(format)
    {
        case RDFXML:
        {
            RedlandWriter redlandWriter;
            redlandWriter.write(filename, ontology);
            break;
        }
        case UNKNOWN:
        case END_FORMAT:
        default:
            throw std::invalid_argument("owlapi::io::OWLWriter::write: unsupported format");

    }
}

owlapi::model::OWLOntology::Ptr OWLOntologyIO::fromFile(const std::string& filename)
{
    OWLOntologyReader reader;
    owlapi::model::OWLOntology::Ptr ontology = reader.fromFile(filename);
    std::string absolutePath = boost::filesystem::canonical(filename).string();
    ontology->setAbsolutePath(absolutePath);
    return ontology;
}

std::string OWLOntologyIO::getOntologyPath()
{
    try {
        utilmm::pkgconfig pkg("owlapi");
        return pkg.get("sharedir") + "/ontologies/";
    } catch(...)
    {
        throw std::runtime_error("owlapi::io::OWLOntologyReader: owlapi is not installed - failed to find ontologies directory");
    }
}

std::string OWLOntologyIO::canonizeForOfflineUsage(const owlapi::model::IRI& iri)
{
    std::string iriString = iri.toString();
    std::replace(iriString.begin(),iriString.end(), '/','_');
    std::replace(iriString.begin(),iriString.end(), ':','_');
    std::replace(iriString.begin(),iriString.end(), '.','_');
    return iriString;
}

std::string OWLOntologyIO::retrieve(const owlapi::model::IRI& iri)
{
    // First check the local file system
    std::string ontologyPath = getOntologyPath();
    if( !boost::filesystem::exists(ontologyPath) )
    {
        boost::filesystem::create_directories(ontologyPath);
    }

    std::string absoluteFilename = ontologyPath + canonizeForOfflineUsage(iri) + getOWLSuffix();
    if( !boost::filesystem::exists( absoluteFilename ) )
    {
        std::string cmd = "wget " + iri.toString() + " -O " + absoluteFilename + " -q";
        if(-1 == system(cmd.c_str()) )
        {
            throw std::runtime_error("owlapi::io::OWLOntologyIO::retrieve: failed to retrieve document using the following command: '" + cmd + "'");
        }
    }

    // document is (now) available
    return absoluteFilename;
}


} // end namespace io
} // end namespace owlapi
