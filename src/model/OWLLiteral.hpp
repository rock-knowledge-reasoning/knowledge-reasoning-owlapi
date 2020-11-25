#ifndef OWLAPI_MODEL_OWL_LITERAL_HPP
#define OWLAPI_MODEL_OWL_LITERAL_HPP

#include <string>
#include "../Vocabulary.hpp"
#include "IRI.hpp"
#include "OWLDataType.hpp"
#include "OWLPropertyAssertionObject.hpp"
#include "OWLAnnotationValue.hpp"

namespace owlapi {
namespace model {

/**
 *  Represents a Literal in the OWL 2 Specification.
 *
 *  Each literal consists of a lexical form, which is a string, and a datatype.
 *  A literal consisting of a lexical form "abc" and a datatype identified by
 *  the IRI datatypeIRI is written as "abc"^^datatypeIRI.
 *
 *  Note that literals whose datatype is rdf:PlainLiteral can be abbreviated.
 *  For example, literals of the form "abc@"^^rdf:PlainLiteral can be
 *  abbreviated in the functional-style syntax, and other concrete syntaxes to
 *  "abc". Literals of the form "abc@langTag"^^rdf:PlainLiteral where "langTag"
 *  is not empty are abbreviated in functional-style syntax documents (and other
 *  concrete syntaxes) to "abc" whenever possible
 */
class OWLLiteral : public OWLPropertyAssertionObject, public OWLAnnotationValue
{
protected:
    std::string mValue;
    std::string mType;

    OWLLiteral();

    OWLLiteral(const std::string& value, const std::string& type);

    OWLLiteral(const std::string& value);

public:
    typedef shared_ptr<OWLLiteral> Ptr;
    typedef std::vector<Ptr> PtrList;

    virtual ~OWLLiteral() {}

    /**
     * Test if literal is typed,
     * e.g. "1^^http://www.w3.org/2001/XMLSchema#nonNegativeInteger"
     */
    bool isTyped() const;

    bool hasType(const IRI& typeIRI) const;

    /**
      * Set the type for the literal
      * \param type as full IRI string, e.g., http://www.w3.org/2001/XMLSchema#nonNegativeInteger
      */
    void setType(const std::string& type) { mType = type; }

    std::string getType() const { return mType; }

    std::string getValue() const { return mValue; }

    OWLObject::Type getObjectType() const { return OWLObject::Literal; }

    // http://owlapi.sourceforge.net/javadoc/index.html?org/semanticweb/owlapi/model/OWLClassExpression.html
    //OWLDatatype getDatatype()
    //std::string getL
    //
    std::string toString() const;

    bool isBoolean() const { return mType == vocabulary::XSD::boolean().toString(); }
    bool isDouble() const { return mType == vocabulary::XSD::resolve("double").toString(); }
    bool isFloat() const { return mType == vocabulary::XSD::resolve("float").toString(); }
    bool isInteger() const { return mType == vocabulary::XSD::integer().toString(); }
    bool isPlainLiteral() const { return mType == vocabulary::RDF::PlainLiteral().toString() || mType.empty(); }

    /**
     * Create a literal based on the given type information
     * \return pointer to an inbuilt literal type
     */
    static OWLLiteral::Ptr create(const std::string& literal);

    /**
     * Create a literal based on the given string and type information
     * \return point to an inbuilt literal type
     */
    static OWLLiteral::Ptr create(const std::string& literal, const OWLDataType& type);

    static OWLLiteral::Ptr integer(int32_t value);
    /**
     * Create a literal typed to non negative integer and given value
     */
    static OWLLiteral::Ptr nonNegativeInteger(uint32_t value);

    static OWLLiteral::Ptr doubleValue(double value);

    virtual unsigned int getNonNegativeInteger() const;

    virtual int getInteger() const;

    virtual double getDouble() const;

    static IRIList toIRIList(const OWLLiteral::PtrList& literals);

    OWLLiteral::Ptr asLiteral() const override;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_LITERAL_HPP
