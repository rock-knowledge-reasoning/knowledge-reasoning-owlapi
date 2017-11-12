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

        std::string path = retrieve(ontology->getIRI());
        ontology->setAbsolutePath(path);
    }

    // load from the file set for the ontology
    ontology = reader.open(ontology->getAbsolutePath());
    reader.loadDeclarationsAndImports(ontology, true /*directImport*/);

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
        importReader->loadDeclarationsAndImports(importedOntology, true /*directImport*/);
        IRIList directImports = importedOntology->getDirectImportsDocuments();
        dependencies[iri] = IRISet(directImports.begin(), directImports.end());

        unprocessed.insert(unprocessed.begin(), directImports.begin(), directImports.end());
        unprocessed.push_back(iri);

        readersMap[importedOntology] = importReader;

        // register IRI and ontology
        processed.push_back(iri);
    }


    LOG_WARN_S << "unprocessed: " << unprocessed;
    LOG_WARN_S << "processed: " << processed;

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
        importReader->loadDeclarationsAndImports(ontology, false);
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
