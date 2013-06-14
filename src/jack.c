// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include <stdio.h>
#include <jack/jack.h>
#include "jack.h"
#include "error.h"
#include "global.h"




static jack_client_t	* jack;
static jack_port_t		* ports_in[2];




void jack_init()
{
	// connect to jack
	if (!(jack = jack_client_open("jopa-ng", JackNoStartServer, NULL)))
		throw(ERROR_JACK_CLIENT_OPEN);

	// register callbacks
	jack_on_shutdown(jack, $(void, (void * $unused) {
		throw(ERROR_OK);
	}), NULL);

	jack_set_buffer_size_callback(jack,
		$(int, (jack_nframes_t nframe, void * $unused) {
			// this will be called once jack activated.
			dbuf_resize(global_dbuf_playback, nframe<<1);
			return 0;
		}), NULL);

	jack_set_process_callback(jack,
		$(int, (jack_nframes_t nframe, void * $unused) {
			float * buf0 = jack_port_get_buffer(ports_in[0], nframe);
			float * buf1 = jack_port_get_buffer(ports_in[1], nframe);
			if (dbuf_add_stereo(global_dbuf_playback, buf0, buf1, nframe))
				warn("got stuck when playback.");
			return 0;
		}), NULL);

	// register ports
	ports_in[0] = jack_port_register(jack, "playback_0",
			JACK_DEFAULT_AUDIO_TYPE,
			JackPortIsInput|JackPortIsPhysical|JackPortIsTerminal, 0);
	ports_in[1] = jack_port_register(jack, "playback_1",
			JACK_DEFAULT_AUDIO_TYPE,
			JackPortIsInput|JackPortIsPhysical|JackPortIsTerminal, 0);

	// other setups
	global_sample_rate = jack_get_sample_rate(jack);
}


void jack_start()
{
	if (jack_activate(jack)) throw(ERROR_JACK_ACTIVATE);
}


void jack_close()
{
	if (jack) {
		jack_deactivate(jack);
		jack_client_close(jack);
	}
}

