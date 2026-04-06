/* vi: set sw=4 ts=4: */
/*
 * Mini syslogd implementation for busybox
 *
 * Copyright (C) 1999-2003 by Erik Andersen <andersen@codepoet.org>
 *
 * Copyright (C) 2000 by Karl M. Hegbloom <karlheg@debian.org>
 *
 * "circular buffer" Copyright (C) 2001 by Gennady Feldman <gfeldman@gena01.com>
 *
 * Maintainer: Gennady Feldman <gfeldman@gena01.com> as of Mar 12, 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
/*
 * Note: simple ratelimit implementation of last n messages for busybox syslogd.
 */


#include <stdio.h>
#include <paths.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <semaphore.h>

#include "busybox.h"

#define SHM_KEY_CODE			'l'
#define SHM_KEY_PATH			"/var/run/syslogagg"
#define MSG_QUEUE_SIZE			1024

typedef struct syslog_ratelimit_msg_s
{
	/* ratelimit */
	time_t	begin;		/* time into pool */
	int		flush;		/* need to flush */
	/* searching performance */
	int		key;		/* searching key */
	char 	*pMsg;		/* message pointer for searching */
	/* messages */
	int		nsupp;		/* nbr of msg suppressed */
	time_t 	time;		/* time */
	int 	pri;		/* priority */
	int 	len;		/* message len */
	char	msg[1024];	/* message text */
} syslog_ratelimit_msg_t;

typedef struct syslog_msg_pool_s
{
	unsigned dead;
	unsigned head;
	unsigned tail;
	unsigned msgs;
	syslog_ratelimit_msg_t queue[MSG_QUEUE_SIZE];
} syslog_msg_pool_t;

static unsigned g_interval = 0;

static int g_shmid = -1; 
static int g_semid = -1;

static syslog_msg_pool_t *g_msg_pool = NULL;

#ifdef DEBUG_SYSLOGD
static inline void debug_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}
#else
static inline void debug_printf(const char *format, ...) { }
#endif

static inline int msg_ratelimit_queue_empty(void)
{
	return g_msg_pool->msgs == 0;
}

static inline int msg_ratelimit_queue_full(void)
{
	return g_msg_pool->msgs == MSG_QUEUE_SIZE;
}

static inline int msg_ratelimit_queue_lock(void)
{
	static struct sembuf lock = { 0, -1, 0 };

	return semop(g_semid, &lock, SEM_UNDO);
}

static inline int msg_ratelimit_queue_unlock(void)
{
	static struct sembuf unlock = { 0, 1, 0 };

	return semop(g_semid, &unlock, SEM_UNDO);
}

static int msg_ratelimit_hash_key_ret(char *msg, int len)
{
	int i = 0;
	int key = 0;

	for (i = 0; i < len; i++)
		key += msg[i];

	return key;
}

static int msg_ratelimit_queue_add(syslog_ratelimit_msg_t *msg)
{
	syslog_ratelimit_msg_t *r = NULL;

	debug_printf("%s() - add ratelimit pool for msg: %s\n", __FUNCTION__,  msg->msg);

	if (msg_ratelimit_queue_full())
	{
		debug_printf("%s() - ratelimit pool is full, return error\n", __FUNCTION__);
		return -1;
	}

	msg_ratelimit_queue_lock();

	r = &g_msg_pool->queue[g_msg_pool->tail];

	if (msg->len && msg->key)
	{
		r->time = msg->time;
		r->pri = msg->pri;
		r->len = msg->len;
		r->key = msg->key;
		memcpy(r->msg, msg->pMsg ? msg->pMsg : msg->msg, msg->len);
	}

	if (r->msg[0] != '\0')
	{
		r->flush = 0;
		r->nsupp = 0;
		r->begin = time(NULL);

		g_msg_pool->tail++;
		g_msg_pool->tail %= MSG_QUEUE_SIZE;
		g_msg_pool->msgs++;
	}

	msg_ratelimit_queue_unlock();

	return 0;
}

static int msg_ratelimit_queue_search_add(syslog_ratelimit_msg_t *msg, syslog_ratelimit_msg_t **pmsg)
{
	int i;
	int found = 0;
	syslog_ratelimit_msg_t *r = NULL;
	
	if (msg_ratelimit_queue_full())
	{
		debug_printf("%s() - ratelimit pool is full, return error\n", __FUNCTION__);
		return -1;
	}

	*pmsg = NULL;

	msg_ratelimit_queue_lock();

	/* search the same message previously for suppression */
	debug_printf("%s() - search ratelimit pool for msg: %s\n", __FUNCTION__, msg->msg);

	/* FIXME: faster searching */
	for (i = g_msg_pool->head; i < g_msg_pool->head + g_msg_pool->msgs; i++)
	{
		r = &g_msg_pool->queue[i % MSG_QUEUE_SIZE];

		if (r->msg[0] != '\0' && !r->flush 
			&& msg->pri == r->pri && msg->len == r->len && msg->key == r->key
			&& !memcmp(msg->msg, r->pMsg ? r->pMsg : r->msg, r->len))
		{
			r->nsupp++;
			found++;
			break;
		}
	}

	msg_ratelimit_queue_unlock();

	if (found)
	{
		*pmsg = r;
		debug_printf("%s() - msg found, nsupp: %u\n", __FUNCTION__, r->nsupp);
		return 0;
	}

    *pmsg = msg;

	return msg_ratelimit_queue_add(msg);
}

static int msg_ratelimit_queue_del(syslog_ratelimit_msg_t *msg, unsigned force)
{
	syslog_ratelimit_msg_t *r = NULL;

	if (msg_ratelimit_queue_empty())
	{
		debug_printf("%s() - ratelimit pool is empty", __FUNCTION__);
		return 0;
	}

	msg_ratelimit_queue_lock();

	r = &g_msg_pool->queue[g_msg_pool->head];

	if (force || r->flush)
	{
		g_msg_pool->head++;
		g_msg_pool->msgs--;

		memcpy(msg, r, sizeof(syslog_ratelimit_msg_t));
		memset(r, 0, sizeof(syslog_ratelimit_msg_t));

		g_msg_pool->head %= MSG_QUEUE_SIZE;
	}

	msg_ratelimit_queue_unlock();

	return 0;
}

static int msg_ratelimit_queue_flush(unsigned num)
{
	unsigned n;
	char buf[1024];
	syslog_ratelimit_msg_t r;

	n = (num < g_msg_pool->msgs ? num : g_msg_pool->msgs);

	debug_printf("%s() - try flushing first %u msgs in pool\n", __FUNCTION__, n);

	while (n-- > 0)
	{
		if (msg_ratelimit_queue_empty())
		{
			debug_printf("%s() - ratelimit pool is empty", __FUNCTION__);
			break;
		}

		memset(&r, 0, sizeof(syslog_ratelimit_msg_t));

		msg_ratelimit_queue_del(&r, 1);

		debug_printf("%s() - try flushing msg = %s (%u)\n", __FUNCTION__, r.msg, r.nsupp);

		if (r.msg[0] != '\0' && r.nsupp)
		{
			snprintf(buf, sizeof(buf), "%s, aggregaed (%u)", r.msg, r.nsupp);
			rtk_timestamp_and_log(r.pri, buf, strlen(buf), r.time);
		}
	}

	return 0;
}

static int log_ratelimit_flush(unsigned force)
{
	unsigned n;
	char buf[1024];
	syslog_ratelimit_msg_t r;

	if (force)
	{
		debug_printf("%s() - try flushing all msgs in pool\n", __FUNCTION__);
		return msg_ratelimit_queue_flush(MSG_QUEUE_SIZE);
	}

	debug_printf("%s() - try flushing timeout msgs in pool\n", __FUNCTION__);

	n = g_msg_pool->msgs;

	while (n-- > 0)
	{
		if (msg_ratelimit_queue_empty())
		{
			debug_printf("%s() - ratelimit pool is empty", __FUNCTION__);
			break;
		}

		memset(&r, 0, sizeof(syslog_ratelimit_msg_t));

		msg_ratelimit_queue_del(&r, 0);

		if (!r.flush)
			break;

		if (r.msg[0] != '\0' && r.nsupp)
		{
			snprintf(buf, sizeof(buf), "%s, aggregated (%u)", r.msg, r.nsupp);
			rtk_timestamp_and_log(r.pri, buf, strlen(buf), r.time);
		}
	}

	return 0;
}

static int log_ratelimit_thread(void)
{
	int i = 0;
	unsigned flush = 0;
	unsigned interval = g_interval;
	syslog_ratelimit_msg_t *r = NULL;
	time_t tt;

	while (!bb_got_signal && !g_msg_pool->dead)
	{
		flush = 0;
		tt = time(NULL);

		msg_ratelimit_queue_lock();

		for (i = g_msg_pool->head; i < g_msg_pool->head + g_msg_pool->msgs; i++)
		{
			r = &g_msg_pool->queue[i % MSG_QUEUE_SIZE];

			if (r->msg[0] != '\0' &&
				tt > r->begin + interval)
			{
				r->flush++;
				flush++;
			}
		}

		msg_ratelimit_queue_unlock();

		if (flush)
			log_ratelimit_flush(0);

		usleep(1000000);
	}

	/* flush all messages in the pool */
	log_ratelimit_flush(1);

	return 0;
}

int log_ratelimit_add(int pri, char *msg, int len, time_t tt)
{
	int ret;
	key_t key;
	syslog_ratelimit_msg_t r;
	syslog_ratelimit_msg_t *rr;

	if (!g_msg_pool)
	{
		if (!access(SHM_KEY_PATH, F_OK))
		{
			key = ftok(SHM_KEY_PATH, SHM_KEY_CODE);

			g_shmid = shmget(key, sizeof(syslog_msg_pool_t), 0666);
			g_semid = semget(key, 1, 0666);

			if (g_shmid >= 0 && g_semid >= 0)
				g_msg_pool = (syslog_msg_pool_t *)shmat(g_shmid, NULL, 0);
		}
	}

	if (!g_msg_pool)
	{
		rtk_timestamp_and_log(pri, msg, len, tt);
		return 0;
	}

	if (g_msg_pool->dead)
	{
		rtk_timestamp_and_log(pri, msg, len, tt);
		shmdt(g_msg_pool);
		g_msg_pool = NULL;
		return 0;
	}

	/* add the new message into pool */
	memset(&r, 0, sizeof(syslog_ratelimit_msg_t));

	r.time = tt;
	r.pri = pri;
	r.len = len;
	r.pMsg = msg;
	r.key = msg_ratelimit_hash_key_ret(msg, len);
	memcpy(r.msg, msg, len);

	ret = msg_ratelimit_queue_search_add(&r, &rr);

	/* For the new one, log it. */
	if (!ret && rr && !rr->nsupp)
	{
		rtk_timestamp_and_log(pri, msg, len, tt);
		return 0;
	}

	/* msg pool full */
	if (ret && msg_ratelimit_queue_full())
	{
		/* flush the oldest one and add the new one */
		msg_ratelimit_queue_flush(1);
		msg_ratelimit_queue_add(&r);
		rtk_timestamp_and_log(pri, msg, len, tt);
	}

	return 0;
}

void log_ratelimit_exit(void)
{
	if (g_msg_pool)
	{
		debug_printf("%s() - detach shared memory\n", __FUNCTION__);
		g_msg_pool->dead = 1;
		shmdt(g_msg_pool);
		g_msg_pool = NULL;
	}
}

int log_ratelimit_init(unsigned interval)
{
	int fd;
	pid_t pid;
	key_t key;

	if (!interval)
		return 0;

	/*
	 * child and parent initialization
	 */
	/* create shared memory key */
	if (creat(SHM_KEY_PATH, 0666) < 0)
		bb_perror_msg_and_die("create ratelimit shm file");

	key = ftok(SHM_KEY_PATH, SHM_KEY_CODE);

	/* create mutex for shared memory */
	if ((g_semid = semget(key, 1, 0666)) == -1)
	{
		g_semid = semget(key, 1, IPC_CREAT | 0666);

		if (g_semid < 0)
			bb_perror_msg_and_die("ratelimit sem mutex");
	}

	/* create shared memory */
	if ((g_shmid = shmget(key, sizeof(syslog_msg_pool_t), 0666)) == -1)
	{
		g_shmid = shmget(key, sizeof(syslog_msg_pool_t), IPC_CREAT | 0666);

		if (g_shmid < 0)
		{
			semctl(g_semid, 1, IPC_RMID, NULL);
			bb_perror_msg_and_die("ratelimit shmget");
		}

		g_msg_pool = (syslog_msg_pool_t *)shmat(g_shmid, NULL, 0);

		if ((char *)g_msg_pool == (char *)-1)
		{
			shmctl(g_shmid, IPC_RMID, NULL);
			semctl(g_semid, 1, IPC_RMID, NULL);
			bb_perror_msg_and_die("ratelimit shmat");
		}

		g_msg_pool->head = 0;
		g_msg_pool->tail = 0;
		g_msg_pool->msgs = 0;

		memset(g_msg_pool->queue, MSG_QUEUE_SIZE, 0);
	}

	/* FIXME: thread, instead of process */
	pid = fork();

	/* fork error */
	if (pid < 0)
	{
		shmdt(g_msg_pool);
		shmctl(g_shmid, IPC_RMID, NULL);
		semctl(g_semid, 1, IPC_RMID, NULL);
		bb_perror_msg_and_die("ratelimit child process");
	}

	/* parent initialization */
	if (pid > 0)
	{
		/* detatch the shm and attach it on adding */
		shmdt(g_msg_pool);
		g_msg_pool = NULL;
	}

	/* child initialization */
	if (pid == 0)
	{
		setsid();

		chdir("/");

		if ((fd = open(_PATH_DEVNULL, O_RDWR, 0)) != -1)
		{
			dup2(fd, STDIN_FILENO);
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);

			if (fd > 2)
				close(fd);
		}

		g_interval = interval;

		/* worker */
		if (g_interval)
			log_ratelimit_thread();

		/* exit */
		debug_printf("ratelimit - detach shared memory and exit (child)\n");

		unlink(SHM_KEY_PATH);

		/* detach message pool */
		shmdt(g_msg_pool);

		/* remove shared memory and mutex */
		shmctl(g_shmid, IPC_RMID, NULL);
		semctl(g_semid, 1, IPC_RMID, NULL);

		exit(0);
	}

	return pid;
}
