/*
 * helper.cpp
 *
 *      Author: zephyr
 */

#include "helper/helper.h"
#include "model/types.h"
#include <iostream>

using namespace std;
using namespace graphdb;

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


graphdb::Node *deserializeNode(const std::string &s) {
	Manager *manager = Manager::getInstance();
	vector<string> data = split(s, stringToChar(SEP0));

	int offset = 0;
	//deserialize id
	long id = stringToLong(data[offset]);
	Node *node = new Node(id);

	//deserialize number of out relations
	offset = 1;
	int numOutRel = stringToInt(data[offset]);

	//deserialize out relations, only deserialize UUID of relations, grouped by type
	offset++;
	map<string, set<string>*> *outRels = node->getOutRelations();
	for(int i = offset; i < offset + numOutRel; i++)
	{
		vector<string> rel = split(data[i], stringToChar(SEP2));
		string type = rel[0];
		//insert relation UUID to map with type as key
		map<string, set<string>*>::iterator it = outRels->find(type);
		if(it == outRels->end())
			(*outRels)[type] = new set<string>;
		for(unsigned int j = 1; j < rel.size(); j++)
			(*outRels)[type]->insert(rel[j]);
	}

	//deserialize number of in relations
	offset += numOutRel;
	int numInRel = stringToInt(data[offset]);

	//deserialize in relations
	offset++;
	map<string, set<string>*> *inRels = node->getInRelations();
	for(int i = offset; i < offset + numInRel; i++)
	{
		vector<string> rel = split(data[i], stringToChar(SEP2));
		string type = rel[0];
		//insert relation UUID to map with type as key
		map<string, set<string>*>::iterator it = inRels->find(type);
		if(it == inRels->end())
			(*inRels)[type] = new set<string>;
		for(unsigned int j = 1; j < rel.size(); j++)
			(*inRels)[type]->insert(rel[j]);
	}

	//deserialize properties, deserialize all the properties values at the same time
	offset += numInRel;
	map<string, Object*> *properties = node->getProperties();

	for(unsigned int i = offset; i < data.size(); i++)
	{
		string pUUID = data[i];
		string key = data[i] + SEP1 + Node::getUUID(id);
		(*properties)[pUUID] = manager->getPropertyValue(key);
	}
	return node;
}

graphdb::Relation *deserializeRelation(const std::string &s) {
	Manager *manager = Manager::getInstance();
	vector<string> data = split(s, stringToChar(SEP0));

	int offset = 0;
	//deserialize id
	long id = stringToLong(data[offset]);

	//deserialize type
	offset++;
	string type = data[offset];
	Relation *rel = new Relation(id,type);

	//deserialize start node, only deserialize the uuid of node, can get node info later
	offset++;
	if(offset < data.size())
		rel->setStartNode(data[offset]);

	//deserialize end node
	offset++;
	if(offset < data.size())
		rel->setEndNode(data[offset]);

	//deserialize properties, deserialize all the properties values at the same time
	map<string, Object*> *properties = rel->getProperties();
	offset++;
	for(unsigned int i = offset; i < data.size(); i++)
	{
		string pUUID = data[i];
		string key = data[i] + SEP1 + Relation::getUUID(id);
		(*properties)[pUUID] = manager->getPropertyValue(key);
	}
	return rel;
}

