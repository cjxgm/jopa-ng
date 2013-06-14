// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "dbuf.h"
#include "global.h"
#include "error.h"




DBuf * dbuf_new()
{
	create(DBuf, dbuf);
	if (!dbuf) throw(ERROR_MALLOC);

	dbuf->length  = 0;
	dbuf->m       = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

	return dbuf;
}


void dbuf_free(DBuf * dbuf)
{
	if (dbuf->length) {
		free(dbuf->bufs[0]);
		free(dbuf->bufs[1]);
	}
	free(dbuf);
}




float * dbuf_used(DBuf * dbuf)
{
	dbuf_lock(dbuf);
	float * buf;
	switch (dbuf->filled_cnt) {
		case 1:  buf = dbuf->bufs[!dbuf->current];	break;
		case 2:  buf = dbuf->bufs[ dbuf->current];	break;
		default: buf = NULL;						break;
	}
	dbuf_unlock(dbuf);

	return buf;
}


float * dbuf_unused(DBuf * dbuf)
{
	if (dbuf->filled_cnt == 2) return NULL;

	dbuf_lock(dbuf);
	float * buf = dbuf->bufs[dbuf->current];
	dbuf_unlock(dbuf);

	return buf;
}


void dbuf_fill(DBuf * dbuf)
{
	dbuf_lock(dbuf);
	dbuf->filled_cnt++;
	dbuf->current = !dbuf->current;
	dbuf_unlock(dbuf);
}


void dbuf_unfill(DBuf * dbuf)
{
	dbuf_lock(dbuf);
	dbuf->filled_cnt--;
	dbuf_unlock(dbuf);
}


void dbuf_resize(DBuf * dbuf, size_t length)
{
	dbuf_lock(dbuf);

	if (dbuf->length) {
		free(dbuf->bufs[0]);
		free(dbuf->bufs[1]);
	}

	dbuf->length = length;
	if (length) {
		dbuf->bufs[0] = new(**dbuf->bufs, * length);
		dbuf->bufs[1] = new(**dbuf->bufs, * length);
		if (!dbuf->bufs[0] || !dbuf->bufs[1]) throw(ERROR_MALLOC);
	}

	dbuf_unlock(dbuf);
}




bool dbuf_add_stereo(DBuf * dbuf, float L[], float R[])
{
	$_(buf, dbuf_unused(dbuf));
	if (!buf) return true;

	size_t n = dbuf_length(dbuf);
	if (n & 1) throw(ERROR_DBUF_LENGTH_NOT_EVEN);
	n >>= 1;

	for (size_t i=0; i<n; i++) {
		buf[i<<1 | 0] = L[i];
		buf[i<<1 | 1] = R[i];
	}

	dbuf_fill(dbuf);

	return false;
}

bool dbuf_get_stereo(DBuf * dbuf, float L[], float R[])
{
	$_(buf, dbuf_used(dbuf));
	if (!buf) return true;

	size_t n = dbuf_length(dbuf);
	if (n & 1) throw(ERROR_DBUF_LENGTH_NOT_EVEN);
	n >>= 1;

	for (size_t i=0; i<n; i++) {
		L[i] = buf[i<<1 | 0];
		R[i] = buf[i<<1 | 1];
	}

	dbuf_unfill(dbuf);

	return false;
}

