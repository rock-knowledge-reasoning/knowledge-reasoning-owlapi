INSTALL(FILES data/owlapi-test-with-custom.owl
    DESTINATION share/ontologies
    RENAME "http___www_rock-robotics_org_2019_01_owlapi_test_create-with-custom.owl")

rock_testsuite(${PROJECT_NAME}-test suite.cpp
        test_KnowledgeBase.cpp
        test_Ontology.cpp
        test_OWLApi.cpp
        test_Sparql.cpp
        test_IO.cpp
    DEPS owlapi
)
