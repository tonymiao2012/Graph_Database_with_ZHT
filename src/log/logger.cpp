/*
 * logger.cpp
 *
 *   Author: Ying Ni(yni6@hawk.iit.edu)
 *   Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "log/logger.h"
#include <string>
#include <cstring>
#include <iostream>

using namespace graphdb;

static FILE *out = stdout;
static std::string header[] = { "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };
static int maxLevel = INFO;

static inline const char *getHeader(LogLevel level) {
	return header[level].c_str();
}

void graphdb::initLogger(FILE *output, int level) {
	out = output;
	maxLevel = level;
}

void graphdb::log(LogLevel level, const char *file, unsigned line, const char *msg, ...) {
	if (level <= maxLevel) {
		va_list args;
		va_start(args, msg);

		fprintf(out, "%s", getHeader(level));
		vfprintf(out, msg, args);
		if (file && line > 0) {
			fprintf(out, " (%s:%u)\n", file, line);
		} else {
			fprintf(out, " (unknown)\n");
		}
		fflush(out);

		va_end(args);
	}
}

