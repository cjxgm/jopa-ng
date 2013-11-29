// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <thread>
#include <pulse/simple.h>
#include "buffer.hh"
#include "warn.hh"
using namespace std;

namespace jopang
{
	class Pulse
	{
		pa_simple* playback;
		pa_simple* capture;
		Buffer* buf_play;
		Buffer* buf_cap;

		bool running{true};
		int nexit{0};

	public:
		Pulse(int sample_rate)
		{
			pa_sample_spec ss = {
				.format	  = PA_SAMPLE_FLOAT32,
				.rate	  = (unsigned int)sample_rate,
				.channels = 2,
			};
			printf("%d", sample_rate);
			getchar();
			playback = pa_simple_new(nullptr, "jopa-ng", PA_STREAM_PLAYBACK,
					nullptr, "playback", &ss, nullptr, nullptr, nullptr);
			capture  = pa_simple_new(nullptr, "jopa-ng", PA_STREAM_RECORD,
					nullptr, "capture", &ss, nullptr, nullptr, nullptr);
		}

		~Pulse()
		{
			running = false;
			while (nexit != 2)
				this_thread::yield();
		}

		void start()
		{
			// playback
			new thread([this]() {
				while (running) {
					float buf[1024];
					if (buf_play->get(buf))
						warn("got stuck when playback.");
					else {
						pa_simple_write(playback, buf, sizeof(buf), NULL);
						pa_simple_drain(playback, NULL);
					}
				}
				pa_simple_free(playback);
				nexit++;
			});

			// capture
			new thread([this]() {
				while (running) {
					float buf[1024];
					pa_simple_read(capture, buf, sizeof(buf), NULL);
					buf_cap->put(buf);
				}
				pa_simple_free(capture);
				nexit++;
			});
		}

		void buffer(Buffer* play, Buffer* cap)
		{
			buf_play = play;
			buf_cap  = cap ;
		}
	};
};

