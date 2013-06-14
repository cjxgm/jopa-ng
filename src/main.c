// vim: ft=c noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include <stdio.h>




int main()
{
	jack_init();		// jack must be initialized before pulse
	pulse_init();

	jack_start();
	pulse_start();

	// suspend main thread
	pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&m);
	pthread_mutex_lock(&m);

	return 0;
}

