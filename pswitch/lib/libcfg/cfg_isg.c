
#include <libcfg/cfg.h>

int32 cfg_isg_port_set(sys_logic_port_t port, sys_enable_t enable, sys_isg_verify_type_t verifyType)
{
    cfg_isg_t *isg = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

    isg->portEnable[port] = enable;
    isg->portVerifyType[port] = verifyType;

    CFG_CLOSE(CFG_ID_ISG);
}

int32 cfg_isg_port_get(sys_logic_port_t port, sys_enable_t *pEnable, sys_isg_verify_type_t *pVerifyType)
{
    cfg_isg_t *isg = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

     *pEnable = isg->portEnable[port];
     *pVerifyType = isg->portVerifyType[port];

    CFG_CLOSE(CFG_ID_ISG);
}

int32 cfg_isg_portMaxEntryNum_set(sys_logic_port_t port, int32 num)
{
    cfg_isg_t *isg = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

    isg->portMaxEntryNum[port] = num;

    CFG_CLOSE(CFG_ID_ISG);
}

int32 cfg_isg_portMaxEntryNum_get(sys_logic_port_t port, int32 *pNum)
{
    cfg_isg_t *isg = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

     *pNum = isg->portMaxEntryNum[port];

    CFG_CLOSE(CFG_ID_ISG);
}

int32 cfg_isg_maxEntryNum_get(uint32 *pMaxEntryNum)
{
    cfg_isg_t *isg = NULL;

    SYS_PARAM_CHK(NULL == pMaxEntryNum, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

    *pMaxEntryNum = isg->maxEntryNum;

    CFG_CLOSE(CFG_ID_ISG);
}

int32 cfg_isg_maxEntryNum_set(uint32 maxEntryNum)
{
    cfg_isg_t *isg = NULL;

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

    isg->maxEntryNum = maxEntryNum;

    CFG_CLOSE(CFG_ID_ISG);
}

int32 cfg_isg_db_get(sys_isg_db_t *pDB)
{
    cfg_isg_t *isg = NULL;

    SYS_PARAM_CHK(NULL == pDB, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

    osal_memcpy(pDB, &isg->db, sizeof(sys_isg_db_t));

    CFG_CLOSE(CFG_ID_ISG);
}

int32 cfg_isg_db_set(sys_isg_db_t *pDB)
{
    cfg_isg_t *isg = NULL;

    SYS_PARAM_CHK(NULL == pDB, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

    osal_memcpy(&isg->db, pDB, sizeof(sys_isg_db_t));

    CFG_CLOSE(CFG_ID_ISG);
}

int32 cfg_isg_db_statistics_get(sys_isg_db_statistics_t *pDBS)
{
    cfg_isg_t *isg = NULL;

    SYS_PARAM_CHK(NULL == pDBS, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

    osal_memcpy(pDBS, &isg->dbStatistics, sizeof(sys_isg_db_statistics_t));

    CFG_CLOSE(CFG_ID_ISG);
}

int32 cfg_isg_db_statistics_set(sys_isg_db_statistics_t *pDBS)
{
    cfg_isg_t *isg = NULL;

    SYS_PARAM_CHK(NULL == pDBS, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_ISG, (cfg_addr)&isg);

    osal_memcpy(&isg->dbStatistics, pDBS, sizeof(sys_isg_db_statistics_t));

    CFG_CLOSE(CFG_ID_ISG);
}

