#ifndef __COMMON_SYS_UTIL_H__
#define __COMMON_SYS_UTIL_H__
#include <common/sys_type.h>
#define SYS_PRINTF(fmt, args...)       osal_printf(fmt, ## args)

#define SYS_STR2UINT(str)              (sys_util_str2ul_ret(str))

#define SYS_MEM_SET(var)                        \
    do { osal_memset(&var, 0xFF, sizeof(var)); } while (0)

#define SYS_MEM_CLEAR(var)                       \
    do { osal_memset(&var, 0, sizeof(var)); } while (0)

#define SYS_MEM_ALLOC(_buf, _size)                      \
do {                                                    \
    if (NULL == (_buf = osal_alloc(_size)))             \
        return SYS_ERR_OUT_OF_MEMORY;                   \
    osal_memset(_buf, 0, _size);                        \
} while(0)

#define SYS_MEM_ALLOC_HDL(_buf, _size, _errHandle, ret) \
do {                                                    \
    if (NULL == (_buf = osal_alloc(_size)))             \
    {                                                   \
        ret = SYS_ERR_OUT_OF_MEMORY;                    \
        goto _errHandle;                                \
    }                                                   \
    osal_memset(_buf, 0, _size);                        \
} while(0)

#define SYS_MEM_FREE(_ptr)  \
do {                        \
    if (NULL != _ptr)       \
        osal_free(_ptr);    \
} while (0)

#define SYS_MAX(a,b)                   ((a) > (b) ? (a) : (b))
#define SYS_MIN(a,b)                   ((a) < (b) ? (a) : (b))

#define SYS_DIM(array)                 (sizeof(array) / sizeof(array[0]))
#define SYS_DIV_ROUNDUP(value,round)   (((value) + (round) - 1) / (round))

#define SYS_UINT32_LOW16(u32)                    \
    ((uint16)((u32) & 0xFFFF))

#define SYS_UINT32_HIGH16(u32)                   \
    ((uint16)(((u32) >> 16) & 0xFFFF))

#define SYS_UINT32_LOW8(u32)                     \
    ((uint8)((u32) & 0xFF))

#define SYS_UINT32_HIGH8(u32)                    \
    ((uint8)(((u32) >> 24) & 0xFF))

#define SYS_UINT32_SWAP(u32)                     \
        ((((u32) & 0xff000000U) >> 24) |        \
         (((u32) & 0x00ff0000U) >>  8) |        \
         (((u32) & 0x0000ff00U) <<  8) |        \
         (((u32) & 0x000000ffU) << 24))

#define SYS_SHIFT_BIT(bit)                  (1 << bit)

#define INC(x) \
    do { if ((x+1) != 0) x = x + 1; } while(0)
#define DEC(x) \
    do { if (x != 0) x = x - 1; } while(0)

#define MAC_PRINT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_PRINT_ARG(mac) \
    (mac)[0], (mac)[1], (mac)[2], (mac)[3], (mac)[4], (mac)[5]

#define IPADDR_PRINT "%d.%d.%d.%d"
#define IPADDR_PRINT_ARG(ip) \
    ((ip) & 0xFF000000) >> 24, ((ip) & 0xFF0000) >> 16, ((ip) & 0xFF00) >> 8, (ip) & 0xFF

#define IPADDRV6_PRINT "%X:%X:%X:%X:%X:%X:%X:%X"
#define IPADDRV6_PRINT_ARG(ipv6) \
    *((uint16 *)&ipv6[0]), *((uint16 *)&ipv6[2]), *((uint16 *)&ipv6[4]), *((uint16 *)&ipv6[6]), \
    *((uint16 *)&ipv6[8]), *((uint16 *)&ipv6[10]), *((uint16 *)&ipv6[12]), *((uint16 *)&ipv6[14])
extern int32 sys_util_getnext(char *src, char separator, char *dest, int32 *len);
extern int32 sys_util_str2Ip(char *str, uint32 *ip);
extern int32 sys_util_ip2Str(uint32 ip, char *str);
extern int32 sys_util_str2Mac(char *str, uint8 *mac);
extern int32 sys_util_mac2Str(uint8 *mac, char *str);
extern int32 sys_util_hexStr2byteArray(const char *pStr, uint8 *valArray);
extern int32 sys_util_byteArray2hexStr(uint8 *valArray, uint16 size, char *pStr);
extern uint32 sys_util_str2ul_ret(const char *pStr);
extern int32 sys_util_str2ulMask(const char *str, uint32 *pUlmask);
extern int32 sys_util_ulMask2Str(uint32 ulmask, char *str);
extern int32 sys_util_namestrcpy(sys_namestr_t *pDest, const char *pSrc);
extern int32 sys_util_bitmaskIsClear_ret(uint32 *maskAddr, uint32 bitIdx, uint32 bitNum);
extern int32 sys_util_bitmaskIsAllSet_ret(uint32 *maskAddr, uint32 bitIdx, uint32 bitNum);
extern int32 sys_util_bitmaskIsEqual_ret(uint32 *maskAddr1, uint32 *maskAddr2, uint32 bitNum);

#endif

