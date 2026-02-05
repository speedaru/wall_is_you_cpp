#pragma once
#include <stdarg.h>

#define LOG_LEVEL_DEBUG		1 << 0
#define LOG_LEVEL_WARNING	1 << 1
#define LOG_LEVEL_ERROR		1 << 2
#define LOG_LEVEL_TRACE		1 << 3

#define LOG_LEVEL LOG_LEVEL_DEBUG | LOG_LEVEL_WARNING | LOG_LEVEL_WARNING

#define __RELATIVE_FILE__ __FILE__ + sizeof(__PROJECT_DIR__) - 1

namespace logging {
	#define LOG(logLevel, format, ...) logging::Log(logLevel, __RELATIVE_FILE__, __LINE__, format, __VA_ARGS__)

	#define LOG_D(format, ...) LOG(LOG_LEVEL_DEBUG, format, __VA_ARGS__)
	#define LOG_W(format, ...) LOG(LOG_LEVEL_WARNING, format, __VA_ARGS__)
	#define LOG_E(format, ...) LOG(LOG_LEVEL_ERROR, format, __VA_ARGS__)
	#define LOG_T(format, ...) LOG(LOG_LEVEL_TRACE, format, __VA_ARGS__)

	inline void Log(int logLevel, const char* file, int line, const char* format, ...) {
		va_list args;
		va_start(args, format);

		// file:line
		printf("[%s:%d] ", file, line);

		// log message
		vprintf_s(format, args);

		va_end(args);
	}
}