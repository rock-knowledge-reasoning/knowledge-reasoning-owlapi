#ifndef OWLAPI_VOCABULARIES_RDF_HPP
#define OWLAPI_VOCABULARIES_RDF_HPP

namespace owlapi {
namespace vocabulary {

/**
 * The vocabulary for RDF
 * \see http://www.w3.org/TR/rdf-schema/#ch_summary
 */
class RDF
{
public:
    VOCABULARY_BASE_IRI("http://www.w3.org/1999/02/22-rdf-syntax-ns#");
    VOCABULARY_ADD_WORD(Alt); // rdfs:Class
    VOCABULARY_ADD_WORD(Bag); // rdfs:Class
    VOCABULARY_ADD_WORD(List); // rdfs:Class
    VOCABULARY_ADD_WORD(Property); // rdfs:Class
    VOCABULARY_ADD_WORD(Seq); // rdfs:Class
    VOCABULARY_ADD_WORD(Statement);  //rdfs:Class
    VOCABULARY_ADD_WORD(first); // rdf:Property
    VOCABULARY_ADD_WORD(object); // rdf:Property
    VOCABULARY_ADD_WORD(nil); // rdf:List
    VOCABULARY_ADD_WORD(language); // rdf:Property
    VOCABULARY_ADD_WORD(direction); // rdf:Property
    VOCABULARY_ADD_WORD(predicate); // rdf:Property
    VOCABULARY_ADD_WORD(rest); // rdf:Property
    VOCABULARY_ADD_WORD(subject); // rdf:Property
    VOCABULARY_ADD_WORD(type); // rdf:Property
    VOCABULARY_ADD_WORD(value);  //
    VOCABULARY_ADD_WORD(_1);
    VOCABULARY_ADD_WORD(_2);
    VOCABULARY_ADD_WORD(_3);
    VOCABULARY_ADD_WORD(CompoundLiteral); // rdfs:Datatype
    VOCABULARY_ADD_WORD(JSON); // rdfs:Datatype
    VOCABULARY_ADD_WORD(PlainLiteral); // rdfs:Datatype
    VOCABULARY_ADD_WORD(XMLLiteral); // rdfs:Datatype
};

} // end namespace vocabulary
} // end namespace owlapi
#endif // OWLAPI_VOCABULARIES_RDF_HPP
