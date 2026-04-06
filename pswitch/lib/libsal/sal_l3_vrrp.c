
#include <common/sys_log.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/vosMsg.h>
#include <libipc/vos.h>
#include <libsal/sal_l3_vrrp.h>

int32 ipc_vrrp_vrid_vip_config(int ifindex, int vrid, sys_ipv4_t vipaddr, int opcode)
{

	int 					  iRet;
	SMD_MSG 				  msg;
	ARG_DEF(4);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, VRRP_FUNC_VSRV_CONFIG_SET, SMD_GLOBAL, SMD_MSG_SET);
	
	ARG_ADD(0, &ifindex, 0, sizeof(int));
    ARG_ADD(1, &vrid, 0, sizeof(int));
    ARG_ADD(2, &vipaddr, 0, sizeof(sys_ipv4_t));
    ARG_ADD(3, &opcode, 0, sizeof(int));

	iRet = smdMsgRequestByName(VRRP_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{
		switch(iRet)
		{
			case VRRP_CFGERR_IFWRONG:
				
				break;
			case VRRP_CFGERR_SUBNETDIFF:
				iRet = SYS_ERR_VRRP_CFGERR_SUBNETDIFF;
				break;
			case VRRP_CFGERR_MAXVSRV:
				iRet = SYS_ERR_VRRP_CFGERR_MAXVSRV;
				break;
			case VRRP_CFGERR_MAXVIP:
				iRet = SYS_ERR_VRRP_CFGERR_MAXVIP;
				break;
			default:
				break;
		}
	}

	return iRet;
}

int32 ipc_vrrp_vrid_priority_config(int ifindex, int vrid, ulong_t value, int *piErrNo)
{

	int 					  iRet;
	SMD_MSG 				  msg;
	ARG_DEF(4);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, VRRP_FUNC_VSRV_PRIO_SET, SMD_GLOBAL, SMD_MSG_SET);
	
	ARG_ADD(0, &ifindex, 0, sizeof(int));
    ARG_ADD(1, &vrid, 0, sizeof(int));
    ARG_ADD(2, &value, 0, sizeof(ulong_t));
    ARG_ADD(3, piErrNo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int));

	iRet = smdMsgRequestByName(VRRP_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

	return iRet;
}

int32 ipc_vrrp_vrid_preempt_mode_config(int ifindex, int vrid, sys_enable_t bPreempt,
    ulong_t value, int *piErrNo)
{

	int 					  iRet;
	SMD_MSG 				  msg;
	ARG_DEF(5);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, VRRP_FUNC_VSRV_PREEMPT_SET, SMD_GLOBAL, SMD_MSG_SET);
	
	ARG_ADD(0, &ifindex, 0, sizeof(int));
    ARG_ADD(1, &vrid, 0, sizeof(int));
    ARG_ADD(2, &bPreempt, 0, sizeof(ulong_t));
    ARG_ADD(3, &value, 0, sizeof(ulong_t));
    ARG_ADD(4, piErrNo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int));

	iRet = smdMsgRequestByName(VRRP_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

	return iRet;
}
	
int32 ipc_vrrp_vrid_advertise_timer_config(int ifindex, int vrid, ulong_t value, int *piErrNo)
{

	int 					  iRet;
	SMD_MSG 				  msg;
	ARG_DEF(4);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, VRRP_FUNC_VSRV_ADVER_TIME_SET, SMD_GLOBAL, SMD_MSG_SET);
	
	ARG_ADD(0, &ifindex, 0, sizeof(int));
	ARG_ADD(1, &vrid, 0, sizeof(int));
	ARG_ADD(2, &value, 0, sizeof(ulong_t));
	ARG_ADD(3, piErrNo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int));

	iRet = smdMsgRequestByName(VRRP_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int ipc_vrrp_vsrv_get_next(int ifindex, vrrp_rt *pVsrvInfo)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(2);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_APP, VRRP_FUNC_VSRV_GETNEXT, 
        SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &ifindex, 0, sizeof(int));
    ARG_ADD(1, pVsrvInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(vrrp_rt));
    
    iRet = smdMsgRequestByName(VRRP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int ipc_vrrp_vsrv_if_track_add(int ifindex, int vrid, 
    ulong_t ulIfTracIndex, ulong_t ulPrio, int *piErrNo)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(5);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_APP, VRRP_FUNC_VSRV_IFTRACK_ADD, 
        SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &ifindex, 0, sizeof(int));
    ARG_ADD(1, &vrid, 0, sizeof(int));
    ARG_ADD(2, &ulIfTracIndex, 0, sizeof(ulong_t));
    ARG_ADD(3, &ulPrio, 0, sizeof(ulong_t));
    ARG_ADD(4, piErrNo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int));
    
    iRet = smdMsgRequestByName(VRRP_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int ipc_vrrp_vsrv_if_track_del(int ifindex, int vrid, 
    ulong_t ulIfTracIndex, int *piErrNo)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(4);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_APP, VRRP_FUNC_VSRV_IFTRACK_DEL, 
        SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &ifindex, 0, sizeof(int));
    ARG_ADD(1, &vrid, 0, sizeof(int));
    ARG_ADD(2, &ulIfTracIndex, 0, sizeof(ulong_t));
    ARG_ADD(3, piErrNo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int));
    
    iRet = smdMsgRequestByName(VRRP_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 ipc_vrrp_route_proto_reg(char *szProcessName, ulong_t ulFunCallBackId)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(2);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_APP, VRRP_FUNC_ROUTE_PROTO_REG, 
        SMD_GLOBAL, SMD_MSG_INFO);

    ARG_ADD(0, szProcessName, SMD_ARG_POINTER, strlen(szProcessName) + 1);
    ARG_ADD(1, &ulFunCallBackId, 0, sizeof(ulong_t));
    
    iRet = smdMsgRequestByName(VRRP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 ipc_vrrp_route_proto_dereg(char *szProcessName, ulong_t ulFunCallBackId)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(2);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_APP, VRRP_FUNC_ROUTE_PROTO_DEREG, 
        SMD_GLOBAL, SMD_MSG_INFO);

    ARG_ADD(0, szProcessName, SMD_ARG_POINTER, strlen(szProcessName) + 1);
    ARG_ADD(1, &ulFunCallBackId, 0, sizeof(ulong_t));
    
    iRet = smdMsgRequestByName(VRRP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 sal_vrrp_vrid_vip_config(int ifindex, int vrid, sys_ipv4_t vipaddr, int opcode)
{
	SYS_ERR_CHK(ipc_vrrp_vrid_vip_config(ifindex, vrid, vipaddr, opcode));

    return SYS_ERR_OK;
}

int32 sal_vrrp_vrid_priority_config(int ifindex, int vrid, ulong_t value, int *piErrNo)
{
	SYS_ERR_CHK(ipc_vrrp_vrid_priority_config(ifindex, vrid, value, piErrNo));

    return SYS_ERR_OK;
}

int32 sal_vrrp_vrid_preempt_mode_config(int ifindex, int vrid, sys_enable_t bPreempt,
    ulong_t value, int *piErrNo)
{
	SYS_ERR_CHK(ipc_vrrp_vrid_preempt_mode_config(ifindex, vrid, bPreempt, value, piErrNo));

    return SYS_ERR_OK;
}

int32 sal_vrrp_vrid_advertise_timer_config(int ifindex, int vrid, ulong_t value, int *piErrNo)
{
	SYS_ERR_CHK(ipc_vrrp_vrid_advertise_timer_config(ifindex, vrid, value, piErrNo));

    return SYS_ERR_OK;
}

int32 sal_vrrp_vsrv_get_next(int ifindex, vrrp_rt *pVsrvInfo)
{
	SYS_ERR_CHK(ipc_vrrp_vsrv_get_next(ifindex, pVsrvInfo));

    return SYS_ERR_OK;
}

int32 sal_vrrp_vsrv_if_track_add(int ifindex, int vrid, 
    ulong_t ulIfTracIndex, ulong_t ulPrio, int *piErrNo)
{
	SYS_ERR_CHK(ipc_vrrp_vsrv_if_track_add(ifindex, vrid, ulIfTracIndex, ulPrio, piErrNo));

    return SYS_ERR_OK;
}
	
int32 sal_vrrp_vsrv_if_track_del(int ifindex, int vrid, 
	ulong_t ulIfTracIndex, int *piErrNo)
{
	SYS_ERR_CHK(ipc_vrrp_vsrv_if_track_del(ifindex, vrid, ulIfTracIndex, piErrNo));

	return SYS_ERR_OK;
}
	
int32 sal_vrrp_route_proto_reg(char *szProcessName, ulong_t ulFunCallBackId)
{
	SYS_ERR_CHK(ipc_vrrp_route_proto_reg(szProcessName, ulFunCallBackId));

	return SYS_ERR_OK;
}
	
int32 sal_vrrp_route_proto_dereg(char *szProcessName, ulong_t ulFunCallBackId)
{
	SYS_ERR_CHK(ipc_vrrp_route_proto_dereg(szProcessName, ulFunCallBackId));

	return SYS_ERR_OK;
}

