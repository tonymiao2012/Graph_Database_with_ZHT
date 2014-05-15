/*
 * prop_value.h
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#ifndef PROP_VALUE_H_
#define PROP_VALUE_H_

#include "model/types.h"
#include "model/object.h"
#include <string>
#include <vector>

namespace graphdb {
class String: public Object {
public:
	String(std::string s);
	~String();

	std::string getValue();
	void setValue(std::string s);

	bool isEmpty();
	std::string toString();
	bool operator==(const Object &obj);
private:
	std::string s;
};

class Integer: public Object {
public:
	Integer(int i);
	~Integer();

	int getValue();
	void setValue(int i);

	std::string toString();
	bool operator==(const Object &obj);
private:
	int i;
};

class Double: public Object {
public:
	Double(double d);
	~Double();

	double getValue();
	void setValue(double d);

	std::string toString();
	bool operator==(const Object &obj);
private:
	double d;
};

class Boolean: public Object {
public:
	Boolean(bool b);
	~Boolean();

	bool getValue();
	void setValue(bool b);

	std::string toString();
	bool operator==(const Object &obj);
private:
	bool b;
};

class Byte: public Object {
public:
	Byte(char c);
	~Byte();

	char getValue();
	void setValue(char c);

	std::string toString();
	bool operator==(const Object &obj);
private:
	char c;
};

}

#endif /* PROP_VALUE_H_ */
