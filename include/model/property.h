/*
 * property.h
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#ifndef PROPERTY_H_
#define PROPERTY_H_

#include "model/types.h"
#include "model/object.h"
#include <string>

using namespace std;

class graphdb::Property: public Object {
public:
	Property(string name, SupportedDataType dataType);
	~Property();

	string getName();
	void setName(string name);

	SupportedDataType getDataType();
	void setDataType(SupportedDataType datatype);

	string getUUID();
	string toString();
	static string getUUID(string name, SupportedDataType dataType);
	bool operator==(const Object &obj);

private:
	string name;
	SupportedDataType dataType;
};

#endif /* PROPERTY_H_ */
