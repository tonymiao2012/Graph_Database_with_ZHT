/*
 * types.cpp
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#include "model/types.h"
#include "model/prop_value.h"
#include "string.h"

using namespace graphdb;
using namespace std;

std::string graphdb::dataTypeToString(SupportedDataType type) {
	std::string dt;
	switch (type) {
	case DT_STRING:
		dt = "string";
		break;
	case DT_INT:
		dt = "integer";
		break;
	case DT_DOUBLE:
		dt = "double";
		break;
	case DT_BOOL:
		dt = "boolean";
		break;
	case DT_BYTE:
		dt = "byte";
		break;
	case DT_STR_ARR:
		dt = "string_array";
		break;
	case DT_INT_ARR:
		dt = "integer_array";
		break;
	case DT_DOUBLE_ARR:
		dt = "double_array";
		break;
	case DT_BOOL_ARR:
		dt = "boolean_array";
		break;
	case DT_BYTE_ARR:
		dt = "byte_array";
		break;
	}
	return dt;
}

SupportedDataType graphdb::stringToDataType(std::string type) {
	SupportedDataType dt = DT_STRING;
	if (type == "string")
		dt = DT_STRING;
	else if (type == "integer")
		dt = DT_INT;
	else if (type == "double")
		dt = DT_DOUBLE;
	else if (type == "boolean")
		dt = DT_BOOL;
	else if (type == "byte")
		dt = DT_BYTE;
	else if (type == "string_array")
		dt = DT_STR_ARR;
	else if (type == "integer_array")
		dt = DT_INT_ARR;
	else if (type == "double_array")
		dt = DT_DOUBLE_ARR;
	else if (type == "boolean_array")
		dt = DT_BOOL_ARR;
	else if (type == "byte_array")
		dt = DT_BYTE_ARR;
	else
		dt = DT_STRING;
	return dt;
}

Object *graphdb::stringToObject(std::string value, SupportedDataType dt)
{
	Object *obj = NULL;
	switch (dt) {
	case DT_STRING:
		obj = new String(value);
		break;
	case DT_INT:
		obj = new Integer(atoi(value.c_str()));
		break;
	case DT_DOUBLE:
		obj = new Double(atof(value.c_str()));
		break;
	case DT_BOOL:
		obj = new Boolean(stringToBool(value));
		break;
	case DT_BYTE:
		obj = new Byte(value.c_str()[0]);
		break;
	case DT_STR_ARR:
		break;
	case DT_INT_ARR:
		break;
	case DT_DOUBLE_ARR:
		break;
	case DT_BOOL_ARR:
		break;
	case DT_BYTE_ARR:
		break;
	default:
		obj = new String(value);
		break;
	}
	return obj;
}

SupportedDataType graphdb::intToDataType(int type)
{
	SupportedDataType dt;
	switch (type) {
	case DT_STRING:
		dt = DT_STRING;
		break;
	case DT_INT:
		dt = DT_INT;
		break;
	case DT_DOUBLE:
		dt = DT_DOUBLE;
		break;
	case DT_BOOL:
		dt = DT_BOOL;
		break;
	case DT_BYTE:
		dt = DT_BYTE;
		break;
	case DT_STR_ARR:
		dt = DT_STR_ARR;
		break;
	case DT_INT_ARR:
		dt = DT_INT_ARR;
		break;
	case DT_DOUBLE_ARR:
		dt = DT_DOUBLE_ARR;
		break;
	case DT_BOOL_ARR:
		dt = DT_BOOL_ARR;
		break;
	case DT_BYTE_ARR:
		dt = DT_BYTE_ARR;
		break;
	default:
		dt = DT_STRING;
		break;
	}
	return dt;
}

bool graphdb::stringToBool(const string& s)
{
	return s != "0" && strcasecmp(s.c_str(), "false") != 0;
}

long graphdb::stringToLong(const string &s)
{
	return atol(s.c_str());
}

int graphdb::stringToInt(const string &s)
{
	return atoi(s.c_str());
}

char graphdb::stringToChar(const string &s)
{
	return s.c_str()[0];
}
