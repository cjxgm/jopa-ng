// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <mutex>
#include <condition_variable>
using namespace std;

namespace jopang
{
	struct Suspender
	{
		mutex m;
		condition_variable cv;

		void suspend() { unique_lock<mutex> ulck{m}; cv.wait(ulck); }
		void resume () { cv.notify_all(); }
	};
};

