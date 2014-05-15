/*
 * common.h
 *
 *      Author: zephyr
 */

#ifndef COMMON_H_
#define COMMON_H_

#define SUCCESS 0
#define FAILURE -1

static const std::string SEP0 = ";"; // to separate fields in values (toString())
static const std::string SEP1 = "|"; // to separate fields in keys (UUID)
static const std::string SEP2 = ","; // to separate sub-fields of a field in value

template<class T> inline void destroy(T *&p) {
	if(p != NULL) {
		delete p;
		p = NULL;
	}
}

template<class T> inline void destroyArray(T *&p) {
	if(p != NULL) {
		delete[] p;
		p = NULL;
	}
}

#endif /* COMMON_H_ */
