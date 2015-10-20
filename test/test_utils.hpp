#ifndef OWL_API_TEST_UTILS
#define OWL_API_TEST_UTILS

#include <string>
/**
 * Retrieve the root directory by analysing the current execution path: 
 * returns /../owlapi/ as path
 */
std::string getRootDir();

std::string getOMSchema();
#endif
