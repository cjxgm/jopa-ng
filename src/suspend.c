// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "suspend.h"
#include "misc.h"





Suspend * suspend_new()
{
	create(Suspend, sus);
	pthread_mutex_init(sus, NULL);
	suspend(sus);
	return sus;
}


void suspend_free(Suspend * sus)
{
	wake_up(sus);
	pthread_mutex_destroy(sus);
	free(sus);
}


void suspend(Suspend * sus)
{
	pthread_mutex_lock(sus);
}


void wake_up(Suspend * sus)
{
	pthread_mutex_unlock(sus);
}

