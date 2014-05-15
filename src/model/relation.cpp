/*
 * realtion.cpp
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#include "model/relation.h"
#include "helper/helper.h"
#include "model/object.h"
#include "model/prop_host.h"
#include "wrapper/zht_wrapper.h"
#include "common.h"
#include "model/types.h"
#include "manager//manager.h"

using namespace std;
using namespace graphdb;

Relation::Relation(long id, string type) :
		Object(), PropertyHost(), id(id), type(type), properties(new map<string, Object*>) {
}

Relation::~Relation() {
	map<string, Object*>::iterator it;
	for (it = this->properties->begin(); it != this->properties->end(); it++) {
		destroy(it->second);
	}
	destroy(this->properties);

}

long Relation::getId() {
	return this->id;
}

long Relation::getId(string UUID) {
	return stringToLong(split(UUID, stringToChar(SEP1))[1]);
}

string Relation::getType() {
	return this->type;
}

string Relation::getStartNode() {
	return this->start;
}

void Relation::setStartNode(string node) {
	this->start = node;
}

string Relation::getEndNode() {
	return this->end;
}

void Relation::setEndNode(string node) {
	this->end = node;
}


bool Relation::hasProperty(string name, SupportedDataType dt) {
	string pUUID = Property::getUUID(name, dt);
	map<string, Object*>::iterator it = this->properties->find(pUUID);
	return (it != this->properties->end());
}

Object *Relation::getPropertyValue(string name, SupportedDataType dt) {
	string pUUID = Property::getUUID(name, dt);
	map<string, Object*>::iterator it = this->properties->find(pUUID);
	if (it != this->properties->end()) {
		return it->second;
	} else {
		return 0;
	}
}

map<string, Object*> *Relation::getProperties() {
	return this->properties;
}

void Relation::addProperty(string name, SupportedDataType dt, Object *propVal) {
	Manager *m = Manager::getInstance();
	(*this->properties)[Property::getUUID(name, dt)] = propVal;
}

void Relation::removeProperty(string name, SupportedDataType dt) {
	string pUUID = Property::getUUID(name, dt);
	destroy((*this->properties)[pUUID]);
	this->properties->erase(pUUID);
}

string Relation::getUUID() {
	return "Relation" + SEP1 + stringValue(this->id);
}

string Relation::getUUID(long id) {
	return "Relation" + SEP1 + stringValue(id);
}

string Relation::toString() {
	string rel;
	rel.append(stringValue(this->id));
	rel.append(SEP0);
	rel.append(this->type);
	rel.append(SEP0);
	rel.append(this->start);
	rel.append(SEP0);
	rel.append(this->end);
	map<string, Object*>::iterator it;
	for (it = this->properties->begin(); it != this->properties->end(); it++) {
		rel.append(SEP0);
		rel.append(it->first);
	}

	return rel;
}

bool Relation::operator==(const Object &obj) {
	if (!instanceOf<Relation>(obj))
		return false;

	const Relation &r = dynamic_cast<const Relation&>(obj);
	return (this->id == r.id);
}

