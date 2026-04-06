#ifdef __KERNEL__
#include <board/board.h>
#include <common/sys_l2.h>
#include <rsd/rsd_mcast.h>
#include <rsd/rsd_sys.h>
#endif
#ifndef __KERNEL__
  #include <libcfg/cfg.h>
#endif
#ifdef __KERNEL__
static uint32 physicPortNum[SYS_DEV_NUM_MAX];
static uint32 userPortNum[SYS_DEV_NUM_MAX];
static uint32 devNum = SYS_DEV_NUM_MAX;
static int32  cpuPort[SYS_DEV_NUM_MAX];
static sys_switch_devInfo_t devInfo[SYS_DEV_NUM_MAX];
#endif

const sys_text_t text_enable[] =
{
    {          "disabled"    },
    {           "enabled"     },
};
#ifdef __KERNEL__
static int32 sys_dev_num_set(uint32 devnum);
int32 sys_dev_init(void)
{
    uint32 devId = SYS_DEV_ID_DEFAULT;
    sys_switch_devInfo_t *pDevInfo = NULL;

    SYS_MEM_CLEAR(devInfo);

    SYS_ERR_CHK(rsd_sys_devInfo_get(devId, &devInfo[devId]));

    pDevInfo = &(devInfo[devId]);

#if defined(CONFIG_SYS_RTL8380)
    if (IS_CHIP_MAPLE())
    {
        switch(pDevInfo->chipId)
        {
            default:
                break;
        }
    }
#endif
#if defined(CONFIG_SYS_RTL8390)
    if (IS_CHIP_CYPRESS())
    {
        switch(pDevInfo->chipId)
        {
            default:
                break;
        }
    }
#endif
    return SYS_ERR_OK;
}
void *sys_dev_info_ret(uint32 unit)
{
    return &(devInfo[unit]);
}
int32 sys_port_num_init(void)
{
    int32 ret = SYS_ERR_OK;
    sys_switch_devInfo_t *pData = NULL;
    uint32 portnumTotal[SYS_DEV_NUM_MAX];
    uint32 portnumCopper[SYS_DEV_NUM_MAX];
    uint32 portnumFiber[SYS_DEV_NUM_MAX];
    uint32 portnumCombo[SYS_DEV_NUM_MAX];
    int32  unitArray[SYS_DEV_NUM_MAX];
    uint32 devId;
    uint32 devnum;
    uint32 portIdx;
    sys_port_conf_t *pPortConf = NULL;
    sys_board_t *pBoardConf = NULL;

    SYS_ERR_CHK(board_conf_get(&pBoardConf));

    pPortConf = (sys_port_conf_t *) pBoardConf->pPortConf;

    if (pPortConf == NULL)
        return SYS_ERR_FAILED;

    for (devId = 0; devId < SYS_DEV_NUM_MAX; devId++)
    {
        portnumTotal[devId] = 0;
        portnumCopper[devId] = 0;
        portnumFiber[devId] = 0;
        portnumCombo[devId] = 0;
        unitArray[devId] = -1;
    }

    portIdx = 0;
    while(pPortConf[portIdx].devId != PORTCONF_END_VAL)
    {
        devId = pPortConf[portIdx].devId;

        SYS_PARAM_CHK(IS_DEV_INVALID(devId), SYS_ERR_DEV_ID);

        unitArray[devId] = devId;

        portnumTotal[devId]++;

        if ((pPortConf[portIdx].portType > PORT_TYPE_LAG)
                && (pPortConf[portIdx].portType < PORT_TYPE_END))
        {
            portnumCombo[devId]++;
        }
        else if (PORT_MEDIA_COPPER == pPortConf[portIdx].mediaType)
        {
            portnumCopper[devId]++;
        }
        else
        {
            portnumFiber[devId]++;
        }

        portIdx++;
    }

    SYS_MEM_ALLOC_HDL(pData, sizeof(sys_switch_devInfo_t), _exit, ret);

    devnum = 0;
    for (devId = 0; devId < SYS_DEV_NUM_MAX; devId++)
    {
        if (unitArray[devId] >= 0)
        {
            osal_memset(pData, 0, sizeof(sys_switch_devInfo_t));
            SYS_ERR_HDL(rsd_sys_devInfo_get(devId, pData), _exit, ret);

            SYS_ERR_HDL(sys_user_port_num_set(devId, portnumTotal[devId]), _exit, ret);

            portnumTotal[devId] += 1; 

            physicPortNum[devId] = pData->cpuPort;
            cpuPort[devId] = pData->cpuPort;

            devnum++;
        }

        SYS_ERR_HDL(sys_board_portNum_set(devId, PORT_NUMTYPE_ALL, portnumTotal[devId]), _exit, ret);
        SYS_ERR_HDL(sys_board_portNum_set(devId, PORT_NUMTYPE_COPPER, portnumCopper[devId]), _exit, ret);
        SYS_ERR_HDL(sys_board_portNum_set(devId, PORT_NUMTYPE_FIBER, portnumFiber[devId]), _exit, ret);
        SYS_ERR_HDL(sys_board_portNum_set(devId, PORT_NUMTYPE_COMBO, portnumCombo[devId]), _exit, ret);
    }

    SYS_ERR_HDL(sys_dev_num_set(devnum), _exit, ret);

_exit:
    SYS_MEM_FREE(pData);

    return SYS_ERR_OK;
}
static int32 sys_dev_num_set(uint32 devnum)
{
    
    SYS_PARAM_CHK((devNum > SYS_DEV_NUM_MAX), SYS_ERR_OUT_OF_RANGE);

    devNum = devnum;

    return SYS_ERR_OK;
}
uint32 sys_dev_num_ret(void)
{
    return devNum;
}
uint32 sys_dev_chipFamilyId_ret(uint32 devId)
{
    return devInfo[devId].familyId;
}
int32 sys_user_port_num_set(uint32 dev, uint32 number)
{
    
    SYS_PARAM_CHK(IS_DEV_INVALID(dev), SYS_ERR_DEV_ID);
    SYS_PARAM_CHK((number > SYS_PORT_NUM_MAX), SYS_ERR_OUT_OF_RANGE);

    userPortNum[dev] = number;

    return SYS_ERR_OK;
}
uint32 sys_user_port_num_ret(uint32 dev)
{
    if (IS_DEV_INVALID(dev))
        return 0;

    return userPortNum[dev];
}
uint32 sys_logic_port_num_ret(void)
{
    uint32 logicPortNum = 0;
    uint32 dev = 0;

    FOR_EACH_DEV(dev)
    {
        logicPortNum += userPortNum[dev];
    }

    logicPortNum += CAPA_LAG_NUM;

    return logicPortNum;
}
uint32 sys_physic_port_num_ret(uint32 dev)
{
    return physicPortNum[dev];
}
uint32 sys_combo_port_num_ret(uint32 dev)
{
    return sys_board_portNum_ret(dev, PORT_NUMTYPE_COMBO);
}
uint32 sys_cpu_port_ret(uint32 dev)
{
    return cpuPort[dev];
}

#else 
uint32 sys_dev_chipFamilyId_ret(uint32 devId)
{
    uint32 familyId = 0;

    cfg_sys_devChipFamilyId_get(devId, &familyId);

    return familyId;
}
uint32 sys_user_port_num_ret(uint32 unit)
{
    uint32 portNum;

    SYS_ERR_CHK(cfg_sys_boardModelPortNum_get(unit, &portNum));

    portNum --;

    return portNum;
}

uint32 sys_logic_port_num_ret(void)
{
    uint32 logicPortNum = 0;

    logicPortNum += SYS_USER_PORT_NUM(SYS_DEV_ID_DEFAULT);

    logicPortNum += CAPA_LAG_NUM;

    return logicPortNum;
}
#endif

