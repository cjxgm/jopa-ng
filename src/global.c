// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "global.h"




size_t global_sample_rate;
DBuf * global_dbuf_playback;




void global_init()
{
	global_dbuf_playback = dbuf_new();
}

void global_close()
{
}

