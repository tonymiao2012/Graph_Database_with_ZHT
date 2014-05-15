/*
 * logger.h
 *
 *   Author: Ying Ni(yni6@hawk.iit.edu)
 *   Description:
 *
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>

namespace graphdb{
enum LogLevel {
	FATAL, ERROR, WARN, INFO, DEBUG, TRACE
};

void initLogger(FILE *output, int level);
void log(LogLevel level, const char *file, unsigned line, const char *msg, ...);

#define LOG_FATAL(msg, ...) log(FATAL, __FILE__, __LINE__, msg, ##__VA_ARGS__);
#define LOG_ERROR(msg, ...) log(ERROR, __FILE__, __LINE__, msg, ##__VA_ARGS__);
#define LOG_WARN(msg, ...) log(WARN, __FILE__, __LINE__, msg, ##__VA_ARGS__);
#define LOG_INFO(msg, ...) log(INFO, __FILE__, __LINE__, msg, ##__VA_ARGS__);
#define LOG_DEBUG(msg, ...) log(DEBUG, __FILE__, __LINE__, msg, ##__VA_ARGS__);
#define LOG_TRACE(msg, ...) log(TRACE, __FILE__, __LINE__, msg, ##__VA_ARGS__);
}

#endif /* LOGGER_H_ */
