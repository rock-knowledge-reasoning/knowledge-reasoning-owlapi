#include "OWLOntologyIO.hpp"
#include "RedlandWriter.hpp"
#include "OWLOntologyReader.hpp"
#include <utilmm/configfile/pkgconfig.hh>
#include <algorithm>
#include <fstream>
#include <boost/filesystem.hpp>

#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace io {

std::map<Format, std::string> FormatTxt =
{
    { RDFXML, "rdfxml" },
    { TURTLE, "turtle" },
    { NTRIPLES, "ntriples" },
    { TRIG, "trig" },
    { JSON, "json" },
    { NQUADS, "nquads" }
};

std::map<Format, std::string> FormatSuffixes =
{
    { RDFXML, ".owl" },
    { TURTLE, ".ttl" },
    { NTRIPLES, ".nt" },
    { TRIG, ".trig" },
    { JSON, ".json" },
    { NQUADS, ".nquads" }
};

std::string OWLOntologyIO::msDownloadDir;

void OWLOntologyIO::write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology, Format format)
{
    switch(format)
    {
        case RDFXML:
        case TURTLE:
        case NTRIPLES:
        case TRIG:
        case JSON:
        case NQUADS:
        {
            RedlandWriter redlandWriter;
            redlandWriter.setFormat( FormatTxt[format] );
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
    load(ontology, ontologyIRI, false);
    return ontology;
}

owlapi::model::OWLOntology::Ptr OWLOntologyIO::loadNew(owlapi::model::OWLOntology::Ptr& ontology,
       const owlapi::model::IRI& ontologyIRI)
{
    return load(ontology, ontologyIRI, true);
}

owlapi::model::OWLOntology::Ptr OWLOntologyIO::load(owlapi::model::OWLOntology::Ptr& ontology,
        const owlapi::model::IRI& ontologyIRI,
        bool isEmptyTopOntology
        )
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

        if(!isEmptyTopOntology)
        {
            std::string path = retrieve(ontology->getIRI());
            ontology->setAbsolutePath(path);
        }
    }

    if(ontology->getIRI().empty() && !ontologyIRI.empty())
    {
        ontology->setIRI(ontologyIRI);
    }

    if(!isEmptyTopOntology)
    {
        // load from the file set for the ontology
        ontology = reader.open(ontology->getAbsolutePath());
        reader.loadImports(ontology, true /*directImport*/);
    }

    IRIList unprocessed = ontology->getDirectImportsDocuments();
    IRIList processed;
    std::vector<OWLOntology::Ptr> processedOntology;

    typedef std::map<OWLOntology::Ptr, OWLOntologyReader*> ReadersMap;
    ReadersMap readersMap;
    std::map<IRI, IRISet> dependencies;

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
        importReader->loadImports(importedOntology, true /*directImport*/);
        iri = importedOntology->getIRI();

        IRIList directImports = importedOntology->getDirectImportsDocuments();
        dependencies[iri] = IRISet(directImports.begin(), directImports.end());

        unprocessed.insert(unprocessed.begin(), directImports.begin(), directImports.end());
        unprocessed.push_back(iri);

        readersMap[importedOntology] = importReader;
        // register IRI and ontology
        processed.push_back(iri);
    }

    // Process the imported ontologies in dependency order of import to
    // make sure all types are properly defined
    IRIList loaded;
    while(!dependencies.empty())
    {
        // Find ontology that has no remaining dependencies
        std::map<IRI, IRISet>::iterator it = std::find_if(dependencies.begin(), dependencies.end(),
                [](const std::pair<IRI,IRISet>& v)
                {
                    return v.second.empty();
                });

        // Find reader by given iri
        IRI iri = it->first;
        ReadersMap::iterator rit = std::find_if(readersMap.begin(), readersMap.end(),
                [iri](const ReadersMap::value_type& v)
                {
                    return v.first->getIRI() == iri;
                });
        if(rit == readersMap.end())
        {
            throw std::runtime_error("owlapi::io::OWLOntologyIO::load: failed to find reader for '" + iri.toString() + "'");
        }

        OWLOntologyReader* importReader = rit->second;

        LOG_INFO_S << "Importing declarations from '" << importReader->getAbsolutePath()
            << "' into ontology " << ontology->getIRI();

        // Load the full ontology
        importReader->loadDeclarations(ontology, false);
        importReader->loadAxioms(ontology);
        delete importReader;

        // Remove iri from list of dependencies
        dependencies.erase(it);
        std::map<IRI, IRISet>::iterator dit = dependencies.begin();
        for(; dit != dependencies.end(); ++dit)
        {
            IRISet& iriSet = dit->second;
            IRISet::iterator sit = iriSet.find(iri);
            if(sit != iriSet.end())
            {
                iriSet.erase(sit);
            }
        }

        loaded.push_back(iri);
    }

    if(!isEmptyTopOntology)
    {
        reader.loadDeclarations(ontology, true /*directImport*/);
    }
    // If the top level is loaded from file make sure the
    // axioms are loaded after all imports have been processed
    if(!ontology->getAbsolutePath().empty())
    {
        reader.loadAxioms(ontology);
    }


    LOG_INFO_S << "Processed all imports: " << loaded;
    return ontology;
}

owlapi::model::OWLOntology::Ptr OWLOntologyIO::fromFile(const std::string& filename)
{
    OWLOntologyReader reader;
    owlapi::model::OWLOntology::Ptr ontology = reader.open(filename);
    std::string absolutePath = boost::filesystem::canonical(filename).string();
    ontology->setAbsolutePath(absolutePath);

    return load(ontology,"",false);
}

std::string OWLOntologyIO::getOntologiesDownloadDir()
{
    if(msDownloadDir.empty())
    {
        boost::filesystem::path path = boost::filesystem::temp_directory_path()
            / "owlapi" / "ontologies";
        if(!boost::filesystem::exists(path))
        {
            boost::filesystem::create_directories(path);
        }
        msDownloadDir = path.string();
    }
    return msDownloadDir;
}

std::set<std::string> OWLOntologyIO::getOntologyPaths()
{
    char* ontologiesPath_cstr = getenv("OWL_ONTOLOGIES_PATH");
    std::set<std::string> paths = { getOntologiesDownloadDir() };

    if(ontologiesPath_cstr)
    {
        std::string ontologiesPath(ontologiesPath_cstr);
        size_t startPos = 0;
        size_t endPos = std::string::npos;
        bool stop = false;
        while(!stop)
        {
            endPos = ontologiesPath.find_first_of(':',startPos);
            std::string path;
            if(endPos == std::string::npos)
            {
                path = ontologiesPath.substr(startPos);
                stop = true;
                if(path.empty())
                {
                    break;
                }
            } else {
                path = ontologiesPath.substr(startPos,endPos-startPos);
            }
            startPos = endPos + 1;

            if( boost::filesystem::exists(path) )
            {
                    paths.insert(path);
            } else {
                LOG_WARN_S << "Path set in OWL_ONTOLOGIES_PATH does not "
                    << "exist: " << path;
            }
        }
    }
    paths.insert(getOntologyPath());
    return paths;
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
    // First check the local file systems
    for(const std::string& path : getOntologyPaths())
    {
        try {
            return retrieve(iri, path);
        } catch(const OWLOntologyNotFound& e)
        {
            LOG_DEBUG_S << "Ontology '" << iri << "' not found in directory '"
                << path << "' -- " << e.what();
        }
    }

    try {
        return download(iri, msDownloadDir);
    } catch(const OWLOntologyNotFound& e)
    {
        LOG_DEBUG_S << "Ontology '" << iri << "' could not be downloaded"
            " into directory '" << msDownloadDir << "' -- " << e.what();
    }

    throw OWLOntologyNotFound("owlapi::io::OWLOntologyIO::retrieve: failed to"
            " retrieve document for iri: '" + iri.toString() + "'");
}

std::string OWLOntologyIO::download(const owlapi::model::IRI& iri,
        const std::string& targetDir)
{
    boost::filesystem::path targetPath(targetDir);
    if( !boost::filesystem::exists(targetPath) )
    {
        boost::filesystem::create_directories(targetPath);
    }

    // file is not locally available, so trying to retrieve
    for(const std::string& suffix : getFormatSuffixes())
    {
        std::string absoluteFilename = (targetPath / (canonizeForOfflineUsage(iri)
            + suffix)).string();
        std::string cmd = "wget " + iri.toString() + " -O " + absoluteFilename + " -q";
        LOG_DEBUG_S << "Trying to retrieve document with command '" << cmd << "'";

        if( system(cmd.c_str()) != 0)
        {
            // cleanup
            boost::filesystem::remove( boost::filesystem::path(absoluteFilename));
            continue;
        }
        // Check if file is empty
        std::ifstream file(absoluteFilename);
        if(file.peek() == std::ifstream::traits_type::eof())
        {
            // cleanup
            boost::filesystem::remove( boost::filesystem::path(absoluteFilename));
            continue;
        }

        // seems we succeeded
        return absoluteFilename;
    }
    throw OWLOntologyNotFound("owlapi::io::OWLOntologyIO::download: failed to"
            " download document for iri: '" + iri.toString() + "'");
}

std::string OWLOntologyIO::retrieve(const owlapi::model::IRI& iri, const
        std::string& ontologyPath)
{
    std::vector<std::string> formatSuffixes = getFormatSuffixes();
    for(const std::string& suffix : formatSuffixes)
    {
        std::string absoluteFilename = ontologyPath +
            canonizeForOfflineUsage(iri) + suffix;
        if(boost::filesystem::exists( absoluteFilename ) )
        {
            return absoluteFilename;
        }
    }

    throw OWLOntologyNotFound("owlapi::io::OWLOntologyIO::retrieve: failed to"
            " retrieve document for iri: '" + iri.toString() + "'");
}

std::vector<std::string> OWLOntologyIO::getFormatSuffixes()
{
    std::vector<std::string> suffixes;
    for(const std::map<Format, std::string>::value_type& v : FormatSuffixes)
    {
        if(v.first != UNKNOWN && v.first != END_FORMAT)
        {
            suffixes.push_back( v.second );
        }
    }
    return suffixes;
}

Format OWLOntologyIO::guessFormat(const std::string& filename)
{
    std::string suffix;
    size_t pos = filename.find_last_of(".");
    if(pos != std::string::npos)
    {
        suffix = filename.substr(pos);
        for(const std::pair<Format, std::string>& v : FormatSuffixes)
        {
            if(v.second == suffix)
            {
                return v.first;
            }
        }
    }
    throw std::runtime_error("owlapi::model::OWLOntologyIO::guessFormat:"
            "failed to guess format from '" + filename + "', found suffix: '" + suffix + "'");
}

} // end namespace io
} // end namespace owlapi
