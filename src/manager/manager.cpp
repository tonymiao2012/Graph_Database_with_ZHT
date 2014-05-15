/*
 * manager.cpp
 *
 *      Author: zephyr
 */

#include "manager/manager.h"
#include "common.h"
#include "log/logger.h"
#include "model/types.h"
#include "model/node.h"
#include "model/object.h"
#include "model/property.h"
#include "model/relation.h"
#include "model/prop_value.h"
#include "wrapper/zht_wrapper.h"
#include "helper/helper.h"
#include <iostream>

using namespace graphdb;
using namespace std;

Manager *Manager::manager = NULL;
int Manager::nodeCount;
int Manager::relationCount;
int Manager::propertyCount;

Manager *Manager::getInstance()
{
	if(!manager)
	{
		manager = new Manager();
		return manager;
	}
	else
		return manager;
}

Manager::Manager() {
	initManager();
}

Manager::~Manager() {
	LOG_DEBUG("Destructing Manager");
}

void Manager::initManager()
{
	this->nodeCount = 0;//TODO should read from zht
	this->propertyCount = 0;
	this->relationCount = 0;
}

Node* Manager::createNode(const long id)
{
	Node *node = new Node(id);
	this->nodeCount++;
	return node;
}

Relation* Manager::createRelation(const long id, const string &type)
{
	Relation *rel = new Relation(id,type);
	this->relationCount++;
	return rel;
}

Node *Manager::getNode(const long id)
{
	ZHT *zht = ZHT::getInstance();
	string nodeUUID = Node::getUUID(id);
	string result;
	int rc = zht->lookupKV(nodeUUID, result);
	if(rc != 0 || result.size() < 2)
		return NULL;
	Node *node = deserializeNode(result);
	return node;
}

Relation *Manager::getRelation(const long id)
{
	ZHT *zht = ZHT::getInstance();
	string relationUUID = Relation::getUUID(id);
	string result;
	int rc = zht->lookupKV(relationUUID, result);
	if(rc != 0)
		return NULL;
	Relation *rel = deserializeRelation(result);
	return rel;
}

Object *Manager::getPropertyValue(string &pUUID)
{
	ZHT *zht = ZHT::getInstance();
	string result;
	zht->lookupKV(pUUID, result);
	char a = stringToChar(SEP1);
	SupportedDataType dt = stringToDataType(split(pUUID, a).back());
	return stringToObject(result, dt);
}

int Manager::removeNode(const long id)
{
	Manager *manager = Manager::getInstance();
	ZHT *zht = ZHT::getInstance();

	Node *node = manager->getNode(id);
	if(node == NULL)
		return -1;

	// remove associated properties from zht
	map<string, Object*> *props = node->getProperties();
	for(map<string, Object*>::iterator it = props->begin(); it != props->end(); it++)
	{
		string key = it->first + SEP1 + node->getUUID();
		zht->removeKV(key);
	}

	// remove all out and in relations of this node from zht
	set<string> rels = node->getRelations();
	for(set<string>::iterator it = rels.begin(); it != rels.end(); it++)
	{
		long relId = Relation::getId(*it);
		manager->removeRelation(relId);
	}

	// remove node itself
	string nodeUUID = node->getUUID();
	int rc = zht->removeKV(nodeUUID);
	destroy(node);
	return rc;
}

int Manager::removeRelation(const long id)
{
	Manager *manager = Manager::getInstance();
	ZHT *zht = ZHT::getInstance();

	Relation *rel = manager->getRelation(id);
	if(rel == NULL)
		return -1;

	// remove associated properties
	map<string, Object*> *props = rel->getProperties();
	for(map<string, Object*>::iterator it = props->begin(); it != props->end(); it++)
	{
		string key = it->first + SEP1 + rel->getUUID();
		zht->removeKV(key);
 	}

	// update start node
	string startUUID = rel->getStartNode();
	if(!startUUID.empty())
	{
		Node *startNode = manager->getNode(Node::getId(startUUID));
		if(startNode != NULL)
		{
			startNode->removeRelation(id);
			zht->insertObject(*startNode);
			destroy(startNode);
		}
	}

	// update end node
	string endUUID = rel->getEndNode();
	if(!endUUID.empty())
	{
		Node *endNode = manager->getNode(Node::getId(endUUID));
		if(endNode != NULL)
		{
			endNode->removeRelation(id);
			zht->insertObject(*endNode);
			destroy(endNode);
		}
	}

	// remove relation itself
	string relationUUID = Relation::getUUID(id);
	int rc = zht->removeKV(relationUUID);
	destroy(rel);
	return rc;
}

void Manager::printNode(const long id) {
	Manager *manager = Manager::getInstance();
	Node *node = manager->getNode(id);

	if(node == NULL)
	{
		cout << "Node " << id << " not found" << endl;
		return;
	}

	cout << "------------Node " << id << " Begin-----------------" << endl;

	// print all relations
	cout << "Node's Relations: " << endl;
	set<string> rels = node->getRelations();
	for(set<string>::iterator it = rels.begin(); it != rels.end(); it++)
		printRelation(Relation::getId(*it));

	// print all properties
	map<string, Object*> *properties = node->getProperties();
	for(map<string, Object*>::iterator it = properties->begin(); it != properties->end(); it++)
		cout << "Node's Property: " << it->first << " Value: " << it->second->toString() << endl;

	cout << "------------Node " << id << " Finish-----------------" << endl << endl;
	destroy(node);
}

void Manager::printRelation(const long id) {
	Manager *manager = Manager::getInstance();
	Relation *rel = manager->getRelation(id);

	if(rel == NULL)
	{
		cout << "Relation " << id << " not found" << endl;
		return;
	}
	// print id
	cout << "------------Relation " << id << " Begin-----------------" << endl;

	// print type
	cout << "Type: " << rel->getType() << endl;

	// print startNode
	cout << "Start Node: " << rel->getStartNode() << endl;

	// print endNode
	cout << "End Node: " << rel->getEndNode() << endl;

	// print all properties
	map<string, Object*> *properties = rel->getProperties();
	for(map<string, Object*>::iterator it = properties->begin(); it != properties->end(); it++)
		cout << "Relation Property: " << it->first << " Value: " << it->second->toString() << endl;

	cout << "------------Relation " << id << " Finish-----------------" << endl << endl;
	destroy(rel);
}

void Manager::commit(Object *obj) {
	ZHT *zht = ZHT::getInstance();

	if(instanceOf<Relation>(*obj))
	{
		//insert relation to zht
		Relation &rel = dynamic_cast<Relation&>(*obj);
		zht->insertObject(rel);

		//insert properties in relation to zht
		map<string, Object*>::iterator it;
		for (it = rel.getProperties()->begin(); it != rel.getProperties()->end(); it++) {
			string key = it->first + SEP1 + rel.getUUID();
			string value = it->second->toString();
			zht->insertKV(key, value);
		}
		destroy(obj);
	}
	else if(instanceOf<Node>(*obj))
	{
		//insert node to zht
		Node &node = dynamic_cast<Node&>(*obj);
		zht->insertObject(node);

		//insert properties in node to zht
		map<string, Object*>::iterator it;
		for (it = node.getProperties()->begin(); it != node.getProperties()->end(); it++) {
			string key = it->first + SEP1 + node.getUUID();
			string value = it->second->toString();
			zht->insertKV(key, value);
		}
		destroy(obj);
	}
}

bool Manager::lookup(string relUUID,long int followee,long int &result){
	Manager *manager = Manager::getInstance();
	long int relid = Relation::getId(relUUID);
	Relation* rel = manager->getRelation(relid);
	long int endNodeId = Node::getId(rel->getEndNode());

	//get UUID of endnode
	if(endNodeId == followee){
		result = relid;
		return true;
	}else{
		result = NULL;
		return false;
	}
}

void Manager::close()
{
	destroy(manager);
	ZHT::getInstance()->close();
}


