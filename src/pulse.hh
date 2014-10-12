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
		static constexpr auto buf_mono_len = 1024;
		static constexpr auto buf_len = buf_mono_len*2;
		static constexpr auto buf_nbyte = buf_len * sizeof(float);

	public:
		Pulse(int sample_rate) :
			ss {
				.format   = PA_SAMPLE_FLOAT32,
				.rate     = (unsigned int)sample_rate,
				.channels = 2,
			}
		{
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
				auto playback = pa_simple_new({}, "jopa-ng",
						PA_STREAM_PLAYBACK, {}, "playback", &ss,
						{}, {}, {});
				while (running) {
					float buf[buf_len];
					if (buf_play->get(buf)) sus_play->suspend();
					else pa_simple_write(playback, buf, buf_nbyte, {});
				}
				pa_simple_free(playback);
				nexit++;
			});

			// capture
			new thread([this]() {
				pa_buffer_attr ba {
					.maxlength = uint32_t(-1),
					.fragsize  = buf_nbyte,
				};
				auto capture = pa_simple_new({}, "jopa-ng",
						PA_STREAM_RECORD, {}, "capture", &ss,
						{}, &ba, {});
				while (running) {
					float buf[buf_len];
					pa_simple_read(capture, buf, buf_nbyte, nullptr);
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

