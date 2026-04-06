
#include <authmgr/proto_authmgr.h>
#include <ski/proto/authmgr/ski_authmgr.h>

int32 ski_authmgr_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_authmgr_ioctl_t);
    ski_authmgr_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_AUTHMGR_TYPE_ENABLE_SET:
            SYS_ERR_HDL(authmgr_typeEnbl_set(pData->typeEnbl.type, pData->typeEnbl.enable), _exit, ret);
            break;

        case SKI_AUTHMGR_GUEST_VLAN_SET:
            SYS_ERR_HDL(authmgr_guestVlan_set(pData->guestVlan), _exit, ret);
            break;

        case SKI_AUTHMGR_PORT_CFG_SET:
            SYS_ERR_HDL(authmgr_portCfg_set(pData->portCfg.port, &pData->portCfg.cfg), _exit, ret);
            break;

        case SKI_AUTHMGR_SESS_CNT_GET:
            SYS_ERR_HDL(authmgr_sessCnt_get(&pData->sessCnt), _exit, ret);
            break;

        case SKI_AUTHMGR_SESS_INFO_GET:
            SYS_ERR_HDL(authmgr_sessInfo_getByIdx(pData->sessInfo.idx, &pData->sessInfo.info), _exit, ret);
            break;

        case SKI_AUTHMGR_SESS_CLEAR_SET:
            SYS_ERR_HDL(authmgr_sess_clear(&pData->sessClear), _exit, ret);
            break;

#ifdef CONFIG_SYS_PROTO_DOT1X
        case SKI_AUTHMGR_DOT1X_RAD_RX_SET:
            SYS_ERR_HDL(authmgr_dot1xRadius_rx(pData->dot1xRadRecv), _exit, ret);
            break;

        case SKI_AUTHMGR_DOT1X_RAD_SENDID_GET:
            SYS_ERR_HDL(authmgr_dot1xRadiusSendId_get(pData->radSendId.sessId, &pData->radSendId.id), _exit, ret);
            break;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
        case SKI_AUTHMGR_MACAUTH_AUTHINFO_SET:
            SYS_ERR_HDL(authmgr_macauthStatus_set(&pData->macauthAuthInfo), _exit, ret);
            break;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        case SKI_AUTHMGR_WEBAUTH_AUTHINFO_SET:
            SYS_ERR_HDL(authmgr_webauthStatus_set(&pData->webauthAuthInfo), _exit, ret);
            break;

        case SKI_AUTHMGR_WEBAUTH_STATUS_GET:
            SYS_ERR_HDL(authmgr_webauthStatusByKey_get(pData->statusByKey.key, &pData->statusByKey.status), _exit, ret);
            break;

        case SKI_AUTHMGR_WEBAUTH_SUPPINFO_GET:
            SYS_ERR_HDL(authmgr_webauthSuppInfoByKey_get(pData->suppByKey.key, &pData->suppByKey.suppInfo), _exit, ret);
            break;
#endif

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if(cmd > SKI_AUTHMGR_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_authmgr_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_AUTHMGR, ski_authmgr_cmd_mux));

    return SYS_ERR_OK;
}

