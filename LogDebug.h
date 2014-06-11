#ifndef LOG_DEBUG
#define LOG_DEBUG

#ifdef DEBUG
#define log_debug(fmt, ...)\
	printf("DEBUG %s:%d: " fmt "\n",\
		__FILE__, __LINE__, ##__VA_ARGS__)
#else
#define log_debug(fmt, ..)
#endif

#endif
