
#include <libcfg/cfg.h>

#include <board/ski_board.h>

static int32 _glue_util_ioctl(uint32 cmd, void *pData)
{
    int32 fd;
    int32 ret;

    if ((fd = open(SKI_CORE_DEV_NAME, O_RDWR)) == -1)
        return SYS_ERR_SOCKET;

    ret = ioctl(fd, cmd, pData);

    close(fd);

    return ret;
}

static int32 _sys_devInfo_get(uint32 dev, sys_switch_devInfo_t *pInfo)
{
    ski_board_ioctl_t data;

    SYS_PARAM_CHK(IS_DEV_INVALID(dev), SYS_ERR_DEV_ID);
    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    data.devInfo.dev = dev;

    SYS_ERR_CHK(_glue_util_ioctl(SKI_BOARD_GET_DEVICE_INFO, &data));

    osal_memcpy(pInfo, &data.devInfo.devInfo, sizeof(sys_switch_devInfo_t));

    return SYS_ERR_OK;
}

static int32 _sys_boardModel_get(sys_boardModel_t *pBoardModel)
{
    ski_board_ioctl_t data;

    SYS_PARAM_CHK((NULL == pBoardModel), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(_glue_util_ioctl(SKI_BOARD_GET_BOARD_MODEL, &data));

    osal_memcpy(pBoardModel, &data.boardModel, sizeof(sys_boardModel_t));

    return SYS_ERR_OK;
}

static int32 _sys_boardPortConf_get(sys_logic_port_t port, sys_port_conf_t *pPortConf)
{
    ski_board_ioctl_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPortConf), SYS_ERR_NULL_POINTER);

    data.boardPortconf.port = port;

    SYS_ERR_CHK(_glue_util_ioctl(SKI_BOARD_GET_BOARD_PORTCONF, &data));

    osal_memcpy(pPortConf, &data.boardPortconf.portConf, sizeof(sys_port_conf_t));

    return SYS_ERR_OK;
}

int32 cfg_sys_devInfo_init(void)
{
    
    sys_switch_devInfo_t devInfo;
    cfg_devInfo_t *pDevInfo;

    CFG_OPEN(CFG_ID_DEVINFO, (cfg_addr)&pDevInfo);

    _sys_devInfo_get(SYS_DEV_ID_DEFAULT, &devInfo);
	osal_memcpy(&pDevInfo->devInfo[SYS_DEV_ID_DEFAULT], &devInfo, sizeof(sys_switch_devInfo_t));

    CFG_CLOSE(CFG_ID_DEVINFO);
}

int32 cfg_sys_devInfo_set(cfg_devInfo_t *pDevInfo)
{
    cfg_devInfo_t *pDevInfoTmp;

    SYS_PARAM_CHK(NULL == pDevInfo, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DEVINFO, (cfg_addr)&pDevInfoTmp);

    *pDevInfoTmp = *pDevInfo;

    CFG_CLOSE(CFG_ID_DEVINFO);
}

int32 cfg_sys_devInfo_get(cfg_devInfo_t *pDevInfo)
{
    cfg_devInfo_t *pDevInfoTmp;

    SYS_PARAM_CHK(NULL == pDevInfo, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DEVINFO, (cfg_addr)&pDevInfoTmp);

    *pDevInfo = *pDevInfoTmp;

    CFG_CLOSE(CFG_ID_DEVINFO);
}

int32 cfg_sys_devPortNum_get(uint32 unit, uint32 *pPortnum)
{
    cfg_devInfo_t *dev;

    SYS_PARAM_CHK(NULL == pPortnum, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DEVINFO, (cfg_addr)&dev);

    *pPortnum = dev->devInfo[unit].port_number;

    CFG_CLOSE(CFG_ID_DEVINFO);
}

int32 cfg_sys_devPortList_get(uint32 unit, sys_portmask_t *pPortmask)
{
    cfg_devInfo_t *dev;

    SYS_PARAM_CHK(NULL == pPortmask, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DEVINFO, (cfg_addr)&dev);

    osal_memcpy(pPortmask, &dev->devInfo[unit].ether.portmask, sizeof(sys_portmask_t));

    CFG_CLOSE(CFG_ID_DEVINFO);
}

int32 cfg_sys_devGeComboPortNum_get(uint32 unit, uint32 *pPortnum)
{
    cfg_devInfo_t *dev;

    SYS_PARAM_CHK(NULL == pPortnum, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DEVINFO, (cfg_addr)&dev);

    *pPortnum = dev->devInfo[unit].combo.portNum;

    CFG_CLOSE(CFG_ID_DEVINFO);
}

int32 cfg_sys_devGeComboMinPortNum_get(uint32 unit, uint32 *pPortnum)
{
    cfg_devInfo_t *dev;

    SYS_PARAM_CHK(NULL == pPortnum, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DEVINFO, (cfg_addr)&dev);

    *pPortnum = dev->devInfo[unit].combo.portNum;

    CFG_CLOSE(CFG_ID_DEVINFO);
}

int32 cfg_sys_devMaxL2Cnt_get(uint32 unit, uint32 *pL2Num)
{
    cfg_devInfo_t *dev;

    SYS_PARAM_CHK(NULL == pL2Num, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DEVINFO, (cfg_addr)&dev);

    *pL2Num = dev->devInfo[unit].capacityInfo.l2_learn_limit_cnt_max;

    CFG_CLOSE(CFG_ID_DEVINFO);
}

int32 cfg_sys_devChipFamilyId_get(uint32 unit, uint32 *pFamilyId)
{
    cfg_devInfo_t *dev;

    SYS_PARAM_CHK(NULL == pFamilyId, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DEVINFO, (cfg_addr)&dev);

    *pFamilyId = dev->devInfo[unit].familyId;

    CFG_CLOSE(CFG_ID_DEVINFO);
}

int32 cfg_sys_boardModel_init(void)
{
    sys_boardModel_t boardModel;
    cfg_boardModel_t *pBoardModel = NULL;

    CFG_OPEN(CFG_ID_BOARD_MODEL, (cfg_addr)&pBoardModel);

    SYS_MEM_CLEAR(boardModel);
    _sys_boardModel_get(&boardModel);
    osal_memcpy(&pBoardModel->boardModel, &boardModel, sizeof(boardModel));

    CFG_CLOSE(CFG_ID_BOARD_MODEL);
}

int32 cfg_sys_boardModel_get(sys_boardModel_t *pBoard)
{
    cfg_boardModel_t *pBoardModel = NULL;

    SYS_PARAM_CHK(NULL == pBoard, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_BOARD_MODEL, (cfg_addr)&pBoardModel);

    osal_memcpy(pBoard, &pBoardModel->boardModel, sizeof(sys_boardModel_t));

    CFG_CLOSE(CFG_ID_BOARD_MODEL);
}

int32 cfg_sys_boardConf_get(sys_board_conf_t *pBoard)
{
    cfg_boardModel_t *pBoardModel = NULL;

    SYS_PARAM_CHK(NULL == pBoard, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_BOARD_MODEL, (cfg_addr)&pBoardModel);

    osal_memcpy(pBoard, &pBoardModel->boardModel.boardConf, sizeof(sys_board_conf_t));

    CFG_CLOSE(CFG_ID_BOARD_MODEL);
}

int32 cfg_sys_boardModelId_get(sys_boardModel_Id_t *pBoardId)
{
    cfg_boardModel_t *pBoardModel = NULL;

    SYS_PARAM_CHK(NULL == pBoardId, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_BOARD_MODEL, (cfg_addr)&pBoardModel);

    *pBoardId = pBoardModel->boardModel.boardConf.boardModelId;

    CFG_CLOSE(CFG_ID_BOARD_MODEL);
}

int32 cfg_sys_boardModelName_get(char *pBoardName)
{
    cfg_boardModel_t *pBoardModel = NULL;

    SYS_PARAM_CHK(NULL == pBoardName, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_BOARD_MODEL, (cfg_addr)&pBoardModel);

    osal_strcpy(pBoardName, pBoardModel->boardModel.boardConf.boardName);

    CFG_CLOSE(CFG_ID_BOARD_MODEL);
}

int32 cfg_sys_boardFlshLayout_get(sys_flsh_layout_t *pFlshLayout)
{
    cfg_boardModel_t *pBoardModel = NULL;

    SYS_PARAM_CHK(NULL == pFlshLayout, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_BOARD_MODEL, (cfg_addr)&pBoardModel);

    *pFlshLayout = pBoardModel->boardModel.flshLayout;

    CFG_CLOSE(CFG_ID_BOARD_MODEL);
}

int32 cfg_sys_boardModelPortNum_get(uint32 dev, uint32 *portnum)
{
    cfg_boardModel_t *pBoardModel = NULL;

    SYS_PARAM_CHK(NULL == portnum, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_BOARD_MODEL, (cfg_addr)&pBoardModel);

    *portnum = pBoardModel->boardModel.portNum[dev][PORT_NUMTYPE_ALL];

    CFG_CLOSE(CFG_ID_BOARD_MODEL);
}

int32 cfg_sys_boardPortConf_get(sys_logic_port_t port, sys_port_conf_t *pPortConf)
{

    SYS_ERR_CHK(_sys_boardPortConf_get(port, pPortConf));

    return SYS_ERR_OK;
}

