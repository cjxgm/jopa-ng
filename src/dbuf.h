// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#ifndef __JOPANG$DBUF__
#define __JOPANG$DBUF__
// double buffer
#include <pthread.h>
#include "misc.h"

typedef struct DBuf
{
	float * bufs[2];
	size_t length;
	size_t current;
	size_t filled_cnt;
	pthread_mutex_t m;
}
DBuf;




// basic manipulation
DBuf * dbuf_new();
void   dbuf_free(DBuf * dbuf);

// lock
static inline void dbuf_lock  (DBuf * dbuf){pthread_mutex_lock  (&dbuf->m);}
static inline void dbuf_unlock(DBuf * dbuf){pthread_mutex_unlock(&dbuf->m);}

// advanced manipulation
float * dbuf_used  (DBuf * dbuf);
float * dbuf_unused(DBuf * dbuf);
void dbuf_fill  (DBuf * dbuf);
void dbuf_unfill(DBuf * dbuf);
void dbuf_resize(DBuf * dbuf, size_t length);

// queries
static inline size_t dbuf_length(DBuf * dbuf) { return dbuf->length; }
static inline size_t dbuf_size  (DBuf * dbuf)
{
	return dbuf_length(dbuf) * sizeof(**dbuf->bufs);
}

// extended functions
bool dbuf_add_stereo(DBuf * dbuf, float L[], float R[]);
bool dbuf_get_stereo(DBuf * dbuf, float L[], float R[]);

#endif

