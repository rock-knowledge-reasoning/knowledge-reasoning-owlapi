# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
rock_init(owlapi 0.1)
rock_activate_cxx11()
rock_standard_layout()

configure_file(modules/owlapi-config.cmake.in owlapi-config.cmake @ONLY)
install(FILES
    modules/owlapi.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/owlapi-config.cmake
    DESTINATION share/owlapi/cmake)
