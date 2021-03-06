#include "logInfo.h"
#include <stdio.h>
#include <stdarg.h>
#include "appTools.h"

void logIntoFile(FILE* file, char* level, const char* fmt, va_list argptr) {
	char value[500] = { 0 };
	char dt[40] = {0};
	getCurrentDateTimeMSecStr(dt, 40);
	vsnprintf(value, 500, fmt, argptr);
	fprintf(file, "%s %s: %s\n", dt, level, value);
}

void debugInfo(const char* fmt, ...) {
	if (cmdParam.debug) {
		va_list argptr;
		va_start(argptr, fmt);
		logIntoFile(stdout, "NORMAL", fmt, argptr);
		va_end(argptr);
	}
}

void logRawLineInfo(const char* fmt, ...) {
	char value[500] = {0};
	va_list argptr;
	va_start(argptr, fmt);
	vsnprintf(value, 500, fmt, argptr);
	va_end(argptr);
	fprintf(stdout, "%s \n", value);
}

void logInfo(const char* fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	logIntoFile(stdout, "NORMAL", fmt, argptr);
	va_end(argptr);
}

void errorInfo(const char* fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	logIntoFile(stderr, "ERROR", fmt, argptr);
	va_end(argptr);
}

