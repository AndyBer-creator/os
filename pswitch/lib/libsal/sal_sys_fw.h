
#ifndef __SAL_SYS_FW_H__
#define __SAL_SYS_FW_H__

#define TMP_FWIMG_FILENAME          "/tmp/tmp_vmlinux.bix"
#define TMP_FWIMG_WRITESTATUS       "/tmp/tmp_img_write_status"

#define TMP_FWIMG_WRITESTATUS_OK                    "0"
#define TMP_FWIMG_WRITESTATUS_FAILED                "1"
#define TMP_FWIMG_WRITESTATUS_FAILED_FILENAME       "2"

#define FLH_FWIMG_FILENAME          "/mnt/fwimg_name_"

#define TMP_FWIMG_HTTPINFO          "/tmp/tmp_fwimg_httpinfo"
#define TMP_FWIMG_CURRSELPART       "/tmp/tmp_fwimg_currselpart"
#define FWIMG_UPGRAING_LOCK_FILE    "/tmp/fwimg_process_lock"
#define SYS_DFLT_FW_PARTITION       0
#define FWIMG_MAX_SIZE_DUAL_16MB    (6976 * 1024) 
#define FWIMG_MAX_SIZE_16MB         (7168 * 1024) 
#define FWIMG_MAX_SIZE_8MB          (6784 * 1024) 
#define FWIMG_MAX_SIZE_4MB          (3456 * 1024) 

#define SYS_SUFFICIENT_MEM_MIN      (1024) 

#if defined(CONFIG_FLASH_LAYOUT_TYPE1)
#define JFFS2_CFG_MTDDEV     "/dev/mtdblock2"
#elif defined(CONFIG_FLASH_LAYOUT_TYPE2)
#define JFFS2_CFG_MTDDEV     "/dev/mtdblock3"
#elif defined(CONFIG_FLASH_LAYOUT_TYPE3) || defined(CONFIG_FLASH_LAYOUT_TYPE4) || defined(CONFIG_FLASH_LAYOUT_TYPE5)
#define JFFS2_CFG_MTDDEV     "/dev/mtdblock3"
#define JFFS2_LOG_MTDDEV     "/dev/mtdblock4"
#ifdef CONFIG_SYS_DUAL_IMAGE
#define JFFS2_OEM_MTDDEV     "/dev/mtdblock7"
#else 
#define JFFS2_OEM_MTDDEV     "/dev/mtdblock6"
#endif
#endif

#define JFFS2_CFG_TARGET_DIR "/mnt"
#if defined(CONFIG_FLASH_LAYOUT_TYPE3) || defined(CONFIG_FLASH_LAYOUT_TYPE4) || defined(CONFIG_FLASH_LAYOUT_TYPE5)
#define JFFS2_LOG_TARGET_DIR "/mntlog"
#define JFFS2_OEM_TARGET_DIR "/oeminfo"
#else
#define JFFS2_LOG_TARGET_DIR "/mnt"
#endif

#if defined(CONFIG_FLASH_SIZE_4MB)
#define SYS_FLASH_MB_SIZE 4
#elif defined(CONFIG_FLASH_SIZE_8MB)
#define SYS_FLASH_MB_SIZE 8
#elif defined(CONFIG_FLASH_SIZE_16MB)
#define SYS_FLASH_MB_SIZE 16
#elif defined(CONFIG_FLASH_SIZE_32MB)
#define SYS_FLASH_MB_SIZE 32
#endif

#define FLSH_LOADER_MTDDEV "/dev/mtdchar0"
#define FLSH_BDINFO_MTDDEV "/dev/mtdchar2"
#define FLSH_IMG_MTDDEV_GET(partition, devStr) \
do { \
    sys_flsh_layout_t flshLayout = SYS_FLSH_LAYOUT_TYPE2; \
    SYS_ERR_CHK(sal_sys_boardFlshLayout_get(&flshLayout));\
    osal_memset(devStr, 0, sizeof(devStr));\
    switch (flshLayout)\
    {\
        case SYS_FLSH_LAYOUT_TYPE1:\
            if (partition == 0) \
                osal_strcpy(devStr, "/dev/mtdchar6");\
            else if (partition == 1)\
                osal_strcpy(devStr, "/dev/mtdchar8");\
            break;\
        case SYS_FLSH_LAYOUT_TYPE2:\
            if (partition == 0) \
                osal_strcpy(devStr, "/dev/mtdchar8");\
            else if (partition == 1)\
                osal_strcpy(devStr, "/dev/mtdchar10");\
            break;\
        case SYS_FLSH_LAYOUT_TYPE3:\
        case SYS_FLSH_LAYOUT_TYPE4:\
        case SYS_FLSH_LAYOUT_TYPE5:\
        default:\
            if (partition == 0) \
                osal_strcpy(devStr, "/dev/mtdchar10");\
            else if (partition == 1)\
                osal_strcpy(devStr, "/dev/mtdchar12");\
    }\
} while (0)

#define FLSH_SYSINFO_MTDDEV_GET(devStr) \
do { \
    sys_flsh_layout_t flshLayout = SYS_FLSH_LAYOUT_TYPE2; \
    SYS_ERR_CHK(sal_sys_boardFlshLayout_get(&flshLayout));\
    osal_memset(devStr, 0, sizeof(devStr));\
    switch (flshLayout)\
    {\
        case SYS_FLSH_LAYOUT_TYPE1:\
            osal_strcpy(devStr, "/dev/mtdchar10");\
            break;\
        case SYS_FLSH_LAYOUT_TYPE2:\
        case SYS_FLSH_LAYOUT_TYPE3:\
        case SYS_FLSH_LAYOUT_TYPE4:\
        case SYS_FLSH_LAYOUT_TYPE5:\
        default:\
            osal_strcpy(devStr, "/dev/mtdchar4");\
    }\
} while (0)

extern int32 sal_sys_fwImageHdrInfo_get(
    uint32 partition,
    uint32 *pMagic,
    uint32 *pTime,
    uint32 *pSize,
    char   *pName);

extern int32 sal_sys_fwImageCrc32_check(char *fullname);

extern int32 sal_sys_fwImageLock_check(void);

extern int32 sal_sys_fwImageLock_take(void);

extern int32 sal_sys_fwImageLock_give(void);

extern int32 sal_sys_fwImage_write(char *fullname, uint32 partition, sys_filenamestr_t *filename);

extern int32 sal_sys_fwImage_backup(uint32 partition, char *fullname);

#ifdef CONFIG_SYS_FW_UPGRADE_SAVEMEM

extern int32 sal_sys_fwImageSaveMem_gen(char *fullname);
#endif 

#ifdef CONFIG_SYS_FW_UPGRADE_CHECKMEM

extern int32 sal_sys_fwImageFreeMem_check(void);
#endif

#ifdef CONFIG_SYS_DUAL_IMAGE

extern int32 sal_sys_dualImage_sel(uint32 partition);

extern int32 sal_sys_dualImageCurrSel_get(uint32 *pPartition);

extern int32 sal_sys_dualImageFileName_set(uint32 partition, sys_filenamestr_t *pFilename);

extern int32 sal_sys_dualImageFileName_get(uint32 partition, sys_filenamestr_t *pFilename);

extern int32 sal_sys_dualImage_erase(uint32 partition);
#endif 

extern int32 sal_sys_loaderInfo_get(char *version, char *date);

extern int32 sal_sys_bdinfoVar_get(char *name, char *pValue);

extern int32 sal_sys_bdinfoVar_set(char *name, char *value);

extern int32 sal_sys_fwImageMd5sum_get(uint32 partition, char *md5sum);
#endif 

