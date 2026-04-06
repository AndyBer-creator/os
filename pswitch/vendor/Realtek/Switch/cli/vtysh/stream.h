
#ifndef _ZEBRA_STREAM_H
#define _ZEBRA_STREAM_H

#include "prefix.h"

struct stream
{
  struct stream *next;

  size_t getp;         
  size_t endp;        
  size_t size;        
  unsigned char *data; 
};

struct stream_fifo
{
  size_t count;

  struct stream *head;
  struct stream *tail;
};

#define STREAM_SIZE(S)  ((S)->size)
  
#define STREAM_WRITEABLE(S) ((S)->size - (S)->endp)
  
#define STREAM_READABLE(S) ((S)->endp - (S)->getp)

#define STREAM_PNT(S)   stream_pnt((S))
#define STREAM_DATA(S)  ((S)->data)
#define STREAM_REMAIN(S) STREAM_WRITEABLE((S))

extern struct stream *stream_new (size_t);
extern void stream_free (struct stream *);
extern struct stream * stream_copy (struct stream *, struct stream *src);
extern struct stream *stream_dup (struct stream *);
extern size_t stream_resize (struct stream *, size_t);
extern size_t stream_get_getp (struct stream *);
extern size_t stream_get_endp (struct stream *);
extern size_t stream_get_size (struct stream *);
extern u_char *stream_get_data (struct stream *);

extern void stream_set_getp (struct stream *, size_t);
extern void stream_forward_getp (struct stream *, size_t);
extern void stream_forward_endp (struct stream *, size_t);

extern void stream_put (struct stream *, const void *, size_t);
extern int stream_putc (struct stream *, u_char);
extern int stream_putc_at (struct stream *, size_t, u_char);
extern int stream_putw (struct stream *, u_int16_t);
extern int stream_putw_at (struct stream *, size_t, u_int16_t);
extern int stream_putl (struct stream *, u_int32_t);
extern int stream_putl_at (struct stream *, size_t, u_int32_t);
extern int stream_putq (struct stream *, uint64_t);
extern int stream_putq_at (struct stream *, size_t, uint64_t);
extern int stream_put_ipv4 (struct stream *, u_int32_t);
extern int stream_put_in_addr (struct stream *, struct in_addr *);
extern int stream_put_prefix (struct stream *, struct prefix *);

extern void stream_get (void *, struct stream *, size_t);
extern u_char stream_getc (struct stream *);
extern u_char stream_getc_from (struct stream *, size_t);
extern u_int16_t stream_getw (struct stream *);
extern u_int16_t stream_getw_from (struct stream *, size_t);
extern u_int32_t stream_getl (struct stream *);
extern u_int32_t stream_getl_from (struct stream *, size_t);
extern uint64_t stream_getq (struct stream *);
extern uint64_t stream_getq_from (struct stream *, size_t);
extern u_int32_t stream_get_ipv4 (struct stream *);

#undef stream_read
#undef stream_write

extern int stream_read (struct stream *, int, size_t);

extern int stream_read_unblock (struct stream *, int, size_t);

extern ssize_t stream_read_try(struct stream *s, int fd, size_t size);

extern ssize_t stream_recvmsg (struct stream *s, int fd, struct msghdr *,
                               int flags, size_t size);
extern ssize_t stream_recvfrom (struct stream *s, int fd, size_t len,
                                int flags, struct sockaddr *from,
                                socklen_t *fromlen);
extern size_t stream_write (struct stream *, const void *, size_t);

extern void stream_reset (struct stream *);
extern int stream_flush (struct stream *, int);
extern int stream_empty (struct stream *); 

extern u_char *stream_pnt (struct stream *);

extern struct stream_fifo *stream_fifo_new (void);
extern void stream_fifo_push (struct stream_fifo *fifo, struct stream *s);
extern struct stream *stream_fifo_pop (struct stream_fifo *fifo);
extern struct stream *stream_fifo_head (struct stream_fifo *fifo);
extern void stream_fifo_clean (struct stream_fifo *fifo);
extern void stream_fifo_free (struct stream_fifo *fifo);

#endif 
