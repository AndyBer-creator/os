/*
 *  Hydra, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996,97 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1997,99 Jon Nelson <jnelson@boa.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
/* RTK: (20131223) Prevent header too long make system crash */

/* $Id: read.c,v 1.9 2003/01/22 07:51:50 nmav Exp $*/

#include "boa.h"
#include "socket.h"

/* RTK: Upgrade Firmware */
#include <pswitch/image.h>
#include <libsal/sal_sys.h>
#include <common/sys_util.h>

struct {
    int fileOffset;
    int bFirstSegment;
    int totalSize;
    int writeSize;
    int reqTolSize;
    int size;
    int magicErr;
    int fileErr;
    int detect_content_type;
    int imageBlock;
}uploadFlags = {
    .fileOffset = 0,
    .bFirstSegment = 1,
    .totalSize = 0,
    .writeSize = 0,
    .reqTolSize = 0,
    .size = 0,
    .magicErr = 0,
    .fileErr = 0,
    .detect_content_type = 0, /* Content-Type man not in first segement*/
    .imageBlock = 0,
    };

void getFdStr(char* buf, int fd, int length);
int extractVmlinuxImage(request * req);
static void _uploadFlagsClear(void);

static void _uploadFlagsClear()
{
    uploadFlags.size = 0;
    uploadFlags.fileOffset = 0;
    uploadFlags.bFirstSegment = 1;
    uploadFlags.totalSize = 0;
    uploadFlags.writeSize = 0;
    uploadFlags.reqTolSize = 0;
    uploadFlags.fileErr = 0;
    uploadFlags.detect_content_type=0;
    uploadFlags.imageBlock = 0;
}

/*
 * Name: read_header
 * Description: Reads data from a request socket.  Manages the current
 * status via a state machine.  Changes status from READ_HEADER to
 * READ_BODY or WRITE as necessary.
 *
 * Return values:
 *  -1: request blocked, move to blocked queue
 *   0: request done, close it down
 *   1: more to do, leave on ready list
 */

int
read_header(server_params *params, request *req)
{
    int bytes,
        buf_bytes_left;
    char *check,
        *buffer;

    check = req->client_stream + req->parse_pos;
    buffer = req->client_stream;
    bytes = req->client_stream_pos;

#ifdef VERY_FASCIST_LOGGING
    if (check < (buffer + bytes)) {
        buffer[bytes] = '\0';
        log_error_time();
        fprintf(stderr, "%s:%d - Parsing headers (\"%s\")\n",
            __FILE__, __LINE__, check);
    }
#endif
    /* RTK: (20131223) Prevent header too long make system crash */
    if (bytes > CLIENT_STREAM_SIZE)
    {
        fprintf(stderr, "bytes %d > CLIENT_STREAM_SIZE %d\n", bytes, CLIENT_STREAM_SIZE);
        send_r_bad_request(req);
        return 0;
    }
    while (check < (buffer + bytes)) {
        switch (req->status) {
        case READ_HEADER:
            if (*check == '\r') {
                req->status = ONE_CR;
                req->header_end = check;
            } else if (*check == '\n') {
                req->status = ONE_LF;
                req->header_end = check;
            }
            break;

        case ONE_CR:
            if (*check == '\n')
                req->status = ONE_LF;
            else if (*check != '\r')
                req->status = READ_HEADER;
            break;

        case ONE_LF:
            /* if here, we've found the end (for sure) of a header */
            if (*check == '\r') /* could be end of headers */
                req->status = TWO_CR;
            else if (*check == '\n')
                req->status = BODY_READ;
            else
                req->status = READ_HEADER;
            break;

        case TWO_CR:
            if (*check == '\n')
                req->status = BODY_READ;
            else if (*check != '\r')
                req->status = READ_HEADER;
            break;

        default:
            break;
        }

#ifdef VERY_FASCIST_LOGGING
        log_error_time();
        fprintf(stderr, "status, check: %d, %d\n", req->status, *check);
#endif

        req->parse_pos++;       /* update parse position */
        check++;

        if (req->status == ONE_LF) {
            *req->header_end = '\0';

            /* terminate string that begins at req->header_line */

     if (req->logline) {
        if (process_option_line(req) == 0) {
           return 0;
        }
     } else {
        if (process_logline(req) == 0)
           return 0;
        if (req->http_version == HTTP_0_9)
           return process_header_end(params, req);
     }
     /* set header_line to point to beginning of new header */
     req->header_line = check;
      } else if (req->status == BODY_READ) {
        /* RTK: Mantis#4504 */
        if (req->method == M_POST && req->content_length == 0)
        {
            log_error_time();
            fprintf(stderr, "Unknown Content-Length POST!\n");
            send_r_bad_request(req);
            return 0;
        }

#ifdef VERY_FASCIST_LOGGING
            int retval;
            log_error_time();
            fprintf(stderr, "%s:%d -- got to body read.\n",
                __FILE__, __LINE__);
            retval = process_header_end(params, req);
#else
            int retval = process_header_end(params, req);
#endif
            /* process_header_end inits non-POST cgi's */

            if (retval && req->method == M_POST) {
                /* for body_{read,write}, set header_line to start of data,
                   and header_end to end of data */
                req->header_line = check;
                req->header_end = req->client_stream + req->client_stream_pos;

                req->status = BODY_WRITE;
                /* so write it */
                /* have to write first, or read will be confused
                 * because of the special case where the
                 * filesize is less than we have already read.
                 */

                /*

                  As quoted from RFC1945:

                  A valid Content-Length is required on all HTTP/1.0 POST requests. An
                  HTTP/1.0 server should respond with a 400 (bad request) message if it
                  cannot determine the length of the request message's content.

                */

                if (req->content_length) {
                    int content_length;

                    content_length = boa_atoi(req->content_length);
                    /* Is a content-length of 0 legal? */
                    if (content_length < 0) {
                        log_error_time();
                        fprintf(stderr,
                            "Invalid Content-Length [%s] on POST!\n",
                            req->content_length);
                        send_r_bad_request(req);
                        return 0;
                    }
                    if (single_post_limit && content_length > single_post_limit) {
                        log_error_time();
                        fprintf(stderr,
                            "Content-Length [%d] > SinglePostLimit [%d] on POST!\n",
                            content_length, single_post_limit);
                      #if 0 /* RTK: Allow big file size for http upgrade firmware */
                        send_r_bad_request(req);
                        return 0;
                      #endif
                    }
                    req->filesize = content_length;
                    req->filepos = 0;
                    if (req->header_end - req->header_line > req->filesize) {
                        req->header_end = req->header_line + req->filesize;
                    }
                } else {
                    log_error_time();
                    fprintf(stderr, "Unknown Content-Length POST!\n");
                    send_r_bad_request(req);
                    return 0;
                }
            }           /* either process_header_end failed or req->method != POST */
            return retval;      /* 0 - close it done, 1 - keep on ready */
        }               /* req->status == BODY_READ */
    }               /* done processing available buffer */

#ifdef VERY_FASCIST_LOGGING
    log_error_time();
    fprintf(stderr, "%s:%d - Done processing buffer.  Status: %d\n",
        __FILE__, __LINE__, req->status);
#endif

    if (req->status < BODY_READ) {
        /* only reached if request is split across more than one packet */

        buf_bytes_left = CLIENT_STREAM_SIZE - req->client_stream_pos;
        if (buf_bytes_left < 1) {
            log_error_time();
            fputs("buffer overrun - read.c, read_header - closing\n", stderr);
            req->status = DEAD;
            return 0;
        }

        bytes = socket_recv(req, buffer + req->client_stream_pos,
                    buf_bytes_left);

        if (bytes < 0) {
            if (bytes == BOA_E_INTR)
                return 1;
            if (bytes == BOA_E_AGAIN)
                return -1;

            log_error_doc(req);
            perror("header read");  /* don't need to save errno because log_error_doc does */
            return 0;
        } else if (bytes == 0) {
            /* this is an error.  premature end of headers! */
            return 0;
        }

        /* bytes is positive */
        req->client_stream_pos += bytes;

#ifdef FASCIST_LOGGING1
        log_error_time();
        req->client_stream[req->client_stream_pos] = '\0';
        fprintf(stderr, "%s:%d -- We read %d bytes: \"%s\"\n",
            __FILE__, __LINE__, bytes,
#ifdef VERY_FASCIST_LOGGING2
            req->client_stream + req->client_stream_pos - bytes);
#else
        "");
#endif
#endif
    }
    return 1;
} /* read_header() */

/*
 * Name: read_body
 * Description: Reads body from a request socket for POST CGI
 *
 * Return values:
 *
 *  -1: request blocked, move to blocked queue
 *   0: request done, close it down
 *   1: more to do, leave on ready list
 *

 As quoted from RFC1945:

 A valid Content-Length is required on all HTTP/1.0 POST requests. An
 HTTP/1.0 server should respond with a 400 (bad request) message if it
 cannot determine the length of the request message's content.

 */

int
read_body(request *req)
{
int bytes_read, bytes_to_read, bytes_free;

bytes_free = BUFFER_SIZE - (req->header_end - req->header_line);
bytes_to_read = req->filesize - req->filepos;

   if (bytes_to_read > bytes_free)
      bytes_to_read = bytes_free;

   if (bytes_to_read <= 0) {
      req->status = BODY_WRITE; /* go write it */
      return 1;
   }

   bytes_read = socket_recv(req, req->header_end, bytes_to_read);

   if (bytes_read < 0) {
      if (bytes_read == BOA_E_AGAIN) {
     return -1;
      } else {
          /* RTK: On IE browser, if the http upgrade firmware procedure is interrupted, this error will happen. */
          if (ECONNRESET == errno)
          {
              if (strcmp(req->pathname, "/home/web/cgi-bin/httpupload.cgi") == 0)
              {
                  fprintf(stderr, "Connection reset during firmware upload\n");
                  /* If image is block by other process, do not delete lock file */
                  if (!uploadFlags.imageBlock)
                       sal_sys_fwImageLock_give();

                  _uploadFlagsClear();
              }
          }
     boa_perror(req, "read body");
     return 0;
      }

   } else if (bytes_read == 0) {
      /* this is an error.  premature end of body! */
      log_error_time();
      fprintf(stderr, "%s:%d - Premature end of body!!\n",
          __FILE__, __LINE__);
      send_r_bad_request(req);

      /* RTK: If the http upgrade firmware procedure is interrupted, this error will happen.
              After this error happened, if user upgrade firmware again, the get http info procedure
              will cause system out of memory. In order to prevent this crash issue, we restart boa
              after this error.*/
      fprintf(stderr, "Abnormal boa status, restart boa\n");

      /* If image is block by other process, do not delete lock file */
      if (!uploadFlags.imageBlock)
          sal_sys_fwImageLock_give();

      system("handler -c boa &");

      return 0;
   }

   req->status = BODY_WRITE;

#ifdef FASCIST_LOGGING1
   log_error_time();
   fprintf(stderr, "%s:%d - read %d bytes.\n",
       __FILE__, __LINE__, bytes_to_read);
#endif

   req->header_end += bytes_read;

   return 1;
}

/*
 * Name: write_body
 * Description: Writes a chunk of data to a file
 *
 * Return values:
 *  -1: request blocked, move to blocked queue
 *   0: EOF or error, close it down
 *   1: successful write, recycle in ready queue
 */

int write_body(request * req)
{
   int bytes_written, bytes_to_write = req->header_end - req->header_line;
   if (req->filepos + bytes_to_write > req->filesize)
      bytes_to_write = req->filesize - req->filepos;

   if (bytes_to_write == 0) {   /* nothing left in buffer to write */
      req->header_line = req->header_end = req->buffer;
      if (req->filepos >= req->filesize)
     return init_cgi(req);
      /* if here, we can safely assume that there is more to read */
      req->status = BODY_READ;
      return 1;
   }

   /* RTK: Upgrade Firmware */
   if(strcmp(req->pathname, "/home/web/cgi-bin/httpupload.cgi") == 0)
       bytes_written = extractVmlinuxImage(req);
   else
       bytes_written = write(req->post_data_fd.fds[1], req->header_line, bytes_to_write);

   if (bytes_written == -1) {
      if (errno == EWOULDBLOCK || errno == EAGAIN)
     return -1;     /* request blocked at the pipe level, but keep going */
      else if (errno == EINTR)
     return 1;
      else if (errno == ENOSPC) {
     /* 20010520 - Alfred Fluckiger */
     /* No test was originally done in this case, which might  */
     /* lead to a "no space left on device" error.             */
     boa_perror(req, "write body"); /* OK to disable if your logs get too big */
     return 0;
      } else {
     boa_perror(req, "write body"); /* OK to disable if your logs get too big */
     return 0;
      }
   }
#ifdef FASCIST_LOGGING
   log_error_time();
   fprintf(stderr, "%s:%d - wrote %d bytes. %ld of %ld\n",
       __FILE__, __LINE__, bytes_written, req->filepos, req->filesize);
#endif

   req->filepos += bytes_written;
   req->header_line += bytes_written;

   return 1;            /* more to do */
}

/* RTK: Upgrade Firmware */
int extractVmlinuxImage(request * req)
{
    int     bytes_written = 0;
    int     bytes_to_write = req->header_end - req->header_line;
    int     byte_read = 0;
    int     byte_write = 0;
    int     uploadRst = 7;
    char    ch;
    char    cmdStr[128];
    char    buf[256];
    FILE    *fp = NULL;
    FILE    *fp_info = NULL;
    unsigned int  magicNum = 0;
    unsigned int  count = 0;
    uint32 checksum = 0;
    uint32 calcksum = 0;

    if (req->filepos + bytes_to_write > req->filesize)
    {
        bytes_to_write = req->filesize - req->filepos;
    }

    bytes_written = write(req->post_data_fd.fds[1], req->header_line, bytes_to_write);

    if( bytes_written <= 0)
        return -1;

    if (uploadFlags.fileErr || uploadFlags.imageBlock)
    {
        char tmpbuf[bytes_to_write];

        lseek(req->post_data_fd.fds[1], 0, SEEK_SET);
        read(req->post_data_fd.fds[1], tmpbuf, bytes_to_write);
        lseek(req->post_data_fd.fds[1], 0, SEEK_SET);

        uploadFlags.reqTolSize += bytes_written;

        //fprintf(stderr, "file uploadFlags.reqTolSize=%d req->filesize=%d\n", uploadFlags.reqTolSize, req->filesize);

        if (uploadFlags.reqTolSize >= req->filesize)
        {
            _uploadFlagsClear();
            //fprintf(stderr, "file end\n");
        }

        return bytes_written;
    }

    /* Find out the start point of the attached file */
    lseek(req->post_data_fd.fds[1], 0, SEEK_SET);

    if (uploadFlags.bFirstSegment)
    {
        char tmpbuf[bytes_to_write];
        int tmpHeaderSize = 0;
        int tmpWriteSize = 0;
        int ret = SYS_ERR_FAILED;
        if(uploadFlags.detect_content_type==0)
        {
            /* If request file size < 1M, it cannot be a valid image, we just abort this transmission. */
            if (req->filesize < (1024 * 1024))
            {
                _uploadFlagsClear();
                return bytes_written;
            }
            /* If request file size > 13M, it cannot be a valid image, we just abort this transmission. */
            else if (req->filesize > single_post_limit)
            {
                _uploadFlagsClear();
                return bytes_written;
            }

            memset(tmpbuf, 0, bytes_to_write);
            fp_info = fopen(TMP_FWIMG_HTTPINFO, "w");
            while (1)
            {
                memset(buf, 0, sizeof(buf));
                getFdStr(buf, req->post_data_fd.fds[1], sizeof(buf));
                tmpHeaderSize += strlen(buf);
                fwrite(buf, 1, strlen(buf), fp_info);
                if (memcmp(buf, "Content-Type", 12) == 0)
                {
                    uploadFlags.detect_content_type = 1;
                    break;
                }
                /* Maybe there is a risk here:
                   1.Not found "Content-Type", will loop forever,Need return and check next packet
                   2.Found  "Content-Type", but data in next packet.Not found in the current pakcet like-- Safari 5.1.7
                   3.Found "Content-Type", and with data in the current packet.
                */
                if (tmpHeaderSize >= bytes_to_write)
                {
                    fclose(fp_info);
                    lseek(req->post_data_fd.fds[1], 0, SEEK_SET);
                    return bytes_to_write;
                }
            }
            fclose(fp_info);

            if (uploadFlags.detect_content_type)
            {
                memset(buf, 0, sizeof(buf));
                getFdStr(buf, req->post_data_fd.fds[1], sizeof(buf));
                tmpHeaderSize += strlen(buf);
                fp_info = fopen(TMP_FWIMG_HTTPINFO, "a+");

                fwrite(buf, 1, strlen(buf), fp_info);
                fclose(fp_info);
            }

            /* Risk Case 1,2 return and check next packet */
            if (tmpHeaderSize == bytes_to_write)
            {
               lseek(req->post_data_fd.fds[1], 0, SEEK_SET);
               return bytes_to_write;
            }
        }

#ifdef CONFIG_SYS_FW_UPGRADE_CHECKMEM
        ret = sal_sys_fwImageFreeMem_check();

        if (SYS_ERR_IMAGE_OUT_OF_MEMORY == ret)
        {
            add_cgi_env(req, "SYS_MEM", "insuff", 0);

            uploadFlags.bFirstSegment = 0;
            uploadFlags.size = 0;
            uploadFlags.totalSize = 0;
            uploadFlags.reqTolSize = 0;

            return bytes_written;
        }
        else
        {
            add_cgi_env(req, "SYS_MEM", "suff", 0);
        }
#endif

        /* Read the image file */
        read(req->post_data_fd.fds[1], tmpbuf, bytes_to_write - tmpHeaderSize);

        /* Check image lock */
        ret = sal_sys_fwImageLock_check();

        if (SYS_ERR_IMAGE_PROCESSING == ret)
        {
            uploadFlags.imageBlock = 1;
            uploadFlags.reqTolSize += bytes_written;
            add_cgi_env(req, "UP_STS", "block", 0);

            return bytes_written;
        }
        else
        {
            add_cgi_env(req, "UP_STS", "upgrading", 0);
        }

        /* Take firmware upgrade lock */
        sal_sys_fwImageLock_take();
        fp = fopen(TMP_FWIMG_FILENAME, "w");
        fclose(fp);

        fp = fopen(TMP_FWIMG_FILENAME, "r+b");
        uploadFlags.reqTolSize += bytes_written;
        uploadFlags.totalSize += bytes_to_write - tmpHeaderSize;

        /* Write image part to tmp file */
        fseek(fp, 0, SEEK_SET);
        fwrite(tmpbuf, 1, bytes_to_write - tmpHeaderSize, fp);
        fclose(fp);

        lseek(req->post_data_fd.fds[1], 0, SEEK_SET);

        uploadFlags.bFirstSegment = 0;

        return bytes_to_write;
    }

    if (uploadFlags.reqTolSize < req->filesize)
    {
        char tmpbuf[bytes_to_write];

        memset(tmpbuf, 0, sizeof(bytes_to_write));
        read(req->post_data_fd.fds[1], tmpbuf, bytes_to_write);

        fp = fopen(TMP_FWIMG_FILENAME, "r+b");
        fseek(fp, uploadFlags.totalSize, SEEK_SET);
        fwrite(tmpbuf, 1, bytes_to_write, fp);
        fclose(fp);

        uploadFlags.totalSize += bytes_to_write;
        uploadFlags.reqTolSize += bytes_written;
    }

    if (uploadFlags.reqTolSize >= req->filesize)
    {
        _uploadFlagsClear();

        add_cgi_env(req, "UP_STS", "done", 0);
        //fprintf(stderr, "file end\n");
    }

    lseek(req->post_data_fd.fds[1], 0, SEEK_SET);

    return bytes_written;
}

/*get a string from file descriptor*/
void getFdStr(char* buf, int fd, int length)
{
    char ch;

    if (length <= 0)
        return;

    while(read(fd, &ch, 1))
    {
        if (length <= 0)
            return;

        if (ch == 0xd)
        {
            read(fd, &ch, 1);

            if (ch == 0xa)
            {
                *buf = 0xd;
                buf++;
                if (--length <= 0)
                    break;

                *buf = 0xa;
                buf++;
                if (--length <= 0)
                    break;

                *buf = 0;
                break;
            }
        }
        *buf = ch;
        buf++;
        if (--length <= 0)
            break;
    }
}

