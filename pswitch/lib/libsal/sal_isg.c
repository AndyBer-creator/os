
#include <common/sys_def.h>
#include <libsal/sal_util.h>
#include <libsal/sal_isg.h>

int32 sal_isg_port_set(sys_logic_port_t port, sys_enable_t enable, sys_isg_verify_type_t verifyType)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(isg_port_enable_set(port, enable, verifyType));
    SYS_ERR_CHK(cfg_isg_port_set(port, enable, verifyType));

    return SYS_ERR_OK;
}

int32 sal_isg_port_get(sys_logic_port_t port, sys_enable_t *pEnable, sys_isg_verify_type_t *pVerifyType)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_isg_port_get(port, pEnable, pVerifyType));

    return SYS_ERR_OK;
}

int32 sal_isg_portMaxEntryNum_set(sys_logic_port_t portId, int32 num)
{
    uint32 cnum = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);

    if (IPSG_ENTRY_UNLIMITED != num)
    {
        SYS_ERR_CHK(sal_isg_portCurrentEntryNum_get(portId, &cnum));

        if (cnum > num)
            return SYS_ERR_ISG_MAX_ENTRY_INVALID;
    }

    SYS_ERR_CHK(isg_port_maxEntryNum_set(portId, num));
    SYS_ERR_CHK(cfg_isg_portMaxEntryNum_set(portId, num));

    return SYS_ERR_OK;
}

int32 sal_isg_portMaxEntryNum_get(sys_logic_port_t portId, int32 *pNum)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pNum == NULL), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_isg_portMaxEntryNum_get(portId, pNum));

    return SYS_ERR_OK;
}

int32 sal_isg_portCurrentEntryNum_get(sys_logic_port_t portId, uint32 *pNum)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pNum == NULL), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(isg_port_currentEntryNum_get(portId, pNum));

    return SYS_ERR_OK;
}

int32 sal_isg_bind_entry_set(sys_isg_entry_t *pBindTblEntry)
{
    SYS_PARAM_CHK(NULL == pBindTblEntry, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(isg_bindingEntry_set(pBindTblEntry, FALSE));

    return SYS_ERR_OK;
}

int32 sal_isg_bind_entry_del(sys_isg_entry_t *pBindTblEntry)
{
    SYS_PARAM_CHK(NULL == pBindTblEntry, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(isg_bindingEntry_del(pBindTblEntry));

    return SYS_ERR_OK;
}

int32 sal_isg_bind_entry_get(uint16 entryIdx, sys_isg_entry_t *pBindTblEntry)
{
    SYS_PARAM_CHK((entryIdx >=  CAPA_IPSG_ENTRY_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pBindTblEntry, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(isg_bindingEntryByIndex_get(entryIdx, pBindTblEntry));

    return SYS_ERR_OK;
}

int32 sal_isg_db_get(sys_isg_db_t *pDB)
{
    SYS_PARAM_CHK(NULL == pDB, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_isg_db_get(pDB));

    return SYS_ERR_OK;
}

int32 sal_isg_db_set(sys_isg_db_t *pDB)
{
    sys_isg_db_t db;

    SYS_PARAM_CHK(NULL == pDB, SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(db);
    SYS_ERR_CHK(sal_isg_db_get(&db));

    SYS_ERR_CHK(isg_db_set(pDB));
    SYS_ERR_CHK(cfg_isg_db_set(pDB));

    return SYS_ERR_OK;
}

int32 sal_isg_db_clear(void)
{
    sys_isg_db_t db;
    char bkFile[SYS_BUF128_LEN];

    SYS_MEM_CLEAR(db);
    SYS_ERR_CHK(sal_isg_db_get(&db));

    if (ISG_DB_TYPE_FLASH == db.dbType)
    {
        SYS_MEM_CLEAR(bkFile);
        osal_sprintf(bkFile, ISG_DB_BACKUP_PREFIX, db.filename);
        unlink(bkFile);
    }

    return SYS_ERR_OK;
}

int32 sal_isg_db_statistics_get(sys_isg_db_statistics_t *pDBS)
{
    SYS_PARAM_CHK(NULL == pDBS, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_isg_db_statistics_get(pDBS));

    return SYS_ERR_OK;
}

int32 sal_isg_db_statistics_set(sys_isg_db_statistics_t *pDBS)
{
    SYS_PARAM_CHK(NULL == pDBS, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_isg_db_statistics_set(pDBS));

    return SYS_ERR_OK;
}

int32 sal_isg_db_timer_get(uint32 *pTimer)
{
    SYS_PARAM_CHK((NULL == pTimer), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(isg_db_timer_get(pTimer));

    return SYS_ERR_OK;
}

int32 sal_isg_db_timer_set(uint32 timer)
{
    SYS_ERR_CHK(isg_db_timer_set(timer));

    return SYS_ERR_OK;
}

