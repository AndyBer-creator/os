
#ifndef __CFG_BOARD_H__
#define __CFG_BOARD_H__

typedef struct cfg_devInfo_s
{
    uint32 userPortviewInitFlag[SYS_DEV_NUM_MAX];
    sys_switch_devInfo_t devInfo[SYS_DEV_NUM_MAX];
    sys_logic_portInfo_t logicPortInfo;
} cfg_devInfo_t;

typedef struct cfg_boardModel_s
{
    sys_boardModel_t boardModel;
} cfg_boardModel_t;

extern int32 cfg_sys_devInfo_init(void);

extern int32 cfg_sys_devInfo_set(cfg_devInfo_t *pDevInfo);

extern int32 cfg_sys_devInfo_get(cfg_devInfo_t *pDevInfo);

extern int32 cfg_sys_devPortNum_get(uint32 unit, uint32 *pPortnum);

extern int32 cfg_sys_devPortList_get(uint32 unit, sys_portmask_t *pPortmask);

extern int32 cfg_sys_devGeComboPortNum_get(uint32 unit, uint32 *pPortnum);

extern int32 cfg_sys_devGeComboMinPortNum_get(uint32 unit, uint32 *pPortnum);

extern int32 cfg_sys_devMaxL2Cnt_get(uint32 unit, uint32 *pL2Num);

extern int32 cfg_sys_devChipFamilyId_get(uint32 unit, uint32 *pFamilyId);

extern int32 cfg_sys_boardModel_init(void);

extern int32 cfg_sys_boardModel_get(sys_boardModel_t *pBoard);

extern int32 cfg_sys_boardConf_get(sys_board_conf_t *pBoard);

extern int32 cfg_sys_boardModelId_get(sys_boardModel_Id_t *pBoardId);

extern int32 cfg_sys_boardModelName_get(char *pBoardName);

extern int32 cfg_sys_boardFlshLayout_get(sys_flsh_layout_t *pFlshLayout);

extern int32 cfg_sys_boardModelPortNum_get(uint32 dev, uint32 *portnum);

extern int32 cfg_sys_boardPortConf_get(sys_logic_port_t port, sys_port_conf_t *pPortConf);
#endif 

