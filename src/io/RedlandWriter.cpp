#include "RedlandWriter.hpp"
#include <base-logging/Logging.hpp>
#include "../OWLApi.hpp"
#include "../model/OWLDataOneOf.hpp"


using namespace owlapi::model;

namespace owlapi {
namespace io {

RedlandVisitor::RedlandVisitor(raptor_world* world, raptor_serializer* serializer)
    : OWLAxiomVisitor()
    , mWorld(world)
    , mSerializer(serializer)
{}

RedlandVisitor::~RedlandVisitor()
{
    while(!mRestrictions.empty())
    {
        Restriction2Term::iterator it = mRestrictions.begin();
        raptor_free_term(it->second);
        mRestrictions.erase(it);
    }
}

void RedlandVisitor::writeTriple(const owlapi::model::IRI& subject,
        const owlapi::model::IRI& predicate,
        const owlapi::model::OWLLiteral::Ptr& literal) const
{
    writeTripleWithLiteral(termFromIRI(subject), predicate, literal);
}

void RedlandVisitor::writeTriple(const owlapi::model::IRI& subject,
        const owlapi::model::IRI& predicate,
        const owlapi::model::IRI& object) const
{
    raptor_statement* triple = raptor_new_statement(mWorld);
    triple->subject = termFromIRI(subject);
    triple->predicate = termFromIRI(predicate);
    triple->object = termFromIRI(object);

    raptor_serializer_serialize_statement(mSerializer, triple);
    raptor_free_statement(triple);
}

raptor_term* RedlandVisitor::writeSequence(const owlapi::model::IRIList& list)
{
    owlapi::model::IRIList rest = list;
    if(list.empty())
    {
        return termFromIRI(vocabulary::RDF::nil());
    } else {
        raptor_term* anonymous = raptor_new_term_from_blank(mWorld, NULL);
        writeAnonymous(anonymous, vocabulary::RDF::first(), *rest.begin());

        // Write rest
        rest.erase(rest.begin());
        raptor_term* termRest = writeSequence(rest);
        writeAnonymous(anonymous, vocabulary::RDF::rest(), termRest);
        raptor_free_term(termRest);
        return anonymous;
    }
}

void RedlandVisitor::visit(const OWLDeclarationAxiom& axiom)
{
    OWLEntity::Ptr entity = axiom.getEntity();

    IRI iri = entity->getIRI();
    switch(entity->getEntityType())
    {
        case OWLEntity::CLASS:
        {
            if(iri == vocabulary::OWL::Class() || iri == vocabulary::OWL::Thing())
            {
                break;
            }
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::Class());
            break;
        }
        case OWLEntity::OBJECT_PROPERTY:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::ObjectProperty());
            break;
        case OWLEntity::DATA_PROPERTY:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::DatatypeProperty());
            break;
        case OWLEntity::ANNOTATION_PROPERTY:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::AnnotationProperty());
            break;
        case OWLEntity::DATATYPE:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::RDFS::Datatype());
            break;
        case OWLEntity::NAMED_INDIVIDUAL:
            writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::NamedIndividual());
            break;
        default:
            break;
    }
}

raptor_term* RedlandVisitor::writeAnonymous(raptor_term* anonymous,
        const owlapi::model::IRI& predicate,
        const owlapi::model::IRI& object) const
{
    LOG_DEBUG_S << "Write anonymous: " << predicate << " " << object << " anon: " << anonymous;
    if(!anonymous)
    {
        anonymous = raptor_new_term_from_blank(mWorld, NULL);
    }

    raptor_statement* triple = raptor_new_statement(mWorld);
    triple->subject = raptor_term_copy(anonymous);
    triple->predicate = termFromIRI(predicate);
    triple->object = termFromIRI(object);

    raptor_serializer_serialize_statement(mSerializer, triple);
    raptor_free_statement(triple);

    return anonymous;
}

raptor_term* RedlandVisitor::writeAnonymous(const owlapi::model::IRI& subject,
        const owlapi::model::IRI& predicate,
        raptor_term* anonymous) const
{
    LOG_DEBUG_S << "Write anonymous: " << subject << " " << predicate;
    if(!anonymous)
    {
        anonymous = raptor_new_term_from_blank(mWorld, NULL);
    }

    raptor_statement* triple = raptor_new_statement(mWorld);
    triple->subject = termFromIRI(subject);
    triple->predicate = termFromIRI(predicate);
    triple->object = raptor_term_copy(anonymous);

    raptor_serializer_serialize_statement(mSerializer, triple);
    raptor_free_statement(triple);
    return anonymous;
}

void RedlandVisitor::writeAnonymous(raptor_term* anonymous,
        const owlapi::model::IRI& predicate,
        raptor_term* anonymousObject) const
{
    LOG_DEBUG_S << "Write anonymous: " << predicate << " anon: s:" << anonymous
        << " o:" << anonymousObject;

    if(!anonymous && !anonymousObject)
    {
        throw std::invalid_argument("owlapi::io::RedlandVisitor: cannot from NULL pointer"
                "for anonymous nodes");
    }

    raptor_statement* triple = raptor_new_statement(mWorld);
    triple->subject = raptor_term_copy(anonymous);
    triple->predicate = termFromIRI(predicate);
    triple->object = raptor_term_copy(anonymousObject);

    raptor_serializer_serialize_statement(mSerializer, triple);
    raptor_free_statement(triple);
}

void RedlandVisitor::writeAnonymousLiteral(raptor_term* anonymous,
        const owlapi::model::IRI& predicate,
        const OWLLiteral::Ptr& literal) const
{
    writeTripleWithLiteral(raptor_term_copy(anonymous), predicate, literal);
}

void RedlandVisitor::writeTripleWithLiteral(raptor_term* subject,
        const owlapi::model::IRI& predicate,
        const OWLLiteral::Ptr& literal) const
{
    raptor_statement* triple = raptor_new_statement(mWorld);
    triple->subject = subject;
    triple->predicate = termFromIRI(predicate);
    raptor_uri* datatype = raptor_new_uri(mWorld, (const unsigned char*) literal->getType().c_str());

    triple->object = raptor_new_term_from_literal(mWorld,
            (const unsigned char*) literal->getValue().c_str(),
            datatype,
            NULL);

    raptor_serializer_serialize_statement(mSerializer, triple);
    raptor_free_statement(triple);
}

raptor_term* RedlandVisitor::writeRestriction(OWLRestriction::Ptr restriction, const owlapi::model::IRI& restrictionType)
{
    std::pair<OWLRestriction::Ptr, IRI> key(restriction, restrictionType);
    std::map< std::pair<OWLRestriction::Ptr, IRI>, raptor_term*>::iterator it = mRestrictions.find(key);
    if(it != mRestrictions.end())
    {
        return it->second;
    }

    raptor_term* anonymous = writeAnonymous(NULL, vocabulary::RDF::type(), vocabulary::OWL::Restriction());
    OWLNamedObject::Ptr namedObject = dynamic_pointer_cast<OWLNamedObject>( restriction->getProperty() );
    if(!namedObject)
    {
        throw std::runtime_error("owlapi::io::RedlandVisitor::writeRestriction: no support for "
                " complex property expression in restriction");
    } else {
        writeAnonymous(anonymous, vocabulary::OWL::onProperty(), namedObject->getIRI());
    }

    // Correct restriction type if cardinality is not qualified
    owlapi::model::IRI actualRestrictionType = restrictionType;
    OWLQualifiedRestriction::Ptr qualifiedRestriction = dynamic_pointer_cast<OWLQualifiedRestriction>(restriction);
    if(qualifiedRestriction)
    {
        writeAnonymous(anonymous, vocabulary::OWL::onClass(), qualifiedRestriction->getQualification());
    } else {
        if(restrictionType == vocabulary::OWL::minQualifiedCardinality())
        {
            actualRestrictionType = vocabulary::OWL::minCardinality();
        } else if(restrictionType == vocabulary::OWL::maxQualifiedCardinality())
        {
            actualRestrictionType = vocabulary::OWL::maxCardinality();
        } else if(restrictionType == vocabulary::OWL::qualifiedCardinality())
        {
            actualRestrictionType = vocabulary::OWL::cardinality();
        }
    }

    OWLCardinalityRestriction::Ptr cardinalityRestriction = dynamic_pointer_cast<OWLCardinalityRestriction>(restriction);
    if(cardinalityRestriction)
    {
        OWLLiteral::Ptr literal = OWLLiteral::nonNegativeInteger( cardinalityRestriction->getCardinality() );
        writeAnonymousLiteral(anonymous, actualRestrictionType, literal);
    }

    mRestrictions[key] = anonymous;
    return anonymous;
}

void RedlandVisitor::visit(const OWLSubClassOfAxiom& axiom)
{
    OWLClassExpression::Ptr sub = axiom.getSubClass();
    OWLClassExpression::Ptr super = axiom.getSuperClass();

    if(sub->getClassExpressionType() == OWLClassExpression::OWL_CLASS
        && super->getClassExpressionType() == OWLClassExpression::OWL_CLASS)
    {
        IRI subIRI = dynamic_pointer_cast<OWLClass>(sub)->getIRI();
        IRI superIRI = dynamic_pointer_cast<OWLClass>(super)->getIRI();
        if(superIRI != vocabulary::OWL::Thing())
        {
            writeTriple(subIRI, vocabulary::RDFS::subClassOf(), superIRI);
        }

        return;
    }

    std::vector<raptor_term*> restrictionTerms;

    owlapi::model::OWLRestriction::Ptr restriction = dynamic_pointer_cast<OWLRestriction>(super);

    switch(super->getClassExpressionType())
    {
        case OWLClassExpression::OWL_CLASS:
            throw std::runtime_error("owlapi::io::RedlandWriter: no support for complex subclass expression");

        case OWLClassExpression::DATA_ALL_VALUES_FROM:
        case OWLClassExpression::DATA_EXACT_CARDINALITY:
        case OWLClassExpression::DATA_HAS_VALUE:
        case OWLClassExpression::DATA_MAX_CARDINALITY:
        case OWLClassExpression::DATA_MIN_CARDINALITY:
        case OWLClassExpression::DATA_SOME_VALUES_FROM:
        case OWLClassExpression::OBJECT_ALL_VALUES_FROM:
        case OWLClassExpression::OBJECT_COMPLEMENT_OF:
        case OWLClassExpression::OBJECT_HAS_SELF:
        case OWLClassExpression::OBJECT_HAS_VALUE:
        case OWLClassExpression::OBJECT_INTERSECTION_OF:
        case OWLClassExpression::OBJECT_ONE_OF:
        case OWLClassExpression::OBJECT_SOME_VALUES_FROM:
        case OWLClassExpression::OBJECT_UNION_OF:
            throw std::runtime_error("owlapi::io::RedlandWriter: no support for expression: "
                    + OWLClassExpression::TypeTxt[ super->getClassExpressionType() ] );

        case OWLClassExpression::OBJECT_EXACT_CARDINALITY:
        {
            raptor_term* exactRestriction = writeRestriction(restriction, vocabulary::OWL::qualifiedCardinality());
            restrictionTerms.push_back(exactRestriction);
            break;
        }
        case OWLClassExpression::OBJECT_MAX_CARDINALITY:
        {
            raptor_term* maxRestriction = writeRestriction(restriction, vocabulary::OWL::maxQualifiedCardinality());
            restrictionTerms.push_back(maxRestriction);
            break;
        }
        case OWLClassExpression::OBJECT_MIN_CARDINALITY:
        {
            raptor_term* minRestriction = writeRestriction(restriction, vocabulary::OWL::minQualifiedCardinality());
            restrictionTerms.push_back(minRestriction);
            break;
        }
        default:
            throw std::runtime_error("owlapi::io::RedlandWriter: no support for expression: "
                    + OWLClassExpression::TypeTxt[ super->getClassExpressionType() ] );
    }

    switch(sub->getClassExpressionType())
    {
        case OWLClassExpression::OWL_CLASS:
        {
            IRI subIRI = dynamic_pointer_cast<OWLClass>(sub)->getIRI();
            std::vector<raptor_term*>::const_iterator cit = restrictionTerms.begin();
            for(; cit != restrictionTerms.end(); ++cit)
            {
                writeAnonymous(subIRI, vocabulary::RDFS::subClassOf(), *cit);
            }
            break;
        }
        default:
            throw std::runtime_error("owlapi::io::RedlandWriter: no support for complex subclass expression");
    }
}

void RedlandVisitor::visit(const OWLSubObjectPropertyOfAxiom& axiom)
{
    IRI subIRI = dynamic_pointer_cast<OWLObjectProperty>(axiom.getSubProperty())->getIRI();
    IRI superIRI = dynamic_pointer_cast<OWLObjectProperty>(axiom.getSuperProperty())->getIRI();

    writeTriple(subIRI, vocabulary::RDFS::subPropertyOf(), superIRI);
}

void RedlandVisitor::visit(const OWLSubDataPropertyOfAxiom& axiom)
{
    IRI subIRI = dynamic_pointer_cast<OWLDataProperty>(axiom.getSubProperty())->getIRI();
    IRI superIRI = dynamic_pointer_cast<OWLDataProperty>(axiom.getSuperProperty())->getIRI();

    writeTriple(subIRI, vocabulary::RDFS::subPropertyOf(), superIRI);
}

void RedlandVisitor::visit(const owlapi::model::OWLObjectPropertyDomainAxiom& axiom)
{
    IRI propertyIRI = dynamic_pointer_cast<OWLObjectProperty>(axiom.getProperty())->getIRI();

    OWLClassExpression::Ptr e_klass = dynamic_pointer_cast<OWLClassExpression>(axiom.getDomain());
    if(e_klass->getClassExpressionType() != OWLClassExpression::OWL_CLASS)
    {
        throw std::runtime_error("owlapi::io::RedlandVisitor: cannot handle complex class expression as domain");
    }

    OWLClass::Ptr klass = dynamic_pointer_cast<OWLClass>(e_klass);
    writeTriple(propertyIRI, vocabulary::RDFS::domain(), klass->getIRI());
}

void RedlandVisitor::visit(const owlapi::model::OWLObjectPropertyRangeAxiom& axiom)
{
    IRI propertyIRI = dynamic_pointer_cast<OWLObjectProperty>(axiom.getProperty())->getIRI();

    OWLClassExpression::Ptr e_klass = dynamic_pointer_cast<OWLClassExpression>(axiom.getRange());
    if(e_klass->getClassExpressionType() != OWLClassExpression::OWL_CLASS)
    {
        throw std::runtime_error("owlapi::io::RedlandVisitor: cannot handle complex class expression as range");
    }

    OWLClass::Ptr klass = dynamic_pointer_cast<OWLClass>(e_klass);
    writeTriple(propertyIRI, vocabulary::RDFS::range(), klass->getIRI());
}

void RedlandVisitor::visit(const owlapi::model::OWLDataPropertyDomainAxiom& axiom)
{
    IRI propertyIRI = dynamic_pointer_cast<OWLDataProperty>(axiom.getProperty())->getIRI();

    OWLClassExpression::Ptr e_klass = dynamic_pointer_cast<OWLClassExpression>(axiom.getDomain());
    if(e_klass->getClassExpressionType() != OWLClassExpression::OWL_CLASS)
    {
        throw std::runtime_error("owlapi::io::RedlandVisitor: cannot handle complex class expression as data property domain");
    }

    OWLClass::Ptr klass = dynamic_pointer_cast<OWLClass>(e_klass);
    writeTriple(propertyIRI, vocabulary::RDFS::domain(), klass->getIRI());
}

void RedlandVisitor::visit(const owlapi::model::OWLDataPropertyRangeAxiom& axiom)
{
    IRI propertyIRI = dynamic_pointer_cast<OWLDataProperty>(axiom.getProperty())->getIRI();

    OWLDataRange::Ptr e_range = axiom.getRange();
    OWLDataRange::Type dataRangeType = e_range->getDataRangeType();
    switch(dataRangeType)
    {
        case OWLDataRange::DATATYPE:
        {
            OWLDataType::Ptr dataType = dynamic_pointer_cast<OWLDataType>(e_range);
            writeTriple(propertyIRI, vocabulary::RDFS::range(), dataType->getIRI());
            break;
        }
        case OWLDataRange::DATA_ONE_OF:
        {
            OWLDataOneOf::Ptr oneOf = dynamic_pointer_cast<OWLDataOneOf>(e_range);

            raptor_term* typeId = raptor_new_term_from_blank(mWorld, NULL);
            writeAnonymous(propertyIRI, vocabulary::RDFS::range(), typeId);
            writeAnonymous(typeId, vocabulary::RDF::type(), vocabulary::RDFS::Datatype());
            IRIList iris = OWLLiteral::toIRIList( oneOf->getLiterals() );
            raptor_term* term = writeSequence(iris);
            writeAnonymous(typeId, vocabulary::OWL::oneOf(), term);
            break;
        }
        default:
            throw std::runtime_error("owlapi::io::RedlandVisitor:visit: no "
                    "support for datarange type: "
                    + OWLDataRange::TypeTxt[dataRangeType]);
            break;
    }

}

void RedlandVisitor::visit(const owlapi::model::OWLInverseObjectPropertiesAxiom& axiom)
{
    IRI first = dynamic_pointer_cast<OWLObjectProperty>(axiom.getFirstProperty())->getIRI();
    IRI second = dynamic_pointer_cast<OWLObjectProperty>(axiom.getSecondProperty())->getIRI();

    writeTriple(first, vocabulary::OWL::inverseOf(), second);
}

void RedlandVisitor::visit(const owlapi::model::OWLFunctionalObjectPropertyAxiom& axiom)
{
    writeObjectProperty(axiom, vocabulary::OWL::FunctionalProperty());
}

void RedlandVisitor::visit(const owlapi::model::OWLInverseFunctionalObjectPropertyAxiom& axiom)
{
    writeObjectProperty(axiom, vocabulary::OWL::InverseFunctionalProperty());
}

void RedlandVisitor::visit(const owlapi::model::OWLReflexiveObjectPropertyAxiom& axiom)
{
    writeObjectProperty(axiom, vocabulary::OWL::ReflexiveProperty());
}

void RedlandVisitor::visit(const owlapi::model::OWLIrreflexiveObjectPropertyAxiom& axiom)
{
    writeObjectProperty(axiom, vocabulary::OWL::IrreflexiveProperty());
}

void RedlandVisitor::visit(const owlapi::model::OWLSymmetricObjectPropertyAxiom& axiom)
{
    writeObjectProperty(axiom, vocabulary::OWL::SymmetricProperty());
}

void RedlandVisitor::visit(const owlapi::model::OWLAsymmetricObjectPropertyAxiom& axiom)
{
    writeObjectProperty(axiom, vocabulary::OWL::AsymmetricProperty());
}

void RedlandVisitor::visit(const owlapi::model::OWLTransitiveObjectPropertyAxiom& axiom)
{
    writeObjectProperty(axiom, vocabulary::OWL::TransitiveProperty());
}

void RedlandVisitor::visit(const owlapi::model::OWLFunctionalDataPropertyAxiom& axiom)
{
    IRI iri = dynamic_pointer_cast<OWLDataProperty>(axiom.getProperty())->getIRI();
    writeTriple(iri, vocabulary::RDF::type(), vocabulary::OWL::FunctionalProperty());
}

void RedlandVisitor::visit(const owlapi::model::OWLObjectPropertyAssertionAxiom& axiom)
{
    OWLIndividual::Ptr subject = axiom.getSubject();
    OWLProperty::Ptr property = dynamic_pointer_cast<OWLProperty>(axiom.getProperty());
    OWLPropertyAssertionObject::Ptr assertionObject = axiom.getObject();

    IRI iri;
    OWLIndividual::Ptr individual = dynamic_pointer_cast<OWLIndividual>(assertionObject);
    if(individual)
    {
        iri = individual->getReferenceID();
    } else {
        OWLLiteral::Ptr literal = dynamic_pointer_cast<OWLLiteral>(assertionObject);
        if(literal)
        {
            iri = IRI( literal->toString() );
        }
    }
    if(iri.empty())
    {
        throw std::runtime_error("owlapi::model::RedlandVisitor::visit:"
                " could not extract object from ObjectPropertyAssertionAxiom");
    }

    writeTriple(subject->getReferenceID(), property->getIRI(), iri);
}

void RedlandVisitor::visit(const owlapi::model::OWLDataPropertyAssertionAxiom& axiom)
{
    OWLIndividual::Ptr subject = axiom.getSubject();
    OWLProperty::Ptr property = dynamic_pointer_cast<OWLProperty>(axiom.getProperty());
    OWLPropertyAssertionObject::Ptr assertionObject = axiom.getObject();

    OWLLiteral::Ptr literal = dynamic_pointer_cast<OWLLiteral>(assertionObject);
    if(!literal)
    {
        throw std::runtime_error("owlapi::model::RedlandVisitor::visit:"
                " could not extract literal from DataPropertyAssertionAxiom");
    }

    writeTriple(subject->getReferenceID(), property->getIRI(), literal);
}


void RedlandVisitor::visit(const owlapi::model::OWLClassAssertionAxiom& axiom)
{
    OWLClassExpression::Ptr e_klass = axiom.getClassExpression();
    if(e_klass->getClassExpressionType() != OWLClassExpression::OWL_CLASS)
    {
        throw std::runtime_error("owlapi::io::RedlandVisitor: cannot handle complex class expression as class assertion");
    }
    OWLClass::Ptr klass = dynamic_pointer_cast<OWLClass>(e_klass);

    OWLIndividual::Ptr individual = axiom.getIndividual();
    IRI individualIRI = individual->getReferenceID();

    writeTriple(individualIRI, vocabulary::RDF::type(), klass->getIRI());
}

void RedlandVisitor::visit(const owlapi::model::OWLSubAnnotationPropertyOfAxiom& axiom)
{
}

void RedlandVisitor::visit(const owlapi::model::OWLAnnotationPropertyDomainAxiom& axiom)
{

}

void RedlandVisitor::visit(const owlapi::model::OWLAnnotationPropertyRangeAxiom& axiom)
{
}

void RedlandVisitor::visit(const owlapi::model::OWLAnnotationAssertionAxiom& axiom)
{
}

raptor_term* RedlandVisitor::termFromIRI(const owlapi::model::IRI& iri) const
{
    return raptor_new_term_from_uri_string(mWorld, (const unsigned char*) iri.toString().c_str());
}

RedlandWriter::RedlandWriter()
    : OWLWriter("rdfxml")
    , mWorld(raptor_new_world())
    , mSerializer(NULL)
{}

RedlandWriter::~RedlandWriter()
{
    raptor_free_world(mWorld);
}

std::vector<std::string> RedlandWriter::getSupportedFormats() const
{
    std::vector<std::string> formats;
    int i = 0;
    while(true)
    {
        const raptor_syntax_description* sd = raptor_world_get_serializer_description(mWorld, i++);
        if(sd == NULL)
        {
            break;
        }
        LOG_DEBUG_S << "format: " << sd->names[0] << " , description: " <<  sd->label;
        formats.push_back(std::string(sd->names[0]));
    }
    return formats;
}


void RedlandWriter::write(const std::string& filename, const owlapi::model::OWLOntology::Ptr& ontology, bool includeImports) const
{
    // rdfxml
    const char *serializer_syntax_name = mFormat.c_str();
    unsigned char* uri_string = raptor_uri_filename_to_uri_string(filename.c_str());
    raptor_uri* base_uri = raptor_new_uri(mWorld, uri_string);

    mSerializer = raptor_new_serializer(mWorld, serializer_syntax_name);
    raptor_serializer_start_to_filename(mSerializer, filename.c_str());

    RedlandVisitor visitor(mWorld, mSerializer);

    // Set the ontologies name and set imports for this name
    if(!ontology->getIRI().empty())
    {
        visitor.writeTriple(ontology->getIRI(), vocabulary::RDF::type(), vocabulary::OWL::Ontology());

        IRIList imports = ontology->getDirectImportsDocuments();
        IRIList::const_iterator cit = imports.begin();
        for(; cit != imports.end(); ++cit)
        {
            const IRI& importDocument = *cit;
            visitor.writeTriple(ontology->getIRI(), vocabulary::OWL::imports(), importDocument);
        }
    }

    // Visit all axioms that are part of this ontology
    const owlapi::model::OWLAxiom::PtrList& axioms = ontology->getAxioms();

    OWLAxiom::PtrList::const_iterator cit = axioms.begin();
    for(; cit != axioms.end(); ++cit)
    {
        const OWLAxiom::Ptr& axiom = *cit;
        // Store only axioms that can be mapped to this ontology
        if(includeImports || axiom->getOrigin() == ontology->getIRI())
        {
            axiom->accept(&visitor);
        } else {
            LOG_DEBUG_S << "Skipping writing axiom since:"
                << " includeImports: " << includeImports << ", and "
                << "origin: " << axiom->getOrigin()
                << " iri: " << ontology->getIRI();
        }
    }
    raptor_serializer_serialize_end(mSerializer);
    raptor_free_serializer(mSerializer);

    raptor_free_uri(base_uri);
    raptor_free_memory(uri_string);

}

} // end namespace io
} // end namespace owlapi

