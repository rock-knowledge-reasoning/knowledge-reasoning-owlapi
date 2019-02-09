INSTALL(FILES data/owlapi-test-with-custom.owl
    DESTINATION share/ontologies
    RENAME "http___www_rock-robotics_org_2019_01_owlapi_test_create-with-custom.owl")

INSTALL(FILES data/test-restrictions-modular-a.ttl
    DESTINATION share/ontologies
    RENAME "http___www_rock-robotics_org_test_turtle_restrictions-modular-a.ttl")

INSTALL(FILES data/test-restrictions-modular-b.ttl
    DESTINATION share/ontologies
    RENAME "http___www_rock-robotics_org_test_turtle_restrictions-modular-b.ttl")

INSTALL(FILES data/test-restrictions-modular-c.ttl
    DESTINATION share/ontologies
    RENAME "http___www_rock-robotics_org_test_turtle_restrictions-modular-c.ttl")

rock_testsuite(${PROJECT_NAME}-test suite.cpp
        test_KnowledgeBase.cpp
        test_Ontology.cpp
        test_OWLApi.cpp
        test_Sparql.cpp
        test_IO.cpp
    DEPS owlapi
)
