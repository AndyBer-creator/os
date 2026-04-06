/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 16227 $
 * $Date: 2011-03-10 18:23:52 +0800 (Thu, 10 Mar 2011) $
 *
 * Purpose : Forward plane manager daemon
 *
 * Feature : Forward plane manager daemon
 */

/*
 * Include Files
 */
#include "fpmd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#ifdef CONFIG_SYS_L3_ROUTE
#include <fpm/fpm.h>
#endif
#include <libsal/sal_crash.h>

/*
 * Symbol Definition
 */
#define FPM_SRV_PORT 2620

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

#ifdef CONFIG_SYS_L3_ROUTE
void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int32 read_from_client (int filedes)
{
    fpm_msg_hdr_t hdr;
    char *buffer = NULL;
    int nbytes;

    SYS_MEM_CLEAR(hdr);

    /* Read hdr first */
    nbytes = recv(filedes, &hdr, sizeof(fpm_msg_hdr_t), 0);

    if (nbytes < 0 || nbytes != sizeof(fpm_msg_hdr_t))
    {
        /* Read error. */
        perror ("read");
        exit (EXIT_FAILURE);
    }
    else if (nbytes == 0)
    {
        /* End-of-file. */
        return -1;
    }
    else
    {
        uint32 dataSize = hdr.msg_len - sizeof(fpm_msg_hdr_t);

        /* Data read. */
        buffer = osal_alloc(dataSize);
        osal_memset(buffer, 0, dataSize);

        nbytes = recv(filedes, buffer, dataSize, 0);

        if (nbytes < 0)
        {
            /* Read error. */
            perror ("read");
            exit (EXIT_FAILURE);
        }
        else if (nbytes == 0)
        {
            /* End-of-file. */
            osal_free(buffer);
            return -1;
        }

        route_transl(buffer, nbytes);

        osal_free(buffer);
        return 0;
    }
}

int32 make_socket (uint16 port)
{
  int sock;
  struct sockaddr_in name;

  /* Create the socket. */
  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
      perror ("socket");
      exit (EXIT_FAILURE);
  }

  osal_memset(&name, 0, sizeof(name));

  /* Give the socket a name. */
  name.sin_family = AF_INET;
  name.sin_port = htons (port);
  name.sin_addr.s_addr = INADDR_LOOPBACK;
  if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
  {
      perror ("bind");
      exit (EXIT_FAILURE);
  }

  return sock;
}
#endif

int main(int argc, char *argv[])
{

#ifdef CONFIG_SYS_L3_ROUTE
    int sock;
    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    size_t size;
    FILE *fp = NULL;

    addr_monitor_create();

    /* Create the socket and set it up to accept connections. */
    sock = make_socket (FPM_SRV_PORT);
    if (listen (sock, 1) < 0)
    {
        perror ("listen");
        exit (EXIT_FAILURE);
    }

    if (NULL != (fp = fopen(FPMD_PID_FILE, "w")))
    {
        fprintf(fp, "%d", getpid());
        fclose(fp);
    }

    /* Initialize the set of active sockets. */
    FD_ZERO(&active_fd_set);
    FD_SET(sock, &active_fd_set);

    while(1)
    {
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        /* Service all the sockets with input pending. */
        for (i = 0; i < FD_SETSIZE; ++i)
        {
            if (FD_ISSET(i, &read_fd_set))
            {
                if (i == sock)
                {
                    /* Connection request on original socket. */
                    int newsock;
                    size = sizeof(clientname);
                    newsock = accept(sock, (struct sockaddr *) &clientname, &size);
                    if (newsock < 0)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    FPMD_DBG("Server: connect from host %s, port %d.\n",
                             inet_ntoa(clientname.sin_addr),
                             ntohs(clientname.sin_port));
                    FD_SET(newsock, &active_fd_set);
                }
                else
                {
                    /* Data arriving on an already-connected socket. */
                    if (read_from_client(i) < 0)
                    {
                        close(i);
                        FD_CLR(i, &active_fd_set);
                    }
                }
            }
        }
    }
#else
    addr_monitor_create();

    while (1)
    {
        sleep(1200);
    }
#endif
}

