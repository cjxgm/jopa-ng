// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include <stdio.h>
#include <pthread.h>
#include <pulse/simple.h>
#include "pulse.h"
#include "dbuf.h"
#include "global.h"

static pa_simple * playback;
static pthread_t   thread_playback;


void pulse_init()
{
	pa_sample_spec ss = {
		.format	  = PA_SAMPLE_FLOAT32,
		.rate	  = global_sample_rate,	// so jack should init before pulse
		.channels = 2,
	};
	playback = pa_simple_new(NULL, "jopa-ng", PA_STREAM_PLAYBACK,
			NULL, "playback", &ss, NULL, NULL, NULL);
}


void pulse_start()
{
	pthread_create(&thread_playback, NULL, (void *)$(void, () {
		while (1) {
			float * buf = dbuf_used(global_dbuf_playback);
			if (buf) {
				pa_simple_write(playback, buf,
						dbuf_size(global_dbuf_playback), NULL);
				dbuf_unfill(global_dbuf_playback);
			}
			else suspend(global_suspend_playback);
		}
	}), NULL);
}


void pulse_close()
{
	if (thread_playback) pthread_cancel(thread_playback);
	if (playback) pa_simple_free(playback);
}

