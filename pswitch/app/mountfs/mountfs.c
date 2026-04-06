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
 * Purpose : Mount File System
 *
 * Feature : Mount File System
 */

 /*
 * Include Files
 */
#include <unistd.h>
#include <mtd/mtd-user.h>

#include <common/sys_def.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_util.h>
#include <pswitch/image.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
uint32 bootmsg = 0;

/*
 * Macro Definition
 */
#define _MBOOTMSG_PRINTF(fmt, args...) \
do { \
    FILE *fp = fopen("/dev/kmsg", "a+"); \
    if (fp) \
    { \
        fprintf(fp, fmt, ## args); \
        fflush(fp); \
        fclose(fp); \
    } \
    else { \
        osal_printf(fmt, ## args); \
    } \
} while (0)

#define MOUNT_RET_PRINTF(ret, fmt, args...) \
do { \
    if (ret == 0) { \
        _MBOOTMSG_PRINTF(fmt, ## args); \
    } \
    else { \
        _MBOOTMSG_PRINTF("\n"); \
        _MBOOTMSG_PRINTF("<3>" SYS_INIT_FAILURE_STR fmt, ## args); \
    } \
} while(0)

/*
 * Function Declaration
 */
static void dev_fs_mount(void)
{
    int32 ret;
    char cmd[SYS_BUF128_LEN];

    /* Mount dev File System */
    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "mount -t devpts devpts /dev/pts > /dev/null");
    ret = system(cmd);

    MOUNT_RET_PRINTF(ret, "Mount DEV File System....%s\n", (ret < 0) ? "Failed" : "OK");
}

static void proc_fs_mount(void)
{
    int32 ret;
    char cmd[SYS_BUF128_LEN];

    /* Mount proc File System */
    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "mount -t proc proc /proc > /dev/null");
    ret = system(cmd);

    MOUNT_RET_PRINTF(ret, "Mount PROC File System........%s\n", (ret < 0) ? "Failed" : "OK");
}

#ifdef CONFIG_SYS_KERNEL_LINUX_COMPRESSED_FS
static void sqfs_fs_mount(void)
{
    int32 ret;
    char cmd[SYS_BUF128_LEN];

    /* Mount Squash File System Main Image*/
    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "mount -o loop /sqfs.img /sqfs > /dev/null");
    ret = system(cmd);

    MOUNT_RET_PRINTF(ret, "Mount Main SQFS File System........%s\n", (ret < 0) ? "Failed" : "OK");


    /* Mount Squash File System Module Image*/
    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "mount -o loop /modsqfs.img /modsqfs > /dev/null");
    ret = system(cmd);

    MOUNT_RET_PRINTF(ret, "Mount Module SQFS File System....%s\n", (ret < 0) ? "Failed" : "OK");
}
#endif

static void jffs2_fs_mount(void)
{
    int32 ret;
    char cmd[SYS_BUF128_LEN];

    /* Mount JFFS2 File System */
    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "mount -t jffs2 -o rw,sync %s %s > /dev/null", JFFS2_CFG_MTDDEV, JFFS2_CFG_TARGET_DIR);
    ret = system(cmd);

    MOUNT_RET_PRINTF(ret, "Mount CFG JFFS2 File System....%s\n", (ret < 0) ? "Failed" : "OK");


#if defined(CONFIG_FLASH_LAYOUT_TYPE3) || defined(CONFIG_FLASH_LAYOUT_TYPE4) || defined(CONFIG_FLASH_LAYOUT_TYPE5)
    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "mount -t jffs2 -o rw,sync %s %s > /dev/null", JFFS2_LOG_MTDDEV, JFFS2_LOG_TARGET_DIR);
    ret = system(cmd);

    MOUNT_RET_PRINTF(ret, "Mount LOG JFFS2 File System....%s\n", (ret < 0) ? "Failed" : "OK");
#endif

	osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "mount -t jffs2 -o rw,sync %s %s > /dev/null", JFFS2_OEM_MTDDEV, JFFS2_OEM_TARGET_DIR);
	ret = system(cmd);
	MOUNT_RET_PRINTF(ret, "Mount OEM JFFS2 File System....%s\n", (ret < 0) ? "Failed" : "OK");

}

int main(int argc, char **argv)
{
    dev_fs_mount();

    proc_fs_mount();

#ifdef CONFIG_SYS_KERNEL_LINUX_COMPRESSED_FS
    sqfs_fs_mount();
#endif

    jffs2_fs_mount();

    return SYS_ERR_OK;
}
