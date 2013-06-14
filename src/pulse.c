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
#include "error.h"




static pa_simple * playback;
static pa_simple * capture;
static pthread_t   thread_playback;
static pthread_t   thread_capture;




void pulse_init()
{
	{ // playback
		pa_sample_spec ss = {
			.format	  = PA_SAMPLE_FLOAT32,
			.rate	  = global_sample_rate,	// that's why jack should
											// init before pulse
			.channels = 2,
		};
		playback = pa_simple_new(NULL, "jopa-ng", PA_STREAM_PLAYBACK,
				NULL, "playback", &ss, NULL, NULL, NULL);
	}

	{ // capture
		pa_sample_spec ss = {
			.format	  = PA_SAMPLE_FLOAT32,
			.rate	  = global_sample_rate,	// that's why jack should
											// init before pulse
			.channels = 1,
		};
		capture = pa_simple_new(NULL, "jopa-ng", PA_STREAM_RECORD,
				NULL, "capture", &ss, NULL, NULL, NULL);
	}
}


void pulse_start()
{
	// playback
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

	// capture
	pthread_create(&thread_capture, NULL, (void *)$(void, () {
		while (1) {
			float * buf = dbuf_unused(global_dbuf_capture);
			if (buf) {
				pa_simple_read(capture, buf,
						dbuf_size(global_dbuf_capture), NULL);
				dbuf_fill(global_dbuf_capture);
			}
			else suspend(global_suspend_playback);
		}
	}), NULL);
}


void pulse_close()
{
	if (thread_playback) pthread_cancel(thread_playback);
	if (thread_capture ) pthread_cancel(thread_capture );
	if (playback) pa_simple_free(playback);
	if (capture ) pa_simple_free(capture );
}

