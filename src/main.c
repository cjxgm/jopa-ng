// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include <stdio.h>
#include <pthread.h>
#include "global.h"
#include "jack.h"
#include "pulse.h"




int main()
{
	global_init();
	jack_init();		// jack must be initialized before pulse
	pulse_init();

	pulse_start();		// FIXME: is pulse_start before jack_start better?
	jack_start();

	// suspend main thread
	pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&m);
	pthread_mutex_lock(&m);

	return 0;
}

