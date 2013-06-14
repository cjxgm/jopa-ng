// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "error.h"
#include "jack.h"
#include "pulse.h"




void throw(ErrorCode err)
{
	static const char * error_code_to_string[] = {
		[0] = NULL,
		[ERROR_JACK_CLIENT_OPEN] = "jack_client_open failed.",
		[ERROR_JACK_ACTIVATE   ] = "jack_activate failed.",
	};

	if (err)
		fprintf(stderr, "\e[1;31merror: \e[1;33m%s\e[0m\n",
				error_code_to_string[err]);

	jack_close();
	pulse_close();
	global_close();

	exit(err);
}

