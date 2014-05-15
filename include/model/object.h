/*
 * object.h
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "ns.h"
#include <string>
#include "model/types.h"

class graphdb::Object {
public:
	Object();
	virtual ~Object();

	virtual std::string getUUID();
	virtual std::string toString();
	virtual bool operator==(const Object &obj);
};

#endif /* OBJECT_H_ */
