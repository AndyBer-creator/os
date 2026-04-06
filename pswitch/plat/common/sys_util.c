#include <common/sys_def.h>
int32 sys_util_getnext(char *pSrc, char separator, char *pDest, int32 *pLen)
{
    char *pC;

    SYS_PARAM_CHK((NULL == pSrc), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pDest), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pLen), SYS_ERR_NULL_POINTER);

    pC = osal_strchr(pSrc, separator);

    if(NULL == pC)
    {
        osal_strcpy(pDest, pSrc);
        *pLen = osal_strlen(pDest) + 1;

        return SYS_ERR_OK;
    }

    *pLen = pC - pSrc;
    osal_strncpy(pDest, pSrc, *pLen);
    pDest[*pLen] = '\0';
    *pLen += 1;

    return SYS_ERR_OK;
} 
int32 sys_util_str2Ip(char *pStr, uint32 *pIp)
{
    int32 c[4]; 
    int32 len, i;
    char *ptr;
    char buf[128] = {0};

    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    ptr = pStr;

    for (i = 0; i < 3; ++i)
    {
        SYS_ERR_CHK(sys_util_getnext(ptr, '.', buf, &len));

        c[i] = SYS_STR2UINT(buf);
        SYS_PARAM_CHK((c[i] < 0) || (c[i] > 255), SYS_ERR_FAILED);

        ptr += len;
    }
    c[3] = SYS_STR2UINT(ptr);
    SYS_PARAM_CHK((c[3] < 0) || (c[3] > 255), SYS_ERR_FAILED);

    *pIp = (c[0] << 24) + (c[1] << 16) + (c[2] << 8) + c[3];
    return SYS_ERR_OK;
} 
int32 sys_util_ip2Str(uint32 ip, char *pStr)
{
    
    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);

    osal_sprintf(pStr, "%d.%d.%d.%d",
                ((ip & 0xFF000000) >> 24),
                ((ip & 0x00FF0000) >> 16),
                ((ip & 0x0000FF00) >> 8),
                (ip & 0x000000FF));

    return SYS_ERR_OK;
} 
int32 sys_util_str2Mac(char *pStr, uint8 *pMac)
{
    int32 i;
    int32 v[6];
    char *ptr;
    char separator = 0;

    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pMac), SYS_ERR_NULL_POINTER);

    ptr = pStr;
    if(NULL != osal_strchr(ptr, ':'))
    {
        separator = ':';
    }
    else if(NULL != osal_strchr(ptr, '-'))
    {
        separator = '-';
    }

    switch(separator)
    {
        case '-':
            osal_sscanf(ptr, "%x-%x-%x-%x-%x-%x", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]);
            break;
        case ':':
        default:
            osal_sscanf(ptr, "%x:%x:%x:%x:%x:%x", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]);
            break;
    }

    for(i = 0; i < 6; i++)
    {
        pMac[i] = v[i];
    }

    return SYS_ERR_OK;
} 
int32 sys_util_mac2Str(uint8 *pMac, char *pStr)
{
    
    SYS_PARAM_CHK((NULL == pMac), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);

    osal_sprintf(pStr, "%02X:%02X:%02X:%02X:%02X:%02X",
        pMac[0], pMac[1], pMac[2], pMac[3], pMac[4], pMac[5]);

    return SYS_ERR_OK;
} 
int32 sys_util_hexStr2byteArray(const char *pStr, uint8 *valArray)
{
    uint8 value = 0;
    uint32 i = 0;

    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == valArray), SYS_ERR_NULL_POINTER);

    for(i = 0; i < osal_strlen(pStr); i++)
    {
        if(!isdigit(pStr[i]) && (pStr[i] > 'F' || pStr[i] < 'A') && (pStr[i] > 'f' || pStr[i] < 'a'))
            return SYS_ERR_INPUT;
        value = isdigit(pStr[i]) ? (pStr[i] - '0') : (toupper(pStr[i]) - 'A' + 10);
        if(value > 0xf)
            return SYS_ERR_INPUT;
        if(0 == (i % 2))
            value = value << 4;

        valArray[i/2] |= value;
    }

    return SYS_ERR_OK;
}
int32 sys_util_byteArray2hexStr(uint8 *valArray, uint16 size, char *pStr)
{
    uint32 i = 0;
    char valStr[2];

    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == valArray), SYS_ERR_NULL_POINTER);

    for(i = 0; i < size; i++)
    {
        osal_sprintf(valStr, "%02X", valArray[i]);

        osal_strcat(pStr, valStr);
    }

    return SYS_ERR_OK;
}
uint32 sys_util_str2ul_ret(const char *pStr)
{
    uint32 i = 0, ul = 0, value = 0, base = 10; 

    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);

    if('X' == toupper(pStr[1]))
    {
        base = 16;  
        i = 2;      
    }

    for(; i < osal_strlen(pStr); i++)
    {
        
        if (pStr[i] == ' ')
            break;

        value = isdigit(pStr[i]) ? (pStr[i] - '0') : (toupper(pStr[i]) - 'A' + 10);
        if(value >= base)
            return 0;

        ul = ul * base + value;
    }

    return ul;
}
int32 sys_util_str2ulMask(const char *str, uint32 *pUlmask)
{
    char *buf1 = NULL;
    char *buf2 = NULL;
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *strBuf = NULL;
    int32 ret = SYS_ERR_OK;
    int32 len1 = 0;
    int32 len2 = 0;
    uint32 val;
    uint32 valEnd;
    uint32 valStart;
    uint32 valTmp;
    uint32 maskTmp;

    SYS_PARAM_CHK((NULL == str), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pUlmask), SYS_ERR_NULL_POINTER);

    SYS_MEM_ALLOC_HDL(buf1, CAPA_VLANLIST_STR_LEN, _exit, ret);
    SYS_MEM_ALLOC_HDL(buf2, CAPA_VLANLIST_STR_LEN, _exit, ret);
    SYS_MEM_ALLOC_HDL(strBuf, CAPA_VLANLIST_STR_LEN, _exit, ret);

    osal_memset(&maskTmp, 0, sizeof(maskTmp));

    osal_strcpy(strBuf, str);
    ptr1 = strBuf;

    while (0 != osal_strlen(ptr1))
    {
        osal_memset(buf1, 0, CAPA_VLANLIST_STR_LEN);
        osal_memset(buf2, 0, CAPA_VLANLIST_STR_LEN);

        SYS_ERR_HDL(sys_util_getnext(ptr1, ',', buf1, &len1), _exit, ret);
        ptr2 = buf1;

        if (NULL != osal_strchr(ptr2, '-')) 
        {
            SYS_ERR_HDL(sys_util_getnext(ptr2, '-', buf2, &len2), _exit, ret);
            valStart = SYS_STR2UINT(buf2);

            if (valStart > 32 || valStart < 0)
            {
                ret = SYS_ERR_INPUT;
                goto _exit;
            }

            ptr2 += len2;
            SYS_ERR_HDL(sys_util_getnext(ptr2, '-', buf2, &len2), _exit, ret);
            valEnd = SYS_STR2UINT(buf2);

            if (valEnd > 32 || valEnd < 0)
            {
                ret = SYS_ERR_INPUT;
                goto _exit;
            }

            if (valEnd < valStart)
            {
                
                valTmp = valStart;
                valStart = valEnd;
                valEnd = valTmp;
            }

            for (val = valStart; val <= valEnd; val++)
            {
                maskTmp |= 1 << (val-1);
            }
        }
        else 
        {
            val = SYS_STR2UINT(buf1);

            if (val > 32 || val < 0)
            {
                ret = SYS_ERR_INPUT;
                goto _exit;
            }

            maskTmp |= 1 << (val-1);
        }

        ptr1 += len1;
    }

    *pUlmask = maskTmp;

_exit:
    SYS_MEM_FREE(buf1);
    SYS_MEM_FREE(buf2);
    SYS_MEM_FREE(strBuf);

    return ret;
}
int32 sys_util_ulMask2Str(uint32 ulmask, char *str)
{
    char strTmp[32];
    uint32 flag = 0;
    uint32 tmpValStart = 0;
    uint32 tmpValEnd = 0;
    uint32 tmpValCurr = 0;
    uint32 valNum = 0;
    uint32 val = 0;

    SYS_PARAM_CHK((NULL == str), SYS_ERR_NULL_POINTER);

    osal_memset(strTmp, 0, sizeof(strTmp));

    osal_strcpy(str, "");

    for(val = 0; val < 32; val ++)
        if(0 != (ulmask & (1 << val)))
        {
            tmpValCurr = val;
            valNum++;
            flag++;

            if (tmpValCurr == (tmpValEnd + 1))
            {
                tmpValEnd++;
                continue;
            }

            if ((0 == tmpValStart) && (0 == tmpValEnd))
            {
                tmpValStart = tmpValCurr;
                tmpValEnd = tmpValCurr;
                continue;
            }

            if (tmpValEnd == tmpValStart)
            {
                osal_memset(strTmp, 0, sizeof(strTmp));

                osal_sprintf(strTmp, "%d,", tmpValEnd + 1);
                osal_strcat(str, strTmp);

                tmpValStart = tmpValCurr;
                tmpValEnd = tmpValCurr;

                flag --;
            }
            else if (tmpValEnd > tmpValStart)
            {
                osal_memset(strTmp, 0, sizeof(strTmp));

                osal_sprintf(strTmp, "%d-%d,", tmpValStart + 1, tmpValEnd + 1);
                osal_strcat(str, strTmp);

                tmpValStart = tmpValCurr;
                tmpValEnd = tmpValCurr;

                flag -= ((tmpValEnd - tmpValStart) + 1);
            }
        }

    if (flag != 0)
    {
        if (tmpValEnd == tmpValStart)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));

            osal_sprintf(strTmp, "%d", tmpValEnd + 1);
            osal_strcat(str, strTmp);
        }
        else if (tmpValEnd > tmpValStart)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));

            osal_sprintf(strTmp, "%d-%d", tmpValStart + 1, tmpValEnd + 1);
            osal_strcat(str, strTmp);
        }
    }
    else if (0 != valNum)
    {
        str[osal_strlen(str) - 1] = '\0'; 
    }

    return SYS_ERR_OK;
}
int32 sys_util_namestrcpy(sys_namestr_t *pDest, const char *pSrc)
{
    SYS_PARAM_CHK((NULL == pDest), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pSrc), SYS_ERR_NULL_POINTER);

    if (osal_strlen(pSrc) > CAPA_NAME_STR_LEN)
    {
        osal_strncpy(pDest->str, pSrc, CAPA_NAME_STR_LEN);
        pDest->str[CAPA_NAME_STR_LEN] = '\0';

        return SYS_ERR_STR_LEN_EXCEED;
    }
    else
    {
        osal_strcpy(pDest->str, pSrc);

        return SYS_ERR_OK;
    }
}
int32 sys_util_bitmaskIsClear_ret(uint32 *maskAddr, uint32 bitIdx, uint32 bitNum)
{
    uint32 i;

    for (i = bitIdx; i < bitNum; i++)
    {
        if (SYS_BITMAP_IS_BITSET(maskAddr, i))
            return FALSE;
    }

    return TRUE;
}
int32 sys_util_bitmaskIsAllSet_ret(uint32 *maskAddr, uint32 bitIdx, uint32 bitNum)
{
    uint32 i;

    for (i = bitIdx; i < bitNum; i++)
    {
        if (SYS_BITMAP_IS_BITSET(maskAddr, i))
            continue;
        else
            return FALSE;
    }

    return TRUE;
}
int32 sys_util_bitmaskIsEqual_ret(uint32 *maskAddr1, uint32 *maskAddr2, uint32 bitNum)
{
    uint32 result = FALSE;

    SYS_BITMAP_IS_EQUAL(maskAddr1, maskAddr2, bitNum, result);

    return result;
}

