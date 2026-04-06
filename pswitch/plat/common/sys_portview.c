#include <common/sys_def.h>

#include <rsd/rsd_sys.h>

#ifdef __KERNEL__
#include <board/board.h>
#include <rtk/port.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_port.h>
#endif

#ifndef __KERNEL__
#include <libcfg/cfg.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_port.h>
#endif

#include <common/sys_portview.h>
#define INVALID_VALUE 0xFFFF
#ifdef __KERNEL__
static uint32 physicPortviewInitFlag[SYS_DEV_NUM_MAX];
static sys_logic_port_t physic2logicPort[SYS_DEV_NUM_MAX][SYS_PORT_NUM_MAX];
static sys_physic_port_t logic2physicPort[SYS_LOGICPORT_NUM_MAX];

static sys_logic_portInfo_t logicPortInfo;
static sys_port_conf_t *pPortConf = NULL;

#endif
sys_portStr_t g_portStr[SYS_LOGICPORT_NUM_MAX];

static sys_port_prefixName_t portPrefixName[PORT_TYPE_END];
int32 _sys_port_portStrNumIndex_get(char *pStr, uint32 *pIndex)
{
    uint32 i;

    *pIndex = 0;

    for (i = 0; i < osal_strlen(pStr); i++)
    {
        if (pStr[i] >= 0x30 && pStr[i] <= 0x39)
        {
            *pIndex = i;

            break;
        }
    }

    return SYS_ERR_OK;
}

void _sys_port_prefixName_init(void)
{
    if (0 == osal_strcmp(portPrefixName[PORT_TYPE_100M].briefPrefix, DFLT_PORT_PREFIX_BRIEF_FA))
        return;

    SYS_MEM_CLEAR(portPrefixName);

    osal_strncpy(portPrefixName[PORT_TYPE_100M].briefPrefix, DFLT_PORT_PREFIX_BRIEF_FA, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_100M].fullPrefix, DFLT_PORT_PREFIX_FA, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_100M].webPrefix, DFLT_PORT_PREFIX_WEB_FA, CAPA_PORT_STR_LEN);

    osal_strncpy(portPrefixName[PORT_TYPE_1000M].briefPrefix, DFLT_PORT_PREFIX_BRIEF_GE, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_1000M].fullPrefix, DFLT_PORT_PREFIX_GE, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_1000M].webPrefix, DFLT_PORT_PREFIX_WEB_GE, CAPA_PORT_STR_LEN);
    
    osal_strncpy(portPrefixName[PORT_TYPE_2500M].briefPrefix, DFLT_PORT_PREFIX_BRIEF_MGE, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_2500M].fullPrefix, DFLT_PORT_PREFIX_MGE, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_2500M].webPrefix, DFLT_PORT_PREFIX_WEB_MGE, CAPA_PORT_STR_LEN);

    osal_strncpy(portPrefixName[PORT_TYPE_10G].briefPrefix, DFLT_PORT_PREFIX_BRIEF_TE, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_10G].fullPrefix, DFLT_PORT_PREFIX_TE, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_10G].webPrefix, DFLT_PORT_PREFIX_WEB_TE, CAPA_PORT_STR_LEN);

    osal_strncpy(portPrefixName[PORT_TYPE_LAG].briefPrefix, DFLT_PORT_PREFIX_BRIEF_LAG, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_LAG].fullPrefix, DFLT_PORT_PREFIX_LAG, CAPA_PORT_STR_LEN);
    osal_strncpy(portPrefixName[PORT_TYPE_LAG].webPrefix, DFLT_PORT_PREFIX_WEB_LAG, CAPA_PORT_STR_LEN);
}

#ifdef __KERNEL__

int32 sys_physic_portview_init(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i;
    uint32 devIdx;
    uint32 port;
    uint32 trunkId;
    sys_physic_port_t physicPort;
    sys_logic_port_t logicPort;
    sys_board_t *pBoardConf = NULL;

    sys_switch_devInfo_t *pDevInfo = NULL;
    sys_physic_portmask_t physicComboMask;

    SYS_MEM_CLEAR(physicComboMask);

    SYS_ERR_CHK(board_conf_get(&pBoardConf));

    pPortConf = (sys_port_conf_t *) pBoardConf->pPortConf;

    if (pPortConf == NULL)
        return SYS_ERR_FAILED;

    SYS_MEM_ALLOC_HDL(pDevInfo, sizeof(sys_switch_devInfo_t), _exit, ret);

    logicPort = 0;
    FOR_EACH_DEV(devIdx)
    {
        SYS_ERR_HDL(rsd_sys_devInfo_get(devIdx, pDevInfo), _exit, ret);
        osal_memcpy(&physicComboMask.portmask[devIdx], &pDevInfo->combo.portmask,
                    sizeof(physicComboMask.portmask[devIdx]));

        for(port = 0; port < SYS_PORT_NUM_MAX; port++)
        {
            
            physic2logicPort[devIdx][port] = INVALID_VALUE;
            logic2physicPort[logicPort].dev = INVALID_VALUE;
            logic2physicPort[logicPort].port = INVALID_VALUE;
            logicPort++;
        }
    }

    osal_memset(&logicPortInfo, 0, sizeof(sys_logic_portInfo_t));

    for (i = 0; i < PORT_TYPE_END; i++)
    {
        logicPortInfo.portTypeStartPort[i] = INVALID_VALUE;
        logicPortInfo.portNum[i] = 0;
    }

    logicPort = 0;
    while (pPortConf[logicPort].devId != PORTCONF_END_VAL)
    {
        physicPort.dev = pPortConf[logicPort].devId;
        physicPort.port = pPortConf[logicPort].phyPort;
        physic2logicPort[physicPort.dev][physicPort.port] = logicPort;
        logic2physicPort[logicPort] = physicPort;

        if (PHYSIC_PORTMASK_IS_PORTSET(physicComboMask, physicPort))
        {
            rtk_port_phyComboPortMedia_set(physicPort.dev, physicPort.port, pPortConf[logicPort].mediaType);
        }

        logicPortInfo.userPort[logicPort] = pPortConf[logicPort].usrPort;

        if ((pPortConf[logicPort].portType > PORT_TYPE_LAG)
                && (pPortConf[logicPort].portType < PORT_TYPE_END))
            logicPortInfo.portType[logicPort] = pPortConf[logicPort].portType - PORT_TYPE_LAG - 1;
        else
            logicPortInfo.portType[logicPort] = pPortConf[logicPort].portType;

        logicPortInfo.portNum[logicPortInfo.portType[logicPort]]++;
        
        if (INVALID_VALUE == logicPortInfo.portTypeStartPort[logicPortInfo.portType[logicPort]])
        {
            logicPortInfo.portTypeStartPort[logicPortInfo.portType[logicPort]] = logicPort;
        }

        logicPort++;
    }

    FOR_EACH_TRUNK(trunkId)
    {
        logicPort = TRK2LP(trunkId);

        logicPortInfo.userPort[logicPort] = (SYS_PORT_TYPE_LAG_IDNUM_BASE + trunkId);
        
        logicPortInfo.portType[logicPort] = PORT_TYPE_LAG;
        
        logicPortInfo.portNum[PORT_TYPE_LAG]++;
        
        if (INVALID_VALUE == logicPortInfo.portTypeStartPort[PORT_TYPE_LAG])
        {
            logicPortInfo.portTypeStartPort[PORT_TYPE_LAG] = logicPort;
        }
    }

    FOR_EACH_DEV(devIdx)
    {
        physicPortviewInitFlag[devIdx] = INIT_COMPLETED;
    }

    SYS_MEM_CLEAR(g_portStr);

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        LP2FULLSTR(logicPort, g_portStr[logicPort].str);
    }

_exit:
    SYS_MEM_FREE(pDevInfo);

    return ret;
}
int32 sys_board_portConf_get(sys_port_conf_t **ppPortConf)
{
    if (pPortConf == NULL)
        return SYS_ERR_FAILED;

    *ppPortConf = pPortConf;
    return SYS_ERR_OK;
}
int32 sys_logic_to_physic_port(
    sys_logic_port_t port,
    sys_physic_port_t *pPhysicPort)
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPhysicPort), SYS_ERR_NULL_POINTER);

    SYS_INIT_CHK(physicPortviewInitFlag[SYS_DEV_ID_DEFAULT]);

    if (IS_TRKLP(port))
    {
        osal_memset(&logicPortmask, 0, sizeof(logicPortmask));

        if(IS_TRKLP_STATIC(port))
        {
            LP2TRKMBRLPM(port, &logicPortmask);
        }
        else
        {
            LP2TRKACTMBRLPM(port, &logicPortmask);

            if (IS_LOGIC_PORTMASK_CLEAR(logicPortmask))
                LP2TRKMBRLPM(port, &logicPortmask);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(logicPortmask))
        {
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
            {
                pPhysicPort->dev = logic2physicPort[logicPort].dev;
                pPhysicPort->port = logic2physicPort[logicPort].port;

                break;
            }
        }
        else
        {
            return SYS_ERR_TRUNK_EMPTY;
        }
    }
    else if (IS_CPU_LP(port))
    {
        pPhysicPort->dev = SYS_DEV_ID_DEFAULT;
        pPhysicPort->port = PHYSIC_CPU_PORT(SYS_DEV_ID_DEFAULT);
    }
    else
    {
        pPhysicPort->dev = logic2physicPort[port].dev;
        pPhysicPort->port = logic2physicPort[port].port;
    }

    return SYS_ERR_OK;
}
int32 sys_physic_to_logic_port(
    sys_physic_port_t physicPort,
    sys_logic_port_t *pLogicPort)
{
    
    SYS_PARAM_CHK(IS_DEV_INVALID(physicPort.dev), SYS_ERR_DEV_ID);
    SYS_PARAM_CHK(IS_PHYSIC_PORT_INVALID(physicPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLogicPort), SYS_ERR_NULL_POINTER);

    SYS_INIT_CHK(physicPortviewInitFlag[physicPort.dev]);

    *pLogicPort = physic2logicPort[physicPort.dev][physicPort.port];

    return SYS_ERR_OK;
}
int32 sys_physic_to_logic_portmask(
    sys_physic_portmask_t *pPhysicPortmask,
    sys_logic_portmask_t *pLogicPortmask)
{
    uint32 devIdx;
    sys_physic_port_t physic_port;
    sys_logic_port_t logic_port;

    SYS_PARAM_CHK((NULL == pPhysicPortmask), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pLogicPortmask), SYS_ERR_NULL_POINTER);
    for(devIdx = 0; devIdx < SYS_DEV_NUM; devIdx ++)
    {
        SYS_INIT_CHK(physicPortviewInitFlag[devIdx]);
    }

    osal_memset(pLogicPortmask, 0, sizeof(*pLogicPortmask));

    FOR_EACH_PORT_IN_PHYSIC_PORTMASK(physic_port, *pPhysicPortmask)
    {
       logic_port = physic2logicPort[physic_port.dev][physic_port.port];

       LOGIC_PORTMASK_SET_PORT(*pLogicPortmask, logic_port);
    }

    return SYS_ERR_OK;
}
int32 sys_logic_to_physic_portmask(
    sys_logic_portmask_t *pLogicPortmask,
    sys_physic_portmask_t *pPhysicPortmask)
{
    sys_logic_port_t logic_port;
    sys_logic_port_t trkMbrPort;
    sys_logic_portmask_t trkMbrPm;
    sys_physic_port_t physic_port;

    SYS_PARAM_CHK((NULL == pPhysicPortmask), SYS_ERR_NULL_POINTER);

    osal_memset(pPhysicPortmask, 0, sizeof(*pPhysicPortmask));

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logic_port, *pLogicPortmask)
    {
        if (IS_TRKLP(logic_port))
        {
            osal_memset(&trkMbrPm, 0, sizeof(trkMbrPm));

            LP2TRKMBRLPM(logic_port, &trkMbrPm);

            if (!IS_LOGIC_PORTMASK_CLEAR(trkMbrPm))
            {
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(trkMbrPort, trkMbrPm)
                {
                    osal_memset(&physic_port, 0, sizeof(physic_port));

                    sys_logic_to_physic_port(trkMbrPort, &physic_port);

                    PHYSIC_PORTMASK_SET_PORT(*pPhysicPortmask, physic_port);
                }
            }

            continue;
        }

        osal_memset(&physic_port, 0, sizeof(physic_port));

        sys_logic_to_physic_port(logic_port, &physic_port);

        PHYSIC_PORTMASK_SET_PORT(*pPhysicPortmask, physic_port);
    }

    return SYS_ERR_OK;
}
int32 sys_logic_port_to_physic_portmask(
    sys_logic_port_t port,
    sys_physic_portmask_t *pPhysicPortmask)
{
    sys_logic_port_t logicPort;
    sys_physic_port_t phyPort;
    sys_logic_portmask_t logicPortmask;
    
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPhysicPortmask), SYS_ERR_NULL_POINTER);

    osal_memset(pPhysicPortmask, 0, sizeof(sys_physic_portmask_t));

    if (IS_TRKLP(port))
    {
        osal_memset(&logicPortmask, 0, sizeof(logicPortmask));

        LP2TRKMBRLPM(port, &logicPortmask);

        if (!IS_LOGIC_PORTMASK_CLEAR(logicPortmask))
        {
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
            {
                osal_memset(&phyPort, 0, sizeof(phyPort));

                sys_logic_to_physic_port(logicPort, &phyPort);

                PHYSIC_PORTMASK_SET_PORT(*pPhysicPortmask, phyPort);
            }
        }
    }
    else if (IS_LP_TRKMBR(port))
    {
    	osal_memset(&phyPort, 0, sizeof(phyPort));
        sys_logic_to_physic_port(port, &phyPort);
        PHYSIC_PORTMASK_SET_PORT(*pPhysicPortmask, phyPort);
        return SYS_ERR_TRUNK_MEMBER_SKIP;
    }
    else
    {
        osal_memset(&phyPort, 0, sizeof(phyPort));

        sys_logic_to_physic_port(port, &phyPort);

        PHYSIC_PORTMASK_SET_PORT(*pPhysicPortmask, phyPort);
    }

    return SYS_ERR_OK;
}
sys_logic_port_t sys_port_lPortmaskLowestPort_ret(sys_logic_portmask_t lPortmask)
{
    sys_logic_port_t logicPort = SYS_PORT_INVALID_ID;
    sys_physic_port_t physicPort;
    sys_physic_portmask_t physicPortmask;

    if (IS_LOGIC_PORTMASK_CLEAR(lPortmask))
        return SYS_PORT_INVALID_ID;

    osal_memset(&physicPortmask, 0, sizeof(physicPortmask));

    sys_logic_to_physic_portmask(&lPortmask, &physicPortmask);

    FOR_EACH_PORT_IN_PHYSIC_PORTMASK(physicPort, physicPortmask)
    {
        sys_physic_to_logic_port(physicPort, &logicPort);

        return logicPort;
    }

    return logicPort;
}
#endif

#ifndef __KERNEL__

int32 sys_user_portview_init(uint32 dev)
{
    uint32 i;
    sys_logic_port_t logicPort;
    cfg_devInfo_t devInfo;
    sys_port_conf_t portConf;
    _sys_port_prefixName_init();

    cfg_sys_devInfo_get(&devInfo);

    SYS_PARAM_CHK((dev >= SYS_DEV_NUM_MAX), SYS_ERR_DEV_ID);

    for (i = 0; i < PORT_TYPE_END; i++)
    {
        devInfo.logicPortInfo.portTypeStartPort[i] = INVALID_VALUE;
        devInfo.logicPortInfo.portNum[i] = 0;
    }

    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        osal_memset(&portConf, 0, sizeof(portConf));
        sal_sys_boardPortConf_get(logicPort, &portConf);

        devInfo.logicPortInfo.userPort[logicPort] = portConf.usrPort;

        if ((portConf.portType > PORT_TYPE_LAG)
                && (portConf.portType < PORT_TYPE_END))
        {
            devInfo.logicPortInfo.portType[logicPort] = portConf.portType - PORT_TYPE_LAG - 1;
        }
        else
        {
            devInfo.logicPortInfo.portType[logicPort] = portConf.portType;
        }

        devInfo.logicPortInfo.portNum[devInfo.logicPortInfo.portType[logicPort]]++;

        if (INVALID_VALUE == devInfo.logicPortInfo.portTypeStartPort[devInfo.logicPortInfo.portType[logicPort]])
        {
            devInfo.logicPortInfo.portTypeStartPort[devInfo.logicPortInfo.portType[logicPort]] = logicPort;
        }

        if ((1 << PORT_ATTR_POE) & portConf.attr)
            devInfo.logicPortInfo.isPoePort[logicPort] = TRUE;
        else
            devInfo.logicPortInfo.isPoePort[logicPort] = FALSE;

        if ((1 << PORT_ATTR_COMBO_FIBER) & portConf.attr)
            devInfo.logicPortInfo.isComboFiberPort[logicPort] = TRUE;
        else
            devInfo.logicPortInfo.isComboFiberPort[logicPort] = FALSE;
    }

    FOR_EACH_TRUNK(i)
    {
        logicPort = TRK2LP(i);

        devInfo.logicPortInfo.userPort[logicPort] = (SYS_PORT_TYPE_LAG_IDNUM_BASE + i);

        devInfo.logicPortInfo.portType[logicPort] = PORT_TYPE_LAG;

        devInfo.logicPortInfo.portNum[PORT_TYPE_LAG]++;

        if (INVALID_VALUE == devInfo.logicPortInfo.portTypeStartPort[PORT_TYPE_LAG])
        {
            devInfo.logicPortInfo.portTypeStartPort[PORT_TYPE_LAG] = logicPort;
        }
    }

    devInfo.userPortviewInitFlag[dev] = INIT_COMPLETED;

    cfg_sys_devInfo_set(&devInfo);

    return SYS_ERR_OK;
}
int32 sys_port_uStr2lPortmask(const char *str, sys_logic_portmask_t *pLPortmask)
{
    char buf1[CAPA_PORTMASK_STR_LEN];
    char buf2[CAPA_PORTMASK_STR_LEN];
    char portPrefix[CAPA_PORT_STR_LEN];
    char strPort[CAPA_PORT_STR_LEN];
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char strBuf[CAPA_PORTMASK_STR_LEN];
    int32 len1 = 0;
    int32 len2 = 0;
    uint32 index = 0;
    uint32 i = 0;
    uint32 j = 0;
    sys_logic_port_t port;
    sys_logic_port_t portEnd;
    sys_logic_port_t portStart;
    sys_logic_port_t portTmp;
    sys_logic_portmask_t portmaskTmp;

    SYS_PARAM_CHK((NULL == pLPortmask), SYS_ERR_NULL_POINTER);

    osal_memset(strBuf, 0, sizeof(strBuf));
    osal_memset(&portmaskTmp, 0, sizeof(portmaskTmp));
    osal_memset(pLPortmask, 0, sizeof(sys_logic_portmask_t));
    SYS_MEM_CLEAR(portPrefix);

    if (0 == osal_strcmp(str, "all"))
    {
        LOGIC_PORTMASK_SET_ALL(*pLPortmask);

        return SYS_ERR_OK;
    }

    i = 0;
    for (j = 0; j < osal_strlen(str); j++)
    {
        if (' ' == str[j])
            continue;

        strBuf[i] = str[j];
        i++;
    }

    ptr1 = strBuf;

    while (0 != osal_strlen(ptr1))
    {
        osal_memset(buf1, 0, sizeof(buf1));
        osal_memset(buf2, 0, sizeof(buf2));

        SYS_ERR_CHK(sys_util_getnext(ptr1, ',', buf1, &len1));
        ptr2 = buf1;

        index = 0;
        _sys_port_portStrNumIndex_get(ptr2, &index);

        if (0 != index)
        {
            SYS_MEM_CLEAR(portPrefix);
            osal_strncpy(portPrefix, ptr2, index);
            ptr2 += osal_strlen(portPrefix);
        }

        if ((NULL != osal_strchr(ptr2, '-')))
        {
            SYS_ERR_CHK(sys_util_getnext(ptr2, '-', buf2, &len2));

            SYS_MEM_CLEAR(strPort);
            osal_sprintf(strPort, "%s%s", portPrefix, buf2);
            SYS_ERR_CHK(STR2LP(strPort, &portStart));

            if (IS_LOGIC_PORT_INVALID(portStart))
                return SYS_ERR_PORT_RANGE;

            ptr2 += len2;
            SYS_MEM_CLEAR(buf2);
            SYS_ERR_CHK(sys_util_getnext(ptr2, '-', buf2, &len2));

            if (buf2[0] >= '0' && buf2[0] <= '9')
            {
                
                SYS_MEM_CLEAR(strPort);
                osal_sprintf(strPort, "%s%s", portPrefix, buf2);
                SYS_ERR_CHK(STR2LP(strPort, &portEnd));
            }
            else
            {
                SYS_ERR_CHK(STR2LP(buf2, &portEnd));
            }

            if (IS_LOGIC_PORT_INVALID(portEnd))
                return SYS_ERR_PORT_RANGE;

            if (portEnd < portStart)
            {
                
                portTmp = portStart;
                portStart = portEnd;
                portEnd = portTmp;
            }

            for (port = portStart; port <= portEnd; port++)
            {
                LOGIC_PORTMASK_SET_PORT(portmaskTmp, port);
            }
        }
        
        else
        {
            
            if (0 == index)
            {
                
                SYS_MEM_CLEAR(strPort);
                osal_sprintf(strPort, "%s%s", portPrefix, buf1);
                SYS_ERR_CHK(STR2LP(strPort, &port));
            }
            
            else
            {
                SYS_ERR_CHK(STR2LP(buf1, &port));
            }

            if (IS_LOGIC_PORT_INVALID(port))
                return SYS_ERR_PORT_RANGE;

            LOGIC_PORTMASK_SET_PORT(portmaskTmp, port);
        }

        ptr1 += len1;
    }

    *pLPortmask = portmaskTmp;

    return SYS_ERR_OK;
}
#endif 
int32 sys_port_uStr2lPort(char *str, sys_logic_port_t *pLPort)
{
    char strBuf[CAPA_PORTMASK_STR_LEN];
    uint32 i;
    uint32 j;
    uint32 k;
    uint32 cnt;
    uint32 prefixNum;
    sys_logic_portInfo_t *pLogicPortInfo = NULL;
    sys_user_port_t userPort;

#ifdef __KERNEL__
    pLogicPortInfo = &logicPortInfo;
#else
    cfg_devInfo_t devInfo;

    osal_memset(&devInfo, 0, sizeof(devInfo));
    cfg_sys_devInfo_get(&devInfo);

    pLogicPortInfo = &devInfo.logicPortInfo;
#endif
    SYS_PARAM_CHK((NULL == str), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pLPort), SYS_ERR_NULL_POINTER);

    prefixNum = 0;
    userPort = 0;

    SYS_MEM_CLEAR(strBuf);

    i = 0;
    for (j = 0; j < osal_strlen(str); j++)
    {
        if (' ' == str[j] || '\r' == str[j] || '\n' == str[j])
            continue;

        strBuf[i] = str[j];
        i++;
    }

    _sys_port_portStrNumIndex_get(strBuf, &i);
    prefixNum = i;

    userPort = SYS_STR2UINT(strBuf + i);

    if (userPort == 0)
        return SYS_ERR_PORT_STR_FMT;

    _sys_port_prefixName_init();

    for (i = 0; i < PORT_TYPE_END; i++)
    {
        for (j = 0; j < SYS_PORT_STRFMT_END; j++)
        {
            switch (j)
            {
                case SYS_PORT_STRFMT_BRIEF:
                    if (prefixNum <= osal_strlen(portPrefixName[i].briefPrefix))
                    {
                        if (0 == osal_memcmp(strBuf,
                                              portPrefixName[i].briefPrefix,
                                              osal_strlen(portPrefixName[i].briefPrefix)))
                           goto match;
                    }
                    break;

                case SYS_PORT_STRFMT_FULL:
                    if (prefixNum <= osal_strlen(portPrefixName[i].fullPrefix))
                    {
                        cnt = 0;
                        for (k = 0; k < prefixNum; k++)
                        {
                            if (tolower(strBuf[k]) != tolower(portPrefixName[i].fullPrefix[k]))
                                cnt++;
                        }

                        if (0 == cnt)
                            goto match;
                    }
                    break;

                case SYS_PORT_STRFMT_WEB:
                    if (prefixNum <= osal_strlen(portPrefixName[i].webPrefix))
                    {
                        if (0 == osal_memcmp(strBuf,
                                             portPrefixName[i].webPrefix,
                                             osal_strlen(portPrefixName[i].webPrefix)))
                            goto match;
                    }
                    break;

                default:
                    break;
            }
        }
    }
    return SYS_ERR_FAILED;

match:
    if (((userPort - 1) >= pLogicPortInfo->portNum[i]))
        return SYS_ERR_PORT_RANGE;

    *pLPort = (userPort - 1 + pLogicPortInfo->portTypeStartPort[i]);

    return SYS_ERR_OK;
}
int32 sys_port_lPort2uStr(sys_logic_port_t logicPort, sys_port_strFormat_t fmt, char *str)
{

    sys_logic_portInfo_t *pLogicPortInfo = NULL;
    sys_user_port_t userPort;
    sys_port_type_t portType;

#ifdef __KERNEL__
    pLogicPortInfo = &logicPortInfo;
#else
    cfg_devInfo_t devInfo;

    osal_memset(&devInfo, 0, sizeof(devInfo));
    cfg_sys_devInfo_get(&devInfo);

    pLogicPortInfo = &devInfo.logicPortInfo;
#endif

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == str), SYS_ERR_NULL_POINTER);

    portType = pLogicPortInfo->portType[logicPort];

    userPort = ((logicPort - pLogicPortInfo->portTypeStartPort[portType]) + 1);

    _sys_port_prefixName_init();

    switch (fmt)
    {
        
        case SYS_PORT_STRFMT_BRIEF:
            osal_sprintf(str, "%s%d",
                         portPrefixName[portType].briefPrefix,
                         userPort);
            break;

        case SYS_PORT_STRFMT_FULL:
            osal_sprintf(str, "%s%d",
                         portPrefixName[portType].fullPrefix,
                         userPort);
            break;

        case SYS_PORT_STRFMT_WEB:
            osal_sprintf(str, "%s%d",
                         portPrefixName[portType].webPrefix,
                         userPort);
            break;

        default:
            break;
    }
    return SYS_ERR_OK;
}
char *sys_port_lPortStr_ret(sys_logic_port_t port)
{
#ifndef __KERNEL__
    sys_logic_port_t lp;

    if (0 == strlen(g_portStr[0].str))
    {
        SYS_MEM_CLEAR(g_portStr);

        FOR_EACH_LOGIC_PORT(lp)
        {
            LP2FULLSTR(lp, g_portStr[lp].str);
        }
    }
#endif

    return g_portStr[port].str;
}
sys_user_port_t sys_port_lport2uport_ret(sys_logic_port_t logicPort)
{
#ifdef __KERNEL__
    return logicPortInfo.userPort[logicPort];
#else
    cfg_devInfo_t devInfo;

    osal_memset(&devInfo, 0, sizeof(devInfo));

    cfg_sys_devInfo_get(&devInfo);

    return devInfo.logicPortInfo.userPort[logicPort];
#endif
}
sys_logic_port_t sys_port_uport2lport_ret(sys_user_port_t userPort)
{
    sys_logic_port_t logicPort;
#ifdef __KERNEL__
    FOR_EACH_LOGIC_PORT(logicPort)
    {
        if (userPort == logicPortInfo.userPort[logicPort])
            return logicPort;
    }
#else
    cfg_devInfo_t devInfo;

    osal_memset(&devInfo, 0, sizeof(devInfo));

    cfg_sys_devInfo_get(&devInfo);

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        if (userPort == devInfo.logicPortInfo.userPort[logicPort])
            return logicPort;
    }
#endif

    return logicPort;
}
sys_port_type_t sys_port_lPort2Type_ret(sys_logic_port_t logicPort)
{
#ifdef __KERNEL__
    return logicPortInfo.portType[logicPort];
#else
    cfg_devInfo_t devInfo;

    osal_memset(&devInfo, 0, sizeof(devInfo));

    cfg_sys_devInfo_get(&devInfo);

    return devInfo.logicPortInfo.portType[logicPort];
#endif
}
uint32 sys_port_lPortTypeNum_ret(sys_port_type_t type)
{
    uint32 num = 0;
    sys_logic_port_t lp = 0;

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (type == LP2TYPE(lp))
            num++;
    }

    return num;
}
sys_port_mediaType_t sys_port_lPortMedia_ret(sys_logic_port_t logicPort)
{
    sys_port_mediaType_t media = SYS_PORT_MEDIA_COPPER;
    sys_logic_port_t trkMbrPort;
    sys_logic_portmask_t trkMbrPm;

#ifdef __KERNEL__
    if (IS_TRKLP(logicPort))
    {
        osal_memset(&trkMbrPm, 0, sizeof(trkMbrPm));

        LP2TRKMBRLPM(logicPort, &trkMbrPm);

        if (!IS_LOGIC_PORTMASK_CLEAR(trkMbrPm))
        {
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(trkMbrPort, trkMbrPm)
            {
                logicPort = trkMbrPort;
                break;
            }
        }
        else
            return media;
    }

    if ((pPortConf[logicPort].portType > PORT_TYPE_LAG)
           && (pPortConf[logicPort].portType < PORT_TYPE_END))
    {
        media = SYS_PORT_MEDIA_COMBO;
    }
    else if (PORT_MEDIA_COPPER == pPortConf[logicPort].mediaType)
    {
        media = SYS_PORT_MEDIA_COPPER;
    }
    else
    {
        media = SYS_PORT_MEDIA_FIBER;
    }

#else
    sys_port_conf_t portConf;

    SYS_MEM_CLEAR(portConf);

    if (IS_TRKLP(logicPort))
    {
        osal_memset(&trkMbrPm, 0, sizeof(trkMbrPm));

        LP2TRKMBRLPM(logicPort, &trkMbrPm);

        if (!IS_LOGIC_PORTMASK_CLEAR(trkMbrPm))
        {
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(trkMbrPort, trkMbrPm)
            {
                logicPort = trkMbrPort;
                break;
            }
        }
        else
            return media;
    }

    sal_sys_boardPortConf_get(logicPort, &portConf);

    if ((portConf.portType > PORT_TYPE_LAG)
            && (portConf.portType < PORT_TYPE_END))
    {
        media = SYS_PORT_MEDIA_COMBO;
    }
    else if (PORT_MEDIA_COPPER == portConf.mediaType)
    {
        media = SYS_PORT_MEDIA_COPPER;
    }
    else
    {
        media = SYS_PORT_MEDIA_FIBER;
    }

#endif

    return media;
}
uint8 sys_port_isLPortPoe_ret(sys_logic_port_t logicPort)
{
#ifdef __KERNEL__
    if (IS_TRKLP(logicPort))
        return FALSE;

    if ((1 << PORT_ATTR_POE) & pPortConf[logicPort].attr)
        return TRUE;
    else
        return FALSE;
#else
    cfg_devInfo_t devInfo;

    if (IS_TRKLP(logicPort))
        return FALSE;

    osal_memset(&devInfo, 0, sizeof(devInfo));

    cfg_sys_devInfo_get(&devInfo);

    return devInfo.logicPortInfo.isPoePort[logicPort];
#endif
}
uint8 sys_port_isLPortComboFiberMode_ret(sys_logic_port_t logicPort)
{
#ifdef __KERNEL__
    if (IS_TRKLP(logicPort))
        return FALSE;

    if ((1 << PORT_ATTR_COMBO_FIBER) & pPortConf[logicPort].attr)
        return TRUE;
    else
        return FALSE;
#else
    cfg_devInfo_t devInfo;

    if (IS_TRKLP(logicPort))
        return FALSE;

    osal_memset(&devInfo, 0, sizeof(devInfo));

    cfg_sys_devInfo_get(&devInfo);

    return devInfo.logicPortInfo.isComboFiberPort[logicPort];
#endif
}
static int32 _sys_port_lPortmask2uStr(
    sys_logic_portmask_t *pLPortmask,
    sys_port_strFormat_t fmt,
    char *str)
{
    char strTmp[32];
    char strPortStart[CAPA_PORT_STR_LEN];
    char strPortEnd[CAPA_PORT_STR_LEN];
    uint32 noProcessPort = 0;
    uint32 portNum = 0;
    uint32 index = 0;
    int32 startPort = -1;
    int32 endPort = -1;
    int32 currPort = -1;
    sys_port_type_t currType;
    sys_logic_port_t port;

    SYS_PARAM_CHK((NULL == pLPortmask), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == str), SYS_ERR_NULL_POINTER);

    osal_strcpy(str, "");

    currType = PORT_TYPE_END;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, *pLPortmask)
    {
        noProcessPort++;
        portNum++;
        currPort = port;

        if (currType == PORT_TYPE_END)
            currType = LP2TYPE(currPort);

        if ((-1 == startPort) || (-1 == endPort))
        {
            startPort = currPort;
            endPort = currPort;
            continue;
        }

        if ((currPort == (endPort + 1))
                && (currType == LP2TYPE(endPort))
                && (currType == LP2TYPE(currPort)))
        {
            endPort++;
            continue;
        }

        if (endPort == startPort)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));
            osal_memset(strPortStart, 0, sizeof(strPortStart));

            sys_port_lPort2uStr(endPort, fmt, strPortStart);
            osal_sprintf(strTmp, "%s,", strPortStart);
            osal_strcat(str, strTmp);

            startPort = currPort;
            endPort = currPort;
            noProcessPort--;

            currType = PORT_TYPE_END;
        }
        else if (endPort > startPort)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));
            osal_memset(strPortStart, 0, sizeof(strPortStart));
            osal_memset(strPortEnd, 0, sizeof(strPortEnd));

            sys_port_lPort2uStr(startPort, fmt, strPortStart);
            
            sys_port_lPort2uStr(endPort, fmt, strPortEnd);

            index = 0;
            if (fmt != SYS_PORT_STRFMT_WEB)
            {
                
                _sys_port_portStrNumIndex_get(strPortEnd, &index);
            }

            osal_strcpy(strPortEnd, strPortEnd + index);

            osal_sprintf(strTmp, "%s-%s,", strPortStart, strPortEnd);

            osal_strcat(str, strTmp);

            startPort = currPort;
            endPort = currPort;

            noProcessPort -= ((endPort - startPort) + 1);

            currType = PORT_TYPE_END;
        }
    }

    if (0 != noProcessPort)
    {
        if (endPort == startPort)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));

            osal_memset(strPortStart, 0, sizeof(strPortStart));

            sys_port_lPort2uStr(endPort, fmt, strPortStart);
            osal_sprintf(strTmp, "%s,", strPortStart);

            osal_strcat(str, strTmp);
        }
        else if (endPort > startPort)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));

            osal_memset(strPortStart, 0, sizeof(strPortStart));
            osal_memset(strPortEnd, 0, sizeof(strPortEnd));

            sys_port_lPort2uStr(startPort, fmt, strPortStart);
            
            sys_port_lPort2uStr(endPort, fmt, strPortEnd);

            index = 0;
            if (fmt != SYS_PORT_STRFMT_WEB)
            {
                
                _sys_port_portStrNumIndex_get(strPortEnd, &index);
            }

            osal_strcpy(strPortEnd, strPortEnd + index);

            osal_sprintf(strTmp, "%s-%s,", strPortStart, strPortEnd);

            osal_strcat(str, strTmp);
        }
    }

    if (0 != portNum)
    {
        
        str[osal_strlen(str) - 1] = '\0';
    }

    return SYS_ERR_OK;
}
int32 sys_port_lPortmask2uStr(
    sys_logic_portmask_t *pLPortmask,
    sys_port_strFormat_t fmt,
    uint32               useAllKey,
    char                 *str)
{
    sys_logic_portmask_t logicPortmaskTmp = *pLPortmask;

    SYS_PARAM_CHK((NULL == str), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pLPortmask), SYS_ERR_NULL_POINTER);

    if (TRUE == useAllKey)
    {
        if (IS_LOGIC_PORTMASK_ALLSET(logicPortmaskTmp))
        {

            osal_strcpy(str, "all");

            return SYS_ERR_OK;
        }
    }

    SYS_ERR_CHK(_sys_port_lPortmask2uStr(pLPortmask, fmt, str));

    return SYS_ERR_OK;

}
int32 sys_port_lPortmask2UPortmask(
    sys_logic_portmask_t *pLPortmask,
    sys_user_portmask_t *pUPortmask)
{
    sys_logic_port_t logicPort;

    SYS_PARAM_CHK((NULL == pLPortmask), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pUPortmask), SYS_ERR_NULL_POINTER);

    osal_memset(pUPortmask, 0, sizeof(sys_user_portmask_t));

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, *pLPortmask)
    {
        
        USER_PORTMASK_SET_PORT(*pUPortmask, LP2UP(logicPort));
    }

    return SYS_ERR_OK;
}
int32 sys_port_uPortmask2LPortmask(
    sys_user_portmask_t *pUPortmask,
    sys_logic_portmask_t *pLPortmask)
{
    sys_user_port_t userPort;

    SYS_PARAM_CHK((NULL == pLPortmask), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pUPortmask), SYS_ERR_NULL_POINTER);

    osal_memset(pLPortmask, 0, sizeof(sys_logic_portmask_t));

    FOR_EACH_PORT_IN_USER_PORTMASK(userPort, *pUPortmask)
    {
        LOGIC_PORTMASK_SET_PORT(*pLPortmask, UP2LP(userPort));
    }

    return SYS_ERR_OK;
}
uint32 sys_port_linkStatus_ret(sys_logic_port_t port)
{
    sys_port_linkStatus_t link = PORT_LINKDOWN;

#ifdef __KERNEL__
    rsd_port_linkStatus_get(port, &link);
#else
    sal_port_linkStatus_get(port, &link);
#endif

    if (PORT_LINKUP == link)
        return TRUE;
    else
        return FALSE;
}
int32 sys_trunk_member2TrunklogicPortmask_get(
    sys_logic_portmask_t *pSrcPortmask,
    sys_logic_portmask_t *pDstPortmask)
{
    sys_logic_port_t logicPort;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, *pSrcPortmask)
    {
        if (IS_LP_TRKMBR(logicPort))
        {
            if (NULL != pDstPortmask)
            {
                
                LOGIC_PORTMASK_SET_PORT(*pDstPortmask, LP2TRKLP(logicPort));
                LOGIC_PORTMASK_CLEAR_PORT(*pDstPortmask, logicPort);
            }
            else
            {
                LOGIC_PORTMASK_CLEAR_PORT(*pSrcPortmask, logicPort);
            }
        }
        else
        {
            if (NULL != pDstPortmask)
                LOGIC_PORTMASK_SET_PORT(*pDstPortmask, logicPort);
        }
    }

    return SYS_ERR_OK;
}
int32 sys_trunk_member_get(uint32 trunkId, sys_logic_portmask_t *pMemberPm)
{
    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pMemberPm), SYS_ERR_NULL_POINTER);

#ifdef __KERNEL__
    SYS_ERR_CHK(rsd_trunk_member_get(trunkId, pMemberPm));
#else
    SYS_ERR_CHK(sal_trunk_member_get(trunkId, pMemberPm));
#endif

    return SYS_ERR_OK;
}
int32 sys_trunk_activeMember_get(uint32 trunkId, sys_logic_portmask_t *pMemberPm)
{
    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pMemberPm), SYS_ERR_NULL_POINTER);

#ifdef __KERNEL__
    SYS_ERR_CHK(rsd_trunk_activeMember_get(trunkId, pMemberPm));
#else
    SYS_ERR_CHK(sal_trunk_activeMember_get(trunkId, pMemberPm));
#endif

    return SYS_ERR_OK;
}
sys_trunk_portRole_t sys_trunk_portRole_ret(sys_logic_port_t port)
{
    sys_trunk_portRole_t type = SYS_PORT_NORMAL;

    rsd_trunk_portRole_get(port, &type);

    return type;
}
uint32 sys_trunk_portTrunkInfo_ret(sys_logic_port_t port, uint32 isRetTrkId)
{
    uint32 trunkId = SYS_TRUNK_INVALID_ID;

    if (isRetTrkId)
    {
        if (IS_TRKLP(port))
            return (port - (TRUNK_LOGIC_PORT_START));

        if (!IS_LP_TRKMBR(port))
            return trunkId;

        rsd_trunk_memberPortTrunkId_get(port, &trunkId);

        return trunkId;
    }
    
    else
    {
        if (IS_TRKLP(port) || !IS_LP_TRKMBR(port))
            return port;

        rsd_trunk_memberPortTrunkId_get(port, &trunkId);

        return (TRK2LP(trunkId));
    }
}
int32 sys_port_lPortmask2lstrPortmask(
    sys_logic_portmask_t *pLPortmask,
    sys_logic_portmask_t *pLstrPortmask)
{
    sys_logic_port_t logicPort;

    SYS_PARAM_CHK((NULL == pLPortmask), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pLstrPortmask), SYS_ERR_NULL_POINTER);

    osal_memset(pLstrPortmask, 0, sizeof(sys_logic_portmask_t));

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, *pLPortmask)
    {
        
        USERSTR_PORTMASK_SET_PORT(*pLstrPortmask, LP2UP(logicPort));
    }

    return SYS_ERR_OK;
}

