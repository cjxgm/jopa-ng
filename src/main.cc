// vim: noet ts=4 sw=4 sts=0
#include <mutex>
#include "thread.hh"
#include "jack.hh"
#include "pulse.hh"
#include "buffer.hh"
using namespace std;




int main()
{
	auto  jack = new jopang::Jack;
	auto pulse = new jopang::Pulse{jack->sample_rate()};

	auto buf_play = new jopang::Buffer;
	auto buf_cap  = new jopang::Buffer;

	 jack->buffer(buf_play, buf_cap);
	pulse->buffer(buf_play, buf_cap);

	 jack->start();
	pulse->start();

	// exit main thread to wait for other threads
	this_thread::exit();
}

