#include "../Vocabulary.hpp"

namespace owlapi {
namespace vocabulary {

owlapi::model::IRISet XSD::mDataTypes = {
    boolean(),
    decimal(),
    integer(),
    nonNegativeInteger(),
    nonPositiveInteger(),
    positiveInteger(),
    negativeInteger(),
    ///// C++ inbuilt types use resolve instead
    resolve("long"),
    resolve("int"),
    resolve("short"),
    byte(),
    unsignedLong(),
    unsignedInt(),
    unsignedShort(),
    unsignedByte(),
    ///// C++ inbuilt types use resolve instead
    resolve("double"),
    resolve("float"),
    string(),
    token(),
    normalizedString(),
    language(),
    Name(),
    QName(),
    NCName(),
    NMTOKEN(),
    NMTOKENS(),
    hexBinary(),
    base64Binary(),
    dateTime(),
    dateTimeStamp(),
    duration(),
    dayTimeDuration(),
    yearMonthDuration(),
    date(),
    time(),
    gDay(),
    gMonth(),
    gMonthDay(),
    gYear(),
    gYearMonth(),
    anyType(),
    anySimpleType(),
    anyAtomicType(),
    anyURI(),
    ID(),
    IDREF(),
    IDREFS(),
    ENTITY(),
    ENTITIES(),
    NOTATION(),
    //// Facets
    whiteSpace(),
    totalDigits(),
    fractionDigits()
    //VOCABULARY_ADD_WORD(pattern);
    //VOCABULARY_ADD_WORD(enumeration);
    //VOCABULARY_ADD_WORD(minInclusive);
    //VOCABULARY_ADD_WORD(maxInclusive);
    //VOCABULARY_ADD_WORD(minExclusive);
    //VOCABULARY_ADD_WORD(maxExclusive);
    //VOCABULARY_ADD_WORD(assertions);
    //VOCABULARY_ADD_WORD(restriction);
    };


bool XSD::isDatatype(const owlapi::model::IRI& iri)
{
    return mDataTypes.find(iri) != mDataTypes.end();
}
} // end namespace vocabulary
} // end namespace owlapi
