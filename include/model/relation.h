/*
 * relation.h
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#ifndef RELATION_H_
#define RELATION_H_

#include "model/object.h"
#include "model/node.h"
#include <map>
#include "model/types.h"
#include "model/property.h"
#include "model/prop_host.h"

using namespace std;

class graphdb::Relation : public Object, public PropertyHost {
public:
	Relation(long id, string type);
	~Relation();

	long getId();
	static long getId(string UUID);
	string getType();
	string getStartNode();
	void setStartNode(string node);
	string getEndNode();
	void setEndNode(string node);

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
	string type;
	string start; //UUID of start node
	string end;
	map<string, Object*> *properties;
};

#endif /* RELATION_H_ */
