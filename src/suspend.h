// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#ifndef __JOPANG$SUSPEND__
#define __JOPANG$SUSPEND__
// use mutex to suspend thread
#include <pthread.h>

typedef pthread_mutex_t Suspend;




Suspend * suspend_new();
void suspend_free(Suspend * sus);
void suspend(Suspend * sus);
void wake_up(Suspend * sus);

#endif

