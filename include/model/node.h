/*
 * node.h
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#ifndef NODE_H_
#define NODE_H_

#include "model/object.h"
#include "model/prop_host.h"
#include "model/relation.h"
#include <map>
#include <set>

using namespace std;

class graphdb::Node : public Object, public PropertyHost {
public:
	Node(long id);
	~Node();

	long getId();
	static long getId(string UUID);

	void addRelationTo(Node &node, Relation &relation);
	bool hasRelation();
	bool hasRelation(Direction direction);
	bool hasRelation(string type);
	bool hasRelation(string type, Direction direction);

	set<string> getRelations();
	set<string> getRelations(Direction direction);
	set<string> getRelations(string type);
	set<string> getRelations(string type, Direction direction);
	map<string, set<string>*> *getOutRelations();
	map<string, set<string>*> *getInRelations();

	void removeRelation(long id);

	bool hasProperty(string name, SupportedDataType dt);
	Object *getPropertyValue(string name, SupportedDataType dt);
	map<string, Object*> *getProperties();
	void addProperty(string name, SupportedDataType dt, Object *propVal);
	void removeProperty(string name, SupportedDataType dt);

	string getUUID();
	static string getUUID(long id);
	string toString();
	bool operator==(const Object &obj);

private:
	long id;
	map<string, set<string>*> *outRelations; // map key is relation type, set key is relation UUID
	map<string, set<string>*> *inRelations;
	map<string, Object*> *properties; // key is property UUID, value is property value

	string toString(map<string, set<string>*> *rels);
};

#endif /* NODE_H_ */
