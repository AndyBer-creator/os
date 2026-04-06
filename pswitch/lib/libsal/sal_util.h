
#ifndef __SAL_UTIL_H__
#define __SAL_UTIL_H__

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <common/sys_util.h>
#include <ski/ski_core.h>
#include <libsal/sal_type.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_qos.h>
#include <libsal/sal_util_lock.h>
#include <libcfg/cfg.h>
#include <libfds/fds_export.h>

#define MAX_FILECOPY_BUF_SIZE           512000

typedef enum sys_crypt_keyType_e
{
#ifdef CONFIG_SYS_APP_SSH
    SYS_CRYPT_KEYTYPE_RSA_V1 = 0,     
    SYS_CRYPT_KEYTYPE_RSA_V2,         
    SYS_CRYPT_KEYTYPE_DSA_V2,         
    SYS_CRYPT_KEYTYPE_RSA_PUBKEY_V1,  
    SYS_CRYPT_KEYTYPE_RSA_PUBKEY_V2,  
    SYS_CRYPT_KEYTYPE_DSA_PUBKEY_V2,  
#endif
    SYS_CRYPT_KEYTYPE_SSL_HTTPS,      
    SYS_CRYPT_KEYTYPE_RSA_HTTP,       
    SYS_CRYPT_KEYTYPE_END,
} sys_crypt_keyType_t;

typedef enum sys_ip_type_e
{
    SYS_IP_TYPE_NORMAL = 0,
    SYS_IP_TYPE_UNICAST,
    SYS_IP_TYPE_MULTICAST,
    SYS_IP_TYPE_BROADCAST,
    SYS_IP_TYPE_END
} sys_ip_type_t;

typedef enum sys_mac_type_e
{
    SYS_MAC_TYPE_NORMAL = 0,
    SYS_MAC_TYPE_UNICAST,
    SYS_MAC_TYPE_MULTICAST,
    SYS_MAC_TYPE_BROADCAST,
    SYS_MAC_TYPE_END
} sys_mac_type_t;

#define FLOCK(_fp)  lockf(fileno(_fp), F_LOCK, 0)
#define FULOCK(_fp) lockf(fileno(_fp), F_ULOCK, 0)
#define UNIT                               (cfg_currUnitId_ret())
#define UNIT_SET(u)                        (cfg_currUnitId_set(u))

#define STR2QMSK(str, pQMsk)               (sal_util_str2Queuemask(str, pQMsk))

#define IS_STR_HAS_INVALID_CHAR(str, validChars) (!sal_util_validStringCheck_ret(str, validChars))

#define IS_IPV4_UADDR_INVALID(ip) \
    (FALSE == sal_util_ipv4addrValid_ret(ip, 1))

#define IS_IPV4_MADDR_INVALID(ip) \
    (FALSE == sal_util_ipv4addrValid_ret(ip, 2))

#define IS_IPV4_MASK_INVALID(mask) \
    ((SYS_ERR_FAILED == sal_util_ipv4subnet_vlidate(mask)) ? 1 : 0)

#define SAL_UTIL_MAC_COPY(d, s)            \
do {                                       \
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; \
    d[3] = s[3]; d[4] = s[4]; d[5] = s[5]; \
} while (0)

#define SYSCMD_NULL_MSG " 1> /dev/null 2> /dev/null"

#define IS_MAC_ADDRSTR_VALID(macStr) (TRUE == sal_util_macStrValidate_ret(macStr, SYS_MAC_TYPE_NORMAL))
#define IS_MAC_ADDRSTR_INVALID(macStr) (FALSE == sal_util_macStrValidate_ret(macStr, SYS_MAC_TYPE_NORMAL))
#define IS_MAC_UADDRSTR_VALID(macStr) (TRUE == sal_util_macStrValidate_ret(macStr, SYS_MAC_TYPE_UNICAST))
#define IS_MAC_UADDRSTR_INVALID(macStr) (FALSE == sal_util_macStrValidate_ret(macStr, SYS_MAC_TYPE_UNICAST))
#define IS_MAC_MADDRSTR_VALID(macStr) (TRUE == sal_util_macStrValidate_ret(macStr, SYS_MAC_TYPE_MULTICAST))
#define IS_MAC_MADDRSTR_INVALID(macStr) (FALSE == sal_util_macStrValidate_ret(macStr, SYS_MAC_TYPE_MULTICAST))
#define IS_MAC_BADDRSTR_VALID(macStr) (TRUE == sal_util_macStrValidate_ret(macStr, SYS_MAC_TYPE_BROADCAST))
#define IS_MAC_BADDRSTR_INVALID(macStr) (FALSE == sal_util_macStrValidate_ret(macStr, SYS_MAC_TYPE_BROADCAST))
#define IS_IPV4_ADDRSTR_VALID(ipStr) (TRUE == sal_util_ipv4StrValidate_ret(ipStr, SYS_IP_TYPE_NORMAL))
#define IS_IPV4_ADDRSTR_INVALID(ipStr) (FALSE == sal_util_ipv4StrValidate_ret(ipStr, SYS_IP_TYPE_NORMAL))
#define IS_IPV4_UADDRSTR_VALID(ipStr) (TRUE == sal_util_ipv4StrValidate_ret(ipStr, SYS_IP_TYPE_UNICAST))
#define IS_IPV4_UADDRSTR_INVALID(ipStr) (FALSE == sal_util_ipv4StrValidate_ret(ipStr, SYS_IP_TYPE_UNICAST))
#define IS_IPV4_MADDRSTR_VALID(ipStr) (TRUE == sal_util_ipv4StrValidate_ret(ipStr, SYS_IP_TYPE_MULTICAST))
#define IS_IPV4_MADDRSTR_INVALID(ipStr) (FALSE == sal_util_ipv4StrValidate_ret(ipStr, SYS_IP_TYPE_MULTICAST))
#define IS_IPV4_BADDRSTR_VALID(ipStr) (TRUE == sal_util_ipv4StrValidate_ret(ipStr, SYS_IP_TYPE_BROADCAST))
#define IS_IPV4_BADDRSTR_INVALID(ipStr) (FALSE == sal_util_ipv4StrValidate_ret(ipStr, SYS_IP_TYPE_BROADCAST))
#define IS_IPV6_ADDRSTR_VALID(ipStr) (TRUE == sal_util_ipv6StrValidate_ret(ipStr, SYS_IP_TYPE_NORMAL))
#define IS_IPV6_ADDRSTR_INVALID(ipStr) (FALSE == sal_util_ipv6StrValidate_ret(ipStr, SYS_IP_TYPE_NORMAL))
#define IS_IPV6_UADDRSTR_VALID(ipStr) (TRUE == sal_util_ipv6StrValidate_ret(ipStr, SYS_IP_TYPE_UNICAST))
#define IS_IPV6_UADDRSTR_INVALID(ipStr) (FALSE == sal_util_ipv6StrValidate_ret(ipStr, SYS_IP_TYPE_UNICAST))
#define IS_IPV6_MADDRSTR_VALID(ipStr) (TRUE == sal_util_ipv6StrValidate_ret(ipStr, SYS_IP_TYPE_MULTICAST))
#define IS_IPV6_MADDRSTR_INVALID(ipStr) (FALSE == sal_util_ipv6StrValidate_ret(ipStr, SYS_IP_TYPE_MULTICAST))
#define IS_HOST_EXIST(hostname) (TRUE == sal_util_hostExistChk_ret(hostname))
#define IS_HOSTNAME_VALID(hostname) (TRUE == sal_util_hostnameStrValidate_ret(hostname))
#define IS_HOSTNAME_INVALID(hostname) (FALSE == sal_util_hostnameStrValidate_ret(hostname))
#define IS_FILE_EXIST(filename) (SYS_ERR_OK == sal_util_file_exist(filename))
#define STRING_VALIDATE(pStr, allowEmpty) (sal_util_string_chk(pStr, FALSE, FALSE, allowEmpty))
#define SNMP_STRING_VALIDATE(pStr, allowEmpty) (sal_util_string_chk(pStr, FALSE, TRUE, allowEmpty))
#define FILENAME_VALIDATE(pStr, allowEmpty) (sal_util_string_chk(pStr, TRUE, FALSE, allowEmpty))

#define STR2IPV4(str, pIp) (sys_util_str2Ip(str, pIp))
#define IPV42STR(ip, str) (sys_util_ip2Str(ip, str))
#define STR2IPV6(str, pIp) (sal_util_str2Ipv6(str, pIp))
#define IPV62STR(ip, str) (sal_util_ipv62Str(ip, str))

#if 1
#define _SYSTEM_CMD_DBG(_cmd)
#else
#define _SYSTEM_CMD_DBG(_cmd) SYS_PRINTF("[SYSTEM CMD][%s:%d] %s\n", __func__, __LINE__, __cmd)
#endif
#define _SYSTEM_CMD_LEN(_len, _hid, _bg, _fmt, _args...)      \
do {                                                          \
    char *__cmd = NULL;                                       \
    if (NULL != (__cmd = osal_alloc(_len)))                   \
    {                                                         \
        osal_memset(__cmd, 0, _len);                          \
        osal_snprintf(__cmd, _len, _fmt _bg _hid, ##_args);   \
        system(__cmd);                                        \
        _SYSTEM_CMD_DBG(_cmd);                                \
        osal_free(__cmd);                                     \
    }                                                         \
} while(0)
#define SYSTEM_CMD_LEN(_len, _fmt, _args...) _SYSTEM_CMD_LEN(_len, "", "", _fmt, ##_args)
#define SYSTEM_CMD_LEN_HID(_len, _fmt, _args...) _SYSTEM_CMD_LEN(_len, " 1> /dev/null 2> /dev/null", "", _fmt, ##_args)
#define SYSTEM_CMD_LEN_BG(_len, _fmt, _args...) _SYSTEM_CMD_LEN(_len, "", " &", _fmt, ##_args)
#define SYSTEM_CMD_LEN_HID_BG(_len, _fmt, _args...) _SYSTEM_CMD_LEN(_len, " 1> /dev/null 2> /dev/null", " &", _fmt, ##_args)
#define SYSTEM_CMD(_fmt, _args...) SYSTEM_CMD_LEN(SYS_BUF256_LEN, _fmt, ##_args)
#define SYSTEM_CMD_HID(_fmt, _args...) SYSTEM_CMD_LEN_HID(SYS_BUF256_LEN, _fmt, ##_args)
#define SYSTEM_CMD_BG(_fmt, _args...) SYSTEM_CMD_LEN_BG(SYS_BUF256_LEN, _fmt, ##_args)
#define SYSTEM_CMD_HID_BG(_fmt, _args...) SYSTEM_CMD_LEN_HID_BG(SYS_BUF256_LEN, _fmt, ##_args)

extern char *crypt(const char *key, const char *salt);

extern int32 sal_util_ioctl(uint32 cmdId, void *pData);

extern int32 sal_util_file_exist(char *filename);

extern uint32 sal_util_fileSize_ret(char *filename);

extern int32 sal_util_isLinkedFile_ret(char *symblink, char *filename);

extern int32 sal_util_isFileConsistent_ret(char *srcFilename, char *tarFilename);

extern int32 sal_util_file_touch(char *filename);

extern int32 sal_util_file_copy(char *fromfile, char *tofile);

extern int32 sal_util_file_rename(char *pOldFileName, char *pNewFileName);

extern int32 sal_util_file_sync(void);

extern int32 sal_util_dir_create(char *dirpath);

extern int32 sal_util_fileDateStr_get(char *file, char *date);

extern int32 sal_util_randomfileName_gen(char *path, char *basename, char *filename);

extern int32 sal_util_str2Queuemask(const char *str, sys_queuemask_t *pQueuemask);

extern uint32 sal_util_crc32_ret(uint32 crc, const char *buf, uint32 len);

extern int sal_util_process_kill(char *keyword);

extern int32 sal_util_currFreeMemSize_ret(void);

extern int32 sal_util_password_oldcrypt(char *passwd, char *saltstr, char *result);

extern int32 sal_util_password_encrypt(char *passwd, char *saltstr, char *result);

extern uint32 sal_util_validStringCheck_ret(char *str, char *validChars);

extern int32 sal_util_processNum_ret(char *keyword);

extern int32 sal_util_processExist_ret(int32 pid);

extern int32 sal_util_fileExtNameValid_ret(char *filename, char *extname);

extern int32 sal_util_macStrValidate_ret(const char *str, sys_mac_type_t type);

extern int32 sal_util_ipv4StrValidate_ret(const char *str, sys_ip_type_t type);

extern int32 sal_util_ipv6StrValidate_ret (const char *str, sys_ip_type_t type);

extern uint32 sal_util_hostnameStrValidate_ret(char *str);

extern int32 sal_util_ipv4subnet_vlidate(uint32 ipSubnet);

extern int32 sal_util_ipv4addrValid_ret(uint32 ip, uint32 type);

extern int32 sal_util_getHostByName(uint32 *pIp, char *pName);
extern int32 sal_util_getHostByName2(char *pIp, char *pName);

extern uint32 sal_util_hostExistChk_ret(char *hostname);

extern int32 sal_util_baudrateValid_ret(uint32 baudrate);

extern int32 sal_util_str2lowerStr_xlate(char *str);

extern int32 sal_util_string_chk(char *pStr, char isFilename, char isSNMP, char allowEmpty);

extern char *sal_util_base64_encode(char *input, int32 length);

extern char *sal_util_base64_decode(char *input, int32 length);

extern int32 sal_util_str_encrypt(char *input, char *output);

extern int32 sal_util_str_decrypt(char *input, char *output);

extern int32 sal_util_md5_hash(char *data, uint32 len, char *md5sum);

extern int32 sal_util_key_generate(sys_crypt_keyType_t type);

extern int32 sal_util_rsaModulus_get(char *pBuf);

extern int32 sal_util_rsa_decrypt(char *privateKey, char *pEcryptMsg, char *pDecryptMsg);

extern int32 sal_util_hostname_resolv(char *hostname, char *pStr, uint32 size);

extern uint32 sal_util_defdbVal_ret(def_t key);

extern char *sal_util_defdbStr_ret(def_t key);

extern int32 sal_util_str2Ipv6(char *pStr, sys_ipv6_t *pIp);

extern int32 sal_util_ipv62Str(sys_ipv6_t ip, char *pStr);

extern uint32 sal_util_file_line(char *filepath);
extern uint32 sal_util_file_del_head_line(char *filename, uint32 endline);
#endif

