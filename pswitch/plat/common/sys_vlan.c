#include <rsd/rsd_vlan.h>

#include <common/sys_def.h>
#include <common/sys_vlan.h>

#ifdef __KERNEL__
#include <rtk/switch.h>
#else
#include <libcfg/cfg.h>
#endif
int32 sys_util_str2Vlanmask(const char *str, sys_vlanmask_t *pVlanmask)
{
    char *buf1 = NULL;
    char *buf2 = NULL;
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *strBuf = NULL;
    char vlanPrefix[SYS_BUF8_LEN] = "vlan";
    int32 ret = SYS_ERR_OK;
    int32 len1 = 0;
    int32 len2 = 0;
    uint32 i = 0;
    uint32 j = 0;
    sys_vid_t vid;
    sys_vid_t vidEnd;
    sys_vid_t vidStart;
    sys_vid_t vidTmp;
    sys_vlanmask_t vlanmaskTmp;

    SYS_PARAM_CHK((NULL == str), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    SYS_MEM_ALLOC_HDL(buf1, CAPA_VLANLIST_STR_LEN, _exit, ret);
    SYS_MEM_ALLOC_HDL(buf2, CAPA_VLANLIST_STR_LEN, _exit, ret);
    SYS_MEM_ALLOC_HDL(strBuf, CAPA_VLANLIST_STR_LEN, _exit, ret);

    osal_memset(&vlanmaskTmp, 0, sizeof(vlanmaskTmp));
    osal_memset(pVlanmask, 0, sizeof(sys_vlanmask_t));

    if (0 == osal_strlen(str))
        goto _exit;

    i = 0;
    for (j = 0; j < osal_strlen(str); j++)
    {
        if (' ' == str[j] || '\r' == str[j] || '\n' == str[j])
            continue;

        strBuf[i] = str[j];
        i++;
    }

    ptr1 = strBuf;

    i = 0;
    while(1)
    {
        if (*ptr1 < 0x30 || *ptr1 > 0x39)
        {
            if (i >= osal_strlen(vlanPrefix))
            {
                ret = SYS_ERR_VLAN_ID;
                goto _exit;
            }

            if (tolower(*ptr1) != vlanPrefix[i])
            {
                ret = SYS_ERR_VLAN_ID;
                goto _exit;
            }

            i++;
            ptr1++;

            continue;
        }

        break;
    }

    while (0 != osal_strlen(ptr1))
    {
        osal_memset(buf1, 0, CAPA_VLANLIST_STR_LEN);
        osal_memset(buf2, 0, CAPA_VLANLIST_STR_LEN);

        SYS_ERR_HDL(sys_util_getnext(ptr1, ',', buf1, &len1), _exit, ret);
        ptr2 = buf1;

        if (NULL != osal_strchr(ptr2, '-')) 
        {
            SYS_ERR_HDL(sys_util_getnext(ptr2, '-', buf2, &len2), _exit, ret);
            vidStart = SYS_STR2UINT(buf2);

            if (IS_VID_INVALID(vidStart))
            {
                ret = SYS_ERR_VLAN_ID;
                goto _exit;
            }

            ptr2 += len2;
            SYS_ERR_HDL(sys_util_getnext(ptr2, '-', buf2, &len2), _exit, ret);
            vidEnd = SYS_STR2UINT(buf2);

            if (IS_VID_INVALID(vidEnd))
            {
                ret = SYS_ERR_VLAN_ID;
                goto _exit;
            }

            if (vidEnd < vidStart)
            {
                
                vidTmp = vidStart;
                vidStart = vidEnd;
                vidEnd = vidTmp;
            }

            for (vid = vidStart; vid <= vidEnd; vid++)
            {
                VLANMASK_SET_VID(vlanmaskTmp, vid);
            }
        }
        else 
        {
            vid = SYS_STR2UINT(buf1);

            if (IS_VID_INVALID(vid))
            {
                ret = SYS_ERR_VLAN_ID;
                goto _exit;
            }

            VLANMASK_SET_VID(vlanmaskTmp, vid);
        }

        ptr1 += len1;
    }

    *pVlanmask = vlanmaskTmp;

_exit:
    SYS_MEM_FREE(buf1);
    SYS_MEM_FREE(buf2);
    SYS_MEM_FREE(strBuf);

    return ret;
}
int32 sys_util_vlanmask2Str(sys_vlanmask_t *pVlanmask, char *str)
{
    char strTmp[32];
    uint32 flag = 0;
    uint32 tmpVidStart = 0;
    uint32 tmpVidEnd = 0;
    uint32 tmpVidCurr = 0;
    uint32 vidNum = 0;
    sys_vid_t vid = 0;

    SYS_PARAM_CHK((NULL == str), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    osal_memset(strTmp, 0, sizeof(strTmp));

    osal_strcpy(str, "");

    FOR_EACH_VID_IN_VLANMASK(vid, *pVlanmask)
    {
        tmpVidCurr = vid;
        vidNum++;
        flag++;

        if ((0 == tmpVidStart) || (0 == tmpVidEnd))
        {
            tmpVidStart = tmpVidCurr;
            tmpVidEnd = tmpVidCurr;
            continue;
        }

        if (tmpVidCurr == (tmpVidEnd + 1))
        {
            tmpVidEnd++;
            continue;
        }

        if (tmpVidEnd == tmpVidStart)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));

            osal_sprintf(strTmp, "%d,", tmpVidEnd);
            osal_strcat(str, strTmp);

            tmpVidStart = tmpVidCurr;
            tmpVidEnd = tmpVidCurr;

            flag --;
        }
        else if (tmpVidEnd > tmpVidStart)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));

            osal_sprintf(strTmp, "%d-%d,", tmpVidStart, tmpVidEnd);
            osal_strcat(str, strTmp);

            tmpVidStart = tmpVidCurr;
            tmpVidEnd = tmpVidCurr;

            flag -= ((tmpVidEnd - tmpVidStart) + 1);
        }
    }

    if (flag != 0)
    {
        if (tmpVidEnd == tmpVidStart)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));

            osal_sprintf(strTmp, "%d", tmpVidEnd);
            osal_strcat(str, strTmp);
        }
        else if (tmpVidEnd > tmpVidStart)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));

            osal_sprintf(strTmp, "%d-%d", tmpVidStart, tmpVidEnd);
            osal_strcat(str, strTmp);
        }
    }
    else if (0 != vidNum)
    {
        str[osal_strlen(str) - 1] = '\0'; 
    }

    return SYS_ERR_OK;
}
int32 sys_util_vlanmask2Cnt(sys_vlanmask_t *pVlanmask, uint32 *pCount)
{
    uint32 vlanCount = 0;
    sys_vid_t vid = 0;

    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pCount), SYS_ERR_NULL_POINTER);

    FOR_EACH_VID_IN_VLANMASK(vid, *pVlanmask)
    {
        vlanCount += 1;
    }

    *pCount = vlanCount;

    return SYS_ERR_OK;
}

#ifdef __KERNEL__

#else 

int32 sys_vlan_init(void)
{
    uint32 maxEntryNum;
    maxEntryNum = 0;

    SYS_ERR_CHK(rsd_vlan_adminVlanMaxEntryNum_get(&maxEntryNum));
    SYS_ERR_CHK(cfg_vlan_adminVlanMaxEntryNum_set(maxEntryNum));

    return SYS_ERR_OK;
}
#endif

