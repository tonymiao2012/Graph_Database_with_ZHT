/*
 * property.cpp
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#include "model/types.h"
#include "model/property.h"
#include "model/prop_value.h"
#include "common.h"
#include <iostream>
using namespace std;
using namespace graphdb;

Property::Property(string name, SupportedDataType dataType) :
		Object(), name(name), dataType(dataType) {

}

Property::~Property() {
}

string Property::getName() {
	return this->name;
}

SupportedDataType Property::getDataType() {
	return this->dataType;
}

void Property::setName(string name) {
	this->name = name;
}

void Property::setDataType(SupportedDataType type) {
	this->dataType = type;
}

string Property::getUUID() {
	string dt = dataTypeToString(this->dataType);
	return "Property" + SEP1 + this->name + SEP1 + dt;
}

string Property::toString() {
	return this->getUUID();
}

string Property::getUUID(string name, SupportedDataType dt) {
	string sdt = dataTypeToString(dt);
	return "Property" + SEP1 + name + SEP1 + sdt;
}


bool Property::operator==(const Object &obj) {
	if (!instanceOf<Property>(obj))
		return false;

	const Property &prop = dynamic_cast<const Property&>(obj);
	return (this->dataType == prop.dataType && this->name == prop.name);
}


