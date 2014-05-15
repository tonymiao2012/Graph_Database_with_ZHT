/*
 * node.cpp
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#include "model/node.h"
#include "model/object.h"
#include "model/prop_host.h"
#include "helper/helper.h"
#include "model/types.h"
#include "common.h"
#include "manager//manager.h"
#include <iostream>

using namespace std;
using namespace graphdb;

Node::Node(long id) :
		Object(), PropertyHost(), id(id), outRelations(new map<string, set<string>*>), inRelations(new map<string, set<string>*>), properties(new map<string, Object*>) {
}

Node::~Node() {
	//deep free out relations
	map<string, set<string>*> *outRels = this->outRelations;
	for(map<string, set<string>*>::iterator it = outRels->begin(); it != outRels->end(); it++)
		destroy(it->second);
	destroy(outRels);

	//deep free in relations
	map<string, set<string>*> *inRels = this->inRelations;
	for(map<string, set<string>*>::iterator it = inRels->begin(); it != inRels->end(); it++)
		destroy(it->second);
	destroy(inRels);

	//deep free properties
	map<string, Object*> *props = this->properties;
	for(map<string, Object*>::iterator it = props->begin(); it != props->end(); it++)
		destroy(it->second);
	destroy(props);
}

long Node::getId() {
	return this->id;
}

long Node::getId(string UUID) {
	return stringToLong(split(UUID, stringToChar(SEP1))[1]);
}

void Node::addRelationTo(Node &node, Relation &relation) {
	string type = relation.getType();

	// add as out relation of this node
	map<string, set<string>*>::iterator it = this->outRelations->find(type);
	if (it == this->outRelations->end()) {
		(*this->outRelations)[type] = new set<string>;
	}
	(*this->outRelations)[type]->insert(relation.getUUID());

	// add as in relation of the other node
	it = node.inRelations->find(type);
	if (it == node.inRelations->end()) {
		(*node.inRelations)[type] = new set<string>;
	}
	(*node.inRelations)[type]->insert(relation.getUUID());

	// set start and end nodes of the relation
	relation.setStartNode(this->getUUID());
	relation.setEndNode(node.getUUID());
}

bool Node::hasRelation() {
	return (hasRelation(OUT) || hasRelation(IN));
}

bool Node::hasRelation(Direction direction) {
	bool result = false;
	switch (direction) {
	case OUT:
		result = !(this->outRelations->empty());
		break;
	case IN:
		result = !(this->inRelations->empty());
		break;
	case BOTH:
		result = !(this->outRelations->empty() || this->inRelations->empty());
		break;
	}
	return result;
}

bool Node::hasRelation(string type) {
	return (hasRelation(type, OUT) || hasRelation(type, IN));
}

bool Node::hasRelation(string type, Direction direction) {
	bool result = false;
	switch (direction) {
	case OUT: {
		map<string, set<string>*>::iterator it = this->outRelations->find(type);
		result = (it != this->outRelations->end());
	}
		break;
	case IN: {
		map<string, set<string>*>::iterator it = this->inRelations->find(type);
		result = (it != this->inRelations->end());
	}
		break;
	case BOTH: {
		map<string, set<string>*>::iterator itOut = this->outRelations->find(type);
		map<string, set<string>*>::iterator itIn = this->inRelations->find(type);
		bool hasOut = itOut != this->outRelations->end();
		bool hasIn = itIn != this->inRelations->end();
		result = (hasOut && hasIn);
	}
		break;
	}
	return result;
}

set<string> Node::getRelations() {
	return getRelations(BOTH);
}

set<string> Node::getRelations(Direction direction) {
	set<string> relUUIDs = set<string>();
	switch (direction) {
	case OUT: {
		map<string, set<string>*>::iterator it = this->outRelations->begin();
		for (; it != this->outRelations->end(); it++) {
			set<string>::iterator itr = it->second->begin();
			for (; itr != it->second->end(); itr++) {
				relUUIDs.insert(*itr);
			}
		}
	}
		break;
	case IN: {
		map<string, set<string>*>::iterator it = this->inRelations->begin();
		for (; it != this->inRelations->end(); it++) {
			set<string>::iterator itr = it->second->begin();
			for (; itr != it->second->end(); itr++) {
				relUUIDs.insert(*itr);
			}
		}
	}
		break;
	case BOTH: {
		map<string, set<string>*>::iterator it = this->outRelations->begin();
		for (; it != this->outRelations->end(); it++) {
			set<string>::iterator itr = it->second->begin();
			for (; itr != it->second->end(); itr++) {
				relUUIDs.insert(*itr);
			}
		}
		it = this->inRelations->begin();
		for (; it != this->inRelations->end(); it++) {
			set<string>::iterator itr = it->second->begin();
			for (; itr != it->second->end(); itr++) {
				relUUIDs.insert(*itr);
			}
		}
	}
		break;
	}
	return relUUIDs;
}

set<string> Node::getRelations(string type) {
	return getRelations(type, BOTH);
}

set<string> Node::getRelations(string type, Direction direction) {
	set<string> relUUIDs = set<string>();
	switch (direction) {
	case OUT: {
		map<string, set<string>*>::iterator it = this->outRelations->find(type);
		if (it != this->outRelations->end()) {
			relUUIDs = *it->second;
		}
	}
		break;
	case IN: {
		map<string, set<string>*>::iterator it = this->inRelations->find(type);
		if (it != this->inRelations->end()) {
			relUUIDs = *it->second;
		}
	}
		break;
	case BOTH: {
		map<string, set<string>*>::iterator it = this->outRelations->find(type);
		if (it != this->outRelations->end()) {
			relUUIDs = *it->second;
		}
		it = this->inRelations->find(type);
		if (it != this->inRelations->end()) {
			set<string>::iterator itr = it->second->begin();
			for (; itr != it->second->end(); itr++) {
				relUUIDs.insert(*itr);
			}
		}
	}
		break;
	}
	return relUUIDs;
}

map<string, set<string>*> *Node::getOutRelations()
{
	return this->outRelations;
}

map<string, set<string>*> *Node::getInRelations()
{
	return this->inRelations;
}

void Node::removeRelation(long id)
{
	// search out relations
	map<string, set<string>*> *outRelations = this->outRelations;
	for(map<string, set<string>*>::iterator it = outRelations->begin(); it != outRelations->end(); it++)
	{
		set<string> *outRels = it->second;
		if(outRels != NULL)
			for(set<string>::iterator iit = outRels->begin(); iit != outRels->end(); iit++)
			{
				string UUID = *iit;
				if(stringToLong(split(UUID, stringToChar(SEP1))[1]) == id)
				{
					outRels->erase(iit);
					if(outRels->size() == 0)
					{
						outRelations->erase(it);
						destroy(outRels);
					}
					return;
				}
			}
	}

	// search in relations
	map<string, set<string>*> *inRelations = this->inRelations;
	for(map<string, set<string>*>::iterator it = inRelations->begin(); it != inRelations->end(); it++)
	{
		set<string> *inRels = it->second;
		if(inRels != NULL)
			for(set<string>::iterator iit = inRels->begin(); iit != inRels->end(); iit++)
			{
				string UUID = *iit;
				if(stringToLong(split(UUID, stringToChar(SEP1))[1]) == id)
				{
					inRels->erase(iit);
					if(inRels->size() == 0)
					{
						inRelations->erase(it);
						destroy(inRels);
					}
					return;
				}
			}
	}
}

bool Node::hasProperty(string name, SupportedDataType dt) {
	string pUUID = Property::getUUID(name, dt);
	map<string, Object*>::iterator it = this->properties->find(pUUID);
	return (it != this->properties->end());
}

Object *Node::getPropertyValue(string name, SupportedDataType dt) {
	string pUUID = Property::getUUID(name, dt);
	map<string, Object*>::iterator it = this->properties->find(pUUID);
	if (it != this->properties->end()) {
		return it->second;
	} else {
		return 0;
	}
}

map<string, Object*> *Node::getProperties() {
	return this->properties;
}

void Node::addProperty(string name, SupportedDataType dt, Object *propVal) {
	(*this->properties)[Property::getUUID(name, dt)] = propVal;
}

void Node::removeProperty(string name, SupportedDataType dt) {
	string pUUID = Property::getUUID(name, dt);
	destroy((*this->properties)[pUUID]);
	this->properties->erase(pUUID);
}

string Node::getUUID() {
	return "Node" + SEP1 + stringValue(this->id);
}

string Node::getUUID(long id) {
	return "Node" + SEP1 + stringValue(id);
}

string Node::toString() {
	string node;
	node.append(stringValue(this->id));
	node.append(toString(this->outRelations));
	node.append(toString(this->inRelations));

	map<string, Object*>::iterator it = this->properties->begin();
	for (; it != this->properties->end(); it++) {
		node.append(SEP0);
		node.append(it->first);
	}
	return node;
}

bool Node::operator==(const Object &obj) {
	if (!instanceOf<Node>(obj))
		return false;

	const Node &n = dynamic_cast<const Node&>(obj);
	return (this->id == n.id);
}

string Node::toString(map<string, set<string>*> *rels) {
	string strRels;
	int numGroupsOfRels = 0;
	map<string, set<string>*>::iterator it = rels->begin();
	for (; it != rels->end(); it++) {
		strRels.append(SEP0);
		strRels.append(it->first);
		set<string>::iterator itr = it->second->begin();
		for (; itr != it->second->end(); itr++) {
			strRels.append(SEP2);
			strRels.append(*itr);
		}
		numGroupsOfRels++;
	}
	string result;
	result.append(SEP0);
	result.append(stringValue(numGroupsOfRels));
	if (numGroupsOfRels > 0) {
		result.append(strRels);
	}
	return result;
}
