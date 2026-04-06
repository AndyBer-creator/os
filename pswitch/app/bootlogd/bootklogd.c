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
 * $Revision: 16116 $
 * $Date: 2011-03-07 09:20:10 +0800 (Mon, 07 Mar 2011) $
 *
 * Purpose : Boot Logging
 *
 * Feature : Execute
 */

 /*
 * Include Files
 */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <paths.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <sys/klog.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>

#include <common/sys_def.h>
#include <libsal/sal_sys.h>
#include <pswitch/image.h>

/*
 * Symbol Definition
 */
#define RTK_INIT_FAILURE_STR    "rtkErr"

/*
 * Data Declaration
 */
static int g_logfd = -1;
static int g_signal = 0;
static int g_failure = 0;

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */
static int32 _sysinfoVar_get(char *name, char *pValue)
{
    char *buf = NULL;
    char *data_ptr = NULL;
    char *varname = NULL;
    char devStr[16];
    int32 fd = -1;
    int32 i = 0;
    int32 nxt = 0;
    int32 varval = 0;
    //uint32 calchksum = 0;
    //uint32 crc = 0;
    uint32 size = 0;
    mtd_info_t mtd_info;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pValue), SYS_ERR_NULL_POINTER);

    osal_memset(devStr, 0, sizeof(devStr));

    osal_strcpy(devStr, "/dev/mtdchar4");

    if ((fd = open(devStr, O_RDONLY)) < 0)
        return SYS_ERR_DEV_OPEN;

    /* Get MTD partition size information */
    if (ioctl(fd, MEMGETINFO, &mtd_info) < 0)
    {
        close(fd);

        return SYS_ERR_IOCTL;
    }

    size = SYSINFO_ENV_SIZE;

    if (size > mtd_info.size)
        size = mtd_info.size;

    buf = malloc(size);
    /* Skip CRC field */
    data_ptr = buf + 4;

    if (read(fd, buf, size) < 0)
    {
        close(fd);
        free(buf);

        return SYS_ERR_DEV_READ;
    }

    close(fd);

    //crc = ((buf[0] & 0xFF) << 24) + ((buf[1] & 0xFF) << 16) + ((buf[2] & 0xFF) << 8) + (buf[3] & 0xFF);

#if 0
    calchksum = sal_util_crc32_ret(calchksum, data_ptr, size - 4);
    /* Check CRC */
    if (crc != calchksum)
    {
        free(buf);

        return SYS_ERR_INFO_BADCRC;
    }
#endif
    for (i = 0; *(data_ptr + i) != '\0'; i = nxt + 1)
    {
        varname = data_ptr + i;
        varval = 0;

        for (nxt = i; *(data_ptr + nxt) != '\0'; ++nxt)
        {
            if (nxt > size)
            {
                free(buf);

                return SYS_ERR_INFO_NOT_EXIST;
            }

            if ('=' == *(data_ptr + nxt))
            {
                if (0 == osal_strncmp(name, varname, nxt - i))
                {
                    varval = nxt + 1;
                }
            }
        }
        if (0 != varval)
        {
            osal_strncpy(pValue, data_ptr + varval, nxt - varval);
            pValue[nxt - varval] = '\0';
            free(buf);

            return SYS_ERR_OK;
        }
    }

    free(buf);

    return SYS_ERR_INFO_NOT_EXIST;
}

static void _overlapping_strcpy(char *dst, const char *src)
{
    if (dst != src)
    {
        while ((*dst = *src) != '\0')
        {
            dst++;
            src++;
        }
    }
}

ssize_t _safe_read (int fd, void *buf, size_t count)
{
    ssize_t r;
    size_t reads = 0;

    while (reads < count &&
           (r = read(fd, (char *)buf + reads, count - reads)) != 0)
    {
        if (r < 0) {
            if (errno == EINTR || errno == EAGAIN)
                continue;

            return -1;
        }
        reads += r;
    }

    return reads;
}

static ssize_t _safe_write(int fd, const void *buf, size_t count)
{
    ssize_t w;
    size_t written = 0;

    while (written < count &&
           (w = write(fd, (char *)buf + written, count - written)) != 0)
    {
        if (w < 0) {
            if (errno == EINTR || errno == EAGAIN)
                continue;

            return -1;
        }
        written += w;
    }

    return written;
}

static ssize_t _file_copy(const char *src, const char *dst)
{
    int src_fd = -1;
    int dst_fd = -1;
    ssize_t r = 0;
    ssize_t total = 0;
    char buf[SYS_BUF4096_LEN];

    src_fd = open(src, O_RDONLY);
    if (src_fd < 0)
        return -1;

    dst_fd = open(dst, O_WRONLY | O_TRUNC | O_CREAT, 0600);
    if (dst_fd < 0)
    {
        close(src_fd);
        return -1;
    }

    while (1)
    {
        r = _safe_read(src_fd, buf, sizeof(buf));

        if (r > 0)
        {
            if (r != _safe_write(dst_fd, buf, r))
                break;
        }
        else if (r == 0)
        {
            break;
        }
        else
        {
            break;
        }

        total += r;
    }

    close(src_fd);
    close(dst_fd);

    return total;
}

static void _file_rotate(const char *file, const int n)
{
    int i = 0;
    char backup[SYS_BUF1024_LEN];
    char rotate[SYS_BUF1024_LEN];

    if (n < 1)
        return;

    /* simple file rotate - boot.log, boot.log.0, boot.log.1, ..., and so on */
    snprintf(backup, sizeof(backup), "%s", file);

    while (!access(backup, F_OK) && i < n - 1)
        snprintf(backup, sizeof(backup), "%s.%u", file, i++);

    if (i-- >= n - 1)
    {
        snprintf(backup, sizeof(backup), "%s.%u", file, i);
        unlink(backup);
    }

    while (i-- > 0)
    {
        snprintf(backup, sizeof(rotate), "%s.%u", file, i);
        snprintf(rotate, sizeof(backup), "%s.%u", file, i + 1);
        rename(backup, rotate);
    }

    rename(file, backup);
}

static void bootmsg_sig_handler(int sig)
{
    g_signal = sig;
}

static void bootmsg_log(const char *str, ...)
{
    char msg[SYS_BUF1024_LEN];
    char *p;
    va_list args;

    if (g_logfd < 0)
    {
        g_logfd = open(SYS_BOOTMSG_TEMP_FILE, O_WRONLY | O_CREAT | O_APPEND, 0600);
        if (g_logfd < 0)
            return;
    }

    va_start(args, str);
    vsprintf(msg, str, args);
    va_end(args);

    p = msg;

    /* remove leading <0..7> */
    if ('<' == p[0] && '0' <= p[1] &&
        '7' >= p[1] && '>' == p[2])
    {
        p += 3;
    }

    _safe_write(g_logfd, p, strlen(p));
    _safe_write(g_logfd, "\n", 1);

    /* boot failure, backup to flash and relocate log file */
    if (strstr(p, SYS_INIT_FAILURE_STR) || strstr(p, RTK_INIT_FAILURE_STR))
        g_failure = TRUE;
}

static void bootmsg_level_init(void)
{
    int bootmsg = BOOTMSG_LEVEL_ALL;
    int ret = SYS_ERR_FAILED;
    int level = 0;
    char val[SYS_BUF16_LEN];

    SYS_MEM_CLEAR(val);

    ret = _sysinfoVar_get("bootmsg", val);

    if (SYS_ERR_OK == ret)
        bootmsg = atoi(val);

    switch (bootmsg)
    {
        case BOOTMSG_LEVEL_NONE:
            level = 1;
            break;

        case BOOTMSG_LEVEL_FAIL:
            level = 4;
            break;

        case BOOTMSG_LEVEL_ALL:
            level = 7;
            break;

        default:
            level = 7;
            break;
    }

    /* set printk level */
    klogctl(8, NULL, level);
}

static void bootmsg_logger(void)
{
    int n = 0;
    int used = 0;
    char buf[SYS_BUF4096_LEN];
    char *p;
    char *e;

    openlog("kernel", 0, LOG_KERN);
    klogctl(1, NULL, 0);

    while (!g_signal)
    {
        p = buf + used;
        n = klogctl(2, p, sizeof(buf) - 1 - used);

        if (n < 0)
            continue;

        p[n] = '\0';

        p = buf;

        while (p)
        {
            e = strchr(p, '\n');

            /* The line is incomplete */
            if (e)
            {
                *e++ = '\0';
            }
            else
            {
                /* buffer relocating */
                _overlapping_strcpy(buf, p);
                used = strlen(buf);

                /* buffer is not full */
                if (used < sizeof(buf) - 1)
                    break;

                /* buffer is full, flush it anyway */
            }

            bootmsg_log(p);

            p = e;
        }
    }

    klogctl(8, NULL, 5);
    klogctl(0, NULL, 0);
}

static void bootmsg_flush(void)
{
    int i = 0;
    char file[SYS_BUF1024_LEN];
    struct stat s;

    if (!g_failure)
        return;

    if (stat(SYS_BOOTMSG_TEMP_FILE, &s) < 0)
        return;

    if (g_logfd >= 0)
        close(g_logfd);

    snprintf(file, sizeof(file), "%s-", SYS_BOOTMSG_BACKUP_FILE);

    for (i = 0; i < 3; i++)
    {
        if (s.st_size == _file_copy(SYS_BOOTMSG_TEMP_FILE, file))
            break;
    }

    _file_rotate(SYS_BOOTMSG_BACKUP_FILE, SYS_BOOTMSG_BACKUP_NUM);

    rename(file, SYS_BOOTMSG_BACKUP_FILE);
}

int main(int argc, char **argv)
{
    int fd;
    pid_t pid;
    struct sigaction sig_act;

    pid = fork();

    switch (pid)
    {
        case -1:        /* error */
            exit(1);
            break;
        case 0:         /* child*/
            break;
        default:        /* parent*/
            exit(0);
            break;
    }

    setsid();

    chdir("/");

    for (fd = getdtablesize(); fd >= 0; --fd)
        close(fd);

    if ((fd = open(_PATH_DEVNULL, O_RDWR, 0)) != -1)
    {
        if (fd != 0)
            dup2(fd, STDIN_FILENO);

        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        if (fd > 2)
            close(fd);
    }

    /* register SIGHUP and SIGTERM for log flush and backup */
    memset(&sig_act, 0, sizeof(struct sigaction));
    sig_act.sa_handler = bootmsg_sig_handler;

    sigaction(SIGHUP, &sig_act, NULL);
    sigaction(SIGTERM, &sig_act, NULL);

    bootmsg_level_init();
    bootmsg_logger();
    bootmsg_flush();

    return 0;
}
