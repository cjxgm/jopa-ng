// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#ifndef __JOPANG$MISC__
#define __JOPANG$MISC__

#include <stdlib.h>
#include <unistd.h>




///////////// the following codes are from cjxgm/libcrude

// anonymous/lambda function
#define $($ret_type, $definition...) \
	({ $ret_type $lambda_function$ $definition &$lambda_function$; })

// act like the "auto" keyword in c++11
#define $_($var, $what...) \
	typeof($what) $var = $what

// object function call
#define $$($object, $func, $args...) ({ \
	$_($object$, $object); \
	$_($func$, $func); \
	$object$->$func$($object$, $args); \
})

// simpler malloc
#define new($type, $expr...) \
	malloc(sizeof($type) $expr)

// simpler malloc
#define create($type, $var, $expr...) \
	$type * $var = malloc(sizeof($type) $expr)

// simpler valloc
#define vnew($type, $expr...) \
	malloc(sizeof($type) $expr)

// simpler valloc
#define vcreate($type, $var, $expr...) \
	$type * $var = malloc(sizeof($type) $expr)

// calculate fixed-sized array length
#define array_length($array) \
	(sizeof($array) / sizeof(*($array)))


// boolean
typedef unsigned char bool;
#define true	1
#define false	0

///////////// the above codes are from cjxgm/libcrude

#endif

