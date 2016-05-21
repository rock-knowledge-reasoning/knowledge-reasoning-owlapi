#include "OWLOntologyIO.hpp"
#include "RedlandWriter.hpp"
#include "OWLOntologyReader.hpp"
#include <utilmm/configfile/pkgconfig.hh>
#include <algorithm>
#include <boost/filesystem.hpp>

#include <owlapi/Vocabulary.hpp>

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

owlapi::model::OWLOntology::Ptr OWLOntologyIO::load(const owlapi::model::IRI& ontologyIRI)
{
    using namespace owlapi::model;
    OWLOntology::Ptr ontology = make_shared<OWLOntology>();
    load(ontology, ontologyIRI);
    return ontology;
}

owlapi::model::OWLOntology::Ptr OWLOntologyIO::load(owlapi::model::OWLOntology::Ptr& ontology, const owlapi::model::IRI& ontologyIRI)
{
    using namespace owlapi::model;

    OWLOntologyReader reader;
    if(ontology->getAbsolutePath().empty())
    {
        if(ontology->getIRI().empty())
        {
            // no file and no iri provided -- well seem to be an
            // empty top level ontology document
            // no need to load anything, but we have to assign a identifying IRI
            if(ontologyIRI.empty())
            {
                throw std::invalid_argument("owlapi::model::OWLOntologyIO::load "
                        " unspecified ontology given, please provide an IRI as second argument");
            } else {
                ontology->setIRI(ontologyIRI);
            }
        }

        // load from IRI
        try {
            std::string path = retrieve(ontology->getIRI());
            ontology = reader.open(path);
            reader.loadDeclarationsAndImports(ontology, true /*directImport*/);
        } catch(const std::exception& e)
        {
            LOG_INFO_S << "Empty ontology document";
        }
    } else {
        // load from given file
        ontology = reader.open(ontology->getAbsolutePath());
        reader.loadDeclarationsAndImports(ontology, true /*directImport*/);
    }

    IRIList unprocessed = ontology->getDirectImportsDocuments();
    IRIList processed;
    std::vector<OWLOntology::Ptr> processedOntology;

    typedef std::map<OWLOntology::Ptr, OWLOntologyReader*> ReadersMap;
    ReadersMap readersMap;

    // load declarations and imports
    while(!unprocessed.empty())
    {
        IRI iri = unprocessed.back();
        unprocessed.pop_back();

        // Check on inbuilt
        std::string iriString = iri.toString();
        if(strncmp(iriString.c_str(), vocabulary::OWL::IRIPrefix().toString().c_str(), iriString.size()) == 0)
        {
            LOG_INFO_S << "Skipping import of builtin vocabulary: " << iri;
            continue;
        } else if(strncmp(iriString.c_str(), vocabulary::RDF::IRIPrefix().toString().c_str(), iriString.size()) == 0)
        {
            LOG_INFO_S << "Skipping import of builtin vocabulary: " << iri;
            continue;
        } else if(strncmp(iriString.c_str(), vocabulary::RDFS::IRIPrefix().toString().c_str(), iriString.size()) == 0)
        {
            LOG_INFO_S << "Skipping import of builtin vocabulary: " << iri;
            continue;
        }

        if(processed.end() != std::find(processed.begin(), processed.end(), iri))
        {
            // has already been processed, but to satisfy earlier dependency
            // requests append this iri if requested
            processed.push_back(iri);
            continue;
        }

        LOG_DEBUG_S << "Processing: " << iri;

        std::string filename = retrieve(iri);

        OWLOntologyReader* importReader = new OWLOntologyReader();
        OWLOntology::Ptr importedOntology = importReader->open(filename);
        importedOntology->setIRI(iri);

        // load the individual ontology to identity direct imports
        importReader->loadDeclarationsAndImports(importedOntology, true /*directImport*/);
        IRIList directImports = importedOntology->getDirectImportsDocuments();
        unprocessed.insert(unprocessed.begin(), directImports.begin(), directImports.end());
        unprocessed.push_back(iri);

        readersMap[importedOntology] = importReader;

        // register IRI and ontology
        processed.push_back(iri);
    }

    // Process the imported ontologies in reverse order of import to
    // make sure all types are properly defined
    IRIList loaded;
    if(!processed.empty())
    {
        IRIList::iterator it = processed.end()-1;
        for(; it != processed.begin(); --it)
        {
            IRI iri = *it;
            if(loaded.end() != std::find(loaded.begin(), loaded.end(), iri))
            {
                // skip already loaded ontologies
                continue;
            }

            // Find reader by given iri
            ReadersMap::iterator rit = readersMap.begin();
            for(; rit != readersMap.end(); ++rit)
            {
                if(rit->first->getIRI() == iri)
                {
                    break;
                }
            }
            OWLOntologyReader* importReader = rit->second;

            LOG_INFO_S << "Importing declarations from '" << importReader->getAbsolutePath()
                << "' into ontology " << ontology->getIRI();

            // Load the full ontology
            importReader->loadDeclarationsAndImports(ontology, false);
            importReader->loadAxioms(ontology);
            delete importReader;

            loaded.push_back(iri);
        }
    }

    // If the top level is loaded from file make sure the
    // axioms are loaded after all imports have been processed
    if(!ontology->getAbsolutePath().empty())
    {
        reader.loadAxioms(ontology);
    }

    LOG_INFO_S << "Processed all imports: " << processed;
    return ontology;
}

owlapi::model::OWLOntology::Ptr OWLOntologyIO::fromFile(const std::string& filename)
{
    OWLOntologyReader reader;
    owlapi::model::OWLOntology::Ptr ontology = reader.fromFile(filename);
    std::string absolutePath = boost::filesystem::canonical(filename).string();
    ontology->setAbsolutePath(absolutePath);

    return load(ontology);
}

std::string OWLOntologyIO::getOntologyPath()
{
    try {
        utilmm::pkgconfig pkg("owlapi");
        return pkg.get("sharedir") + "/ontologies/";
    } catch(...)
    {
        LOG_WARN_S << "could not find installation of: 'owlapi', thus could not infer installation path for ontologies";
        throw std::runtime_error("owlapi::io::OWLOntologyReader: owlapi is not installed - failed to find ontologies directory");
    }
}

std::string OWLOntologyIO::canonizeForOfflineUsage(const owlapi::model::IRI& iri)
{
    if(iri.empty())
        return "";

    std::string iriString = iri.toString();
    std::replace(iriString.begin(),iriString.end(), '/','_');
    std::replace(iriString.begin(),iriString.end(), ':','_');
    std::replace(iriString.begin(),iriString.end(), '.','_');

    // remove trailing '#'
    iriString.erase(std::remove(iriString.end()-1,iriString.end(),'#'), iriString.end());

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
        LOG_DEBUG_S << "Trying to retrieve document with command '" << cmd << "'";
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
