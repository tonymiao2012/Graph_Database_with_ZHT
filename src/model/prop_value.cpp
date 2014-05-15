/*
 * prop_value.cpp
 *
 * 	 Author: Ying Ni(yni6@hawk.iit.edu)
 * 	 Description:
 *
 */

#include "model/prop_value.h"
#include "helper/helper.h"

using namespace std;
using namespace graphdb;

String::String(string s) :
		Object(), s(s) {
}

String::~String() {
}

string String::getValue() {
	return this->s;
}

void String::setValue(string s) {
	this->s = s;
}

bool String::isEmpty() {
	return this->s.empty();
}

string String::toString() {
	return this->s;
}

bool String::operator==(const Object &obj) {
	if (!instanceOf<String>(obj))
		return false;

	const String &s = dynamic_cast<const String&>(obj);
	return (this->s == s.s);
}

Integer::Integer(int i) :
		Object(), i(i) {
}

Integer::~Integer() {
}

int Integer::getValue() {
	return this->i;
}

void Integer::setValue(int i) {
	this->i = i;
}

string Integer::toString() {
	return stringValue(this->i);
}

bool Integer::operator==(const Object &obj) {
	if (!instanceOf<Integer>(obj))
		return false;
	const Integer &i = dynamic_cast<const Integer&>(obj);
	return (this->i == i.i);
}

Double::Double(double d) :
		Object(), d(d) {
}

Double::~Double() {
}

double Double::getValue() {
	return this->d;
}

void Double::setValue(double d) {
	this->d = d;
}

string Double::toString() {
	return stringValue(this->d);
}

bool Double::operator==(const Object &obj) {
	if (!instanceOf<Double>(obj))
		return false;

	const Double &d = dynamic_cast<const Double&>(obj);
	return (this->d == d.d);
}

Boolean::Boolean(bool b) :
		Object(), b(b) {
}

Boolean::~Boolean() {
}

bool Boolean::getValue() {
	return this->b;
}

void Boolean::setValue(bool b) {
	this->b = b;
}

string Boolean::toString() {
	return stringValue(this->b);
}

bool Boolean::operator==(const Object &obj) {
	if (!instanceOf<Boolean>(obj))
		return false;

	const Boolean &b = dynamic_cast<const Boolean&>(obj);
	return (this->b == b.b);
}

Byte::Byte(char c) :
		Object(), c(c) {
}

Byte::~Byte() {
}

char Byte::getValue() {
	return this->c;
}

void Byte::setValue(char c) {
	this->c = c;
}

string Byte::toString() {
	return stringValue(this->c);
}

bool Byte::operator==(const Object &obj) {
	if (!instanceOf<Byte>(obj))
		return false;

	const Byte &byte = dynamic_cast<const Byte&>(obj);
	return (this->c == byte.c);
}
