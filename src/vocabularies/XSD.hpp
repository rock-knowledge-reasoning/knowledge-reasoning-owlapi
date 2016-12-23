#ifndef OWLAPI_VOCABULARIES_XSD_HPP
#define OWLAPI_VOCABULARIES_XSD_HPP

#ifndef VOCABULARY_BASE_IRI
#error "Direct inclusion of this header is forbidden. Use Vocabulary.hpp instead"
#endif

namespace owlapi {
namespace vocabulary {

/**
 * For inbuilt type in c++: long, int, short, double, float
 * use the syntax XSD::resolve("long")
 * e.g., http://www.w3.org/2001/XMLSchema#int
 * \see http://www.w3.org/TR/owl2-syntax/#Real_Numbers.2C_Decimal_Numbers.2C_and_Integers
 * \see https://www.w3.org/TR/xmlschema11-2/
 */
class XSD
{
public:
    VOCABULARY_BASE_IRI("http://www.w3.org/2001/XMLSchema#");
    VOCABULARY_DYNAMIC_EXTENSION

    VOCABULARY_ADD_WORD(boolean);
    VOCABULARY_ADD_WORD(decimal);
    VOCABULARY_ADD_WORD(integer);
    VOCABULARY_ADD_WORD(nonNegativeInteger);
    VOCABULARY_ADD_WORD(nonPositiveInteger);
    VOCABULARY_ADD_WORD(positiveInteger);
    VOCABULARY_ADD_WORD(negativeInteger);

    /// C++ inbuilt types use resolve instead
    //VOCABULARY_ADD_WORD(long);
    //VOCABULARY_ADD_WORD(int);
    //VOCABULARY_ADD_WORD(short);

    VOCABULARY_ADD_WORD(byte);
    VOCABULARY_ADD_WORD(unsignedLong);
    VOCABULARY_ADD_WORD(unsignedInt);
    VOCABULARY_ADD_WORD(unsignedShort);
    VOCABULARY_ADD_WORD(unsignedByte);

    /// C++ inbuilt types use resolve instead
    //VOCABULARY_ADD_WORD(double);
    //VOCABULARY_ADD_WORD(float);

    VOCABULARY_ADD_WORD(string);
    VOCABULARY_ADD_WORD(token);
    VOCABULARY_ADD_WORD(normalizedString);
    VOCABULARY_ADD_WORD(language);
    VOCABULARY_ADD_WORD(Name);
    VOCABULARY_ADD_WORD(QName);
    VOCABULARY_ADD_WORD(NCName);
    VOCABULARY_ADD_WORD(NMTOKEN);
    VOCABULARY_ADD_WORD(NMTOKENS);

    VOCABULARY_ADD_WORD(hexBinary);
    VOCABULARY_ADD_WORD(base64Binary);

    VOCABULARY_ADD_WORD(dateTime);
    VOCABULARY_ADD_WORD(dateTimeStamp);
    VOCABULARY_ADD_WORD(duration);
    VOCABULARY_ADD_WORD(dayTimeDuration);
    VOCABULARY_ADD_WORD(yearMonthDuration);
    VOCABULARY_ADD_WORD(date);
    VOCABULARY_ADD_WORD(time);
    VOCABULARY_ADD_WORD(gDay);
    VOCABULARY_ADD_WORD(gMonth);
    VOCABULARY_ADD_WORD(gMonthDay);
    VOCABULARY_ADD_WORD(gYear);
    VOCABULARY_ADD_WORD(gYearMonth);

    VOCABULARY_ADD_WORD(anyType);
    VOCABULARY_ADD_WORD(anySimpleType);
    VOCABULARY_ADD_WORD(anyAtomicType);
    VOCABULARY_ADD_WORD(anyURI);
    VOCABULARY_ADD_WORD(ID);
    VOCABULARY_ADD_WORD(IDREF);
    VOCABULARY_ADD_WORD(IDREFS);
    VOCABULARY_ADD_WORD(ENTITY);
    VOCABULARY_ADD_WORD(ENTITIES);
    VOCABULARY_ADD_WORD(NOTATION);

    // Facets
    VOCABULARY_ADD_WORD(whiteSpace);

    VOCABULARY_ADD_WORD(totalDigits);
    VOCABULARY_ADD_WORD(fractionDigits);
    VOCABULARY_ADD_WORD(pattern);
    VOCABULARY_ADD_WORD(enumeration);
    VOCABULARY_ADD_WORD(minInclusive);
    VOCABULARY_ADD_WORD(maxInclusive);
    VOCABULARY_ADD_WORD(minExclusive);
    VOCABULARY_ADD_WORD(maxExclusive);
    VOCABULARY_ADD_WORD(assertions);

    VOCABULARY_ADD_WORD(restriction);
};

} // end namespace vocabulary
} // end namespace owlapi
#endif // OWLAPI_VOCABULARIES_XSD_HPP
