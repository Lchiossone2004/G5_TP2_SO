#include <time.h>
#include <sleep.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	sleep_queue_tick_handler();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks;
}
