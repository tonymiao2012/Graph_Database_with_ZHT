/*
 * manager.h
 *
 *      Author: zephyr
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include "model/node.h"
#include "model/relation.h"
#include "model/property.h"
#include "model/types.h"
#include "ns.h"
#include <vector>


class graphdb::Manager {
public:
	static Manager *getInstance();
	virtual ~Manager();

	Node *createNode(const long id); //create node in memory, not save into zht
	Relation *createRelation(const long id, const string &type);

	Node *getNode(const long id); // restore node from zht
	Relation *getRelation(const long id);
	Object *getPropertyValue(string &pUUID);

	int removeNode(const long id); //remove node and associated property, modify related relations from zht
	int removeRelation(const long id);

	void printNode(const long id);
	void printRelation(const long id);

	bool Manager::lookup(string relUUID,long int followee,long int &result);	//lookup whether the endnode id of relation is same as followee. If so, keep rel_id in result.

	void commit(Object *obj); // save obj schema and property value to zht
	void close();
private:
	Manager();
	static Manager *manager;

	void initManager();

	static int nodeCount;
	static int relationCount;
	static int propertyCount;

}; //class Manager

#endif /* MANAGER_H_ */
