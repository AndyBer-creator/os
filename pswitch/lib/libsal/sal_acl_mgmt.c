
#include <libsal/sal_util.h>
#include <libsal/sal_acl_mgmt.h>
#include <rsd/rsd_acl_mgmt.h>
#include <common/sys_error.h>

int32 sal_acl_mgmt_list_add(char *pcName)
{
    SYS_PARAM_CHK((NULL == pcName), SYS_ERR_NULL_POINTER);
    SYS_ERR_CHK(rsd_acl_mgmt_list_add(pcName));

    return SYS_ERR_OK;
}

int32 sal_acl_mgmt_list_num_get(uint32 *pcListNum)
{
    uint32 uiListNum = 0;

    SYS_PARAM_CHK((NULL == pcListNum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_acl_mgmt_list_num_get( &uiListNum));
    *pcListNum = uiListNum;

   return SYS_ERR_OK;
}

int32 sal_acl_mgmt_list_acenum_get(char *pcName,uint32 *puiAceNum)
{
    uint32 uiAceNum = 0;

    SYS_PARAM_CHK((NULL == pcName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == puiAceNum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_acl_mgmt_list_acenum_get(pcName, &uiAceNum));
    *puiAceNum = uiAceNum;
     return SYS_ERR_OK;
}

int32 sal_acl_mgmt_all_acenum_get(uint32 *puiAceNum)
{
    uint32 uiAceNum = 0;

    SYS_PARAM_CHK((NULL == puiAceNum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_acl_mgmt_all_acenum_get(&uiAceNum));
    *puiAceNum = uiAceNum;

   return SYS_ERR_OK;
}

int32 sal_acl_mgmt_list_del(char *pcName)
{
    SYS_PARAM_CHK((NULL == pcName), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_acl_mgmt_list_del(pcName));

    return SYS_ERR_OK;
}

int32 sal_acl_mgmt_ace_add( char *pcListName, sys_acl_mgmt_ace_priFlag_t stAcepriFlag, sys_acl_mgmt_ace_t *pstAce)
{
    sys_err_code_t ret = SYS_ERR_OK;

    SYS_PARAM_CHK((NULL == pcListName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pstAce), SYS_ERR_NULL_POINTER);

    if (  PRIFLAG_VALID == stAcepriFlag)
    {
         if ( (pstAce->uiMgmtAcePri < 1) ||(pstAce->uiMgmtAcePri > SYS_MGMT_ACL_PRIORITY_MAX  ))
         {
             return SYS_ERR_MGMT_ACE_PRIEXCEED;
         }
    }
    else
    {
        pstAce->uiMgmtAcePri = 0;
    }

    ret = sal_acl_mgmt_list_add(pcListName);
    if (( SYS_ERR_OK != ret ) && (SYS_ERR_MGMT_ACL_EXIST !=ret ))
    {
         return ret;
    }

    SYS_ERR_CHK(rsd_acl_mgmt_ace_add(pcListName, pstAce));

    return SYS_ERR_OK;
}

int32 sal_acl_mgmt_name_get(char *pcListName,uint32 idx)
{
    SYS_PARAM_CHK((NULL == pcListName), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_acl_mgmt_name_get(pcListName,idx));

    return SYS_ERR_OK;
}

int32 sal_acl_mgmt_ace_get( char *pcListName,uint32 idx,sys_acl_mgmt_ace_t *pstAce)
{
    SYS_PARAM_CHK((NULL == pcListName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pstAce), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_acl_mgmt_ace_get(pcListName,idx, pstAce));

    return SYS_ERR_OK;
}

int32 sal_acl_mgmt_ace_del( char *pcListName, uint32 uiMgmtAcePri)
{

   SYS_PARAM_CHK((NULL == pcListName), SYS_ERR_NULL_POINTER);

    if ( (uiMgmtAcePri < 1) ||(uiMgmtAcePri > SYS_MGMT_ACL_PRIORITY_MAX  ))
    {
        return SYS_ERR_MGMT_ACE_PRIEXCEED;
    }
    SYS_ERR_CHK(rsd_acl_mgmt_ace_del(pcListName,uiMgmtAcePri));

    return SYS_ERR_OK;
}

int32 sal_acl_mgmt_class_set( char *pcListName)
{

    SYS_PARAM_CHK((NULL == pcListName), SYS_ERR_NULL_POINTER);
    SYS_ERR_CHK(rsd_acl_mgmt_class_add(pcListName));

    return SYS_ERR_OK;
}

int32 sal_acl_mgmt_class_get( char *pcListName)
{
    sys_err_code_t stRet;
    SYS_PARAM_CHK((NULL == pcListName), SYS_ERR_NULL_POINTER);

    stRet = rsd_acl_mgmt_class_get(pcListName);
    return stRet;
 }

int32 sal_acl_mgmt_class_del(void)
{
    SYS_ERR_CHK(rsd_acl_mgmt_class_del());

    return SYS_ERR_OK;
}

