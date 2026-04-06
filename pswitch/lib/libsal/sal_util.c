
#include <sys/mman.h>
#include <netdb.h>
#include <dirent.h>
#include <libsal/sal_util.h>

#define IPV6_ADDR_STR      "0123456789abcdefABCDEF:.%"
#define IPV6_PREFIX_STR    "0123456789abcdefABCDEF:.%/"

static char *g_db = NULL;

uint32 sal_util_file_line(char *filepath)
{
    int fd;
    char *map;
    char *p;
    uint32 lineCnt = 0;

    struct stat s;

    fd = open(filepath, O_RDONLY);

    if (fd < 0)
        return SYS_ERR_OK;

    if (fstat(fd, &s))
    {
        close(fd);
        return SYS_ERR_OK;
    }

    map = mmap(0, s.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        return SYS_ERR_FILE_OPEN;
    }

    p = map + s.st_size - 1;
    while (p >= map)
    {
        if (*p == '\n')
        {
			lineCnt ++;
        }
        p--;
    }

    munmap(map, s.st_size);
    close(fd);

    return lineCnt;
}

uint32 sal_util_file_del_head_line(char *filename, uint32 endline)
{
    int fd;
    char *map;
    char *p;
    char *e;
    uint32 lineCnt = 0;

    struct stat s;

    fd = open(filename, O_RDWR);

    if (fd < 0)
        return SYS_ERR_OK;

    if (fstat(fd, &s))
    {
        close(fd);
        return SYS_ERR_OK;
    }

    map = mmap(0, s.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        return SYS_ERR_FILE_OPEN;
    }

    e = map + s.st_size - 1;
    p = map;
    while (e >= p)
    {
        if (*p == '\n')
        {
			lineCnt ++;
			if(lineCnt == endline)
			{
				p++;
				osal_memcpy(map, p, (e-p));
				osal_memset(map+(e-p), 0, p-map);
				break;
			}
        }
        p++;
    }

    munmap(map, s.st_size);
    close(fd);

    return SYS_ERR_OK;
}

int32 sal_util_ioctl(uint32 cmdId, void *pData)
{
    int32 fd;

    if (-1 == (fd = open(SKI_CORE_DEV_NAME, O_RDWR)))
        return SYS_ERR_SOCKET;

    if (ioctl(fd, cmdId, pData) < 0)
    {
        close(fd);

        return SYS_ERR_IOCTL;
    }

    close(fd);

    return SYS_ERR_OK;
}

int32 sal_util_file_exist(char *filename)
{
    FILE *fp = NULL;

    if (NULL == (fp = fopen(filename, "r")))
    {
        
        return SYS_ERR_FILE_NOT_EXIST;
    }

    fclose(fp);

    return SYS_ERR_OK;
}

uint32 sal_util_fileSize_ret(char *filename)
{
    FILE *fp = NULL;
    uint32 size = 0;

    if (NULL == (fp = fopen(filename, "r")))
        return 0;

    if (0 != fseek(fp, 0, SEEK_END))
    {
        fclose(fp);

        return 0;
    }

    size = ftell(fp);

    fclose(fp);

    return size;
}

int32 sal_util_isLinkedFile_ret(char *symblink, char *filename)
{
    char *pCfg = NULL;
    char cfg[CAPA_FILE_FULLNAME_LEN];

    osal_memset(cfg, 0, sizeof(cfg));

    pCfg = realpath(symblink, cfg);

    if ((NULL != pCfg) && (0 == osal_strcmp(cfg, filename)))
        return TRUE;
    else
        return FALSE;
}

int32 sal_util_isFileConsistent_ret(char *srcFilename, char *tarFilename)
{
    char *buf;
    uint32 srcSize = 0;
    uint32 tarSize = 0;
    uint32 srcChecksum = 0;
    uint32 tarChecksum = 0;
    FILE *fp = NULL;

    srcSize = sal_util_fileSize_ret(srcFilename);
    tarSize = sal_util_fileSize_ret(tarFilename);

    if ((srcSize != tarSize) || (0 == srcSize))
        return FALSE;

    if (NULL == (fp = fopen(srcFilename, "r")))
        return FALSE;

    if (NULL == (buf = malloc(srcSize)))
    {
        fclose(fp);

        return FALSE;
    }

    osal_memset(buf, 0, srcSize);

    fread(buf, 1, srcSize, fp);

    srcChecksum = sal_util_crc32_ret(srcChecksum, buf, srcSize);

    free(buf);

    fclose(fp);

    if (NULL == (fp = fopen(tarFilename, "r")))
        return FALSE;

    if (NULL == (buf = malloc(tarSize)))
    {
        fclose(fp);

        return FALSE;
    }

    osal_memset(buf, 0, tarSize);

    fread(buf, 1, tarSize, fp);

    tarChecksum = sal_util_crc32_ret(tarChecksum, buf, tarSize);

    free(buf);

    fclose(fp);

    if (srcChecksum != tarChecksum)
        return FALSE;

    return TRUE;
}

int32 sal_util_file_touch(char *filename)
{
    FILE *fp;

    SYS_PARAM_CHK((NULL == filename), SYS_ERR_NULL_POINTER);

    if (NULL == (fp = fopen(filename, "w")))
        return SYS_ERR_FILE_OPEN;

    fclose(fp);

    return SYS_ERR_OK;
}

int32 sal_util_file_copy(char *fromfile, char *tofile)
{
    char *buf = NULL;
    FILE *fp = NULL;
    uint32 size = 0;

    SYS_PARAM_CHK((NULL == fromfile), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == tofile), SYS_ERR_NULL_POINTER);

    if (NULL == (fp = fopen(fromfile, "r")))
    {
        
        return SYS_ERR_FILE_OPEN;
    }

    if (0 != fseek(fp, 0, SEEK_END))
    {
        fclose(fp);

        return SYS_ERR_FILE_SEEK;
    }

    size = ftell(fp);

    if ((size <= 0) || (size >= MAX_FILECOPY_BUF_SIZE))
    {
        fclose(fp);

        return SYS_ERR_FILE_SIZE;
    }

    if (0 != fseek(fp, 0L, SEEK_SET))
    {
        fclose(fp);

        return SYS_ERR_FILE_SEEK;
    }

    buf = malloc(size);
    osal_memset(buf, 0, size);

    if (fread(buf, 1, size, fp) != size)
    {
        fclose(fp);
        free(buf);

        return SYS_ERR_FILE_READ;
    }

    fclose(fp);

    if (NULL == (fp = fopen(tofile, "w")))
    {
        free(buf);

        return SYS_ERR_FILE_OPEN;
    }

    if (fwrite(buf, 1, size, fp) != size)
    {
        fclose(fp);
        free(buf);

        return SYS_ERR_FILE_WRITE;
    }

    fclose(fp);
    free(buf);

    return SYS_ERR_OK;
}

int32 sal_util_file_rename(char *pOldFileName, char *pNewFileName)
{
    
    if (strcmp(pOldFileName, pNewFileName) == 0)
    {
        return(SYS_ERR_OK);
    }

    remove(pNewFileName);

    if (rename(pOldFileName, pNewFileName) == -1)
    {
        return(SYS_ERR_FAILED);
    }

    return(SYS_ERR_OK);
}

int32 sal_util_file_sync(void)
{
    sync();
    sync();
    sync();
    return SYS_ERR_OK;
}

int32 sal_util_dir_create(char *dirpath)
{
    char cmd[SYS_BUF512_LEN];

    SYS_MEM_CLEAR(cmd);

    if (SYS_ERR_OK == sal_util_file_exist(dirpath))
        return SYS_ERR_OK;

    osal_sprintf(cmd, "mkdir -p %s", dirpath);

    system(cmd);

    if (SYS_ERR_OK != sal_util_file_exist(dirpath))
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}

int32 sal_util_fileDateStr_get(char *file, char *date)
{
    int32 ret;
    struct tm* clock;
    struct stat attrib;
    char tzStr[SYS_BUF16_LEN];
    char *minStr = NULL;
    int32 diffHr = 0;
    int32 diffMin = 0;

    ret = stat(file, &attrib);

    if (ret < 0)
        return SYS_ERR_FAILED;

    SYS_MEM_CLEAR(tzStr);
    sal_timezoneStr_get(tzStr);

    if (tzStr[3] == '+')
    {
        if (NULL != (minStr = strchr(tzStr+4, ':')))
            sscanf(tzStr + 4, "%d:%d", &diffHr, &diffMin);
        else
            sscanf(tzStr + 4, "%d", &diffHr);
    }
    else
    {
        if (tzStr[3] == '-')
        {
            if (NULL != (minStr = strchr(tzStr+4, ':')))
                sscanf(tzStr + 4, "%d:%d", &diffHr, &diffMin);
            else
                sscanf(tzStr + 4, "%d", &diffHr);
        }
        else
        {
            if (NULL != (minStr = strchr(tzStr+3, ':')))
                sscanf(tzStr + 3, "%d:%d", &diffHr, &diffMin);
            else
                sscanf(tzStr + 3, "%d", &diffHr);
        }

        diffHr = diffHr * (-1);
        diffMin = diffMin * (-1);
    }

    attrib.st_mtime = attrib.st_mtime  + (diffHr * 3600) + (diffMin * 60);

    clock = gmtime(&(attrib.st_mtime));

    osal_sprintf(date, "%4d-%02d-%02d %02d:%02d:%02d",
                 (1900 + clock->tm_year),
                 (1 + clock->tm_mon),
                 clock->tm_mday,
                 clock->tm_hour,
                 clock->tm_min,
                 clock->tm_sec);

    return SYS_ERR_OK;
}

int32 sal_util_randomfileName_gen(char *path, char *basename, char *filename)
{
    uint32 fileno = 0;

    srand(time(NULL));
    fileno = rand();

    osal_sprintf(filename, "%s/%s_%d", path, basename, fileno);

    return SYS_ERR_OK;
}

int32 sal_util_str2Queuemask(const char *str, sys_queuemask_t *pQueuemask)
{
    char buf1[SYS_BUF32_LEN];
    char buf2[SYS_BUF32_LEN];
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char strBuf[SYS_BUF32_LEN];
    int32 len1;
    int32 len2;
    sys_qid_t qid;
    sys_qid_t qidEnd;
    sys_qid_t qidStart;
    sys_qid_t qidTmp;
    sys_queuemask_t queuemaskTmp;

    SYS_PARAM_CHK((NULL == str), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pQueuemask), SYS_ERR_NULL_POINTER);

    osal_memset(&queuemaskTmp, 0, sizeof(queuemaskTmp));
    osal_memset(strBuf, 0, sizeof(strBuf));

    osal_strcpy(strBuf, str);
    ptr1 = strBuf;

    while (0 != osal_strlen(ptr1))
    {
        osal_memset(buf1, 0, sizeof(buf1));
        osal_memset(buf2, 0, sizeof(buf2));
        SYS_ERR_CHK(sys_util_getnext(ptr1, ',', buf1, &len1));
        ptr2 = buf1;

        if (NULL != osal_strchr(ptr2, '-')) 
        {
            SYS_ERR_CHK(sys_util_getnext(ptr2, '-', buf2, &len2));
            qidStart = SYS_STR2UINT(buf2) - 1;

            if (qidStart > CAPA_QOS_QUEUE_NUM)
                return SYS_ERR_QUEUE_ID;

            ptr2 += len2;
            SYS_ERR_CHK(sys_util_getnext(ptr2, '-', buf2, &len2));
            qidEnd = SYS_STR2UINT(buf2) - 1;

            if (qidEnd > CAPA_QOS_QUEUE_NUM)
                return SYS_ERR_QUEUE_ID;

            if (qidEnd < qidStart)
            {
                
                qidTmp = qidStart;
                qidStart = qidEnd;
                qidEnd = qidTmp;
            }

            for (qid = qidStart; qid <= qidEnd; qid++)
            {
                QUEUEMASK_SET_QID(queuemaskTmp, qid);
            }
        }
        else 
        {
            qid = SYS_STR2UINT(buf1) - 1;

            if (qid > CAPA_QOS_QUEUE_NUM)
                return SYS_ERR_QUEUE_ID;

            QUEUEMASK_SET_QID(queuemaskTmp, qid);
        }

        ptr1 += len1;
    }

    *pQueuemask = queuemaskTmp;

    return SYS_ERR_OK;
}

int32 sal_util_process_kill(char *keyword)
{
    char buf[1024];
    char *name = NULL;
    char status[32];
    static DIR *dir;
    struct dirent *entry;
    FILE *fp;
    int32 n;
    int32 pid = 0;

    if (!dir)
    {
        dir = opendir("/proc");

        if (NULL == dir)
            return SYS_ERR_FAILED;
    }

    for (;;)
    {
        if (NULL == (entry = readdir(dir)))
        {
            closedir(dir);
            dir = NULL;

            return SYS_ERR_OK;
        }

        name = entry->d_name;

        if (!(*name >= '0' && *name <= '9'))
            continue;

        pid = atoi(name);

        osal_sprintf(status, "/proc/%d/cmdline", pid);

        if (NULL == (fp = fopen(status, "r")))
            continue;

        if ((n = fread(buf, 1, (sizeof(buf) - 1), fp)) > 0)
        {
            if ('\n' == buf[n-1])
                buf[--n] = 0;

            name = buf;

            while (n)
            {
                if (((unsigned char)*name) < ' ')
                    *name = ' ';

                name++;
                n--;
            }
            *name = 0;

            if(buf[0])
            {
                if (osal_strstr(buf, keyword))
                    kill(pid, SIGKILL);
            }
            
        }
        fclose(fp);
    }

    return SYS_ERR_OK;
}

int32 sal_util_currFreeMemSize_ret(void)
{
    char tmp[SYS_BUF128_LEN];
    FILE *fp;
    int32 tmpSize = -1;
    int32 size = 0;

    osal_memset(tmp, 0, sizeof(tmp));

    if ( NULL == (fp = fopen("/proc/meminfo", "r")))
        return -1;
    
    while (NULL != fgets(tmp, sizeof(tmp), fp))
    {
        
        if (0 != osal_strstr(tmp, "MemFree:"))
        {
            tmpSize = 0;

            osal_sscanf(tmp, "MemFree:\t%d kB", &tmpSize);

            size += tmpSize;

            continue;
        }

        if (0 != osal_strstr(tmp, "Buffers:"))
        {
            tmpSize = 0;

            osal_sscanf(tmp, "Buffers:\t%d kB", &tmpSize);

            fclose(fp);

            size += tmpSize;

            return size;
        }
    }

    fclose(fp);

    return -1;
}

int32 sal_util_password_oldcrypt(char *passwd, char *saltstr, char *result)
{
    char *crypt_passwd;
    char salt[3];
    static unsigned char itoa64[] =     
           "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    if (NULL == saltstr)
    {
        salt[0] = itoa64[random() % 64];
        salt[1] = itoa64[random() % 64];
        salt[2] = '\0';

        crypt_passwd = crypt(passwd, salt);
    }
    else
    {
        crypt_passwd = crypt(passwd, saltstr);
    }

    osal_strcpy(result, crypt_passwd);

    free(crypt_passwd);

    return SYS_ERR_OK;
}

int32 sal_util_password_encrypt(char *passwd, char *saltstr, char *result)
{
    char crypt_passwd[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    char *b64 = NULL;

    SYS_MEM_CLEAR(crypt_passwd);

    if (NULL != saltstr)
    {
        sal_util_password_oldcrypt(passwd, saltstr, crypt_passwd);

        if (0 == osal_strcmp(saltstr, crypt_passwd))
        {
            osal_strcpy(result, crypt_passwd);

            return SYS_ERR_OK;
        }
    }

    SYS_MEM_CLEAR(crypt_passwd);

    sal_util_md5_hash(passwd, strlen(passwd), crypt_passwd);

    b64 = sal_util_base64_encode(crypt_passwd, strlen(crypt_passwd));

    if (NULL != b64)
    {
        osal_strcpy(result, b64);

        osal_free(b64);
    }
    else
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

uint32 sal_util_validStringCheck_ret(char *str, char *validChars)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;

    if ((str == NULL) || (validChars == NULL))
        return FALSE;

    ptr1 = str;

    while (0 != *ptr1)
    {
        ptr2 = NULL;
        ptr2 = osal_strchr(validChars, *ptr1);

        if (ptr2 == NULL)
            return FALSE;

        ptr1++;
    }

    return TRUE;
}

int32 sal_util_processNum_ret(char *keyword)
{
    char buf[1024];
    char *name;
    char status[32];
    static DIR *dir;
    struct dirent *entry;
    FILE *fp;
    int32 cnt = 0;
    int32 n;
    int32 pid = 0;

    if (!dir)
    {
        dir = opendir("/proc");

        if (NULL == dir)
            return 0;
    }

    for(;;)
    {
        if ((entry = readdir(dir)) == NULL)
        {
            closedir(dir);
            dir = 0;

            return cnt;
        }

        name = entry->d_name;

        if (!(*name >= '0' && *name <= '9'))
            continue;

        pid = atoi(name);

        osal_sprintf(status, "/proc/%d/cmdline", pid);

        if (NULL == (fp = fopen(status, "r")))
            continue;

        if ((n = fread(buf, 1, (sizeof(buf) - 1), fp)) > 0)
        {
            if ('\n' == buf[n-1])
                buf[--n] = 0;

            name = buf;

            while (n)
            {
                if (((unsigned char)*name) < ' ')
                    *name = ' ';

                name++;
                n--;
            }

            *name = 0;

            if (buf[0])
            {
                if (osal_strstr(buf, keyword))
                    cnt++;
            }
            
        }

        fclose(fp);
    }

    return cnt;
}

int32 sal_util_processExist_ret(int32 pid)
{
    FILE *fp = NULL;
    char buf[SYS_BUF32_LEN];

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "/proc/%d/cmdline", pid);

    if (NULL != (fp = fopen(buf, "r")))
    {
        fclose(fp);

        return TRUE;
    }

    return FALSE;
}

int32 sal_util_fileExtNameValid_ret(char *filename, char *extname)
{
    if (0 == osal_memcmp((filename + osal_strlen(filename) - osal_strlen(extname)), extname, osal_strlen(extname)))
    {
        return TRUE;
    }

    return FALSE;
}

int32 sal_util_macStrValidate_ret(const char *str, sys_mac_type_t type)
{
    char macStr[CAPA_MAC_STR_LEN];
    char sep = 0;
    sys_mac_t mac;

    if (str == NULL)
        return FALSE;

    if (strlen(str) != 17) 
        return FALSE;

    SYS_MEM_CLEAR(mac);

    osal_strcpy(macStr, str);
    sal_util_str2lowerStr_xlate(macStr);
    if (NULL != osal_strchr(macStr, ':'))
    {
        sep = ':';

        if (!sal_util_validStringCheck_ret(macStr, "0123456789abcdef:"))
            return FALSE;
    }
    else if (NULL != osal_strchr(macStr, '-'))
    {
        sep = '-';
        if (!sal_util_validStringCheck_ret(macStr, "0123456789abcdef-"))
            return FALSE;
    }

    if (macStr[2] != sep || macStr[5] != sep || macStr[8] != sep || macStr[11] != sep || macStr[14] != sep)
        return FALSE;

    sys_util_str2Mac(macStr, mac.octet);

    switch (type)
    {
        case SYS_MAC_TYPE_UNICAST:
            if ((0x1 & mac.octet[0]))
                return FALSE;
            break;

        case SYS_MAC_TYPE_MULTICAST:
            if (!(0x1 & mac.octet[0]))
                return FALSE;
            break;

        case SYS_MAC_TYPE_BROADCAST:
            if (0xFF != mac.octet[0] || 0xFF != mac.octet[1] || 0xFF != mac.octet[2]
                || 0xFF != mac.octet[3] || 0xFF != mac.octet[4] || 0xFF != mac.octet[5])
                return FALSE;
            break;

        default:
            break;
    }

    return TRUE;
}

int32 sal_util_ipv4StrValidate_ret(const char *str, sys_ip_type_t type)
{
    const char *sp;
    int32 dots = 0;
    int32 nums = 0;
    char buf[4];
    uint32 groups[4];

    if (str == NULL)
        return FALSE;

    if (strlen(str) > CAPA_IPV4_STR_LEN)
        return FALSE;

    for (;;)
    {
        SYS_MEM_CLEAR(buf);

        sp = str;
        while (*str != '\0')
        {
            if (*str == '.')
            {
                if (dots >= 3)
                    return FALSE;

                if (*(str + 1) == '.')
                    return FALSE;

                if (*(str + 1) == '\0')
                    return FALSE;

                dots++;
                break;
            }
            if (!isdigit((int) *str))
                return FALSE;

            str++;
        }

        if (str - sp > 3)
            return FALSE;

        osal_strncpy(buf, sp, str - sp);

        groups[nums] = atoi(buf);

        if (groups[nums] > 255)
            return FALSE;

        nums++;

        if (*str == '\0')
            break;

        str++;
    }

    if (nums != 4 || dots != 3)
        return FALSE;

    if (SYS_IP_TYPE_NORMAL == type)
        return TRUE;

    switch (type)
    {
        case SYS_IP_TYPE_UNICAST:
            if (groups[0] >= 1 && groups[0] <= 126)
            {
                if ((groups[1] == 0 && groups[2] == 0 && groups[3] == 0)
                    || (groups[1] == 255 && groups[2] == 255 && groups[3] == 255))
                    return FALSE;
            }
            else if (groups[0] >= 128 && groups[0] <= 191)
            {
                if ((groups[2] == 0 && groups[3] == 0) || (groups[2] == 255 && groups[3] == 255))
                    return FALSE;
            }
            else if (groups[0] >= 192 && groups[0] <=223)
            {
                if ((groups[3] == 0) || (groups[3] == 255))
                    return FALSE;
            }
            else
            {
                return FALSE;
            }
            break;

        case SYS_IP_TYPE_MULTICAST:
            if (groups[0] >= 224 && groups[0] <= 239)
                return TRUE;
            else
                return FALSE;
            break;

        case SYS_IP_TYPE_BROADCAST:
            if (groups[0] == 255 && groups[1] == 255 && groups[2] == 255 && groups[3] == 255)
                return TRUE;
            else
                return FALSE;

        default:
            break;
    }

    return TRUE;
}

int32 sal_util_ipv6StrValidate_ret (const char *str, sys_ip_type_t type)
{
    struct sockaddr_in6 sin6_dummy;

    SYS_PARAM_CHK((NULL == str), SYS_ERR_FAILED);

    if (1 == inet_pton(AF_INET6, str, &sin6_dummy.sin6_addr))
    {
        if (SYS_IP_TYPE_NORMAL == type)
            return TRUE;
    }
    else
    {
        return FALSE;
    }

    switch (type)
    {
        case SYS_IP_TYPE_MULTICAST:
        case SYS_IP_TYPE_BROADCAST:
            if (strlen(str) > 2 && (str[0] == 'f' || str[0] == 'F') && (str[1] == 'f' || str[1] == 'F'))
                return TRUE;
            break;

        default:
            if (!(strlen(str) > 2 && (str[0] == 'f' || str[0] == 'F') && (str[1] == 'f' || str[1] == 'F')))
                return TRUE;
            break;
    }

    return FALSE;
}

uint32 sal_util_hostnameStrValidate_ret(char *str)
{
    char *pch = NULL;
    int i = 0;
    char buf[CAPA_HOSTNAME_LEN + 1];

    if (str == NULL)
        return FALSE;

    if (strlen(str) > CAPA_HOSTNAME_LEN)
        return FALSE;

    if ('-' == str[0])
        return FALSE;

    if (NULL != strstr(str, ".."))
        return FALSE;

    SYS_MEM_CLEAR(buf);

    osal_strcpy(buf, str);

    for (i = 0; i < strlen(buf); i++)
    {
        if (!('-' == buf[i] || '.' == buf[i] || isalnum(buf[i])))
            return FALSE;
    }

    pch = strtok(buf,".");

    if (NULL == pch)
    {
        
        if (strlen(buf) > 63 || strlen(buf) == 0)
            return FALSE;

        if (strlen(buf) > 0)
        {
            if (buf[0] == '-' || buf[strlen(buf) - 1] == '-')
                return FALSE;
        }
    }
    else
    {
        goto check_label;

        while (NULL != pch)
        {
            pch = strtok(NULL, ".");

            if (NULL == pch)
                break;

check_label:
            
            if (strlen(pch) > 63 || strlen(pch) == 0)
                return FALSE;

            if (strlen(pch) > 0)
            {
                if (pch[0] == '-' || pch[strlen(pch) - 1] == '-')
                    return FALSE;
            }
        }
    }

    return TRUE;
}

int32 sal_util_ipv4subnet_vlidate(uint32 ipSubnet)
{
    uint32 tmp;
    if (0 == ipSubnet)
        return SYS_ERR_FAILED;

    tmp = ~ipSubnet;

    if((tmp + 1) & tmp)
        return SYS_ERR_FAILED;
    else
        return SYS_ERR_OK;
}

int32 sal_util_ipv4addrValid_ret(uint32 ip, uint32 type)
{
    uint32 isMulti = FALSE;

    if ((0 == ip) || (0xFFFFFFFF == ip))
        return FALSE;

    if (((ip >> 24) >= 224) && ((ip >> 24) <= 239))
        isMulti = TRUE;

    if (1 == type && TRUE == isMulti)
        return FALSE;
    else if (2 == type && FALSE == isMulti)
        return FALSE;

    return TRUE;
}

int32 sal_util_getHostByName(uint32 *pIp, char *pName)
{
    uint8   i;
    struct  hostent *hptr;

    if ((hptr = gethostbyname(pName)) == NULL)
    {
        return SYS_ERR_FAILED;
    }

    i = 0;
    if (hptr->h_addrtype == AF_INET)
    {
        while (hptr->h_addr_list[i] != 0) {
            *pIp = *(uint32 *) hptr->h_addr_list[i++];
        }
    }

    return SYS_ERR_OK;
}

int32 sal_util_getHostByName2(char *pIp, char *pName)
{
    uint8   i;
    struct  hostent *hptr;

    if ((hptr = gethostbyname2(pName, AF_INET6)) == NULL)
    {
        return SYS_ERR_FAILED;
    }

    i = 0;
    while (hptr->h_addr_list[i] != 0) {
        *pIp = *hptr->h_addr_list[i++];
    }

    return SYS_ERR_OK;
}

uint32 sal_util_hostExistChk_ret(char *hostname)
{
    struct  hostent *hptr = NULL;

    if (NULL == (hptr = gethostbyname(hostname)))
    {
        return FALSE;
    }

    return TRUE;
}

int32 sal_util_baudrateValid_ret(uint32 baudrate)
{
    switch (baudrate)
    {
        case 2400:
            return TRUE;

        case 4800:
            return TRUE;

        case 9600:
            return TRUE;

        case 19200:
            return TRUE;

        case 38400:
            return TRUE;

        case 57600:
            return TRUE;

        case 115200:
            return TRUE;

        default:
            return FALSE;
    }

    return FALSE;
}

int32 sal_util_str2lowerStr_xlate(char *str)
{
    char c = 0;
    uint32 i = 0;

    while (str[i])
    {
        c = tolower(str[i]);

        str[i] = c;

        i++;
    }

    return SYS_ERR_OK;
}

int32 sal_util_string_chk(char *pStr, char isFilename, char isSNMP, char allowEmpty)
{
    int32 i ,len = 0;
    char a;

    if (NULL == pStr)
        return SYS_ERR_NULL_POINTER;

    len = osal_strlen(pStr);

    if (len == 0 && !allowEmpty)
        return SYS_ERR_STR_LEN_EMPTY;

    for (i = 0; i < len ; i++)
    {
        a = pStr[i];

        if (a < 0x20 || a > 0x7E) 
          return SYS_ERR_STRING;

        if (a == 0x3f || a == 0x22) 
          return SYS_ERR_STRING;
        else if (isSNMP)
        {

          if (a == 0x5c || a == 0x2c)
            return SYS_ERR_STRING;
        }
        else if (isFilename)
        {

          if (a == 0x5c || a == 0x2f || a == 0x3a || a == 0x2a || a == 0x3c || a == 0x3e || a == 0x7c)
            return SYS_ERR_STRING;
        }
    }

    return SYS_ERR_OK;
}

int32 sal_util_hostname_resolv(char *hostname, char *pStr, uint32 size)
{
    int32 ret = SYS_ERR_DNS_RESOLVE;
    int32 i = 0;
    int32 af_family[] = {
                          AF_INET,
#ifdef CONFIG_SYS_IPV6
                          AF_INET6,
#endif
                        };

    struct addrinfo req;
    struct addrinfo *res = NULL;
    char host[CAPA_HOSTNAME_LEN + 1];

    osal_memset(&host, 0, sizeof(host));

    for (i = 0; i < sizeof(af_family) / sizeof(int32); i++)
    {
        osal_memset(&req, 0, sizeof(req));

        ret = SYS_ERR_DNS_RESOLVE;

        req.ai_family = af_family[i];
        req.ai_socktype = SOCK_DGRAM;

        if (0 != getaddrinfo(hostname, NULL, &req, &res)
            || !res || !res->ai_addr)
        {
            if (res)
                freeaddrinfo(res);

            continue;
        }

#ifdef CONFIG_SYS_IPV6
        if (res->ai_family == AF_INET6)
        {
            if (inet_ntop(AF_INET6,
                          &((struct sockaddr_in6 *)res->ai_addr)->sin6_addr,
                          host, sizeof(host)))
            {
                ret = SYS_ERR_OK;
            }
        }
        else
#endif
        {
            if (inet_ntop(AF_INET,
                          &((struct sockaddr_in *)res->ai_addr)->sin_addr,
                          host, sizeof(host)))
            {
                ret = SYS_ERR_OK;
            }
        }

        freeaddrinfo(res);

        if (SYS_ERR_OK == ret)
            break;
    }

    if (SYS_ERR_OK == ret)
        osal_strncpy(pStr, host, size);

    return ret;
}

uint32 _sal_sys_defdbMem_map(void)
{
    int32 fd = -1;
    int32 len = 0;
    int32 dbSize = 0;
    int32 ret = -1;

    if (NULL != g_db)
        return 1;

    if ((fd = open(DEFDB_DEV, O_RDONLY)) < 0)
        return 0;

    len = getpagesize();
    g_db = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);

    osal_memcpy(&dbSize, g_db, sizeof(int32));

    if (dbSize > len)
    {
        ret = munmap(g_db, len);
        if (ret < 0)
            goto fail;

        len = dbSize;
        g_db = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
        if (MAP_FAILED == g_db)
            goto fail;
    }

    close(fd);

    return TRUE;

fail:
    close(fd);

    return FALSE;
}

uint32 sal_util_defdbVal_ret(def_t key)
{
    defdb_tok_t *pTok = NULL;
    uint32 *pVal = NULL;

    if (!_sal_sys_defdbMem_map())
        return 0;

    pTok = (defdb_tok_t *) (g_db + sizeof(uint32) + (key * sizeof(defdb_tok_t)));

    if (DEF_TYPE_INT == pTok->type || DEF_TYPE_UINT == pTok->type)
    {
        pVal = (uint32 *) (g_db + pTok->offset);

        return *pVal;
    }

    return 0;
}

char *sal_util_defdbStr_ret(def_t key)
{
    defdb_tok_t *pTok = NULL;

    if (!_sal_sys_defdbMem_map())
        return "";

    pTok = (defdb_tok_t *) (g_db + sizeof(uint32) + (key * sizeof(defdb_tok_t)));

    if (DEF_TYPE_STR == pTok->type)
    {
        return ((char *) (g_db + pTok->offset));
    }

    return "";
}

int32 sal_util_str2Ipv6(char *pStr, sys_ipv6_t *pIp)
{
    int32 ret = 0;

    ret = inet_pton(AF_INET6, pStr, pIp->hex);

    if (ret < 0)
        return SYS_ERR_IPV6_ADDR;

    return SYS_ERR_OK;
}

int32 sal_util_ipv62Str(sys_ipv6_t ip, char *pStr)
{
    if (NULL == inet_ntop(AF_INET6, ip.hex, pStr, CAPA_IPV6_STR_LEN))
        return SYS_ERR_IPV6_ADDR;

    return SYS_ERR_OK;
}

