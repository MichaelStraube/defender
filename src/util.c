#include <time.h>
#include "util.h"

// return milliseconds
long get_ticks(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	//timespec_get(&ts, TIME_UTC); // c11
	return (long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}
