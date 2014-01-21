// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <thread>
#include <pulse/simple.h>
#include "buffer.hh"
using namespace std;

namespace jopang
{
	class Pulse
	{
		Buffer* buf_play;
		Buffer* buf_cap;
		Suspender* sus_play;

		bool running{true};
		int nexit{0};

		pa_sample_spec ss;

	public:
		Pulse(int sample_rate)
		{
			ss.format   = PA_SAMPLE_FLOAT32;
			ss.rate     = (unsigned int)sample_rate;
			ss.channels = 2;
		}

		~Pulse()
		{
			nexit = 0;
			running = false;
			while (nexit != 2) this_thread::yield();
		}

		void start()
		{
			// playback
			new thread([this]() {
				auto playback = pa_simple_new(nullptr, "jopa-ng",
						PA_STREAM_PLAYBACK, nullptr, "playback", &ss,
						nullptr, nullptr, nullptr);
				while (running) {
					float buf[128];
					if (buf_play->get(buf)) sus_play->suspend();
					else pa_simple_write(playback, buf, sizeof(buf), nullptr);
				}
				pa_simple_free(playback);
				nexit++;
			});

			// capture
			new thread([this]() {
				auto capture = pa_simple_new(nullptr, "jopa-ng",
						PA_STREAM_RECORD, nullptr, "capture", &ss,
						nullptr, nullptr, nullptr);
				while (running) {
					float buf[128];
					pa_simple_read(capture, buf, sizeof(buf), nullptr);
					buf_cap->put(buf);
				}
				pa_simple_free(capture);
				nexit++;
			});
		}

		void suspender(Suspender* play) { sus_play = play; }

		void buffer(Buffer* play, Buffer* cap)
		{
			buf_play = play;
			buf_cap  = cap ;
		}
	};
};

