# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 3.9)
project(owlapi
    VERSION 0.2
    DESCRIPTION "C++-based owlapi implementation to handle ontologies"
)
find_package(Rock)
rock_init()
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
rock_standard_layout()

configure_file(modules/owlapi-config.cmake.in owlapi-config.cmake @ONLY)
install(FILES
    modules/owlapi.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/owlapi-config.cmake
    DESTINATION share/owlapi/cmake)
