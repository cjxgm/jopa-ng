// vim: noet ts=4 sw=4 sts=0
#include "thread.hh"
#include <pthread.h>

void std::this_thread::exit()
{
	pthread_exit(nullptr);
}

