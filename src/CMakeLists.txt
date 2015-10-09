find_package(Boost REQUIRED regex system)
set(ICU_LIBS icutu
    icudata
    icui18n
    icuio
    icule
    iculx
    icutest
    icuuc
)

rock_library(owlapi
    SOURCES
        KnowledgeBase.cpp
        csp/ModelBound.cpp
        csp/ResourceMatch.cpp
        db/FileBackend.cpp
        db/rdf/SopranoDB.cpp
        db/rdf/Sparql.cpp
        db/rdf/SparqlInterface.cpp
        db/rdf/Variable.cpp
        io/OWLOntologyIO.cpp
        io/RedlandWriter.cpp
        io/XMLUtils.cpp
        model/IRI.cpp
        model/NodeID.cpp
        model/OWLAxiom.cpp
        model/OWLCardinalityRestriction.cpp
        model/OWLClass.cpp
        model/OWLClassAssertionAxiom.cpp
        model/OWLClassExpression.cpp
        model/OWLDataProperty.cpp
        model/OWLDataPropertyAssertionAxiom.cpp
        model/OWLDataRange.cpp
        model/OWLDataType.cpp
        model/OWLDeclarationAxiom.cpp
        model/OWLDisjointClassesAxiom.cpp
        model/OWLDisjointUnionAxiom.cpp
        model/OWLEntity.cpp
        model/OWLEquivalentClassesAxiom.cpp
        model/OWLExactCardinalityRestriction.cpp
        model/OWLLiteral.cpp
        model/OWLLiteralDouble.cpp
        model/OWLLiteralInteger.cpp
        model/OWLObject.cpp
        model/OWLObjectPropertyAssertionAxiom.cpp
        model/OWLOntology.cpp
        model/OWLOntologyAsk.cpp
        model/OWLOntologyReader.cpp
        model/OWLOntologyTell.cpp
        model/OWLPropertyExpression.cpp
        model/OWLQualifiedRestriction.cpp
        model/OWLRestriction.cpp
        model/OWLSubClassOfAxiom.cpp
        model/URI.cpp
        reasoner/factpp/Types.cpp
    HEADERS
        Exceptions.hpp
        KnowledgeBase.hpp
        OWLApi.hpp
        Vocabulary.hpp
        csp/ModelBound.hpp
        csp/ResourceMatch.hpp
        db/FileBackend.hpp
        db/rdf/SopranoDB.hpp
        db/rdf/Sparql.hpp
        db/rdf/SparqlInterface.hpp
        db/rdf/Variable.hpp
        io/OWLOntologyIO.hpp
        io/OWLWriter.hpp
        io/RedlandWriter.hpp
        io/XMLUtils.hpp
        model/HasAnnotations.hpp
        model/HasIRI.hpp
        model/IRI.hpp
        model/NodeID.hpp
        model/OWLAnnotation.hpp
        model/OWLAnnotationAxiom.hpp
        model/OWLAnnotationObject.hpp
        model/OWLAnnotationProperty.hpp
        model/OWLAnnotationValue.hpp
        model/OWLAnonymousClassExpression.hpp
        model/OWLAnonymousIndividual.hpp
        model/OWLAsymmetricObjectPropertyAxiom.hpp
        model/OWLAxiom.hpp
        model/OWLAxiomVisitor.hpp
        model/OWLCardinalityRestriction.hpp
        model/OWLClass.hpp
        model/OWLClassAssertionAxiom.hpp
        model/OWLClassAxiom.hpp
        model/OWLClassExpression.hpp
        model/OWLDataProperty.hpp
        model/OWLDataPropertyAssertionAxiom.hpp
        model/OWLDataPropertyAxiom.hpp
        model/OWLDataPropertyDomainAxiom.hpp
        model/OWLDataPropertyExpression.hpp
        model/OWLDataPropertyRangeAxiom.hpp
        model/OWLDataRange.hpp
        model/OWLDataType.hpp
        model/OWLDeclarationAxiom.hpp
        model/OWLDisjointClassesAxiom.hpp
        model/OWLDisjointObjectProperties.hpp
        model/OWLDisjointUnionAxiom.hpp
        model/OWLEntity.hpp
        model/OWLEquivalentClassesAxiom.hpp
        model/OWLEquivalentObjectPropertiesAxiom.hpp
        model/OWLExactCardinalityRestriction.hpp
        model/OWLFunctionalDataPropertyAxiom.hpp
        model/OWLFunctionalObjectPropertyAxiom.hpp
        model/OWLIndividual.hpp
        model/OWLIndividualAxiom.hpp
        model/OWLInverseFunctionalObjectPropertyAxiom.hpp
        model/OWLInverseObjectPropertiesAxiom.hpp
        model/OWLInverseObjectProperty.hpp
        model/OWLIrreflexiveObjectPropertyAxiom.hpp
        model/OWLLiteral.hpp
        model/OWLLiteralDouble.hpp
        model/OWLLiteralInteger.hpp
        model/OWLLiteralNonNegativeInteger.hpp
        model/OWLLogicalAxiom.hpp
        model/OWLLogicalEntity.hpp
        model/OWLMaxCardinalityRestriction.hpp
        model/OWLMinCardinalityRestriction.hpp
        model/OWLNamedIndividual.hpp
        model/OWLNamedObject.hpp
        model/OWLNaryAxiom.hpp
        model/OWLNaryClassAxiom.hpp
        model/OWLObject.hpp
        model/OWLObjectExactCardinality.hpp
        model/OWLObjectMaxCardinality.hpp
        model/OWLObjectMinCardinality.hpp
        model/OWLObjectProperty.hpp
        model/OWLObjectPropertyAssertionAxiom.hpp
        model/OWLObjectPropertyAxiom.hpp
        model/OWLObjectPropertyCharacteristicAxiom.hpp
        model/OWLObjectPropertyDomainAxiom.hpp
        model/OWLObjectPropertyExpression.hpp
        model/OWLObjectPropertyRangeAxiom.hpp
        model/OWLOntology.hpp
        model/OWLOntologyAsk.hpp
        model/OWLOntologyReader.hpp
        model/OWLOntologyTell.hpp
        model/OWLProperty.hpp
        model/OWLPropertyAssertionAxiom.hpp
        model/OWLPropertyAssertionObject.hpp
        model/OWLPropertyAxiom.hpp
        model/OWLPropertyDomainAxiom.hpp
        model/OWLPropertyExpression.hpp
        model/OWLPropertyRange.hpp
        model/OWLPropertyRangeAxiom.hpp
        model/OWLQualifiedRestriction.hpp
        model/OWLReasoner.hpp
        model/OWLReflexiveObjectPropertyAxiom.hpp
        model/OWLRestriction.hpp
        model/OWLSubClassOfAxiom.hpp
        model/OWLSubDataPropertyOfAxiom.hpp
        model/OWLSubObjectPropertyOfAxiom.hpp
        model/OWLSubPropertyAxiom.hpp
        model/OWLSymmetricObjectPropertyAxiom.hpp
        model/OWLTransitiveObjectPropertyAxiom.hpp
        model/OWLUnaryPropertyAxiom.hpp
        model/URI.hpp
        reasoner/factpp/AxiomVisitor.hpp
        reasoner/factpp/Types.hpp
        vocabularies/OM.hpp
        vocabularies/OWL.hpp
        vocabularies/RDF.hpp
        vocabularies/RDFS.hpp
        vocabularies/XSD.hpp
    DEPS_PKGCONFIG
        base-lib
        factpp
        liburiparser
        numeric
        soprano
        redland
        raptor2
)


find_library(gecode0 NAMES gecodeflatzinc   PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode1 NAMES gecodedriver     PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode2 NAMES gecodegist       PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode3 NAMES gecodesearch     PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode4 NAMES gecodeminimodel  PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode5 NAMES gecodeset        PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode6 NAMES gecodefloat      PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode7 NAMES gecodeint        PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode8 NAMES gecodekernel     PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode9 NAMES gecodesupport    PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )

set(GECODE_LIBRARIES
    ${gecode0} ${gecode1} ${gecode2} ${gecode3} ${gecode4} ${gecode5} ${gecode6} ${gecode7} ${gecode8} ${gecode9}
)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(owlapi ${Boost_LIBRARIES} ${ICU_LIBS} ${GECODE_LIBRARIES})

rock_executable(owlapi-ontology_reader
    SOURCES utils/OntologyReaderMain.cpp
    DEPS owlapi)
