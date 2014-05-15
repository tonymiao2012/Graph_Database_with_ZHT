/*-------------------------------------------------------------------------
 *
 * command_line_main.cpp
 *    ONE_LINE_DESCRIPTION
 *
 *        Here starts the more detailed description where we 
 *        explain in more detail how this works.
 *
 *-------------------------------------------------------------------------
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "wrapper/zht_wrapper.h"
#include "model/object.h"
#include "model/property.h"
#include "model/prop_value.h"
#include "model/relation.h"
#include "model/node.h"
#include "model/types.h"
#include "manager/manager.h"

using namespace std;
using namespace graphdb;

static void printUsage();
static void printDT();
static void printInvalid();
static void processRequest(int request);
static void createNode();
static void createRelation();
static void lookupNode();
static void lookupRelation();
static void removeNode();
static void removeRelation();

int main(){
	Manager *manager = Manager::getInstance();

	while(true)
	{
		printUsage();
		int request = 0;
		cin >> request;
		if(request == 7)
			break;
		processRequest(request);
	}

	manager->close();
	cout << "Graph Database Client Closed";
	return SUCCESS;
}

static void processRequest(int request)
{
	switch(request)
	{
	case 1:
		createNode();
		break;
	case 2:
		createRelation();
		break;
	case 3:
		lookupNode();
		break;
	case 4:
		lookupRelation();
		break;
	case 5:
		removeNode();
		break;
	case 6:
		removeRelation();
		break;
	default:
		printInvalid();
		break;
	}
}


static void createNode()
{
	Manager *manager = Manager::getInstance();

	long id = 0;
	cout << "Input a Non-negative Integer for Node ID:" << endl;
	cin >> id;
	Node *node = manager->createNode(id);

	while(true)
	{
		cout << "1. Add new property to node(" << id << ")" << endl;
		cout << "2. Remove property from node(" << id << ")" << endl;
		cout << "3. Add relation to other node" << endl;
		cout << "4. Commit" << endl;

		int request = 0;
		cin >> request;
		if(request == 4)
			break;
		switch(request)
		{
		case 1:
		{
			cout << "Name?" << endl;
			string name;
			cin >> name;
			cout << "Type?" << endl;
			printDT();
			int dt_;
			cin >> dt_;
			SupportedDataType dt = intToDataType(dt_);
			cout << "Value?" << endl;
			string v;
			cin >> v;
			Object *value = stringToObject(v, dt);
			node->addProperty(name, dt, value);
			cout << "Property [Name: " << name << " Type: " << dataTypeToString(dt) << " Value: " << value->toString() << "] added." << endl;
		}
			break;
		case 2:
		{
			cout << "Name?" << endl;
			string name;
			cin >> name;
			cout << "Type?" << endl;
			printDT();
			string dt_;
			cin >> dt_;
			SupportedDataType dt = stringToDataType(dt_);
			node->removeProperty(name, dt);
			cout << "Property Name: " << name << " Type: " << dataTypeToString(dt) << " removed." << endl;
		}
			break;
		case 3:
		{
			cout << "Relation ID?" << endl;
			int rID;
			cin >> rID;
			Relation *rel = manager->getRelation(rID);
			if(rel == NULL)
			{
				cout << "No such relation." << endl;
				return;
			}
			cout << "Target Node ID?" << endl;
			int nID;
			cin >> nID;
			Node *target = manager->getNode(nID);
			if(target == NULL)
			{
				cout << "No such node." << endl;
				return;
			}
			cout << "Node: " << node->toString() << endl;
			cout << "Add relation: " << rel->toString() << endl;
			cout << "To node: " << target->toString() << endl;
			node->addRelationTo(*target, *rel);

			cout << "Relation("<< rID <<"), Type("<< rel->getType()<< ") added." << endl;
			manager->commit(target);
			manager->commit(rel);
		}
			break;
		default:
			printInvalid();
			break;
		}
	}

	manager->commit(node);
	manager->printNode(id);
	cout << "Node(" << id << ") Created" << endl << endl;
}

static void createRelation()
{
	Manager *manager = Manager::getInstance();

	long id = 0;
	string type;
	cout << "Input a Non-negative Integer for Relation ID:" << endl;
	cin >> id;
	cout << "Input the relation type:" << endl;
	cin >> type;
	Relation *rel = manager->createRelation(id, type);

	while(true)
	{
		cout << "1. Add new property to relation(" << id << ")" << endl;
		cout << "2. Remove property from relation(" << id << ")" << endl;
		cout << "3. Commit" << endl;

		int request = 0;
		cin >> request;
		if(request == 3)
			break;
		switch(request)
		{
		case 1:
		{
			cout << "Name?" << endl;
			string name;
			cin >> name;
			cout << "Type?" << endl;
			printDT();
			string dt_;
			cin >> dt_;
			SupportedDataType dt = stringToDataType(dt_);
			cout << "Value?" << endl;
			string v;
			cin >> v;
			Object *value = stringToObject(v, dt);
			rel->addProperty(name, dt, value);
			cout << "Property [Name: " << name << " Type: " << dataTypeToString(dt) << " Value: " << v << "] added." << endl;
		}
			break;
		case 2:
		{
			cout << "Name?" << endl;
			string name;
			cin >> name;
			cout << "Type?" << endl;
			printDT();
			string dt_;
			cin >> dt_;
			SupportedDataType dt = stringToDataType(dt_);
			rel->removeProperty(name, dt);
			cout << "Property Name: " << name << " Type: " << dataTypeToString(dt) << " removed." << endl;
		}
			break;
		default:
			printInvalid();
			break;
		}
	}
	manager->commit(rel);
	manager->printRelation(id);
	cout << "Relation(" << id << "), Type (" << type << ") Created" << endl << endl;
}

static void lookupNode()
{
	Manager *manager = Manager::getInstance();

	long id = 0;
	cout << "Input a Non-negative Integer for Node ID:" << endl;
	cin >> id;
	manager->printNode(id);
}

static void lookupRelation()
{
	Manager *manager = Manager::getInstance();

	long id = 0;
	string type;
	cout << "Input a Non-negative Integer for Relation ID:" << endl;
	cin >> id;
	manager->printRelation(id);
}

static void removeNode()
{
	Manager *manager = Manager::getInstance();

	long id = 0;
	string type;
	cout << "Input a Non-negative Integer for Node ID:" << endl;
	cin >> id;
	int rc = manager->removeNode(id);
	if(rc != 0)
		cout << "Node(" << id << ") not found" << endl << endl;
	else
		cout << "Node(" << id << ") Removed" << endl << endl;
}

static void removeRelation()
{
	Manager *manager = Manager::getInstance();

	long id = 0;
	string type;
	cout << "Input a Non-negative Integer for Relation ID:" << endl;
	cin >> id;
	int rc = manager->removeRelation(id);
	if(rc != 0)
		cout << "Relation(" << id << ") not found" << endl << endl;
	else
		cout << "Relation(" << id << ") Removed" << endl << endl;
}

static void printUsage()
{
	cout << "Distributed Graph Database on ZHT Version 0.1" << endl;
	cout << "========================================" << endl;
	cout << "1. Create Node" << endl;
	cout << "2. Create Relation" << endl;
	cout << "3. Lookup Node" << endl;
	cout << "4. Lookup Relation" << endl;
	cout << "5. Remove Node" << endl;
	cout << "6. Remove Relation" << endl;
	cout << "7. Exit" << endl << endl;
	cout << "Please choose what you want to do by typing the number: " << endl;
}

static void printInvalid()
{
	cout << "Invalid Request" << endl;
}

static void printDT()
{
	cout << "1. DT_STRING" << endl;
	cout << "2. DT_INT" << endl;
	cout << "3. DT_DOUBLE" << endl;
	cout << "4. DT_BOOL" << endl;
	cout << "5. DT_BYTE" << endl;
	cout << "6. DT_STR_ARR" << endl;
	cout << "7. DT_INT_ARR" << endl;
	cout << "8. DT_DOUBLE_ARR" << endl;
	cout << "9. DT_BOOL_ARR" << endl;
	cout << "10. DT_BYTE_ARR" << endl;
}

