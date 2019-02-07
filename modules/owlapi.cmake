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
exec_program(${OWLAPI}
    ARGS "-c ${URL}"
    OUTPUT_VARIABLE URL_OWL)

INSTALL(FILES ${OWL_FILE}
    DESTINATION share/ontologies
    RENAME ${URL_OWL}.owl)
endfunction(owlapi_install_ontology)
