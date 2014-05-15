/*
 * object.cpp
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#include "model/object.h"

using namespace graphdb;
using namespace std;

Object::Object() {
}

Object::~Object() {
}

string Object::getUUID() {
	return "";
}

string Object::toString() {
	return "";
}

bool Object::operator==(const Object &obj) {
	if (this == &obj)
		return true;
	else
		return false;
}

