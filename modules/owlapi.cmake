find_program(OWLAPI owlapi-utils)

if(NOT OWLAPI)
    message(SEND_ERROR "Could not find program: 'owlapi-utils' -- did you install 'owlapi' (and updated you environmental variables)")
else()
    message("Found 'owlapi-utils': ${OWLAPI}")
endif()

# Install an ontology into the share folder
# by renaming the file to a canonized filename
# see 'owlapi-utils --help' for information
function(owlapi_install_ontology URL OWL_FILE)
execute_process(COMMAND ${OWLAPI} -c ${URL}
    OUTPUT_VARIABLE URL_OWL
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REGEX MATCH "\\.[a-z]+$" OWL_SUFFIX ${OWL_FILE})

# Validate the ontology
execute_process(
    COMMAND rapper -q --guess ${CMAKE_SOURCE_DIR}/${OWL_FILE} -o rdfxml
    OUTPUT_FILE ${CMAKE_BINARY_DIR}/${URL_OWL}.rdf
    ERROR_QUIET
    RESULT_VARIABLE RAPPER_EXIT_STATUS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
)
if(RAPPER_EXIT_STATUS EQUAL 0)
    message(STATUS "Conversion of ontology file ${OWL_FILE} to rdfxml "
        "succeeded -- your ontology appears to be well formatted")
else()
    message(SEND_ERROR "Conversion of ontology file ${OWL_FILE} to rdfxml "
        "failed -- your ontology seems to be malformatted/invalid")
endif()

INSTALL(FILES ${OWL_FILE}
    DESTINATION share/ontologies
    RENAME ${URL_OWL}${OWL_SUFFIX})
endfunction(owlapi_install_ontology)
