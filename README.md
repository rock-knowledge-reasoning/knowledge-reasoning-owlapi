# owlapi

This library reimplements owlapi (https://github.com/owlcs/owlapi) functionality
for C++ projects.
It does not (yet) support the full feature set of the original JAVA-based owlapi
implementation, but a significant subset.

## Installation

### Dependencies

This library depends on the following packages:

| Name       | Description |
|--------|-----|
| [boost](https://www.boost.org/users/license.html) | Boost: portable C++ source libraries            |
| [base/cmake](http://github.com/rock-core/base-cmake.git) | Essential CMake Macros for simplification of the build management            |
| [base/logging](https://github.com/rock-core/base-logging) | C++ Logging Library |
| [knowledge_reasoning/fact++](https://bitbucket.org/dtsarkov/factplusplus)  | Description Logic (DL) Reasoner |
| [uriparser](https://uriparser.github.io/) | Library for RFC 3986 compliant URI parsing |
| [soprano](http://soprano.sourceforge.net) | A Qt-based framework for RDF data |
| [redland](http://librdf.org/LICENSE.html) | Resource Description Framework (RDF) handling, including (de)serialization and in memory representation |
| [raptor2](http://librdf.org/LICENSE.html) | RDF handling as part of the Redland libraries |
| [utilmm](https://github.com/orocos-toolchain/utilmm) | C++-helper libraries, here using a C++-Wrapper for pkg-config |


### Installation

Either install all dependencies by hand, then:
```
    mkdir build
    cd build
    cmake ..
    make
```

## Installation from GitHub

Create a new Rock-based installation in a development folder, here called dev:
```
    mkdir dev
    cd dev
    wget http://www.rock-robotics.org/master/autoproj_bootstrap
    ruby autoproj_bootstrap
```

In autoproj/manifest add the respective manifest and add the package to the
layout section:
```
    package_set:
        - github: rock-core/rock-package_set

    layout:
        - knowledge_reasoning/owlapi
```

```
$>source env.sh
$>autoproj update
$>autoproj osdeps
$>amake knowledge_reasoning/owlapi
```

## Documentation

### API Documentation
To see the documentation for this project please do the following after
checkout to generate the doxygen-based documentation:

```
    mkdir build
    cd build
    make doc
```

Open the doxygen documentation with a browser:

```
    browse build/doc/index.html
```

The doxygen documentation contains all information on how to start
using the library along with the general API documentation.

### General Documentation
In general we recommend the usage of the OWL editor protége: http://protege.stanford.edu
in case you want to edit/create OWL model files.
Make sure you export as RDF/XML (.owl) or in Turtle Syntax (.ttl) -- as supported
formats for import.

Currently this library supports a limit modelling capability, i.e., the main
limitations arises from the fact that modelling from user level is done
mainly through NamedInvididuals.
Anonymous classes and classes representing collections of OneOf,
UnionOf classes are currently not supported.

#### Examples
The following examples shall help you to understand the user interface and
applicability.

```
#include <owlapi/model/OWLOntology.hpp>

using namespace owlapi::model;

OWLOntology::Ptr ontology = io::OWLOntologyIO::fromFile("om-schema-v0.6.owl");

// Add information to ontology
OWLOntologyTell tell(ontology);
OWLClass::Ptr robotKlass = tell->klass("http:://www.rock-robotics.org/2014/01/om-schema#Sherpa")
OWLClass::Ptr cameraKlass = tell->klass("http:://www.rock-robotics.org/2014/01/om-schema#Camera")
OWLObjectPropery::Ptr oProperty = tell->objectProperty("http://www.rock-robotics.org/2014/01/om-schema#has");

// either
{
    OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestriction::Ptr( new OWLMaxCardinalityRestriction(oProperty, 10, cameraKlass.getIRI()));
    tell->restrictClass(robotKlass, restriction); // alternatively: tell->subClassOf(robotKlass, restriction);
}
// or
{
    OWLCardinalityRestriction::Ptr restriction = tell->cardinalityRestriction(oProperty, 10, cameraKlass.getIRI(), OWLCardinalityRestriction::MAX);
    tell->restrictClass(robotKlass, restriction); // alternatively: tell->subClassOf(robotKlass, restriction);
}

// Retrieve information from ontology
OWLOntologyAsk ask(ontology);
IRI robot("http:://www.rock-robotics.org/2014/01/om-schema#Sherpa")
std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = ask.getCardinalityRestrictions(robot);

```

#### Vocabularies
 
 To facilitate the handling of OWL statements, you can use inbuilt
 vocabularies or define your own -- see owlapi/vocabularies/OWL.hpp
 for a starting example including some macros that facilitate the definition
 of new vocabularies.
 To define a custom vocabulary:
 
```
  #include <owlapi/Vocabulary.hpp>

  owlapi::vocabulary::Custom custom("http://base-url/");
  owlapi::model::IRI iri = custom.resolve("my-instance");
```

 Use an existing vocabulary. Note that the iri for types which collide
 with C++ inbuilt types/keyword have to rely on the 'resolve' function, e.g.,
 double, float, int

```
 #include <owlapi/Vocabulary.hpp>

 owlapi::model::IRI iriThing = owlapi::vocabulary::OWL::Thing();
 owlapi::model::IRI iriDouble = owlapi::vocabulary::OWL::resolve("double");
```
 
 
#### DataProperties Handling of DataProperties
 
Retrieving values of data properties, e.g.,
when the datatype for the property is known as 'double'

```
IRI instance = owlapi::vocabulary::OM::resolve("MyRobot");
IRI property = owlapi::vocabulary::OM::resolve("mass");

OWLLiteral::Ptr value = ask.getDataValue(instance, property);
double robotMass = value->getDouble();
```

Setting values of data properties, e.g.,
when the datatype for the property is known as 'double'

```
IRI instance = owlapi::vocabulary::OM::resolve("MyRobot");
IRI property = owlapi::vocabulary::OM::resolve("mass");

OWLLiteral::Ptr value = OWLLiteral::create("0.5", owlapi::vocabulary::XSD::resolve("double"));
tell.valueOf(instance, property, literal);
```

 
### Architecture
 
This library is a C++-Clone of the JAVA-based owlapi: http://owlapi.sourceforge.net.
The motivation for implementation is to allow a consistent application of
C/C++-based programs on robotic systems -- especially since we are intending
to target small/embedded devices.

Furthermore, the embedded reasoner FACT++ (Reasoner for the SROIQ(D) Description Logic v1.6.3) is actually written in C++
and thus can be accessed almost directly.
This implementation of owlapi is not as complete as the original JAVA-based one.
Among other things it does not fully support is processing of construct such as DataComplementOf.
DataOneOf is supported however.
This library provides core features to handle ontologies and supports also some
more complex modelling using qualified cardinality restrictions.
 
The user can load the model from file, and manipulate the resulting ontology
using two separate accessor classes: the OWLOntologyTell and the OWLOntologyAsk interface --
similar to the DIG Interface (http://dl.kr.org/dig/interface.html).

## Tests
To run the tests you have to enable the testing suite first.
Either by explicitly setting ROCK_TEST_ENABLED to ON, or by
calling
```
    autoproj test enable knowledge_reasoning/owlapi
    amake knowledge_reasoning/owlapi
```

Tests are based on Boost Testing.
They can be run with the following command within the project folder, after
building the project.

Either via CTest with
```
export BOOST_TEST_CATCH_SYSTEM_ERRORS="no"
make -C build test

```
or directly

```
export BOOST_TEST_CATCH_SYSTEM_ERRORS="no"
./build/test/owlapi-test

```

## COPYRIGHT
Copyright (c) 2013-2019 Thomas M Roehr and Contributors, DFKI GmbH Robotics Innovation Center

## LICENSE
This project is licensed under [LGPL v2.1 or later](https://www.gnu.org/licenses/lgpl-2.1.txt)

