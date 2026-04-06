
#ifndef _ZEBRA_NETWORK_H
#define _ZEBRA_NETWORK_H

extern int readn (int, u_char *, int);
extern int writen (int, const u_char *, int);

extern int set_nonblocking(int fd);

#define ERRNO_IO_RETRY(EN) \
    (((EN) == EAGAIN) || ((EN) == EWOULDBLOCK) || ((EN) == EINTR))

#endif 
