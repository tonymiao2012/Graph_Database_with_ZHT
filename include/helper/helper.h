/*
 * helper.h
 *
 *      Author: zephyr
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "model/node.h"
#include "manager/manager.h"
#include "common.h"

extern std::vector<std::string> split(const std::string &s, char delim);
extern graphdb::Node *deserializeNode(const std::string &s);
extern graphdb::Relation *deserializeRelation(const std::string &s);

template<typename T> inline std::string stringValue(T t) {
	std::stringstream ss;
	ss << t;
	std::string s;
	ss >> s;
	return s;
}

#endif /* HELPER_H_ */
