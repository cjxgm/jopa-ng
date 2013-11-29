// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <jack/jack.h>
#include <exception>
#include "buffer.hh"
using namespace std;

namespace jopang
{
	class Jack
	{
		jack_client_t* jack;
		jack_port_t*   ports_in [2];
		jack_port_t*   ports_out[2];
		Buffer* buf_play;
		Buffer* buf_cap;

		// exceptions
		struct open_failed {};
		struct activation_failed {};

	public:
		Jack()
		{
			// connect to jack
			jack = jack_client_open("jopa-ng", JackNoStartServer, nullptr);
			if (!jack) throw open_failed{};

			// register ports
			ports_in[0] = jack_port_register(jack, "playback_0",
					JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
			ports_in[1] = jack_port_register(jack, "playback_1",
					JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

			ports_out[0] = jack_port_register(jack, "capture_0",
					JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
			ports_out[1] = jack_port_register(jack, "capture_1",
					JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

			// register callbacks
			jack_on_shutdown(jack, &static_shutdown, this);
			jack_set_process_callback(jack, &static_process, this);
		}

		~Jack()
		{
			jack_deactivate(jack);
			jack_client_close(jack);
		}

		void start()
		{
			if (jack_activate(jack))
				throw activation_failed{};
		}

		int sample_rate() { return jack_get_sample_rate(jack); }

		void buffer(Buffer* play, Buffer* cap)
		{
			buf_play = play;
			buf_cap  = cap ;
		}

	private:	// callbacks
		void shutdown() { terminate(); }

		inline float* jack_buffer(jack_port_t* port, int nframe)
		{
			return static_cast<float*>(jack_port_get_buffer(port, nframe));
		}

		void process(int nframe)
		{
			{ // playback
				auto L = jack_buffer(ports_in[0], nframe);
				auto R = jack_buffer(ports_in[1], nframe);
				buf_play->put_stereo(L, R, nframe);
			}

			{ // capture
				auto L = jack_buffer(ports_out[0], nframe);
				auto R = jack_buffer(ports_out[1], nframe);
				buf_cap->get_stereo(L, R, nframe);
			}
		}

	private:	// static callbacks
		static inline Jack* cast(void* self)
		{
			return static_cast<Jack*>(self);
		}

		static void static_shutdown(void* self)
		{
			cast(self)->shutdown();
		}

		static int static_process(jack_nframes_t nframe, void* self)
		{
			return (cast(self)->process(nframe), 0);
		}
	};
};

