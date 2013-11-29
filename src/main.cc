// vim: noet ts=4 sw=4 sts=0
#include <mutex>
#include "thread.hh"	// for std::this_thread::exit()
#include "jack.hh"
#include "pulse.hh"
#include "buffer.hh"
#include "suspender.hh"
using namespace std;


int main()
{
	// create pulse and jack clients
	auto  jack = new jopang::Jack;
	auto pulse = new jopang::Pulse{jack->sample_rate()};

	// create playback and capture buffer, attaching them to the clients
	auto buf_play = new jopang::Buffer;
	auto buf_cap  = new jopang::Buffer;
	 jack->buffer(buf_play, buf_cap);
	pulse->buffer(buf_play, buf_cap);

	// create playback suspender, attaching them to the clients
	auto sus_play = new jopang::Suspender;
	 jack->suspender(sus_play);
	pulse->suspender(sus_play);

	// start both clients
	 jack->start();
	pulse->start();

	// exit main thread to wait for other threads
	this_thread::exit();
}

