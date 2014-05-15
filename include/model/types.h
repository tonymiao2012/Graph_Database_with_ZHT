/*
 * types.h
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <string>
#include <stdlib.h>
#include <typeinfo>
#include "model/object.h"

namespace graphdb {

typedef enum SupportedDataType {
	DT_STRING = 1,
	DT_INT,
	DT_DOUBLE,
	DT_BOOL,
	DT_BYTE,
	DT_STR_ARR,
	DT_INT_ARR,
	DT_DOUBLE_ARR,
	DT_BOOL_ARR,
	DT_BYTE_ARR
} SupportedDataType;

typedef enum Direction {
	OUT,
	IN,
	BOTH
} Direction;

template<class Sub, class Super> inline bool instanceOf(const Super &o) {
	try {
		dynamic_cast<const Sub&>(o);
	} catch (std::bad_cast &e) {
		return false;
	}
	return true;
}

std::string dataTypeToString(SupportedDataType type);
SupportedDataType stringToDataType(std::string type);
Object *stringToObject(std::string value, SupportedDataType dt);
SupportedDataType intToDataType(int type);
long stringToLong(const std::string &s);
int  stringToInt(const std::string &s);
char stringToChar(const std::string &s);
bool stringToBool(const std::string &s);

} // namespace graphdb

#endif /* TYPES_H_ */
