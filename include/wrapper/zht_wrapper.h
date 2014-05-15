/*
 * zht_wrapper.h
 *
 *      Author: zephyr
 */

#ifndef ZHT_WRAPPER_H_
#define ZHT_WRAPPER_H_

#include "zht/cpp_zhtclient.h"
#include <iostream>
#include <string>
#include "common.h"
#include "model/relation.h"
#include "model/prop_value.h"

using namespace std;

namespace graphdb {

class ZHT
{
public:
	virtual ~ZHT();
	static ZHT* getInstance();

	int insertObject(Object &obj);
	int lookupObject(Object &obj, string &result);
	int removeObject(Object &obj);
	int insertKV(string &key, string &value);
	int lookupKV(string &key, string &value);
	int removeKV(string &key);

	void close();

private:
	//use singleton pattern
	static ZHT *zht;
	ZHT(); //private constructor
	static ZHTClient zc;

	template<class Object> inline int insertObject_(Object &obj)
	{
		return zc.insert(obj.getUUID(), obj.toString()) == 0 ? SUCCESS : FAILURE;
	}

	template<class Object> inline int lookupObject_(Object &obj, string &result)
	{
		return zc.lookup(obj.getUUID(), result) == 0 ? SUCCESS : FAILURE;
	}

	template<class Object> inline int removeObject_(Object &obj)
	{
		return zc.remove(obj.getUUID()) == 0 ? SUCCESS : FAILURE;
	}

};//class ZHT

}//namespace graphdb
#endif /* ZHT_WRAPPER_H_ */
