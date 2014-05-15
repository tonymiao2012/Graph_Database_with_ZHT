/*
 * prop_host.h
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#ifndef PROP_HOST_H_
#define PROP_HOST_H_

#include <map>
#include "model/property.h"
#include "model/object.h"

using namespace std;

class graphdb::PropertyHost {
public:
	PropertyHost() {};
	virtual ~PropertyHost() {};

	virtual bool hasProperty(string name, SupportedDataType dt) = 0;
	virtual Object* getPropertyValue(string name, SupportedDataType dt) = 0;
	virtual map<string, Object*>* getProperties() = 0;
	virtual void addProperty(string name, SupportedDataType dt, Object *propVal) = 0;
	virtual void removeProperty(string name, SupportedDataType dt) = 0;
};

#endif /* PROP_HOST_H_ */
