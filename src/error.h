// vim: ft=c noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#ifndef __JOPANG$ERROR__
#define __JOPANG$ERROR__

typedef struct ErrorCode
{
	ERROR_OK = 0,
}
ErrorCode;




void throw(ErrorCode err);

#endif

