rock_testsuite(${PROJECT_NAME}-test suite.cpp
        test_CSP.cpp
        test_Ontology.cpp
        test_OWLApi.cpp
        test_Sparql.cpp
        test_IO.cpp
    DEPS owlapi
)
