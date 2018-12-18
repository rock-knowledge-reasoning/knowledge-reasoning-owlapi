#include "SopranoDB.hpp"
#include <base-logging/Logging.hpp>
#include <fstream>

namespace owlapi {
namespace db {

SopranoDB::SopranoDB(const std::string& filename, const std::string& baseUri)
    : FileBackend(filename, baseUri)
    , mRDFModel(0)
{
    mRDFModel = fromFile(mFilename, mBaseUri);
}

Soprano::Model* SopranoDB::fromFile(const std::string& filename, const std::string& baseUri)
{
    QUrl qBaseUri( QString::fromStdString(baseUri) );

    {
        std::ifstream infile(filename.c_str());
        if(! infile.good() )
        {
            std::string msg = "SopranoDB: file does not exist: '" + filename + "\n";
            throw std::runtime_error(msg);
        }
    }

    std::vector<Soprano::RdfSerialization> serializationFormats;
    serializationFormats.push_back(Soprano::SerializationRdfXml);
// Segfault here in redland
//    serializationFormats.push_back(Soprano::SerializationN3);
    serializationFormats.push_back(Soprano::SerializationNTriples);
    serializationFormats.push_back(Soprano::SerializationTrig);
    serializationFormats.push_back(Soprano::SerializationNQuads);

    std::vector<Soprano::RdfSerialization>::const_iterator cit = serializationFormats.begin();
    std::string triedFormats;
    for(; cit != serializationFormats.end(); ++cit)
    {
        const Soprano::Parser* p = Soprano::PluginManager::instance()->discoverParserForSerialization(*cit);
        std::string format = serializationMimeType(*cit).toStdString();

        try {
            Soprano::StatementIterator it = p->parseFile(QString::fromStdString(filename), qBaseUri, *cit);

            QList<Soprano::Statement> allStatements = it.allStatements();
            if(allStatements.empty())
            {
                throw std::runtime_error("owlapi::db::rdf::SopranoDB:fromFile " + filename +
                        " no statements found using parser format: '" + format + "'. Trying other parser.");
            }
            LOG_INFO_S << "Successfully parsed '" << filename << "' using format: " << format;

            Soprano::Model* sopranoModel = Soprano::createModel();
            Q_FOREACH( Soprano::Statement s, allStatements)
            {
                sopranoModel->addStatement(s);
            }

            return sopranoModel;
        } catch(const std::exception& e)
        {
            triedFormats += format + ";";
        }
    }

    throw std::runtime_error("owlapi::db::rdf::SopranoDB: file format of '" + filename + "' not supported. Tried the following: " + triedFormats);
}

query::Results SopranoDB::query(const std::string& query, const query::Bindings& bindings) const
{
    query::Results queryResults;

    Soprano::QueryResultIterator qit = mRDFModel->executeQuery(QString(query.c_str()), Soprano::Query::QueryLanguageSparql);
    while( qit.next())
    {
        query::Bindings::const_iterator bit = bindings.begin();

        query::Row row;
        for(; bit != bindings.end(); ++bit)
        {
            query::Variable binding = *bit;
            std::string boundValue = qit.binding(QString(binding.getName().c_str())).toString().toStdString();
            row[binding] = owlapi::model::IRI::create( boundValue );
        }

        queryResults.rows.push_back(row);
    }

    return queryResults;
}

} // end namespace db
} // end namespace owlapi
