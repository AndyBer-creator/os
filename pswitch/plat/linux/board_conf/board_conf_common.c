
#include <board/board.h>

sys_boardModel_t glbBoardModel;

int32 sys_boardModel_get(sys_boardModel_t *pBoardModel)
{
    *pBoardModel = glbBoardModel;

    return SYS_ERR_OK;
}

int32 sys_boardModel_set(sys_boardModel_t *pBoardModel)
{
    glbBoardModel = *pBoardModel;

    return SYS_ERR_OK;
}

sys_boardModel_Id_t sys_boardModelId_ret(void)
{
    return glbBoardModel.boardConf.boardModelId;
}

int32 sys_board_portNum_set(uint32 dev, sys_port_numType_t type, uint32 num)
{
    SYS_PARAM_CHK(type >= PORT_NUMTYPE_END, SYS_ERR_INPUT);

    glbBoardModel.portNum[dev][type] = num;

    return SYS_ERR_OK;
}

uint32 sys_board_portNum_ret(uint32 dev, sys_port_numType_t type)
{
    return glbBoardModel.portNum[dev][type];
}

uint32 sys_board_bootmsgLevel_ret(void)
{
    return glbBoardModel.bootmsg;
}

