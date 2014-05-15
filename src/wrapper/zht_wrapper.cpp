/*
 * zht_wrapper.cpp
 *
 *      Author: zephyr
 */

#include "wrapper/zht_wrapper.h"
#include "zht/cpp_zhtclient.h"
#include "model/types.h"
#include "model/relation.h"
#include "log/logger.h"
#include "common.h"

using namespace graphdb;
using namespace std;

ZHT* ZHT::zht = NULL;
ZHTClient ZHT::zc;

#define CHECK_TYPE(obj) \
		!instanceOf<Node>(obj) && !instanceOf<Property>(obj) && !instanceOf<Relation>(obj)

ZHT::ZHT()
{
	zc.init("zht.conf", "neighbor.conf");
}

ZHT::~ZHT()
{
	LOG_DEBUG("Destructing ZHT");
}

ZHT* ZHT::getInstance()
{
	if(!zht)
	{
		zht = new ZHT();
		return zht;
	}
	else
		return zht;
}

int ZHT::insertObject(Object &obj)
{
	if(CHECK_TYPE(obj))
	{
		LOG_ERROR("ZHT insert %s failed.", obj.toString().c_str());
		return FAILURE;
	}
	return ZHT::insertObject_(obj);
}

int ZHT::lookupObject(Object &obj, string &result)
{
	if(CHECK_TYPE(obj))
	{
		LOG_ERROR("ZHT lookup %s failed.", obj.toString().c_str());
		return FAILURE;
	}
	return ZHT::lookupObject_(obj, result);
}

int ZHT::removeObject(Object &obj)
{
	if(CHECK_TYPE(obj))
	{
		LOG_ERROR("ZHT remove %s failed.", obj.toString().c_str());
		return FAILURE;
	}
	return ZHT::removeObject_(obj);
}

int ZHT::insertKV(string &key, string &value)
{
	return this->zc.insert(key, value);
}

int ZHT::lookupKV(string &key, string &value)
{
	return this->zc.lookup(key, value);
}

int ZHT::removeKV(string &key)
{
	return this->zc.remove(key);
}

void ZHT::close()
{
	zc.teardown();
	destroy(zht);
}

