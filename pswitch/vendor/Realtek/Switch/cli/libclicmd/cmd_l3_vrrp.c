
#include <libsal/sal_l3_vrrp.h>
#include <libcmd/cmd.h>

static const char *vrrp_state_str[] =
{
  "Initialize",
  "Backup",
  "Master"
};

int32 cmd_vrrp_vip_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 vrid = 0;
	sys_ipv4_t vipv4;
	uint32 l3index = 0;
	sys_l3_intfId_t l3intf;
	
	SYS_MEM_CLEAR(vipv4);

	vrid = SYS_STR2UINT(argv[0]);
	SYS_ERR_CHK(STR2IPV4(argv[1], &vipv4));
	l3index = g_selVid;

	l3intf.type = SYS_L3_INTFTYPE_VLAN;
	l3intf.id = l3index;

	if (!IS_L3_INTF_EXIST(&l3intf))
	{	
		return SYS_ERR_L3_INTF_IP_NOT_EXIST;	
	}
	
	if (CMD_IS_NO_FORM_EXIST())
	{
		SYS_ERR_CHK(sal_vrrp_vrid_vip_config(l3index, vrid, vipv4, VRRP_OPCODE_DELIP));
	}
	else
	{
		SYS_ERR_CHK(sal_vrrp_vrid_vip_config(l3index, vrid, vipv4, VRRP_OPCODE_ADDIP));
	}
    return rc;
}

int32 cmd_vrrp_priority_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 vrid = 0;
    uint32 priority = VRRP_PRIO_DFL;
    int iErrNo = 0;
	uint32 l3index = 0;
	sys_l3_intfId_t l3intf;

	vrid = SYS_STR2UINT(argv[0]);
	l3index = g_selVid;

	l3intf.type = SYS_L3_INTFTYPE_VLAN;
	l3intf.id = l3index;

	if (!IS_L3_INTF_EXIST(&l3intf))
	{	
		return SYS_ERR_L3_INTF_IP_NOT_EXIST;	
	}
	
	if (CMD_IS_NO_FORM_EXIST())
	{
		priority = VRRP_PRIO_DFL;
	}
	else
	{
		priority = SYS_STR2UINT(argv[1]);
	}
	
	SYS_ERR_CHK(sal_vrrp_vrid_priority_config(l3index, vrid, priority, &iErrNo));
	
    return rc;
}

int32 cmd_vrrp_preempt_mode_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 vrid = 0;
    uint32 timer = 0;
	uint32 l3index = 0;
    int iErrNo = 0;
	sys_l3_intfId_t l3intf;

	vrid = SYS_STR2UINT(argv[0]);
	l3index = g_selVid;

	l3intf.type = SYS_L3_INTFTYPE_VLAN;
	l3intf.id = l3index;

	if (!IS_L3_INTF_EXIST(&l3intf))
	{	
		return SYS_ERR_L3_INTF_IP_NOT_EXIST;	
	}
	
	if (CMD_IS_NO_FORM_EXIST())
	{
		SYS_ERR_CHK(sal_vrrp_vrid_preempt_mode_config(l3index, vrid, DISABLED, timer, &iErrNo));
	}
	else
	{
		if (argc > 1)
			timer = SYS_STR2UINT(argv[1]);
		
		SYS_ERR_CHK(sal_vrrp_vrid_preempt_mode_config(l3index, vrid, ENABLED, timer, &iErrNo));
	}

    return rc;
}

int32 cmd_vrrp_advertise_timer_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 vrid = 0;
    uint32 timer = 0;
	uint32 l3index = 0;
    int iErrNo = 0;
	sys_l3_intfId_t l3intf;

	vrid = SYS_STR2UINT(argv[0]);
	l3index = g_selVid;

	l3intf.type = SYS_L3_INTFTYPE_VLAN;
	l3intf.id = l3index;

	if (!IS_L3_INTF_EXIST(&l3intf))
	{	
		return SYS_ERR_L3_INTF_IP_NOT_EXIST;	
	}
	
	if (CMD_IS_NO_FORM_EXIST())
	{
		timer = VRRP_ADVER_DFL;
	}
	else
	{
		timer = SYS_STR2UINT(argv[1]);
	}

	SYS_ERR_CHK(sal_vrrp_vrid_advertise_timer_config(l3index, vrid, timer, &iErrNo));
	
    return rc;
}

int32 cmd_vrrp_track_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 vrid = 0;
    uint32 priority = VRRP_PRI_TRACK;
	uint32 l3index = 0;
	int iErrNo = 0;
    sys_vid_t vid;
	sys_l3_intfId_t l3intf;
	sys_l3_intfId_t tracckIntf;

	vrid = SYS_STR2UINT(argv[0]);
	l3index = g_selVid;

	l3intf.type = SYS_L3_INTFTYPE_VLAN;
	l3intf.id = l3index;

	if (!IS_L3_INTF_EXIST(&l3intf))
	{	
		return SYS_ERR_L3_INTF_IP_NOT_EXIST;	
	}
	
	vid = SYS_STR2UINT(argv[1]);
	tracckIntf.type = SYS_L3_INTFTYPE_VLAN;
	tracckIntf.id = vid;
	
	if (!IS_L3_INTF_EXIST(&tracckIntf))
	{	
		return SYS_ERR_L3_INTF_IP_NOT_EXIST;	
	}
	
	if (CMD_IS_NO_FORM_EXIST())
	{
		SYS_ERR_CHK(sal_vrrp_vsrv_if_track_del(l3index, vrid, vid, &iErrNo));
	}
	else
	{
		if (argc > 2)
		{
			priority = SYS_STR2UINT(argv[2]);
		}
		
		SYS_ERR_CHK(sal_vrrp_vsrv_if_track_add(l3index, vrid, vid, priority, &iErrNo));
	}
	
	return rc;
}

int32 cmd_vrrp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int i, j;
	ulong_t vrid = 0, useIf = 0, useVrid = 0;
    int iIndex;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1] = {0};
	char ipStr[CAPA_IPV4_STR_LEN];

	ushort_t usVid, usLinkup;
	vrrp_rt vsrvInfo;
	ulong_t  ulSum = 0;

	if (argc > 0)
	{
		intf.type = SYS_L3_INTFTYPE_VLAN;
		intf.id = SYS_STR2UINT(argv[0]);
		if (!IS_L3_INTF_EXIST(&intf))
		{
			return SYS_ERR_FAILED;
		}
		
		useIf = 1;
		sys_l3_intfId2Str(&intf, ifStr);
	}
	
	if (argc > 1)
	{
		vrid = SYS_STR2UINT(argv[1]);
		useVrid = 1;
	}
	
	iIndex = -1;
	for (i = 1; i <= VRRP_VSRV_SIZE_MAX; i++)
	{		
		if ( SYS_ERR_OK != sal_vrrp_vsrv_get_next(iIndex, &vsrvInfo) )
		{
			break;
		}

		iIndex = vsrvInfo.no;
		
		if (useIf && intf.id != vsrvInfo.vif.ifindex)
		{
			continue;
		}
		if (useVrid && vrid != vsrvInfo.vrid)
		{
			continue;
		}
		ulSum++;

		if (ulSum > 1)
			XMORE(" \n");

		XMORE("Virtual Router ID : %d \n", vsrvInfo.vrid);
		
		XMORE("VLAN-IF           : %s \n", vsrvInfo.vif.ifname);
		
		XMORE("State             : %s \n", vrrp_state_str[vsrvInfo.state - 1]);

		XMORE("Virtual IP        :");
		for (j = 0; j < vsrvInfo.naddr; j++)
		{
			if (j != 0)
			{
				XMORE(" ");
			}
			IPV42STR(vsrvInfo.vaddr[j].addr, ipStr);
			XMORE(" %s", ipStr);
		}
		XMORE(" \n");

		XMORE("Priority          : %d \n", vsrvInfo.priority);

		if (vsrvInfo.preempt)
		{
			XMORE("Preempt           : YES \n");
		}
		else
		{
			XMORE("Preempt           : NO \n");
		}

		XMORE("Delay Time (secs) : %d \n", vsrvInfo.delay / VRRP_TIMER_HZ);

		XMORE("Timer (secs)      : %d \n", vsrvInfo.adver_int / VRRP_TIMER_HZ);

		#if 1
		XMORE("track interfaces  : \n");
		for (j = 0;j < vsrvInfo.niftrack; j++)
		{
			usVid = vsrvInfo.iftrack[j];
			if (usVid == 0)
			{
				continue;
			}
			intf.type = SYS_L3_INTFTYPE_VLAN;
			intf.id = usVid;
			usLinkup = IS_L3_INTF_LINKUP(&intf);
			if ( !usLinkup )
			{
				XMORE(" VLAN-IF %d, reduced priority: %d, status: down\n", usVid, vsrvInfo.pritrack[j]);
			}
			else
			{
				XMORE(" VLAN-IF %d, reduced priority: %d, status: up\n", usVid, vsrvInfo.pritrack[j]);
			}
		}
		#endif
	}
	
	if (ulSum)
	{
		XMORE("Total entries:%d \n", ulSum);		
	}
	else
	{
		XMORE("There is no virtual router or the virtual router does not exist. \n");		
	}

    return SYS_ERR_OK;
}

int32 cmd_l3_vrrp_interface_save(FILE *fp, sys_vid_t vid)
{
	char cmd[MAX_CMD_STRLEN];
    char tmpStr[SYS_BUF32_LEN];
	sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];
	char ipv4[32];
	vrrp_rt vsrvInfo;
	int i, j;
    int iIndex;
#ifndef GLN_INCLUDE_TRACK
	int trackvid;
#endif

    SYS_MEM_CLEAR(intf);

    intf.type = SYS_L3_INTFTYPE_VLAN;
    intf.id = vid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);
	
	iIndex = -1;
	for (i = 1; i <= VRRP_VSRV_SIZE_MAX; i++)
	{		
		if ( SYS_ERR_OK != sal_vrrp_vsrv_get_next(iIndex, &vsrvInfo) )
		{
			break;
		}
		
		iIndex = vsrvInfo.no;
		
		if (intf.id != vsrvInfo.vif.ifindex)
		{
			continue;
		}

		for (j = 0; j < vsrvInfo.naddr; j++)
		{	
			if (vsrvInfo.vaddr[j].addr)
			{
	            SYS_MEM_CLEAR(cmd);
				SYS_MEM_CLEAR(ipv4);
				IPV42STR(vsrvInfo.vaddr[j].addr, ipv4);
	            osal_sprintf(cmd, "vrrp vrid %d virtual-ip %s\n", vsrvInfo.vrid, ipv4);
	            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}
		}

		if (VRRP_PRIO_DFL != vsrvInfo.oldpriority)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "vrrp vrid %d priority %d\n", vsrvInfo.vrid, vsrvInfo.oldpriority);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		if (!vsrvInfo.preempt)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "no vrrp vrid %d preempt-mode\n", vsrvInfo.vrid);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}
		else if (vsrvInfo.delay)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "vrrp vrid %d preempt-mode timer delay %d\n", vsrvInfo.vrid, vsrvInfo.delay);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		if (VRRP_ADVER_DFL != vsrvInfo.adver_int / VRRP_TIMER_HZ)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "vrrp vrid %d timer advertise %d\n", vsrvInfo.vrid, vsrvInfo.adver_int / VRRP_TIMER_HZ);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		for (j = 0; j < vsrvInfo.niftrack; j++)
		{
#ifdef GLN_INCLUDE_TRACK
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "vrrp %d track %d", vsrvInfo.vrid, vsrvInfo.iftrack[j]);
			if ( VRRP_PRI_TRACK != vsrvInfo.pritrack[j] )
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " reduced %d", vsrvInfo.pritrack[j]);
				osal_strcat(cmd, tmpStr);
			}
			
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
#else
			trackvid = vsrvInfo.iftrack[j];
			if (trackvid == 0)
			{
				continue;
			}
			
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "vrrp vrid %d track vlan-interface %d", vsrvInfo.vrid, vsrvInfo.iftrack[j]);
			if ( VRRP_PRI_TRACK != vsrvInfo.pritrack[j] )
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " reduced %d", vsrvInfo.pritrack[j]);
				osal_strcat(cmd, tmpStr);
			}
			
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
#endif
		}

	}

	return SYS_ERR_OK;
}

