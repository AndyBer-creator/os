
#include <dirent.h>
#include <libgen.h>
#include <mtd/mtd-user.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#include <image.h>
#include <rtk_flash_common.h>
#include <libfds/fds_export.h>
#include <libsal/sal_util.h>
#include <libsal/sal_sys.h>

#ifdef CONFIG_SYS_DUAL_IMAGE
static int32 _sal_sys_flashPartition_erase(char *mtddev);
#endif

#ifdef CONFIG_SYS_DUAL_IMAGE

static int32 _sal_sys_flashPartition_erase(char *mtddev)
{
    int32 fd;
    mtd_info_t mtd_info;
    erase_info_t erase_info;

    if ((fd = open(mtddev, O_RDWR)) < 0)
        return SYS_ERR_DEV_OPEN;

    if (ioctl(fd, MEMGETINFO, &mtd_info) < 0)
    {
        close(fd);

        return SYS_ERR_IOCTL;
    }

    erase_info.start = 0x0;
    erase_info.length = mtd_info.size;

    if (ioctl(fd, MEMERASE, &erase_info) < 0)
    {
        close(fd);

        return SYS_ERR_IOCTL;
    }

    close(fd);

    return SYS_ERR_OK;
}
#endif 

int32 sal_sys_fwImageHdrInfo_get(
    uint32 partition,
    uint32 *pMagic,
    uint32 *pTime,
    uint32 *pSize,
    char   *pName)
{
    char devStr[32];
    image_header_t ih;
    int32 fd = -1;
    uint32 checksum = 0;
    uint32 calcksum = 0;

    SYS_PARAM_CHK(IS_IMGPART_INVALID(partition), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pMagic), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    osal_memset(devStr, 0, sizeof(devStr));

    FLSH_IMG_MTDDEV_GET(partition, devStr);

    if ((fd = open(devStr, O_RDONLY)) < 0)
        return SYS_ERR_DEV_OPEN;

    if (read(fd, &ih, sizeof(image_header_t)) < 0)
    {
        close(fd);

        return SYS_ERR_DEV_READ;
    }
    close(fd);

    checksum = ntohl(ih.ih_hcrc);
        
    ih.ih_hcrc = 0;
    calcksum = sal_util_crc32_ret(0, (char *)&ih, sizeof(image_header_t));

    if (checksum != calcksum)
        return SYS_ERR_IMAGE_INVALID;

    *pMagic = ih.ih_magic;
    *pTime = ih.ih_time;
    *pSize = ih.ih_size;
    osal_strcpy(pName, (char *)ih.ih_name);

    return SYS_ERR_OK;
}

int32 sal_sys_fwImageCrc32_check(char *fullname)
{
    char *tmp;
    FILE *fp = NULL;
    image_header_t ih;
    uint32 size = 0;
    uint32 checksum = 0;
    uint32 calcksum = 0;
    uint32 tmpSize = 0;

    SYS_PARAM_CHK((NULL == fullname), SYS_ERR_NULL_POINTER);

    if (NULL == (fp = fopen(fullname, "r")))
    {
        return SYS_ERR_FILE_OPEN;
    }

    fread(&ih, 1, sizeof(image_header_t), fp);

#ifdef CONFIG_SYS_FW_SIGNATURE_CHECK
    if (ntohl(ih.ih_magic) != SYS_FIRMWARE_MAGICNUM)
    {
        fclose(fp);

        SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_IMG_INVALID);

        return SYS_ERR_IMAGE_MAGIC_INVALID;
    }
#endif 

    checksum = ntohl(ih.ih_hcrc);
        
    ih.ih_hcrc = 0;
    calcksum = sal_util_crc32_ret(0, (char *)&ih, sizeof(image_header_t));

    if (checksum != calcksum)
    {
        fclose(fp);

        SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_IMG_CHKSUM);

        return SYS_ERR_IMAGE_INVALID;
    }

    checksum = ntohl(ih.ih_dcrc);
    calcksum = 0;
    size = ih.ih_size; 

    while (size > 0)
    {
        if (size >= 65536)
            tmpSize = 65536;
        else
            tmpSize = size;

        size -= tmpSize;

        tmp = malloc(tmpSize);
        fread(tmp, 1, tmpSize, fp);

        calcksum = sal_util_crc32_ret(calcksum, (char *)tmp, tmpSize);

        free(tmp);
    }

    fclose(fp);

    if (checksum != ntohl(calcksum))
    {
        SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_IMG_CHKSUM);

        return SYS_ERR_IMAGE_INVALID;
    }

    return SYS_ERR_OK;
}

int32 sal_sys_fwImageLock_check(void)
{
    int32 ret = SYS_ERR_FAILED;

    ret = sal_util_file_exist(FWIMG_UPGRAING_LOCK_FILE);

    if (SYS_ERR_OK == ret)
    {
        SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_IMG_LOCKED);

        return SYS_ERR_IMAGE_PROCESSING;
    }

    return SYS_ERR_OK;
}

int32 sal_sys_fwImageLock_take(void)
{
    int32 ret = SYS_ERR_FAILED;

    ret = sal_util_file_touch(FWIMG_UPGRAING_LOCK_FILE);

    DO_POLLD_ACT(SYS_POLLD_ACT_FWLOCK_TIMER);

    return ret;
}

int32 sal_sys_fwImageLock_give(void)
{
    FILE *fp = NULL;
    int32 pid = -1;

    if (NULL != (fp = fopen(SYS_FWLOCK_TIMER_PID_FILE, "r")))
    {
        fscanf(fp, "%d", &pid);
        fclose(fp);
        unlink(SYS_FWLOCK_TIMER_PID_FILE);

        if (-1 != pid)
        {
            kill(pid, SIGTERM);
        }
    }

    unlink(FWIMG_UPGRAING_LOCK_FILE);

    return SYS_ERR_OK;
}

int32 sal_sys_fwImage_write(char *fullname, uint32 partition, sys_filenamestr_t *filename)
{
    char devStr[32];
#ifdef CONFIG_SYS_DUAL_IMAGE
    sys_filenamestr_t orgFilename;
#endif
    char *tmp = NULL;
    FILE *fp = NULL;
    int32 ret = -1;
    int32 fd = -1;
    int32 size = 0;
    uint32 write_size = 0;
    uint32 file_size = 0;
    uint32 erase_next_block = FALSE;
    mtd_info_t mtd_info;
    erase_info_t erase_info;

    SYS_PARAM_CHK((NULL == fullname), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_IMGPART_INVALID(partition), SYS_ERR_INPUT);

#ifdef CONFIG_SYS_DUAL_IMAGE
    SYS_MEM_CLEAR(orgFilename);
    sal_sys_dualImageFileName_get(partition, &orgFilename);

    ret = sal_sys_dualImageFileName_set(partition, filename);

    if (SYS_ERR_OK != ret)
        goto fail_filename;
#endif

    osal_memset(devStr, 0, sizeof(devStr));

    file_size = sal_util_fileSize_ret(fullname);

    if (sal_util_currFreeMemSize_ret() < 512)
    {
        sal_sys_fwImageLock_give();

        ret = SYS_ERR_FAILED;

        SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_MEMORY_LACK);

        goto fail;
    }

    FLSH_IMG_MTDDEV_GET(partition, devStr);

    if ((fd = open(devStr, O_RDWR)) < 0)
    {
        sal_sys_fwImageLock_give();

        ret = SYS_ERR_DEV_OPEN;

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        goto fail;
    }

    if (NULL == (fp = fopen(fullname, "r")))
    {
        close(fd);

        sal_sys_fwImageLock_give();

        ret = SYS_ERR_FILE_OPEN;

        goto fail;
    }

    if (ioctl(fd, MEMGETINFO, &mtd_info) < 0)
    {
        close(fd);

        sal_sys_fwImageLock_give();

        ret = SYS_ERR_IOCTL;

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        goto fail;
    }
    
    erase_info.start = 0x0;
    erase_info.length = mtd_info.erasesize;
    size = mtd_info.size;

    while (size > 0)
    {
        if (ioctl(fd, MEMERASE, &erase_info) < 0)
        {
            close(fd);

            ret = SYS_ERR_IOCTL;

            if (size != mtd_info.size)
                SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_FLASH_IMG_BROKEN,
                            partition);
            else
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

            goto fail;
        }

        erase_info.start += erase_info.length;
        size -= erase_info.length;

        if (file_size <= 0)
        {
            
            if (FALSE == erase_next_block)
            {
                erase_next_block = TRUE;
                continue;
            }
            else
            {
                break;
            }
        }

        while (NULL == (tmp = malloc(erase_info.length)))
        {
        }

        if (file_size >= erase_info.length)
        {
            write_size = erase_info.length;
        }
        else
        {
            write_size = file_size;
        }

        fread(tmp, 1, write_size, fp);
        if (write(fd, tmp, write_size) < 0)
        {
            free(tmp);
            close(fd);

            ret = SYS_ERR_DEV_WRITE;

            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_FLASH_IMG_BROKEN,
                        partition);

            goto fail;
        }
        file_size -= write_size;

        free(tmp);

    }

    close(fd);
    fclose(fp);

#ifdef CONFIG_SYS_FW_UPGRADE_SAVEMEM
    sal_sys_fwImageSaveMem_gen(fullname);
#else
    unlink(fullname);
#endif

    sal_sys_fwImageLock_give();

    return SYS_ERR_OK;

fail:
#ifdef CONFIG_SYS_DUAL_IMAGE
    sal_sys_dualImageFileName_set(partition, &orgFilename);
fail_filename:
#endif
    sal_sys_fwImageLock_give();
#ifdef CONFIG_SYS_FW_UPGRADE_SAVEMEM
    sal_sys_fwImageSaveMem_gen(fullname);
#else
    unlink(fullname);
#endif

    if (NULL != fp)
        fclose(fp);

    return ret;
}

int32 sal_sys_fwImage_backup(uint32 partition, char *fullname)
{
    char devStr[32];
    char tmp_buf[4096];
    FILE *fp = NULL;
    int32 fd = -1;
    int32 size = 0;
    uint32 read_size = 0;
    uint32 write_size = 0;
    image_header_t ih;

    SYS_PARAM_CHK(IS_IMGPART_INVALID(partition), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == fullname), SYS_ERR_NULL_POINTER);

    osal_memset(devStr, 0, sizeof(devStr));

    FLSH_IMG_MTDDEV_GET(partition, devStr);

    if ((fd = open(devStr, O_RDONLY)) < 0)
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_OPEN;
    }

    if (read(fd, &ih, sizeof(image_header_t)) < 0)
    {
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_READ;
    }

    close(fd);

    size = ih.ih_size;

    if ((fd = open(devStr, O_RDONLY)) < 0)
        return SYS_ERR_DEV_OPEN;

    if (NULL == (fp = fopen(fullname, "w")))
    {
        close(fd);

        return SYS_ERR_FILE_OPEN;
    }

    size += sizeof(image_header_t);

    while (size > 0)
    {
        osal_memset(tmp_buf, 0, sizeof(tmp_buf));

        if (size >= sizeof(tmp_buf))
            read_size = sizeof(tmp_buf);
        else
            read_size = size;

        if (read(fd, tmp_buf, read_size) < 0)
        {
            close(fd);
            fclose(fp);

            SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

            return SYS_ERR_DEV_READ;
        }

        write_size = fwrite(tmp_buf, 1, read_size, fp);

        if (write_size != read_size)
        {
            close(fd);
            fclose(fp);

            return SYS_ERR_FILE_WRITE;
        }

        size -= read_size;
    }

    close(fd);
    fclose(fp);

    SYS_ERR_CHK(sal_sys_fwImageCrc32_check(fullname));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_FW_UPGRADE_SAVEMEM

int32 sal_sys_fwImageSaveMem_gen(char *fullname)
{
    FILE *fp = NULL;
    uint32 fwImgSize = 0;

    SYS_PARAM_CHK((NULL == fullname), SYS_ERR_NULL_POINTER);

    fwImgSize = CONFIG_SYS_FW_UPGRADE_SAVEMEM_SIZE * 1024; 

    if (0 == fwImgSize)
    {
#if defined(CONFIG_FLASH_SIZE_16MB)
    #ifdef CONFIG_SYS_DUAL_IMAGE
        fwImgSize = FWIMG_MAX_SIZE_DUAL_16MB;
    #else
        fwImgSize = FWIMG_MAX_SIZE_16MB;
    #endif
#elif defined(CONFIG_FLASH_SIZE_8MB)
        fwImgSize = FWIMG_MAX_SIZE_8MB;
#elif defined(CONFIG_FLASH_SIZE_4MB)
        fwImgSize = FWIMG_MAX_SIZE_4MB;
#endif
    }

    if (NULL == (fp = fopen(fullname, "wb")))
        return SYS_ERR_FILE_OPEN;

    while (fwImgSize > 0)
    {
        if (EOF == fputc('\0', fp))
        {
            fclose(fp);

            return SYS_ERR_FAILED;
        }

        fwImgSize--;
    }

    fclose(fp);

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_FW_UPGRADE_CHECKMEM

int32 sal_sys_fwImageFreeMem_check(void)
{
    uint32 fwImgSize = 0;
    uint32 suffMem = 0;

    fwImgSize = CONFIG_SYS_FW_UPGRADE_SAVEMEM_SIZE; 

    if (0 == fwImgSize)
    {
#if defined(CONFIG_FLASH_SIZE_16MB)
  #ifdef CONFIG_SYS_DUAL_IMAGE
        fwImgSize = FWIMG_MAX_SIZE_DUAL_16MB;
  #else
        fwImgSize = FWIMG_MAX_SIZE_16MB;
  #endif
#elif defined(CONFIG_FLASH_SIZE_8MB)
        fwImgSize = FWIMG_MAX_SIZE_8MB;
#elif defined(CONFIG_FLASH_SIZE_4MB)
        fwImgSize = FWIMG_MAX_SIZE_4MB;
#endif
    }

  #ifdef CONFIG_SYS_FW_UPGRADE_SAVEMEM
    
    suffMem = SYS_SUFFICIENT_MEM_MIN; 
  #else
    
    suffMem = fwImgSize + SYS_SUFFICIENT_MEM_MIN; 
  #endif

    if (sal_util_currFreeMemSize_ret() < suffMem)
        return SYS_ERR_IMAGE_OUT_OF_MEMORY;

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_DUAL_IMAGE

int32 sal_sys_dualImage_sel(uint32 partition)
{
    char valStr[256];
    int32 ret = SYS_ERR_FAILED;
    uint32 currSel = 0;
    uint32 magic;
    uint32 time;
    uint32 size;
    char name[SYS_BUF32_LEN];

    SYS_PARAM_CHK(IS_IMGPART_INVALID(partition), SYS_ERR_INPUT);

    osal_memset(valStr, 0, sizeof(valStr));

    SYS_ERR_CHK(sal_sys_fwImageHdrInfo_get(partition, &magic, &time, &size, name));

    ret = sal_sys_sysinfoVar_get(SYSINFO_VAR_DUALACTPART, valStr);

    if (SYS_ERR_INFO_BADCRC == ret)
        currSel = IMG_PART0;
    else
        currSel = SYS_STR2UINT(valStr);

    if (currSel == partition)
        return SYS_ERR_OK;

    osal_memset(valStr, 0, sizeof(valStr));
    osal_sprintf(valStr, "%d", partition);

    SYS_ERR_CHK(sal_sys_sysinfoVar_set(SYSINFO_VAR_DUALACTPART, valStr));

    SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_IMG_ACTIVE,
                partition);

    return SYS_ERR_OK;
}

int32 sal_sys_dualImageCurrSel_get(uint32 *pPartition)
{
    char valStr[256];
    int32 ret = SYS_ERR_FAILED;

    SYS_PARAM_CHK((NULL == pPartition), SYS_ERR_NULL_POINTER);

    osal_memset(valStr, 0, sizeof(valStr));

    ret = sal_sys_sysinfoVar_get(SYSINFO_VAR_DUALACTPART, valStr);

    if (SYS_ERR_INFO_BADCRC == ret || SYS_ERR_INFO_NOT_EXIST == ret)
    {
        *pPartition = IMG_PART0;

        return SYS_ERR_OK;
    }
    else if (SYS_ERR_OK == ret)
    {
        *pPartition = SYS_STR2UINT(valStr);

        return SYS_ERR_OK;
    }

    return ret;
}

int32 sal_sys_dualImageFileName_set(uint32 partition, sys_filenamestr_t *pFilename)
{
    sys_filenamestr_t fileNameNew;
    sys_filenamestr_t fileNameExist;
    sys_filenamestr_t fileNameforSaving;
    int32 len = 0;
    FILE *fp = NULL;

    SYS_PARAM_CHK(IS_IMGPART_INVALID(partition), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pFilename), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(fileNameNew);
    SYS_MEM_CLEAR(fileNameExist);
    SYS_MEM_CLEAR(fileNameforSaving);

    osal_snprintf(fileNameforSaving.str, sizeof(sys_filenamestr_t), "%s%u", FLH_FWIMG_FILENAME, partition);

    if (NULL != (fp = fopen(fileNameforSaving.str, "r")))
    {
        fgets(fileNameExist.str, sizeof(sys_filenamestr_t), fp);
        fclose(fp);
    }

    if (0 != osal_strlen(pFilename->str))
    {
        
        if (0 != osal_memcmp((pFilename->str + osal_strlen(pFilename->str) - osal_strlen(DFLT_SYS_IMG_EXTNAME)),
                             DFLT_SYS_IMG_EXTNAME,
                             osal_strlen(DFLT_SYS_IMG_EXTNAME)))
        {
            if((osal_strlen(pFilename->str)+osal_strlen(DFLT_SYS_IMG_EXTNAME)) >= sizeof(sys_filenamestr_t))
            {
                sal_sys_fwImageLock_give();
                return SYS_ERR_STR_LEN_EXCEED;
            }

            strlcpy(fileNameNew.str, pFilename->str, sizeof(sys_filenamestr_t));
            strlcat(fileNameNew.str, DFLT_SYS_IMG_EXTNAME, sizeof(sys_filenamestr_t));
        }
        else
        {
            if(osal_strlen(pFilename->str) >= sizeof(sys_filenamestr_t))
            {
                sal_sys_fwImageLock_give();
                return SYS_ERR_STR_LEN_EXCEED;
            }

            strlcpy(fileNameNew.str, pFilename->str, sizeof(sys_filenamestr_t));
        }
    }

    if (0 == osal_strcmp(fileNameExist.str, fileNameNew.str))
        return SYS_ERR_OK;

    if (NULL == (fp = fopen(fileNameforSaving.str, "w")))
        return SYS_ERR_FILE_OPEN;

    len = fputs(fileNameNew.str, fp);
    fclose(fp);

    if (len != osal_strlen(fileNameNew.str))
    {
        unlink(fileNameforSaving.str);

        return SYS_ERR_FILE_WRITE;
    }

    return SYS_ERR_OK;
}

int32 sal_sys_dualImageFileName_get(uint32 partition, sys_filenamestr_t *pFilename)
{
    sys_filenamestr_t fileNameforSaving;
    FILE *fp = NULL;

    SYS_PARAM_CHK(IS_IMGPART_INVALID(partition), SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pFilename, SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(fileNameforSaving);

    osal_snprintf(fileNameforSaving.str, sizeof(sys_filenamestr_t), "%s%u", FLH_FWIMG_FILENAME, partition);

    if (NULL != (fp = fopen(fileNameforSaving.str, "r")))
    {
        fgets(pFilename->str, CAPA_FILE_NAME_LEN, fp);
        fclose(fp);

        return SYS_ERR_OK;
    }

    return SYS_ERR_FAILED;
}

int32 sal_sys_dualImage_erase(uint32 partition)
{
    char devStr[16];
    uint32 selPartition;
    sys_filenamestr_t filename;

    SYS_PARAM_CHK(IS_IMGPART_INVALID(partition), SYS_ERR_INPUT);

    osal_memset(devStr, 0, sizeof(devStr));
    SYS_MEM_CLEAR(filename);

    SYS_ERR_CHK(sal_sys_dualImageCurrSel_get(&selPartition));

    if (selPartition == partition)
        return SYS_ERR_IMAGE_ERASE_ACTIVE;

    FLSH_IMG_MTDDEV_GET(partition, devStr);

    SYS_ERR_CHK(_sal_sys_flashPartition_erase(devStr));

    SYS_ERR_CHK(sal_sys_dualImageFileName_set(partition, &filename));

    return SYS_ERR_OK;
}
#endif 

int32 sal_sys_loaderInfo_get(char *version, char *date)
{
    char *pch = NULL;
    char *pDate_s = NULL;
    char *pDate_e = NULL;
    char *pFree = NULL;
    char *tmp_buf = NULL;
    int32 fd = -1;
    uint32 size = 0;
    mtd_info_t mtd_info;

    SYS_PARAM_CHK(NULL == version, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == date, SYS_ERR_NULL_POINTER);

    if ((fd = open(FLSH_LOADER_MTDDEV, O_RDONLY)) < 0)
        return SYS_ERR_DEV_OPEN;

    if (ioctl(fd, MEMGETINFO, &mtd_info) < 0)
    {
        close(fd);
        return SYS_ERR_IOCTL;
    }

    size = mtd_info.size;

    tmp_buf = malloc(size);
    pFree = tmp_buf;

    if (read(fd, tmp_buf, size) < 0)
    {
        close(fd);
        free(pFree);

        return SYS_ERR_DEV_READ;
    }

    close(fd);

    while (size > 0)
    {
        if (NULL != (pch = memchr(tmp_buf, 'U', 1)))
        {

            if (0 == osal_strncmp(pch, "U-Boot 2011.", 12))
            {
                if (*(pch + 15) == '(')
                {
                    if (NULL != (pDate_s = memchr(pch + 16, '(', osal_strlen(pch + 16))))
                    {
                        pDate_s += 1;
                        pDate_e = memchr(pDate_s, ')', osal_strlen(pch + 16));
                    }

                    osal_strncpy(version, pch + 16, (pDate_s - 3 - (pch + 17)) + 1);
                    osal_strncpy(date, pDate_s, pDate_e - pDate_s);
                    free(pFree);
                }
                else
                {
                    if (NULL != (pDate_s = memchr(pch, '(', osal_strlen(pch))))
                    {
                        pDate_s += 1;
                        pDate_e = memchr(pDate_s, ')', osal_strlen(pch));
                    }

                    osal_strncpy(version, pch + 7, (pDate_s - 2 - (pch + 8)) + 1);
                    osal_strncpy(date, pDate_s, pDate_e - pDate_s);
                    free(pFree);
                }

                return SYS_ERR_OK;
            }
            
            else if (0 == osal_strncmp(pch, "U-Boot Version ", 15))
            {
                if (NULL != (pDate_s = memchr(pch, '(', osal_strlen(pch))))
                {
                    pDate_s += 1;
                    pDate_e = memchr(pDate_s, ')', osal_strlen(pch));
                }

                osal_strncpy(version, pch + 15, (pDate_s - 2 - (pch + 16)) + 1);
                osal_strncpy(date, pDate_s, pDate_e - pDate_s);
                free(pFree);

                return SYS_ERR_OK;
            }
        }

        tmp_buf += 1;
        size -= 1;
    }

    free(pFree);

    return SYS_ERR_FAILED;
}

int32 sal_sys_bdinfoVar_get(char *name, char *pValue)
{
    char *buf = NULL;
    char *data_ptr = NULL;
    char *varname = NULL;
    int32 i = 0;
    int32 fd = -1;
    int32 nxt = 0;
    int32 varval = 0;
    uint32 calchksum = 0;
    uint32 crc = 0;
    uint32 size = 0;
    mtd_info_t mtd_info;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pValue), SYS_ERR_NULL_POINTER);

    if ((fd = open(FLSH_BDINFO_MTDDEV, O_RDONLY)) < 0)
        return SYS_ERR_DEV_OPEN;

    if (ioctl(fd, MEMGETINFO, &mtd_info) < 0)
    {
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_IOCTL;
    }

    size = LOADER_BDINFO_SIZE;

    if (size > mtd_info.size)
        size = mtd_info.size;

    buf = malloc(size);
    osal_memset(buf, 0, size);
    data_ptr = buf + 4; 

    if (read(fd, buf, size) < 0)
    {
        close(fd);
        free(buf);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_READ;
    }

    close(fd);
    crc = ((buf[0] & 0xFF) << 24) + ((buf[1] & 0xFF) << 16) + ((buf[2] & 0xFF) << 8) + (buf[3] & 0xFF);

    calchksum = sal_util_crc32_ret(0, data_ptr, size - sizeof(crc));

    if (crc != ntohl(calchksum))
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_BDINFO_CHKSUM_ERROR);

    }

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
                if ((strlen(name) == (nxt - i)) && 0 == osal_strncmp(name, varname, nxt - i))
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

int32 sal_sys_bdinfoVar_set(char *name, char *value)
{
    char *buf = NULL;
    char *buf2 = NULL;
    char *data_ptr = NULL;
    char *data_ptr2 = NULL;
    char *varname = NULL;
    char *varname_end = NULL;
    char devStr[16];
    int32 fd = -1;
    int32 i = 0;
    int32 nxt = 0;
    int32 varval = 0;
    uint32 crc = 0;
    uint32 idx = 0;
    uint32 size = 0;
    uint32 varSet = FALSE;
    uint32 varExist = FALSE;
    mtd_info_t mtd_info;
    erase_info_t erase_info;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == value), SYS_ERR_NULL_POINTER);

    osal_memset(devStr, 0, sizeof(devStr));

    osal_strcpy(devStr, FLSH_BDINFO_MTDDEV);

    if ((fd = open(devStr, O_RDONLY)) < 0)
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_OPEN;
    }

    if (ioctl(fd, MEMGETINFO, &mtd_info) < 0)
    {
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_IOCTL;
    }

    size = LOADER_BDINFO_SIZE;

    if (size > mtd_info.size)
        size = mtd_info.size;

    buf = malloc(size);
    buf2 = malloc(size);
    osal_memset(buf, 0, size);
    osal_memset(buf2, 0, size);

    data_ptr = buf + 4;
    data_ptr2 = buf2 + 4;

    if (read(fd, buf, size) < 0)
    {
        close(fd);
        free(buf);
        free(buf2);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_READ;
    }

    close(fd);

    crc = sal_util_crc32_ret(0, data_ptr, size - 4);

    for (i = 0; *(data_ptr + i) != '\0'; i = nxt + 1)
    {
        varname = data_ptr + i;
        varval = 0;
        varExist = FALSE;

        for (nxt = i; *(data_ptr + nxt) != '\0'; ++nxt)
        {
            if (nxt > size)
            {
                free(buf);
                free(buf2);

                return SYS_ERR_FAILED;
            }

            if (*(data_ptr + nxt) == '=')
            {
                
                if ((strlen(name) == (nxt - i)) && 0 == osal_strncmp(name, varname, nxt - i))
                {
                    varname_end = data_ptr + nxt;
                    varExist = TRUE;
                }
                varval = nxt + 1;
            }
        }

        if ((0 != varval) && (FALSE == varExist))
        {
            osal_strncpy((data_ptr2 + idx), varname, (data_ptr + nxt) - varname);

            idx += (data_ptr + nxt) - varname;
            data_ptr2[idx] = '\0';
            idx++;
        }
        
        else if ((varval != 0) && (varExist == TRUE))
        {
            if (0 != osal_strcmp(value, ""))
            {
                osal_strncpy((data_ptr2 + idx), varname, varname_end - varname);

                idx += varname_end - varname;
                data_ptr2[idx] = '=';
                idx++;

                osal_strcpy((data_ptr2 + idx), value);
                idx += osal_strlen(value);
                data_ptr2[idx] = '\0';
                idx++;
            }
            varSet = TRUE;
        }
    }

    if ((FALSE == varSet) && (0 != osal_strcmp(value, "")))
    {
        osal_sprintf((data_ptr2 + idx), "%s=%s", name, value);
    }

    free(buf);

    crc = sal_util_crc32_ret(0, data_ptr2, size - 4);
    buf2[0] = (crc >> 24) & 0xFF;
    buf2[1] = (crc >> 16) & 0xFF;
    buf2[2] = (crc >> 8) & 0xFF;
    buf2[3] = crc & 0xFF;

    if ((fd = open(devStr, O_RDWR)) < 0)
    {
        free(buf2);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_OPEN;
    }

    erase_info.start = 0x0;
    erase_info.length = mtd_info.size;
    if (ioctl(fd, MEMERASE, &erase_info) < 0)
    {
        free(buf2);
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_IOCTL;
    }

    if (write(fd, buf2, size) < 0)
    {
        free(buf2);
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_WRITE;
    }
    close(fd);

    free(buf2);

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_BDINFO_CFG,
                name,
                value);

    return SYS_ERR_OK;
}

int32 sal_sys_fwImageMd5sum_get(uint32 partition, char *md5sum)
{
    int i;
    int fd;
    char dev[SYS_BUF32_LEN];
    unsigned char buffer[SYS_BUF4096_LEN];
    unsigned char digest[MD5_DIGEST_LENGTH];

    MD5_CTX md5_ctx;
    uint32 size = 0;
    uint32 reads = 0;
    image_header_t ih;
    uint32 checksum = 0;
    uint32 calcksum = 0;

    SYS_PARAM_CHK(IS_IMGPART_INVALID(partition), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == md5sum), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(dev);
    SYS_MEM_CLEAR(buffer);
    SYS_MEM_CLEAR(digest);

    FLSH_IMG_MTDDEV_GET(partition, dev);

    if (0 > (fd = open(dev, O_RDONLY)))
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_OPEN;
    }

    if (sizeof(ih) != read(fd, &ih, sizeof(image_header_t)))
    {
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_READ;
    }

    checksum = ntohl(ih.ih_hcrc);
        
    ih.ih_hcrc = 0;
    calcksum = sal_util_crc32_ret(0, (char *)&ih, sizeof(image_header_t));

    if (checksum != calcksum)
    {
        close(fd);
        return SYS_ERR_IMAGE_INVALID;
    }

    ih.ih_hcrc = checksum;

    MD5_Init(&md5_ctx);

    MD5_Update(&md5_ctx, (unsigned char *)&ih, sizeof(ih));

    size = ih.ih_size;

    while (size > 0)
    {
        if ((reads = read(fd, buffer, size > sizeof(buffer) ? sizeof(buffer) : size)) < 0)
        {
            close(fd);
            return SYS_ERR_DEV_READ;
        }

        size -= reads;

        MD5_Update(&md5_ctx, buffer, reads);
    }

    close(fd);

    MD5_Final(digest, &md5_ctx);

    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
        osal_sprintf(&md5sum[i * 2], "%02x", digest[i]);

    return SYS_ERR_OK;
}
