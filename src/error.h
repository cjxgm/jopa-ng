// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#ifndef __JOPANG$ERROR__
#define __JOPANG$ERROR__

typedef enum ErrorCode
{
	ERROR_OK = 0,

	ERROR_MALLOC, 

	ERROR_JACK_CLIENT_OPEN,
	ERROR_JACK_ACTIVATE,
}
ErrorCode;




void throw(ErrorCode err);
void error(const char * msg);
void warn(const char * msg);

#endif

