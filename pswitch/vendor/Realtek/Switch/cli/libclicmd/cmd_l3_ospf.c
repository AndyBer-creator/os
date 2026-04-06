
#include <libsal/sal_l3_route_ospf.h>
#include <libsal/sal_vlan.h>
#include <libipc/quaggaTask.h>

#include <libcmd/cmd.h>

uint32   g_ospf_prosseId;
uint32   g_ospf_areaId;

const char *ospf_abr_type_descr_str[] =
{
  "Unknown",
  "Standard (RFC2328)",
  "Alternative IBM",
  "Alternative Cisco",
  "Alternative Shortcut"
};
const char *ospf_shortcut_mode_descr_str[] =
{
  "Default",
  "Enabled",
  "Disabled"
};

const char *ospf_abr_type_str[] =
{
	"unknown",
	"standard",
	"ibm",
	"cisco",
	"shortcut"
};

char *ospf_network_type_str[] =
{
	"Null",
	"POINTOPOINT",
	"BROADCAST",
	"NBMA",
	"POINTOMULTIPOINT",
	"VIRTUALLINK",
	"LOOPBACK"
};

static const char *show_database_desc[] =
{
	"unknown",
	"Router Link States",
	"Net Link States",
	"Summary Link States",
	"ASBR-Summary Link States",
	"AS External Link States",
	"Group Membership LSA",
	"NSSA-external Link States",
#ifdef HAVE_OPAQUE_LSA
	"Type-8 LSA",
	"Link-Local Opaque-LSA",
	"Area-Local Opaque-LSA",
	"AS-external Opaque-LSA",
#endif 
};

static const char *show_database_header[] =
{
	"",
	"Link ID         ADV Router      Age  Seq#       CkSum  Link count",
	"Link ID         ADV Router      Age  Seq#       CkSum",
	"Link ID         ADV Router      Age  Seq#       CkSum  Route",
	"Link ID         ADV Router      Age  Seq#       CkSum",
	"Link ID         ADV Router      Age  Seq#       CkSum  Route",
	" --- header for Group Member ----",
	"Link ID         ADV Router      Age  Seq#       CkSum  Route",
#ifdef HAVE_OPAQUE_LSA
	" --- type-8 ---",
	"Opaque-Type/Id  ADV Router 	 Age  Seq#		 CkSum",
	"Opaque-Type/Id  ADV Router 	 Age  Seq#		 CkSum",
	"Opaque-Type/Id  ADV Router 	 Age  Seq#		 CkSum",
#endif 
};

const char *ospf_int_type_str[] =
{
  "unknown",		
  "point-to-point",
  "broadcast",
  "non-broadcast",
  "point-to-multipoint",
  "virtual-link",	
  "loopback"
};

static int32 cmd_str2L3IPv4(char *str, sys_l3_ipv4_t *pIp)
{
    char *buf[2];
    char tmp[SYS_BUF64_LEN];

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(tmp);

    osal_strcpy(tmp, str);

    buf[0] = strtok(tmp, "/");
    buf[1] = strtok(NULL, "/");

    SYS_ERR_CHK(STR2IPV4(buf[0], &pIp->addr));
    pIp->plen = SYS_STR2UINT(buf[1]);

    return SYS_ERR_OK;
}

#if 0
static int32 _cmd_l3_str2L3IPv6(char *str, sys_l3_ipv6_t *pIp)
{
    char *buf[2];
    char tmp[SYS_BUF128_LEN];

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(tmp);

    osal_strcpy(tmp, str);

    buf[0] = strtok(tmp, "/");
    buf[1] = strtok(NULL, "/");

    SYS_ERR_CHK(STR2IPV6(buf[0], &pIp->addr));
    pIp->plen = SYS_STR2UINT(buf[1]);

    return SYS_ERR_OK;
}
#endif
const char *
zebra_route_string(u_int zroute)
{
	switch (zroute)
	{
		case ZEBRA_ROUTE_SYSTEM:
			return "system";
		case ZEBRA_ROUTE_KERNEL:
			return "kernel";
		case ZEBRA_ROUTE_CONNECT:
			return "connected";
		case ZEBRA_ROUTE_STATIC:
			return "static";
		case ZEBRA_ROUTE_RIP:
			return "rip";
		case ZEBRA_ROUTE_RIPNG:
			return "ripng";
		case ZEBRA_ROUTE_OSPF:
			return "ospf";
		case ZEBRA_ROUTE_OSPF6:
			return "ospf6";
		case ZEBRA_ROUTE_ISIS:
			return "isis";
		case ZEBRA_ROUTE_BGP:
			return "bgp";
		case ZEBRA_ROUTE_HSLS:
			return "hsls";
		default:
			break;
	}
	return NULL;
}

static int show_ospf_area (OSPF_AREA_INFO_T *pAreaInfo)
{
    char        szTmpString[ 512 ] = { 0 };
    ulong_t k = 0; 
	char		area_id[ 16 ];

    SYS_MEM_CLEAR(area_id);
	IPV42STR(pAreaInfo->area_id, area_id);
    XMORE("\r\n Area ID: %s", area_id);

    if (pAreaInfo->bBackBone)
    {
        XMORE(" (Backbone)");
    }
    else
    {
        if (pAreaInfo->external_routing == FRR_OSPF_AREA_STUB)
        {
            XMORE(" (Stub%s%s)",
                 pAreaInfo->bNoSummary ? ", no summary" : "",
                 pAreaInfo->shortcut_configured ? "; " : "");
        }
        else if (pAreaInfo->external_routing == FRR_OSPF_AREA_NSSA)
        {
            XMORE(" (NSSA%s%s)",
                 pAreaInfo->bNoSummary ? ", no summary" : "",
                 pAreaInfo->shortcut_configured ? "; " : "");
        }

        XMORE("\r\n   Shortcutting mode: %s, S-bit consensus: %s",
                ospf_shortcut_mode_descr_str[pAreaInfo->shortcut_configured],
                pAreaInfo->shortcut_capability ? "ok" : "no");

    }

    XMORE("\r\n   Number of interfaces in this area: Total: %u, ""Active: %d",
        pAreaInfo->if_cnt,pAreaInfo->act_ints);

    if (pAreaInfo->external_routing == FRR_OSPF_AREA_NSSA)
    {
        XMORE("\r\n   It is an NSSA configuration.\r\n   Elected NSSA/ABR performs type-7/type-5 LSA translation.");
        if (!pAreaInfo->bABR)
        {
            XMORE("\r\n   It is not ABR, therefore not Translator.");
        }
        else if (pAreaInfo->NSSATranslatorState)
        {
            k = 0;
            SYS_MEM_CLEAR(szTmpString);
            k += sprintf(szTmpString+k, "\r\n   We are an ABR and ");
            if (pAreaInfo->NSSATranslatorRole == FRR_OSPF_NSSA_ROLE_CANDIDATE)
                k += sprintf(szTmpString+k, "the NSSA Elected Translator.");
            else if (pAreaInfo->NSSATranslatorRole == FRR_OSPF_NSSA_ROLE_ALWAYS)
                k += sprintf(szTmpString+k, "always an NSSA Translator.");
        }
        else
        {
            k = 0;
            SYS_MEM_CLEAR(szTmpString);
            k += sprintf(szTmpString+k, "\r\n   We are an ABR, but ");
            if (pAreaInfo->NSSATranslatorRole == FRR_OSPF_NSSA_ROLE_CANDIDATE)
                k += sprintf(szTmpString+k, "not the NSSA Elected Translator.");
            else
                k += sprintf(szTmpString+k, "never an NSSA Translator. ");
        }
    }
	XMORE("%s", szTmpString);

#if 0
    
    if (CHECK_FLAG (pAreaInfo->stub_router_state, OSPF_AREA_IS_STUB_ROUTED))
    {
      char timebuf[OSPF_TIME_DUMP_SIZE];
      vty_out (vty, "   Originating stub / maximum-distance Router-LSA%s",
               RN);
      if (CHECK_FLAG(pAreaInfo->stub_router_state, OSPF_AREA_ADMIN_STUB_ROUTED))
        vty_out (vty, "     Administratively activated (indefinitely)%s",
                 RN);
      if (pAreaInfo->t_stub_router)
        vty_out (vty, "     Active from startup, %s remaining%s",
                 ospf_timer_dump (pAreaInfo->t_stub_router, timebuf,
                                  sizeof(timebuf)), RN);
    }
#endif

    XMORE("\r\n   Number of fully adjacent neighbors in this area:"" %d", pAreaInfo->full_nbrs);

    k = 0;
    SYS_MEM_CLEAR(szTmpString);
    k += sprintf(szTmpString+k, "   Area has ");
    if (pAreaInfo->auth_type == FRR_OSPF_AUTH_NULL)
        k += sprintf(szTmpString+k, "no authentication");
    else if (pAreaInfo->auth_type == FRR_OSPF_AUTH_SIMPLE)
        k += sprintf(szTmpString+k, "simple password authentication");
    else if (pAreaInfo->auth_type == FRR_OSPF_AUTH_CRYPTOGRAPHIC)
        k += sprintf(szTmpString+k, "message digest authentication");
	XMORE("\r\n%s", szTmpString);

    if (!pAreaInfo->bBackBone)
    {
        XMORE("\r\n   Number of full virtual adjacencies going through"
         " this area: %d", pAreaInfo->full_vls);
    }
    
    XMORE("\r\n   SPF algorithm executed %u times", pAreaInfo->spf_calculation);

    XMORE("\r\n   Number of LSA %d", pAreaInfo->lsa_cnt);

    XMORE("\r\n   Number of router LSA %d. Checksum Sum 0x%08x",
       pAreaInfo->rtr_lsa_cnt,
       pAreaInfo->rtr_lsa_chksum);

    XMORE("\r\n   Number of network LSA %d. Checksum Sum 0x%08x",
           pAreaInfo->network_lsa_cnt,
           pAreaInfo->network_lsa_chksum);

    XMORE("\r\n   Number of summary LSA %d. Checksum Sum 0x%08x",
           pAreaInfo->summary_lsa_cnt,
           pAreaInfo->summary_lsa_chksum);

    XMORE("\r\n   Number of ASBR summary LSA %d. Checksum Sum 0x%08x",
           pAreaInfo->asbr_lsa_cnt,
           pAreaInfo->asbr_lsa_chksum);

    XMORE("\r\n   Number of NSSA LSA %d. Checksum Sum 0x%08x",
           pAreaInfo->nssa_lsa_cnt,
           pAreaInfo->nssa_lsa_chksum);

    XMORE("\r\n   Number of opaque link LSA %d. Checksum Sum 0x%08x",
           pAreaInfo->link_lsa_cnt,
           pAreaInfo->link_lsa_chksum);

    XMORE("\r\n  Number of opaque area LSA %d. Checksum Sum 0x%08x",
           pAreaInfo->area_lsa_cnt,
           pAreaInfo->area_lsa_chksum);

    XMORE("\r\n");

    return 0;

}

int32 cmd_l3_router_ospf_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint16 ProcessID = 0;

	if (argc)
	{
		ProcessID = SYS_STR2UINT(argv[0]);
	}
	else
	{
		ProcessID = 1;
	}

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_routeV4_ospf_cloce(ProcessID);
	}
	else
	{
		rc = sal_l3_routeV4_ospf_open(ProcessID);

		vty->node = OSPF_NODE;
		SYS_MEM_CLEAR(vty->exPrompt);
		osal_sprintf(vty->exPrompt, "%d", ProcessID);
		g_ospf_prosseId = ProcessID;
	}
    return rc;
}

int32 cmd_l3_ospf_area_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 ipformat = 0;
	uint32 area_id = 0;
	char areaStr[32];

	if (NULL != osal_strchr(argv[0], '.'))
        ipformat = TRUE;

	SYS_MEM_CLEAR(areaStr);

    if (ipformat)
    {
        STR2IPV4(argv[0], &area_id);
        osal_sprintf(areaStr, "%s", argv[0]);
    }
    else
    {
    	area_id = SYS_STR2UINT(argv[0]);
		IPV42STR(area_id, areaStr);
    }

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_set(DISABLED, area_id, g_ospf_prosseId);
	}
	else
	{
		vty->node = AREA_NODE;
		SYS_MEM_CLEAR(vty->exPrompt);
		osal_sprintf(vty->exPrompt, "%d-area-%s", g_ospf_prosseId, areaStr);
		g_ospf_areaId = area_id;
		rc = sal_l3_ospf_area_set(ENABLED, area_id, g_ospf_prosseId);
	}
    return rc;
}

int32 cmd_l3_ospf_network_area_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	sys_l3_ipv4_t dstIp;
	sys_ipv4_t addr;
	sys_ipv4_t mask;

	cmd_str2L3IPv4(argv[0], &dstIp);
	addr = dstIp.addr;
	mask = IPV4_PFX2MASK(dstIp.plen);

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_network_area_set(ENABLED, addr, mask, g_ospf_areaId, g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_network_area_set(DISABLED, addr, mask, g_ospf_areaId, g_ospf_prosseId);
	}
    return rc;
}

int32 cmd_l3_ospf_area_range_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 cost = -1;
	uint32 advertise = 1;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_range_set(ENABLED, g_ospf_areaId, argv[0], advertise, cost, g_ospf_prosseId);
	}
	else
	{
		if (argc == 2)
		{
			if ((CMD_TOKENNUM_GET(cmd->vline) > 2))
			{
				if ('a' == CMD_TOKENSTR_GET(cmd->vline,2)[0])
				{
					advertise = 1;
				}
				else if ('n' == CMD_TOKENSTR_GET(cmd->vline,2)[0])
				{
					advertise = 0;
				}
				else if ('c' == CMD_TOKENSTR_GET(cmd->vline,2)[0])
				{
					cost = SYS_STR2UINT(argv[1]);
				}
			}
		}
        else if (argc == 3)
        {
	        if ('a' == CMD_TOKENSTR_GET(cmd->vline,2)[0])
			{
				advertise = 1;
			}
			else if ('n' == CMD_TOKENSTR_GET(cmd->vline,2)[0])
			{
				advertise = 0;
			}

			cost = SYS_STR2UINT(argv[2]);
        }

		rc = sal_l3_ospf_area_range_set(DISABLED, g_ospf_areaId, argv[0], advertise, cost, g_ospf_prosseId);
	}
    return rc;
}

int32 cmd_l3_ospf_area_range_substitute_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_range_substitute_set(ENABLED, g_ospf_areaId, argv[0], argv[1], g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_area_range_substitute_set(DISABLED, g_ospf_areaId, argv[0], argv[1], g_ospf_prosseId);
	}
	return rc;
}

int32 cmd_l3_ospf_area_vlink_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	struct ospf_vllink_config_data vl_config = {0};
	int i;

	STR2IPV4(argv[0], &vl_config.vl_peer);
    vl_config.format = OSPF_AREA_ID_FORMAT_DECIMAL;
    vl_config.auth_type = OSPF_AUTH_CMD_NOTSEEN;
	vl_config.area_id = g_ospf_areaId;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_vlink_set(ENABLED, &vl_config, g_ospf_prosseId);
	}
	else
	{
		if (argc > 1)
		{
			for (i = 1; i < argc; i++)
			{
				if ('h' == CMD_TOKENSTR_GET(cmd->vline, i * 2)[0])
				{
					vl_config.hello_interval = SYS_STR2UINT(argv[i]);
				}
				else if ('r' == CMD_TOKENSTR_GET(cmd->vline, i * 2)[0])
				{
					vl_config.retransmit_interval = SYS_STR2UINT(argv[i]);
				}
				else if ('t' == CMD_TOKENSTR_GET(cmd->vline, i * 2)[0])
				{
					vl_config.transmit_delay = SYS_STR2UINT(argv[i]);
				}
				else if ('d' == CMD_TOKENSTR_GET(cmd->vline, i * 2)[0])
				{
					vl_config.dead_interval = SYS_STR2UINT(argv[i]);
				}
				else if ('a' == CMD_TOKENSTR_GET(cmd->vline, i * 2)[0])
				{
					vl_config.auth_type = OSPF_AUTH_SIMPLE;
					strncpy (vl_config.auth_key, argv[i], OSPF_AUTH_SIMPLE_SIZE);
				}
				else if ('m' == CMD_TOKENSTR_GET(cmd->vline, i * 2)[0])
				{
	                vl_config.auth_type = OSPF_AUTH_CRYPTOGRAPHIC;
					vl_config.crypto_key_id = strtol (argv[i], NULL, 10);
					if (vl_config.crypto_key_id < 0)
						return SYS_ERR_FAILED;

					strncpy (vl_config.md5_key, argv[i + 1], OSPF_AUTH_MD5_SIZE);

					i += 1;
				}
			}
		}

		if (vl_config.dead_interval && vl_config.hello_interval)
		{
			if( vl_config.dead_interval < 4 * vl_config.hello_interval )
			{
				return SYS_ERR_FAILED;
			}
		}

		rc = sal_l3_ospf_area_vlink_set(DISABLED, &vl_config, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_area_stub_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    int AreaSummaryStatus = 1;  

	if (CMD_IS_NO_FORM_EXIST())
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 2) && ('n' == CMD_TOKENSTR_GET(cmd->vline,2)[0]))
			AreaSummaryStatus = 0;

		rc = sal_l3_ospf_area_stub_set(ENABLED, g_ospf_areaId, AreaSummaryStatus, g_ospf_prosseId);
	}
	else
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 1) && ('n' == CMD_TOKENSTR_GET(cmd->vline,1)[0]))
			AreaSummaryStatus = 0;

		rc = sal_l3_ospf_area_stub_set(DISABLED, g_ospf_areaId, AreaSummaryStatus, g_ospf_prosseId);
	}
	return rc;
}

int32 cmd_l3_ospf_area_nssa_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    int AreaSummaryStatus = 1;  
	char *role = NULL;

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (CMD_TOKENNUM_GET(cmd->vline) > 2)
		{
			AreaSummaryStatus = 0;
		}

		rc = sal_l3_ospf_area_nssa_set(ENABLED, g_ospf_areaId, role, AreaSummaryStatus, g_ospf_prosseId);
	}
	else
	{
		if (CMD_TOKENNUM_GET(cmd->vline) > 2)
		{
			role = CMD_TOKENSTR_GET(cmd->vline, 1);
			AreaSummaryStatus = 0;
		}
		else if (CMD_TOKENNUM_GET(cmd->vline) > 1)
		{
			if ('t' == CMD_TOKENSTR_GET(cmd->vline,1)[0])
			{
				role = CMD_TOKENSTR_GET(cmd->vline, 1);
			}
			else
			{
				AreaSummaryStatus = 0;
			}
		}

		rc = sal_l3_ospf_area_nssa_set(DISABLED, g_ospf_areaId, role, AreaSummaryStatus, g_ospf_prosseId);
	}
	return rc;
}

int32 cmd_l3_ospf_area_default_cost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 cost =  0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_default_cost_set(ENABLED, g_ospf_areaId, cost, g_ospf_prosseId);
	}
	else
	{
		cost = SYS_STR2UINT(argv[0]);
		rc = sal_l3_ospf_area_default_cost_set(DISABLED, g_ospf_areaId, cost, g_ospf_prosseId);
	}
	return rc;
}

int32 cmd_l3_ospf_area_export_list_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_export_list_set(ENABLED, g_ospf_areaId, argv[0], g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_area_export_list_set(DISABLED, g_ospf_areaId, argv[0], g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_area_import_list_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_import_list_set(ENABLED, g_ospf_areaId, argv[0], g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_area_import_list_set(DISABLED, g_ospf_areaId, argv[0], g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_area_filter_list_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_filter_list_set(ENABLED, g_ospf_areaId, argv[0], argv[1], g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_area_filter_list_set(DISABLED, g_ospf_areaId, argv[0], argv[1], g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_area_authentication_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    int auth_type = OSPF_AUTH_NULL;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_authentication_set(ENABLED, g_ospf_areaId, auth_type, g_ospf_prosseId);
	}
	else
	{
		if (0 == osal_strncmp(argv[0], "simple", 6))
			auth_type = OSPF_AUTH_SIMPLE;
		else
			auth_type = OSPF_AUTH_CRYPTOGRAPHIC;

		rc = sal_l3_ospf_area_authentication_set(DISABLED, g_ospf_areaId, auth_type, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_area_shortcut_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_area_shortcut_set(ENABLED, g_ospf_areaId, argv[0], g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_area_shortcut_set(DISABLED, g_ospf_areaId, argv[0], g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_router_id_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	sys_ipv4_t router_id = 0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_router_id_set(ENABLED, router_id, g_ospf_prosseId);
	}
	else
	{
		STR2IPV4(argv[0], &router_id);
		rc = sal_l3_ospf_router_id_set(DISABLED, router_id, g_ospf_prosseId);
	}
    return rc;
}

int32 cmd_l3_ospf_abr_type_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_abr_type_set(ENABLED, NULL, g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_abr_type_set(DISABLED, argv[0], g_ospf_prosseId);
	}
    return rc;
}

int32 cmd_l3_ospf_log_adjacency_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	int detail = 0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_log_adjacency_set(ENABLED, detail, g_ospf_prosseId);
	}
	else
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 1))
		{
			detail = 1;
		}

		rc = sal_l3_ospf_log_adjacency_set(DISABLED, detail, g_ospf_prosseId);
	}
	return rc;
}

int32 cmd_l3_ospf_rfc1583compatibility_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_rfc1583compatibility_set(ENABLED, g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_rfc1583compatibility_set(DISABLED, g_ospf_prosseId);
	}
	return rc;
}

int32 cmd_l3_ospf_spf_timer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 delay = 0, hold = 0, max = 0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_spf_timer_set(ENABLED, delay, hold, max, g_ospf_prosseId);
	}
	else
	{
		delay = SYS_STR2UINT(argv[0]);
		hold = SYS_STR2UINT(argv[1]);
		max = SYS_STR2UINT(argv[2]);
		rc = sal_l3_ospf_spf_timer_set(DISABLED, delay, hold, max, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_static_neighbor_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 nbr_id = 0, priority = 0, interval = 0;
    char *ridx = NULL;
    uint32 ridx_len = 0;
	int i;

	STR2IPV4(argv[0], &nbr_id);

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_static_neighbor_set(ENABLED, nbr_id, priority, interval, g_ospf_prosseId);
	}
	else
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 2))
		{
			for (i = 2; i < CMD_TOKENNUM_GET(cmd->vline); i++)
			{
				ridx = CMD_TOKENSTR_GET(cmd->vline, i);
				ridx_len = osal_strlen(ridx);
				if (osal_strncmp (ridx, "priority", ridx_len) == 0)
				{
					priority = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
				}
				else if (osal_strncmp (ridx, "poll-interval", ridx_len) == 0)
				{
					priority = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
				}
			}
		}

		rc = sal_l3_ospf_static_neighbor_set(DISABLED, nbr_id, priority, interval, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_refresh_timer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 time = 0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_refresh_timer_set(ENABLED, time, g_ospf_prosseId);
	}
	else
	{
		time = SYS_STR2UINT(argv[0]);
		rc = sal_l3_ospf_refresh_timer_set(DISABLED, time, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_auto_cost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 refbw = 0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_auto_cost_set(ENABLED, refbw, g_ospf_prosseId);
	}
	else
	{
		refbw = SYS_STR2UINT(argv[0]);
		rc = sal_l3_ospf_auto_cost_set(DISABLED, refbw, g_ospf_prosseId);
	}

	return rc;
}

#define VALID_POINTER "str"
int32 cmd_l3_ospf_redistribute_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	char *protoName = NULL;
	char *metric_str = NULL;
    char *type_str = NULL;
	char *rmapName = NULL;
    char *ridx = NULL;
    uint32 ridx_len = 0;
	int i;

	protoName = argv[0];

	if (CMD_IS_NO_FORM_EXIST())
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 3))
		{
			for (i=3; i < CMD_TOKENNUM_GET(cmd->vline); i++)
			{
				ridx = CMD_TOKENSTR_GET(cmd->vline, i);
				ridx_len = osal_strlen(ridx);
				if (osal_strncmp (ridx, "metric", ridx_len) == 0)
				{
					metric_str = VALID_POINTER;
				}
				else if (osal_strncmp (ridx, "metric-type", ridx_len) == 0)
				{
					type_str = VALID_POINTER;
				}
				else if (osal_strncmp (ridx, "route-map", ridx_len) == 0)
				{
					rmapName = VALID_POINTER;
				}
			}
		}
		rc = sal_l3_ospf_redistribute_set(ENABLED, protoName, metric_str, type_str, rmapName, g_ospf_prosseId);
	}
	else
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 2))
		{
			for (i=2; i < CMD_TOKENNUM_GET(cmd->vline); i++)
			{
				ridx = CMD_TOKENSTR_GET(cmd->vline, i);
				ridx_len = osal_strlen(ridx);
				if (osal_strncmp (ridx, "metric", ridx_len) == 0)
				{
					i++;
					metric_str = CMD_TOKENSTR_GET(cmd->vline,i);
				}
				else if (osal_strncmp (ridx, "metric-type", ridx_len) == 0)
				{
					i++;
					type_str = CMD_TOKENSTR_GET(cmd->vline,i);
				}
				else if (osal_strncmp (ridx, "route-map", ridx_len) == 0)
				{
					i++;
					rmapName = CMD_TOKENSTR_GET(cmd->vline,i);
				}
			}
		}

		rc = sal_l3_ospf_redistribute_set(DISABLED, protoName, metric_str, type_str, rmapName, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_dft_info_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    sys_enable_t always = FALSE;
    char *metric_str = NULL;
    char *type_str = NULL;
    char *rmapName = NULL;
    char *ridx = NULL;
    uint32 ridx_len = 0;
	int i;

	if (CMD_IS_NO_FORM_EXIST())
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 2))
		{
			for (i=2; i < CMD_TOKENNUM_GET(cmd->vline); i++)
			{
				ridx = CMD_TOKENSTR_GET(cmd->vline, i);
				ridx_len = osal_strlen(ridx);
				if (osal_strncmp (ridx, "metric", ridx_len) == 0)
				{
					metric_str = VALID_POINTER;
				}
				else if (osal_strncmp (ridx, "metric-type", ridx_len) == 0)
				{
					type_str = VALID_POINTER;
				}
				else if (osal_strncmp (ridx, "route-map", ridx_len) == 0)
				{
					rmapName = VALID_POINTER;
				}
				else if (osal_strncmp (ridx, "always", ridx_len) == 0)
				{
					always = TRUE;
				}
			}
		}
		rc = sal_l3_ospf_dft_info_set(ENABLED, always, metric_str, type_str, rmapName, g_ospf_prosseId);
	}
	else
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 2))
		{
			for (i=2; i < CMD_TOKENNUM_GET(cmd->vline); i++)
			{
				ridx = CMD_TOKENSTR_GET(cmd->vline, i);
				ridx_len = osal_strlen(ridx);
				if (osal_strncmp (ridx, "metric", ridx_len) == 0)
				{
					i++;
					metric_str = CMD_TOKENSTR_GET(cmd->vline,i);
				}
				else if (osal_strncmp (ridx, "metric-type", ridx_len) == 0)
				{
					i++;
					type_str = CMD_TOKENSTR_GET(cmd->vline,i);
				}
				else if (osal_strncmp (ridx, "route-map", ridx_len) == 0)
				{
					i++;
					rmapName = CMD_TOKENSTR_GET(cmd->vline,i);
				}
				else if (osal_strncmp (ridx, "always", ridx_len) == 0)
				{
					always = TRUE;
				}
			}
		}
		rc = sal_l3_ospf_dft_info_set(DISABLED, always, metric_str, type_str, rmapName, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_distri_list_out_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	char *alistName = NULL;
	char *protoName = NULL;

	alistName = argv[0];
	protoName = argv[1];

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_distri_list_out_set(ENABLED, alistName, protoName, g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_distri_list_out_set(DISABLED, alistName, protoName, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_default_metric_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 default_metric = 0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_default_metric_set(ENABLED, default_metric, g_ospf_prosseId);
	}
	else
	{
		default_metric = SYS_STR2UINT(argv[0]);
		rc = sal_l3_ospf_default_metric_set(DISABLED, default_metric, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_distance_ospf_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 intra_dis = 0;
    uint32 inter_dis = 0;
    uint32 external_dis = 0;
    char *ridx = NULL;
    uint32 ridx_len = 0;
	int i;

	if (CMD_IS_NO_FORM_EXIST())
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 3))
		{
			for (i=3; i < CMD_TOKENNUM_GET(cmd->vline); i++)
			{
				ridx = CMD_TOKENSTR_GET(cmd->vline, i);
				ridx_len = osal_strlen(ridx);
				if (osal_strncmp (ridx, "intra-area", ridx_len) == 0)
				{
					intra_dis = 0;
				}
				else if (osal_strncmp (ridx, "inter-area", ridx_len) == 0)
				{
					inter_dis = 0;
				}
				else if (osal_strncmp (ridx, "external", ridx_len) == 0)
				{
					external_dis = 0;
				}
			}
		}

		rc = sal_l3_ospf_distance_ospf_set(ENABLED, intra_dis, inter_dis, external_dis, g_ospf_prosseId);
	}
	else
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 2))
		{
			for (i=2; i < CMD_TOKENNUM_GET(cmd->vline); i++)
			{
				ridx = CMD_TOKENSTR_GET(cmd->vline, i);
				ridx_len = osal_strlen(ridx);
				if (osal_strncmp (ridx, "intra-area", ridx_len) == 0)
				{
					intra_dis = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
				}
				else if (osal_strncmp (ridx, "inter-area", ridx_len) == 0)
				{
					inter_dis = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
				}
				else if (osal_strncmp (ridx, "external", ridx_len) == 0)
				{
					external_dis = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
				}
			}
		}

		rc = sal_l3_ospf_distance_ospf_set(DISABLED, intra_dis, inter_dis, external_dis, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_distance_prefix_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	char *dis_str = NULL;
	char *prefix_str = NULL;
	char *alistName = NULL;

	dis_str = argv[0];
	prefix_str = argv[1];
	if (argc >1)
	{
		alistName = argv[2];
	}

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_distance_prefix_set(ENABLED, dis_str, prefix_str, alistName, g_ospf_prosseId);
	}
	else
	{
		rc = sal_l3_ospf_distance_prefix_set(DISABLED, dis_str, prefix_str, alistName, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_distance_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 distance = 0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_ospf_distance_set(ENABLED, distance, g_ospf_prosseId);
	}
	else
	{
		distance = SYS_STR2UINT(argv[0]);
		rc = sal_l3_ospf_distance_set(DISABLED, distance, g_ospf_prosseId);
	}

	return rc;
}

int32 cmd_l3_ospf_passive_interface_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	sys_ipv4_t addr = 0;
    sys_vid_t vid;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc)
		{
			vid = SYS_STR2UINT(argv[0]);
			intf.type = SYS_L3_INTFTYPE_VLAN;
			intf.id = vid;

			if (IS_L3_INTF_EXIST(&intf))
			{
				sys_l3_intfId2Str(&intf, ifStr);
				if (argc == 2)
					STR2IPV4(argv[1], &addr);

				rc = sal_l3_ospf_passive_interface_set(ENABLED, ifStr, addr);
			}
		}
		else
		{
			rc = sal_l3_ospf_passive_interface_default(ENABLED);

		}
	}
	else
	{
		if (argc)
		{
			vid = SYS_STR2UINT(argv[0]);
			intf.type = SYS_L3_INTFTYPE_VLAN;
			intf.id = vid;

			if (IS_L3_INTF_EXIST(&intf))
			{
				sys_l3_intfId2Str(&intf, ifStr);
				if (argc == 2)
					STR2IPV4(argv[1], &addr);

				rc = sal_l3_ospf_passive_interface_set(DISABLED, ifStr, addr);
			}
		}
		else
		{
			rc = sal_l3_ospf_passive_interface_default(DISABLED);
		}
	}

	return rc;
}

int32 cmd_ip_ospf_authentication_mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    int auth_type = OSPF_AUTH_NOTSET;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{

		rc = sal_ip_ospf_authentication_mode_set(ENABLED, ifStr, auth_type, addr);
	}
	else
	{
		if (argc == 1)
		{
			if (osal_strncmp (argv[0], "simple", osal_strlen("simple")) == 0)
			{
				auth_type = OSPF_AUTH_SIMPLE;
			}
			else if (osal_strncmp (argv[0], "message-digest", osal_strlen("message-digest")) == 0)
			{
				auth_type = OSPF_AUTH_CRYPTOGRAPHIC;
			}
			if (osal_strncmp (argv[0], "null", osal_strlen("null")) == 0)
			{
				auth_type = OSPF_AUTH_NULL;
			}			 
		}
		else if(argc > 1)
		{
			
		}

		rc = sal_ip_ospf_authentication_mode_set(DISABLED, ifStr, auth_type, addr);
	}

    return rc;
}

int32 cmd_ip_ospf_authentication_simple_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc)
			STR2IPV4(argv[0], &addr);

		rc = sal_ip_ospf_authentication_simple_set(ENABLED, ifStr, NULL, addr);
	}
	else
	{
		if (argc > 1)
			STR2IPV4(argv[1], &addr);
		rc = sal_ip_ospf_authentication_simple_set(DISABLED, ifStr, argv[0], addr);
	}
    return rc;
}

int32 cmd_ip_ospf_authentication_md5_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 keyId = 0;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	keyId = SYS_STR2UINT(argv[0]);

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc > 1)
			STR2IPV4(argv[1], &addr);
		rc = sal_ip_ospf_authentication_md5_set(ENABLED, ifStr, keyId, NULL, addr);
	}
	else
	{
		if (argc > 2)
			STR2IPV4(argv[2], &addr);
		rc = sal_ip_ospf_authentication_md5_set(DISABLED, ifStr, keyId, argv[1], addr);
	}

    return rc;
}

int32 cmd_ip_ospf_cost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 val = OSPF_OUTPUT_COST_DEFAULT;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc)
			STR2IPV4(argv[0], &addr);
		rc = sal_ip_ospf_cost_set(ENABLED, ifStr, val, addr);
	}
	else
	{
		val = SYS_STR2UINT(argv[0]);
		if (argc > 1)
			STR2IPV4(argv[1], &addr);
		rc = sal_ip_ospf_cost_set(DISABLED, ifStr, val, addr);
	}
	return rc;
}

int32 cmd_ip_ospf_priority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 val = OSPF_ROUTER_PRIORITY_DEFAULT;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc)
			STR2IPV4(argv[0], &addr);
		rc = sal_ip_ospf_priority_set(ENABLED, ifStr, val, addr);
	}
	else
	{
		val = SYS_STR2UINT(argv[0]);
		if (argc)
			STR2IPV4(argv[1], &addr);
		rc = sal_ip_ospf_priority_set(DISABLED, ifStr, val, addr);
	}
	return rc;
}

int32 cmd_ip_ospf_hello_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 val = OSPF_HELLO_INTERVAL_DEFAULT;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc)
			STR2IPV4(argv[0], &addr);
		rc = sal_ip_ospf_hello_set(ENABLED, ifStr, val, addr);
	}
	else
	{
		val = SYS_STR2UINT(argv[0]);
		if (argc > 1)
			STR2IPV4(argv[1], &addr);
		rc = sal_ip_ospf_hello_set(DISABLED, ifStr, val, addr);
	}
	return rc;
}

int32 cmd_ip_ospf_dead_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    char *interval_str = NULL;
    char *fast_hello_str = NULL;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];
    char tmpStr[SYS_BUF32_LEN];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc)
			STR2IPV4(argv[0], &addr);

		osal_sprintf(tmpStr, "%u", OSPF_ROUTER_DEAD_INTERVAL_DEFAULT);
		rc = sal_ip_ospf_dead_set(ENABLED, ifStr, tmpStr, fast_hello_str, addr);
	}
	else
	{
		if ((CMD_TOKENNUM_GET(cmd->vline) > 4))
		{
			fast_hello_str = argv[0];
		}
		else
		{
			interval_str = argv[0];
		}

		if (argc > 1)
			STR2IPV4(argv[1], &addr);
		rc = sal_ip_ospf_dead_set(DISABLED, ifStr, interval_str, fast_hello_str, addr);
	}
	return rc;
}

int32 cmd_ip_ospf_transmit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 val = OSPF_RETRANSMIT_INTERVAL_DEFAULT;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc)
			STR2IPV4(argv[0], &addr);
		rc = sal_ip_ospf_transmit_set(ENABLED, ifStr, val, addr);
	}
	else
	{
		val = SYS_STR2UINT(argv[0]);
		if (argc > 1)
			STR2IPV4(argv[1], &addr);
		rc = sal_ip_ospf_transmit_set(DISABLED, ifStr, val, addr);
	}
	return rc;
}

int32 cmd_ip_ospf_retransmit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    uint32 val = OSPF_TRANSMIT_DELAY_DEFAULT;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc)
			STR2IPV4(argv[0], &addr);
		rc = sal_ip_ospf_retransmit_set(ENABLED, ifStr, val, addr);
	}
	else
	{
		val = SYS_STR2UINT(argv[0]);
		if (argc > 1)
			STR2IPV4(argv[1], &addr);
		rc = sal_ip_ospf_retransmit_set(DISABLED, ifStr, val, addr);
	}
	return rc;
}

int32 cmd_ip_ospf_network_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
    int type = OSPF_IFTYPE_BROADCAST;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_ip_ospf_network_set(ENABLED, ifStr, type);
	}
	else
	{
		if (argc > 0)
		{
			if (osal_strncmp (argv[0], "broadcast", osal_strlen("broadcast")) == 0)
			{
				type = OSPF_IFTYPE_BROADCAST;
			}
			else if (osal_strncmp (argv[0], "non-broadcast", osal_strlen("non-broadcast")) == 0)
			{
				type = OSPF_IFTYPE_NBMA;
			}
			else if (osal_strncmp (argv[0], "point-to-multipoint", osal_strlen("point-to-multipoint")) == 0)
			{
				type = OSPF_IFTYPE_POINTOMULTIPOINT;
			}
			else if (osal_strncmp (argv[0], "point-to-point", osal_strlen("point-to-point")) == 0)
			{
				type = OSPF_IFTYPE_POINTOPOINT;
			}
		}
		rc = sal_ip_ospf_network_set(DISABLED, ifStr, type);
	}
	return rc;
}

int32 cmd_ip_ospf_mtu_ignore_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	sys_ipv4_t addr = 0;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];

    SYS_MEM_CLEAR(intf);

    intf.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == intf.type)
        intf.id = g_selVid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	if (CMD_IS_NO_FORM_EXIST())
	{
		if (argc)
			STR2IPV4(argv[0], &addr);
		rc = sal_ip_ospf_mtu_ignore_set(ENABLED, ifStr, addr);
	}
	else
	{
		if (argc > 1)
			STR2IPV4(argv[0], &addr);
		rc = sal_ip_ospf_mtu_ignore_set(DISABLED, ifStr, addr);
	}
	return rc;
}

int show_ip_ospf_route_network (uint16 process)
{
    char szName[128];
    OSPF_ROUTE_PATH_INFO_T *path;
    int i ;
    char buf1[19];
    int                       rc;
    char szTmpString[512];
    OSPF_ROUTE_INFO_T routeInfo;
    char        szDstIP[ 16 ] = { 0 };
    char        szArea[ 32 ] = { 0 };
	char		szNextIp[ 32 ] = { 0 };

	SYS_MEM_CLEAR(szTmpString);
    sprintf(szTmpString, "\r\n============ OSPF network routing table ============\r\n");
    XMORE("%s", szTmpString);

    routeInfo.process = process;
    routeInfo.DsIPv4 = 0;
    routeInfo.prefixlen = 32;
    routeInfo.route_type = OSPF_ROUTE_TYPE_NETWORK;
    while (1)
    {
        rc = sal_ospf_route_info_get(DATA_OP_GETNEXT, &routeInfo);
        if (SYS_ERR_OK != rc)
        {
            break;
        }
        SYS_MEM_CLEAR(szDstIP);
  		IPV42STR(routeInfo.DsIPv4, szDstIP);
        snprintf (buf1, 19, "%s/%d", szDstIP, routeInfo.prefixlen);

		SYS_MEM_CLEAR(szTmpString);
        switch (routeInfo.path_type)
        {
            case FRR_OSPF_PATH_INTER_AREA:
                if (routeInfo.type == FRR_OSPF_DESTINATION_NETWORK)
                {
                	SYS_MEM_CLEAR(szArea);
                	IPV42STR(routeInfo.area_id, szArea);
                    sprintf(szTmpString, "N IA %-18s    [%u] area: %s\r\n", buf1, routeInfo.cost, szArea);
                }
                else if (routeInfo.type == FRR_OSPF_DESTINATION_DISCARD)
                {
                    sprintf(szTmpString, "D IA %-18s    Discard entry\r\n", buf1);
                }
 				XMORE("%s", szTmpString);

                break;
            case FRR_OSPF_PATH_INTRA_AREA:
                SYS_MEM_CLEAR(szArea);
               	IPV42STR(routeInfo.area_id, szArea);
                sprintf(szTmpString, "N    %-18s    [%u] area: %s\r\n", buf1, routeInfo.cost, szArea);
				XMORE("%s", szTmpString);

                break;
            default:
                break;
        }

		SYS_MEM_CLEAR(szTmpString);
        if (routeInfo.type == FRR_OSPF_DESTINATION_NETWORK)
        {
            for (i = 0; i < routeInfo.path_num; i++)
            {
                path = &routeInfo.path[i];
                SYS_MEM_CLEAR(szName);
                rc = sys_l3_str2IntfName(path->ifname, szName);
                if(rc != SYS_ERR_OK)
                	continue;
                if (path->nexthop == 0)
                {
                    sprintf(szTmpString, "%24s   directly attached to %s\r\n",
                         "", szName);
                }
                else
                {
                	SYS_MEM_CLEAR(szNextIp);
                	IPV42STR(path->nexthop, szNextIp);
                    sprintf(szTmpString, "%24s   via %s, %s\r\n", "", szNextIp, szName);
                }
				XMORE("%s", szTmpString);
            }
        }
    }

    return 0;
}

int show_ip_ospf_route_router (uint16 process)
{
    char szName[128];
    OSPF_ROUTE_PATH_INFO_T *path;
    int i, j;
    int                       rc;
    char szTmpString[512];
    int k = 0;
    OSPF_ROUTE_INFO_T routeInfo;
    OSPF_RTR_ROUTE_INFO_T rtrRouteInfo;
	char		szDstIP[ 32 ] = { 0 };
	char        szArea[32];
	char		szNextIp[ 32 ] = { 0 };
    sprintf(szTmpString, RN"============ OSPF router routing table ============%s",
             RN);
	XMORE("%s", szTmpString);

    routeInfo.process = process;
    routeInfo.DsIPv4 = 0;
    routeInfo.prefixlen = 32;
    routeInfo.route_type = OSPF_ROUTE_TYPE_ROUTER;
    while (1)
    {
        rc = sal_ospf_route_info_get(DATA_OP_GETNEXT, &routeInfo);
        if (SYS_ERR_OK != rc)
        {
            break;
        }

        k = 0;
        SYS_MEM_CLEAR(szDstIP);
    	IPV42STR(routeInfo.DsIPv4, szDstIP);
        k += sprintf(szTmpString+k, "R    %-15s    ", szDstIP);

        for (i = 1; ; i++)
        {
        	rtrRouteInfo.process = routeInfo.process;
            rtrRouteInfo.DsIPv4 = routeInfo.DsIPv4;
            rtrRouteInfo.prefixlen = routeInfo.prefixlen;
            rtrRouteInfo.route_index = i;
            rc = sal_ospf_route_border_info_get(DATA_OP_GET, &rtrRouteInfo);
            if (SYS_ERR_OK != rc)
            {
                break;
            }

            if (i > 1)
            {
                k = 0;
                k += sprintf(szTmpString+k, "%24s", "");
            }
            
            SYS_MEM_CLEAR(szArea);
            IPV42STR(rtrRouteInfo.area_id, szArea);
            k += sprintf(szTmpString+k, "%s [%u] area: %s%s%s%s",
                     (rtrRouteInfo.path_type == FRR_OSPF_PATH_INTER_AREA ? "IA" : "  "),
                      rtrRouteInfo.cost, szArea,
                     (rtrRouteInfo.flags & FRR_ROUTER_LSA_BORDER ? ", ABR" : ""),
                     (rtrRouteInfo.flags & FRR_ROUTER_LSA_EXTERNAL ? ", ASBR" : ""),
                     RN);
			XMORE("%s", szTmpString);

            for (j = 0; j < rtrRouteInfo.path_num; j++)
            {
                path = &rtrRouteInfo.path[j];
               	SYS_MEM_CLEAR(szName);
                rc = sys_l3_str2IntfName(path->ifname, szName);
                if(rc != SYS_ERR_OK)
                	continue;
               	SYS_MEM_CLEAR(szTmpString);
                if (path->nexthop == 0)
                {
                    sprintf(szTmpString, "%24s   directly attached to %s%s",
                         "", szName, RN);
                }
                else
                {
                	SYS_MEM_CLEAR(szNextIp);
                	IPV42STR(path->nexthop, szNextIp);
                    sprintf(szTmpString, "%24s   via %s, %s%s", "", szNextIp, szName, RN);
                }
          		XMORE("%s", szTmpString);
            }
        }
    }

    return 0;
}

int show_ip_ospf_route_external (uint16 process)
{
    char szName[128];

    OSPF_ROUTE_PATH_INFO_T *path;
    int i ;
    char buf1[19];
    int                       rc;
    char szTmpString[512];
    OSPF_ROUTE_INFO_T routeInfo;
	char		szDstIP[ 32 ];
	char		szNextIp[ 32 ];

    sprintf(szTmpString, RN"============ OSPF external routing table ============%s",
            RN);
	XMORE("%s", szTmpString);

    routeInfo.process = process;
    routeInfo.DsIPv4 = 0;
    routeInfo.prefixlen = 32;
    routeInfo.route_type = OSPF_ROUTE_TYPE_EXTERNAL;
    while (1)
    {
        rc = sal_ospf_route_info_get(DATA_OP_GETNEXT, &routeInfo);
        if (SYS_ERR_OK != rc)
        {
            break;
        }

        SYS_MEM_CLEAR(szDstIP);
    	IPV42STR(routeInfo.DsIPv4, szDstIP);
        snprintf (buf1, 19, "%s/%d", szDstIP, routeInfo.prefixlen);

        switch (routeInfo.path_type)
        {
            case FRR_OSPF_PATH_TYPE1_EXTERNAL:
                sprintf(szTmpString, "N E1 %-18s    [%u] tag: %u%s", buf1,
                   routeInfo.cost, routeInfo.tag, RN);
				XMORE("%s", szTmpString);
                break;

            case FRR_OSPF_PATH_TYPE2_EXTERNAL:
                sprintf(szTmpString, "N E2 %-18s    [%u/%u] tag: %u%s", buf1, routeInfo.cost,
                   routeInfo.type2_cost, routeInfo.tag, RN);
				XMORE("%s", szTmpString);
                break;

            default:
                break;
        }

        if (routeInfo.type == FRR_OSPF_DESTINATION_NETWORK)
        {
            for (i = 0; i < routeInfo.path_num; i++)
            {
                path = &routeInfo.path[i];
               	SYS_MEM_CLEAR(szName);
                rc = sys_l3_str2IntfName(path->ifname, szName);
                if(rc != SYS_ERR_OK)
                	continue;

                if (path->nexthop == 0)
                    sprintf(szTmpString, "%24s   directly attached to %s%s",
                         "", szName, RN);
                else
                {
                	SYS_MEM_CLEAR(szNextIp);
                	IPV42STR(path->nexthop, szNextIp);
                    sprintf(szTmpString, "%24s   via %s, %s%s", "", szNextIp, szName, RN);
                }

				XMORE("%s", szTmpString);
            }
        }
    }
	XMORE("\r\n");

    return 0;
}

int32 cmd_show_ospf_route_table(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint16 process = 0;
	OSPF_INFO_T ospfinfo;
	char		szrouter[ 32 ];

	SYS_MEM_CLEAR(ospfinfo);

	while(1)
	{

		rc = sal_ospf_process_status_get(DATA_OP_GETNEXT, &process);
		if(rc != SYS_ERR_OK)
			return SYS_ERR_OK;

		ospfinfo.instance = process;
        rc = sal_ospf_Info_get(&ospfinfo);
		if(rc != SYS_ERR_OK)
			return SYS_ERR_OK;

		SYS_MEM_CLEAR(szrouter);
		IPV42STR(ospfinfo.router_id, szrouter);
		XMORE("\r\n     OSPF Process %d with Router ID :%s", process, szrouter);
	    XMORE("\r\n     		Routing Tables\n");

		show_ip_ospf_route_network(process);

		show_ip_ospf_route_router(process);

		show_ip_ospf_route_external(process);
	}

    XMORE("\r\n");
    return SYS_ERR_OK;
}

int32 cmd_show_ospf(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	OSPF_INFO_T ospfinfo;
	OSPF_AREA_INFO_T areaInfo;
	char        szTmpString[ 512 ] = { 0 };
	char        szrouter[ 32 ];
	ulong_t k = 0; 

	SYS_MEM_CLEAR(ospfinfo);

	if (argc)
	{
		ospfinfo.instance = SYS_STR2UINT(argv[0]);
	}
	else
	{
		ospfinfo.instance = 1;
	}

	rc = sal_ospf_Info_get(&ospfinfo);
	if(rc != SYS_ERR_OK)
	{
		XMORE ("\r\n ospf is not running\n");
		XMORE("\r\n");
		return SYS_ERR_OK;
	}

	SYS_MEM_CLEAR(szrouter);
	IPV42STR(ospfinfo.router_id, szrouter);
    XMORE("\r\n OSPF Process %d, Router ID: %s", ospfinfo.instance, szrouter);

    if ('\0' != ospfinfo.remain_time[0])
    {
        XMORE("\r\n Deferred shutdown in progress, %s remaining", ospfinfo.remain_time);

    }

    XMORE("\r\n Supports only single TOS (TOS0) routes");
    XMORE("\r\n This implementation conforms to RFC2328");
    XMORE("\r\n RFC1583Compatibility flag is %s", ospfinfo.bRfc1583Compatible ? "enabled" : "disabled");

    XMORE("\r\n OpaqueCapability flag is %s", ospfinfo.bOpaqueCapable? "enabled" : "disabled");

    XMORE("\r\n Initial SPF scheduling delay %u millisec(s)"
                "\r\n Minimum hold time between consecutive SPFs %u millisec(s)"
                "\r\n Maximum hold time between consecutive SPFs %u millisec(s)"
                "\r\n Hold time multiplier is currently %u",
                ospfinfo.spf_delay,
                ospfinfo.spf_holdtime,
                ospfinfo.spf_max_holdtime,
                ospfinfo.spf_hold_multiplier);

    k = 0;
    SYS_MEM_CLEAR(szTmpString);
    k += sprintf(szTmpString+k, " SPF algorithm ");
    if (ospfinfo.ts_spf.tv_sec || ospfinfo.ts_spf.tv_usec)
    {
        k += sprintf(szTmpString+k, "last executed %s ago", ospfinfo.last_spf_time);
    }
    else
    {
        k += sprintf(szTmpString+k, "has not been run");
    }

    XMORE("\r\n%s", szTmpString);

    XMORE("\r\n SPF timer %s%s", (ospfinfo.bSpfCalc ? "due in " : "is "), ospfinfo.spf_calc_time);
    XMORE ("\r\n LSA minimum interval %d msecs", ospfinfo.min_ls_interval);
    XMORE ("\r\n LSA minimum arrival %d msecs", ospfinfo.min_ls_arrival);

    XMORE ("\r\n Write Multiplier set to %d", ospfinfo.write_oi_count);

    XMORE ("\r\n Refresh timer %d secs", ospfinfo.lsa_refresh_interval);

    if (ospfinfo.bABR)
    {
        XMORE("\r\n This router is an ABR, ABR type is: %s", ospf_abr_type_descr_str[ospfinfo.abr_type]);
    }
    if (ospfinfo.bASBR)
    {
        XMORE("\r\n This router is an ASBR " "(injecting external routing information)");
    }

    XMORE("\r\n Number of external LSA %d. Checksum Sum 0x%08x", ospfinfo.ase_lsa_cnt, ospfinfo.ase_lsa_chksum);
    XMORE("\r\n Number of opaque AS LSA %d. Checksum Sum 0x%08x", ospfinfo.opa_as_cnt, ospfinfo.opa_as_chksum);

    XMORE("\r\n Number of areas attached to this router: %d", ospfinfo.area_cnt);
    if (ospfinfo.bLogAdj)
    {
        if (ospfinfo.bLogAdjDetail)
            XMORE("\r\n All adjacency changes are logged");
        else
            XMORE("\r\n Adjacency changes are logged");
    }

    areaInfo.bFirst = TRUE;
    areaInfo.instance = ospfinfo.instance;
    while (1)
    {
        rc = sal_ospf_area_info_get(DATA_OP_GETNEXT, &areaInfo);
        if (SYS_ERR_OK != rc)
        {
            break;
        }

        if (show_ospf_area (&areaInfo))
        {
            break;
        }
    }
    XMORE("\r\n");
    return SYS_ERR_OK;
}

int32 cmd_ospf_neighbor_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_enable_t bEnable = FALSE;
	int rc;
	char ipStr[CAPA_IPV4_STR_LEN];
	char szrouter[ 32 ];
	OSPF_INFO_T ospfinfo;
	OSPF_NBR_INFO_T nbrInfo;
	int first;
	char szName[128];

	if (argc)
	{
		ospfinfo.instance = SYS_STR2UINT(argv[0]);
	}
	else
	{
		ospfinfo.instance = 1;
	}

	rc = sal_l3_routeV4_ospf_get(&bEnable);
	if (VOS_ERR_NONE != rc || DISABLED == bEnable)
	{
		XMORE("\nOSPF protocol is not active!\n");
		return SYS_ERR_OK;
	}

	rc = sal_ospf_Info_get(&ospfinfo);
	if(rc != SYS_ERR_OK)
		return SYS_ERR_OK;

	SYS_MEM_CLEAR(szrouter);
	IPV42STR(ospfinfo.router_id, szrouter);

	XMORE("\r\n 	OSPF Process %d with Router ID :%s", ospfinfo.instance, szrouter);
	XMORE("\r\n     		neighbors:\n");

    XMORE("\n");
    XMORE("Type: D - Dynamic, S - Static\n");
    XMORE("%-5s %-15s %-3s %-15s %-9s %-15s %-15s %-5s %-5s %-5s\n", "Type",
			 "Neighbor ID", "Pri", "State", "Dead Time",
			 "Address", "Interface", "RXmtL", "RqstL", "DBsmL");

	nbrInfo.instance = ospfinfo.instance;
	nbrInfo.type = OSPF_IFTYPE_NONE;
	nbrInfo.src = 0;
	while (1)
	{
		rc = sal_l3_routeV4_ospf_nbr_info_get(DATA_OP_GETNEXT, &nbrInfo);
		if (VOS_ERR_NONE != rc)
		{
			break;
		}
#if 0
		if (OSPF_IFTYPE_VIRTUALLINK != nbrInfo.type)
		{
			if (OK != l3ifSwIntfNameGetByifname(nbrInfo.ifname, szName))
			{
				sprintf(szName, "Invalid");
			}
		}
#endif
		XMORE("%-5s ", "D");
		if (nbrInfo.state == NSM_Attempt && nbrInfo.router_id == 0)
		{
			XMORE("%-15s %-3d %-15s ", "-", nbrInfo.priority, nbrInfo.msgbuf);
		}
		else
		{
			SYS_ERR_CHK(IPV42STR(nbrInfo.router_id, ipStr));
			XMORE("%-15s %-3d %-15s ", ipStr, nbrInfo.priority, nbrInfo.msgbuf);

		}

		XMORE("%-9s ", nbrInfo.timebuf);
		SYS_ERR_CHK(IPV42STR(nbrInfo.src, ipStr));
		XMORE("%-15s ", ipStr);

		SYS_MEM_CLEAR(szName);
		SYS_ERR_CHK(sys_l3_str2IntfName(nbrInfo.ifname, szName));
		if (OSPF_IFTYPE_VIRTUALLINK != nbrInfo.type)
		{
			XMORE("%-15s %5u %5u %5u\n", szName, nbrInfo.ls_retransmit_cnt,
						 nbrInfo.ls_request_cnt, nbrInfo.db_summary_cnt);
		}
		else
		{
			SYS_ERR_CHK(IPV42STR(nbrInfo.areaId, ipStr));
			 XMORE("%-15s %-5u %-5u %-5u\n", ipStr, nbrInfo.ls_retransmit_cnt,
						 nbrInfo.ls_request_cnt, nbrInfo.db_summary_cnt);
		}

	}

	nbrInfo.src = 0;
	while (1)
	{
		rc = sal_l3_routeV4_ospf_nbma_nbr_info_get(DATA_OP_GETNEXT, &nbrInfo);
		if (VOS_ERR_NONE != rc)
		{
			break;
		}
#if 0
		if (OK != l3ifSwIntfNameGetByifname(nbrInfo.ifname, szName))
		{
			sprintf(szName, "Invalid");
		}
#endif
		SYS_MEM_CLEAR(szName);
		SYS_ERR_CHK(sys_l3_str2IntfName(nbrInfo.ifname, szName));
		XMORE("%-5s ", "S");
		if (nbrInfo.state == NSM_Down)
		{
			XMORE("%-15s %-3d %-15s %-9s ",
				 "-", nbrInfo.priority, "Down", "-");

			SYS_ERR_CHK(IPV42STR(nbrInfo.src, ipStr));
			
			XMORE("%-15s %-15s %-5d %-5d %-5d\n",
				 ipStr, szName,
				 0, 0, 0);
		}
		else
		{
			if (nbrInfo.state == NSM_Attempt && nbrInfo.router_id == 0)
			{
				XMORE("%-15s %-3d %-15s ",
							 "-", nbrInfo.priority,
							 nbrInfo.msgbuf);
			}
			else
			{
				SYS_ERR_CHK(IPV42STR(nbrInfo.router_id, ipStr));
				XMORE("%-15s %-3d %-15s ",
							 ipStr, nbrInfo.priority,
							 nbrInfo.msgbuf);
			}
			XMORE("%9s ", nbrInfo.timebuf);

			SYS_ERR_CHK(IPV42STR(nbrInfo.src, ipStr));
			XMORE("%-15s ", ipStr);
			XMORE("%-15s %-5u %-5u %-5u\n",
						 szName, nbrInfo.ls_retransmit_cnt,
						 nbrInfo.ls_request_cnt, nbrInfo.db_summary_cnt);
		}

	}

	first = 1;
	
	nbrInfo.type = OSPF_IFTYPE_VIRTUALLINK;
	nbrInfo.src = 0;
	while (1)
	{
		rc = sal_l3_routeV4_ospf_nbr_info_get(DATA_OP_GETNEXT, &nbrInfo);
		if (VOS_ERR_NONE != rc)
		{
			break;
		}
		if (first)
		{
			XMORE("\nVirtual neighbors:\n");

			XMORE("%-5s %-15s %-3s %-15s %-9s %-15s %-15s %-5s %-5s %-5s\n",
					 "Type",
					 "Neighbor ID", "Pri", "State", "Dead Time",
					 "Address", "Area", "RXmtL", "RqstL", "DBsmL");
			first = 0;
		}
#if 0
		if (OSPF_IFTYPE_VIRTUALLINK != nbrInfo.type)
		{
			if (OK != l3ifSwIntfNameGetByifname(nbrInfo.ifname, szName))
			{
				sprintf(szName, "Invalid");
			}
		}
#endif
		XMORE("%-5s ", "D");
		if (nbrInfo.state == NSM_Attempt && nbrInfo.router_id == 0)
		{
			XMORE("%-15s %-3d %-15s ",
						 "-", nbrInfo.priority,
						 nbrInfo.msgbuf);
		}
		else
		{

			SYS_ERR_CHK(IPV42STR(nbrInfo.router_id, ipStr));
			XMORE("%-15s %-3d %-15s ",
						 ipStr, nbrInfo.priority,
						 nbrInfo.msgbuf);
		}

		XMORE("%-9s ", nbrInfo.timebuf);

		SYS_ERR_CHK(IPV42STR(nbrInfo.src, ipStr));
		XMORE("%-15s ", ipStr);

		if (OSPF_IFTYPE_VIRTUALLINK != nbrInfo.type)
		{
			XMORE("%-15s %-5u %-5u %-5u\n",
				 nbrInfo.ifname, nbrInfo.ls_retransmit_cnt,
				 nbrInfo.ls_request_cnt, nbrInfo.db_summary_cnt);
		}
		else
		{
			SYS_ERR_CHK(IPV42STR(nbrInfo.areaId, ipStr));
			XMORE("%-15s %-5u %-5u %-5u\n",
				 ipStr, nbrInfo.ls_retransmit_cnt,
				 nbrInfo.ls_request_cnt, nbrInfo.db_summary_cnt);
		}

	}

	return SYS_ERR_OK;
}

int32 cmd_ospf_virtual_link_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_enable_t bEnable = FALSE;
	int rc;
    
	char ipStr[CAPA_IPV4_STR_LEN];
	char szrouter[ 32 ];
	OSPF_INFO_T ospfinfo;
    OSPF_INTF_INFO_T virIntfInfo;
    DATA_OP_ENT op = DATA_OP_GETNEXT;

	if (argc)
	{
		ospfinfo.instance = SYS_STR2UINT(argv[0]);
	}
	else
	{
		ospfinfo.instance = 1;
	}

	rc = sal_l3_routeV4_ospf_get(&bEnable);
	if (VOS_ERR_NONE != rc || DISABLED == bEnable)
	{
		XMORE("\nOSPF protocol is not active!\n");
		return SYS_ERR_OK;
	}

	rc = sal_ospf_Info_get(&ospfinfo);
	if(rc != SYS_ERR_OK)
		return SYS_ERR_OSPF_INTERFACE_NOT_EXIST;

	SYS_MEM_CLEAR(szrouter);
	IPV42STR(ospfinfo.router_id, szrouter);

	XMORE("\r\n 	OSPF Process %d with Router ID :%s", ospfinfo.instance, szrouter);
	XMORE("\r\n     		Vritual links:\n");

    XMORE("\nOSPF vritual link information\n");
    XMORE("%-15s %-15s %-5s %-4s %-5s %-5s %-20s\n", "Area",
			 "Neighbor", "Hello", "Dead", "Retra", "Trans", "State");

    virIntfInfo.bFirst = TRUE;
	virIntfInfo.instance = ospfinfo.instance;
	while (1)
    {
        rc = sal_l3_ospf_virtual_link_info_get(op, &virIntfInfo);
        if (VOS_ERR_NONE != rc)
        {
            break;
        }

		SYS_ERR_CHK(IPV42STR(virIntfInfo.areaId, ipStr));
		XMORE("%-15s ",ipStr);

		SYS_ERR_CHK(IPV42STR(virIntfInfo.prefix, ipStr));
		XMORE("%-15s ",ipStr);
		
		XMORE("%-5d %-4d %-5d %-5d ",virIntfInfo.hello_time, virIntfInfo.dead_time, virIntfInfo.retransmit_interval, virIntfInfo.transmit_delay);

		XMORE("%-20s \n", virIntfInfo.szState);

	}

	return SYS_ERR_OK;
}

int show_lsa_summary (OSPF_LSA_INFO_T *pLsaInfo)
{
    struct ROUTER_LSA_INFO *pRouter;
    struct SUMMARY_LSA_INFO *pSummary;
    struct AS_EXTERNAL_LSA_INFO *pExternal;
	char ipStr[CAPA_IPV4_STR_LEN];

	SYS_ERR_CHK(IPV42STR(pLsaInfo->u.common.header.id, ipStr));
	XMORE("%-15s ", ipStr);
	SYS_ERR_CHK(IPV42STR(pLsaInfo->u.common.header.adv_router, ipStr));
	XMORE("%-15s %-4d 0x%08x 0x%04x ", ipStr, pLsaInfo->u.common.header.ls_age,
		ntohl (pLsaInfo->u.common.header.ls_seqnum), ntohs (pLsaInfo->u.common.header.checksum));

    switch (pLsaInfo->u.common.header.type)
    {
        case OSPF_ROUTER_LSA:
            pRouter = &pLsaInfo->u.router;
			XMORE("%d ", ntohs (pRouter->links));
            break;
        case OSPF_SUMMARY_LSA:
            pSummary = &pLsaInfo->u.summary;
			XMORE("%s ", pSummary->prefix_str);
            break;
        case OSPF_AS_EXTERNAL_LSA:
        case OSPF_AS_NSSA_LSA:
            pExternal = &pLsaInfo->u.external;
			XMORE("%s %s [0x%x]", pExternal->bExternalMetric ? "E2" : "E1",
				pExternal->prefix_str, ntohl (pExternal->route_tag));
            break;
        case OSPF_NETWORK_LSA:
        case OSPF_ASBR_SUMMARY_LSA:
#ifdef HAVE_OPAQUE_LSA
        case OSPF_OPAQUE_LINK_LSA:
        case OSPF_OPAQUE_AREA_LSA:
        case OSPF_OPAQUE_AS_LSA:
#endif 
        default:
            break;
    }

	XMORE("\r\n");

    return 0;
}

int32 cmd_ospf_database_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_enable_t bEnable = FALSE;
	int rc;
	char ipStr[CAPA_IPV4_STR_LEN];
	char szrouter[ 32 ];
	OSPF_INFO_T ospfinfo;
    OSPF_LSA_INFO_T lsaInfo;
    OSPF_AREA_INFO_T areaInfo;
    int lsa_type = OSPF_UNKNOWN_LSA;
    int lsa_argc = 0;
    int type;
    int first = 1;

	if (argc > 1)
	{
		ospfinfo.instance = SYS_STR2UINT(argv[0]);
		lsa_argc = 2;
	}
	else if (argc == 1)
	{
		if ('d' == CMD_TOKENSTR_GET(cmd->vline,2)[0])
		{
			ospfinfo.instance = 1;
			lsa_argc = 1;
		}
		else
		{
			ospfinfo.instance = SYS_STR2UINT(argv[0]);
		}
	}
	else
	{
		ospfinfo.instance = 1;
	}

	rc = sal_l3_routeV4_ospf_get(&bEnable);
	if (VOS_ERR_NONE != rc || DISABLED == bEnable)
	{
		XMORE("\nOSPF protocol is not active!\n");
		return SYS_ERR_OK;
	}

	rc = sal_ospf_Info_get(&ospfinfo);
	if(rc != SYS_ERR_OK)
		return SYS_ERR_OK;

	if (lsa_argc)
	{
		if (osal_strncmp (argv[lsa_argc - 1], "r", 1) == 0)
			lsa_type = OSPF_ROUTER_LSA;
		else if (osal_strncmp (argv[lsa_argc - 1], "ne", 2) == 0)
			lsa_type = OSPF_NETWORK_LSA;
		else if (osal_strncmp (argv[lsa_argc - 1], "ns", 2) == 0)
			lsa_type = OSPF_AS_NSSA_LSA;
		else if (osal_strncmp (argv[lsa_argc - 1], "su", 2) == 0)
			lsa_type = OSPF_SUMMARY_LSA;
		else if (osal_strncmp (argv[lsa_argc - 1], "a", 1) == 0)
			lsa_type = OSPF_ASBR_SUMMARY_LSA;
		else if (osal_strncmp (argv[lsa_argc - 1], "e", 1) == 0)
			lsa_type = OSPF_AS_EXTERNAL_LSA;
#ifdef HAVE_OPAQUE_LSA
		else if (osal_strncmp (argv[lsa_argc - 1], "opaque-l", 8) == 0)
			lsa_type = OSPF_OPAQUE_LINK_LSA;
		else if (osal_strncmp (argv[lsa_argc - 1], "opaque-ar", 9) == 0)
			lsa_type = OSPF_OPAQUE_AREA_LSA;
		else if (osal_strncmp (argv[lsa_argc - 1], "opaque-as", 9) == 0)
			lsa_type = OSPF_OPAQUE_AS_LSA;
#endif 
		else
			return SYS_ERR_FAILED;
	}

	SYS_MEM_CLEAR(szrouter);
	IPV42STR(ospfinfo.router_id, szrouter);

	XMORE("\r\n 	OSPF Process %d with Router ID :%s", ospfinfo.instance, szrouter);
	XMORE("\r\n     		Database information:\n");
    areaInfo.bFirst = TRUE;
    areaInfo.instance = ospfinfo.instance;
    while (1)
    {
        rc = sal_l3_ospf_area_info_get(DATA_OP_GETNEXT, &areaInfo);
        if (VOS_ERR_NONE != rc)
        {
            break;
        }

        for (type = OSPF_MIN_LSA; type < OSPF_MAX_LSA; type++)
        {
            switch (type)
            {
                case OSPF_AS_EXTERNAL_LSA:
#ifdef HAVE_OPAQUE_LSA
                case OSPF_OPAQUE_AS_LSA:
#endif 
                  continue;
                default:
                  break;
            }

            if (type == lsa_type ||
                OSPF_UNKNOWN_LSA == lsa_type)
            {
                first = 1;
                
                lsaInfo.u.common.header.type = type;
                lsaInfo.bFirst = TRUE;
                lsaInfo.bAsExternal = FALSE;
                lsaInfo.area_id = areaInfo.area_id;
                while (1)
                {
                    rc = sal_l3_ospf_lsa_info_get(DATA_OP_GETNEXT, &lsaInfo);
                    if (VOS_ERR_NONE != rc)
                    {
                        break;
                    }

                    if (1 == first)
                    {
						SYS_ERR_CHK(IPV42STR(areaInfo.area_id, ipStr));
                        XMORE("                %s (Area %s)%s",
                                show_database_desc[type], ipStr, RN);

                        XMORE("%s%s", show_database_header[type], RN);

                        first = 0;
                    }

                    if (show_lsa_summary(&lsaInfo))
                    {
                        return 1;
                    }
                }
            }

        }
    }

    for (type = OSPF_MIN_LSA; type < OSPF_MAX_LSA; type++)
    {
        switch (type)
        {
            case OSPF_AS_EXTERNAL_LSA:
	#ifdef HAVE_OPAQUE_LSA
            case OSPF_OPAQUE_AS_LSA:
	#endif 
                break;
            default:
                continue;
        }

        if (type == lsa_type ||
            OSPF_UNKNOWN_LSA == lsa_type)
        {
            first = 1;
            
            lsaInfo.u.common.header.type = type;
            lsaInfo.bFirst = TRUE;
            lsaInfo.bAsExternal = TRUE;
            while (1)
            {
                rc = sal_l3_ospf_lsa_info_get(DATA_OP_GETNEXT, &lsaInfo);
                if (VOS_ERR_NONE != rc)
                {
                    break;
                }

                if (1 == first)
                {
                    XMORE("                %s%s",show_database_desc[type], RN);

                    XMORE("%s%s", show_database_header[type], RN);

                    first = 0;
                }

                if (show_lsa_summary(&lsaInfo))
                {
                    return SYS_ERR_OK;
                }
            }
        }
    }

	return SYS_ERR_OK;
}

int32 cmd_ospf_interface_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_enable_t bEnable = FALSE;
	int rc;
	char ipStr[CAPA_IPV4_STR_LEN];
	char areaStr[CAPA_IPV4_STR_LEN];
	OSPF_INTF_INFO_T intfInfo;
    sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1] = {0};

	rc = sal_l3_routeV4_ospf_get(&bEnable);
	if (VOS_ERR_NONE != rc || DISABLED == bEnable)
	{
		XMORE("\nOSPF protocol is not active!\n");
		return SYS_ERR_OK;
	}

	if (argc)
	{
		intf.type = SYS_L3_INTFTYPE_VLAN;
		intf.id = SYS_STR2UINT(argv[0]);
		if (!IS_L3_INTF_EXIST(&intf))
		{
			return SYS_ERR_OSPF_INTERFACE_NOT_EXIST;
		}

		sys_l3_intfId2Str(&intf, ifStr);
	}
	else
	{
		ifStr[0] = '\0';
	}

    XMORE("OSPF interface information:\n");

	 intfInfo.bFirst = TRUE;
	 intfInfo.instance = 1;
	 while (1)
	 {
		SYS_MEM_CLEAR(intfInfo.ifname);
		rc = sal_l3_ospf_intf_info_get(DATA_OP_GETNEXT, &intfInfo);
		if (VOS_ERR_NONE != rc)
		{
			break;
		}

		if ('\0' != ifStr[0] &&
			strcmp(ifStr, intfInfo.ifname))
		{
			continue;
		}
			
		SYS_ERR_CHK(IPV42STR(intfInfo.prefix, ipStr));
		SYS_ERR_CHK(IPV42STR(intfInfo.areaId, areaStr));
		XMORE("\nInterface: %s (%s) Area: %s\n", ipStr, intfInfo.ifname, areaStr);

		XMORE("%-15s %-15s %-20s %-16s \n", "DR", "BDR", "State", "Type");

		if (0 == intfInfo.dr)
		{
			XMORE("%-15s ", "none");
		}
		else
		{
			SYS_ERR_CHK(IPV42STR(intfInfo.dr, ipStr));
			XMORE("%-15s ",ipStr);
		}

		if (0 == intfInfo.bdr)
		{
			XMORE("%-15s ", "none");
		}
		else
		{
			SYS_ERR_CHK(IPV42STR(intfInfo.bdr, ipStr));
			XMORE("%-15s ",ipStr);
		}

		XMORE("%-20s ", intfInfo.szState);
		XMORE("%-16s \n", ospf_network_type_str[intfInfo.type]);
		
		XMORE("Cost: %d \n", intfInfo.output_cost);
		XMORE("Priority: %d \n", intfInfo.priority);
		XMORE("Times: Hello %d, Dead %d, Retra %d, Trans %d \n", intfInfo.hello_time,
			intfInfo.dead_time, intfInfo.retransmit_interval, intfInfo.transmit_delay);
	 }

	return SYS_ERR_OK;
}

int32 cmd_l3_router_ospf_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    
	int rc = SYS_ERR_OK;
	OSPF_AREA_INFO_T areaInfo;
	OSPF_AREA_NETWORK_T network;
	OSPF_INFO_T ospfinfo;
	OSPF_AREA_RANGE_T range;
	OSPF_REDIST_INFO_T pRedInfo;
	OSPF_PASSIVE_INTF_INFO_T pIntfInfo;
	OSPF_DISTANCE_INFO_T pDistInfo;
	OSPF_INTF_INFO_T virIntfInfo;
    sys_l3_intfId_t intfId;
	uint16 process = 0;
	int type;
	char ipv4[32];
    char tmpStr[SYS_BUF32_LEN];
	char *ospf_shortcut_mode_str[] =
	{
	  "default",
	  "enable",
	  "disable"
	};

	SYS_MEM_CLEAR(areaInfo);
	SYS_MEM_CLEAR(network);

	while(1)
	{
		
		SYS_ERR_CHK(sal_ospf_process_status_get(DATA_OP_GETNEXT, &process));

		if(process != 0)
		{
			ospfinfo.instance = process;
			rc = sal_ospf_Info_get(&ospfinfo);
			if(rc != SYS_ERR_OK)
				return SYS_ERR_OK;

			SYS_MEM_CLEAR(cmd);
	        osal_sprintf(cmd, "ospf %d\n", process);
	        CMD_WRITE_TO_FILE(fp, cmd);

			if (ospfinfo.router_id_static != 0)
			{
				SYS_MEM_CLEAR(ipv4);
				IPV42STR(ospfinfo.router_id_static, ipv4);
				osal_sprintf(cmd, "router-id %s\n", ipv4);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if (ospfinfo.abr_type != OSPF_ABR_DEFAULT)
			{
				osal_sprintf(cmd, "ospf abr-type %s\n", ospf_abr_type_str[ospfinfo.abr_type]);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if (ospfinfo.bLogAdj)
			{
				if (ospfinfo.bLogAdjDetail)
					osal_sprintf(cmd, "log-adjacency-changes detail\n");
				else
					osal_sprintf(cmd, "log-adjacency-changes\n");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if (ospfinfo.bRfc1583Compatible)
			{
				osal_sprintf(cmd, "compatible rfc1583\n");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if (ospfinfo.ref_bandwidth != OSPF_DEFAULT_REF_BANDWIDTH/1000)
			{
				osal_sprintf(cmd, "auto-cost reference-bandwidth %d\n", ospfinfo.ref_bandwidth);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if (ospfinfo.spf_delay != OSPF_SPF_DELAY_DEFAULT ||
				ospfinfo.spf_holdtime != OSPF_SPF_HOLDTIME_DEFAULT ||
				ospfinfo.spf_max_holdtime != OSPF_SPF_MAX_HOLDTIME_DEFAULT)
			{
				osal_sprintf(cmd, "timers throttle spf %d %d %d\n", ospfinfo.spf_delay, ospfinfo.spf_holdtime,
							ospfinfo.spf_max_holdtime);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if (ospfinfo.lsa_refresh_interval != OSPF_LSA_REFRESH_INTERVAL_DEFAULT)
			{
				osal_sprintf(cmd, "refresh timer %d\n", ospfinfo.lsa_refresh_interval);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if (ospfinfo.default_metric != -1)
		   	{
				osal_sprintf(cmd, "default-metric %d\n", ospfinfo.default_metric);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		   	}

			SYS_MEM_CLEAR(pRedInfo);
		    pRedInfo.instance = process;
			rc = sal_ospf_redist_nfo_get(&pRedInfo);
			if(rc != SYS_ERR_OK)
				return SYS_ERR_OK;

			for (type = 0; type < ZEBRA_ROUTE_MAX; type++)
			{
				if (pRedInfo.redist[type].distribute[0] != 0)
				{
					osal_sprintf(cmd, "distribute-list %s out %s\n",
							 pRedInfo.redist[type].distribute, zebra_route_string(type));
					CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
				}
			}

			if (pRedInfo.default_originate != DEFAULT_ORIGINATE_NONE)
			{
				osal_sprintf(cmd, "default-information originate");
				if (pRedInfo.default_originate == DEFAULT_ORIGINATE_ALWAYS)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " always");
					osal_strcat(cmd, tmpStr);
				}

				if (pRedInfo.redist[DEFAULT_ROUTE].value >= 0)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " metric %d", pRedInfo.redist[DEFAULT_ROUTE].value);
					osal_strcat(cmd, tmpStr);
				}

				if (pRedInfo.redist[DEFAULT_ROUTE].type == EXTERNAL_METRIC_TYPE_1)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " metric-type 1");
					osal_strcat(cmd, tmpStr);
				}

				if (pRedInfo.redist[DEFAULT_ROUTE].name[0] != 0)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " route-map %s", pRedInfo.redist[DEFAULT_ROUTE].name);
					osal_strcat(cmd, tmpStr);
				}

				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, "\n");
				osal_strcat(cmd, tmpStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			for (type = 0; type < ZEBRA_ROUTE_MAX; type++)
			{
				if (!pRedInfo.redist[type].bFlag)
					continue;

				osal_sprintf(cmd, "redistribute %s", zebra_route_string(type));
				if (pRedInfo.redist[type].value >= 0)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " metric %d", pRedInfo.redist[type].value);
					osal_strcat(cmd, tmpStr);
				}

				if (pRedInfo.redist[type].type == EXTERNAL_METRIC_TYPE_1)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " metric-type 1");
					osal_strcat(cmd, tmpStr);
				}

				if (pRedInfo.redist[DEFAULT_ROUTE].name[0] != 0)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " route-map %s", pRedInfo.redist[type].name);
					osal_strcat(cmd, tmpStr);
				}

				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, "\n");
				osal_strcat(cmd, tmpStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}
			
			if (ospfinfo.distance_all)
			{
				osal_sprintf(cmd, "distance %d\n", ospfinfo.distance_all);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		  	}
			  
			if (ospfinfo.distance_intra
				|| ospfinfo.distance_inter
				|| ospfinfo.distance_external)
			{
				osal_sprintf(cmd, "distance ospf");
				if (ospfinfo.distance_intra)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " intra-area %d", ospfinfo.distance_intra);
					osal_strcat(cmd, tmpStr);
				}
				
				if (ospfinfo.distance_inter)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " inter-area %d", ospfinfo.distance_inter);
					osal_strcat(cmd, tmpStr);
				}
				
				if (ospfinfo.distance_external)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " external %d", ospfinfo.distance_external);
					osal_strcat(cmd, tmpStr);
				}

				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, "\n");
				osal_strcat(cmd, tmpStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}
			
			SYS_MEM_CLEAR(pDistInfo);
			pDistInfo.bFirst = TRUE;
		    pDistInfo.instance = process;
			pDistInfo.addr = 0;
			pDistInfo.prefixlen = 32;
			pDistInfo.flags = FALSE;
			while(1)
			{
				rc = sal_ospf_distance_get(DATA_OP_GETNEXT, &pDistInfo);
				if (SYS_ERR_OK != rc)
				{
					break;
				}
				SYS_MEM_CLEAR(cmd);
				SYS_MEM_CLEAR(ipv4);
				IPV42STR(pDistInfo.addr, ipv4);
				osal_sprintf(cmd, "distance %d %s/%d %s\n", pDistInfo.distance,
						 ipv4, pDistInfo.prefixlen, pDistInfo.name);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if (ospfinfo.passive_interface_default == OSPF_IF_PASSIVE)
			{
				osal_sprintf(cmd, "passive-interface default\n");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

		    pIntfInfo.bFirst = TRUE;
		    pIntfInfo.instance = process;
			pIntfInfo.flags = FALSE;
			while(1)
			{
				rc = sal_ospf_passive_interface_get(DATA_OP_GETNEXT, &pIntfInfo);
				if (SYS_ERR_OK != rc)
				{
					break;
				}
				
				SYS_MEM_CLEAR(intfId);
				sys_l3_str2IntfId(pIntfInfo.name, &intfId);
				osal_sprintf(cmd, "%spassive-interface vlan-interface %d\n",
						 ospfinfo.passive_interface_default ? "no " : "",
						 intfId.id);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			SYS_MEM_CLEAR(cmd);
		    pIntfInfo.bFirst = TRUE;
		    pIntfInfo.instance = process;
			pIntfInfo.flags = TRUE;
			while(1)
			{
				rc = sal_ospf_passive_interface_get(DATA_OP_GETNEXT, &pIntfInfo);
				if (SYS_ERR_OK != rc)
				{
					break;
				}
				
				SYS_MEM_CLEAR(intfId);
				SYS_MEM_CLEAR(ipv4);
				sys_l3_str2IntfId(pIntfInfo.name, &intfId);
				IPV42STR(pIntfInfo.addr, ipv4);
				osal_sprintf(cmd, "%spassive-interface vlan-interface %d %s\n",
						 ospfinfo.passive_interface_default ? "no " : "",
						 intfId.id, ipv4);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

		    areaInfo.bFirst = TRUE;
		    areaInfo.instance = process;
		    while (1)
		    {
		        rc = sal_ospf_area_info_get(DATA_OP_GETNEXT, &areaInfo);
		        if (SYS_ERR_OK != rc)
		        {
		            break;
		        }
		        SYS_MEM_CLEAR(cmd);
	        	osal_sprintf(cmd, "area %d\n", areaInfo.area_id);
	        	CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
	        	network.networkIp = 0;
	        	network.prefixlen = 32;
	        	network.instance = process;
	        	network.areaId = areaInfo.area_id;
	        	while(1)
	        	{
					rc = sal_ospf_area_network_get(DATA_OP_GETNEXT, &network);
					if (SYS_ERR_OK != rc)
			        {
			            break;
			        }
			        SYS_MEM_CLEAR(cmd);
			        SYS_MEM_CLEAR(ipv4);
			        IPV42STR(network.networkIp, ipv4);
		        	osal_sprintf(cmd, "network %s/%d\n", ipv4, network.prefixlen);
		        	CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
	        	}

	        	range.rangeIp = 0;
	        	range.prefixlen = 32;
	        	range.instance = process;
	        	range.areaId = areaInfo.area_id;
				while(1)
	        	{
					rc = sal_ospf_area_range_get(DATA_OP_GETNEXT, &range);
					if (SYS_ERR_OK != rc)
			        {
			            break;
			        }
			        SYS_MEM_CLEAR(cmd);
			        SYS_MEM_CLEAR(ipv4);
			        IPV42STR(range.rangeIp, ipv4);
		        	osal_sprintf(cmd, "range %s/%d", ipv4, range.prefixlen);
				    if (range.cost != OSPF_AREA_RANGE_COST_UNSPEC)
			    	{
						SYS_MEM_CLEAR(tmpStr);
						osal_sprintf(tmpStr, " cost %d", range.cost);
						osal_strcat(cmd, tmpStr);
			    	}

				    if (!CHECK_FLAG (range.flags, OSPF_AREA_RANGE_ADVERTISE))
			    	{
						SYS_MEM_CLEAR(tmpStr);
						osal_sprintf(tmpStr, " not-advertise");
						osal_strcat(cmd, tmpStr);
			    	}

				    if (CHECK_FLAG (range.flags, OSPF_AREA_RANGE_SUBSTITUTE))
					{
						SYS_MEM_CLEAR(tmpStr);
						SYS_MEM_CLEAR(ipv4);
						IPV42STR(range.subst_addr, ipv4);
						osal_sprintf(tmpStr, " substitute %s/%d", ipv4, range.subst_masklen);
						osal_strcat(cmd, tmpStr);
					}

					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, "\n");
					osal_strcat(cmd, tmpStr);

		        	CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
	        	}

				if (areaInfo.auth_type != OSPF_AUTH_NULL)
				{
				  if (areaInfo.auth_type == OSPF_AUTH_SIMPLE)
					  osal_sprintf(cmd, "authentication simple\n");
				  else
					  osal_sprintf(cmd, "authentication message-digest\n");

				  CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
				}

				if (areaInfo.shortcut_configured != OSPF_SHORTCUT_DEFAULT)
				{
					osal_sprintf(cmd, "shortcut %s\n", ospf_shortcut_mode_str[areaInfo.shortcut_configured]);
					CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
				}

				if ((areaInfo.external_routing == OSPF_AREA_STUB)
					|| (areaInfo.external_routing == OSPF_AREA_NSSA))
				{
					if (areaInfo.external_routing == OSPF_AREA_STUB)
				  	{
						osal_sprintf(cmd, "stub");
					}
					else if (areaInfo.external_routing == OSPF_AREA_NSSA)
				    {
						osal_sprintf(cmd, "nssa");
						switch (areaInfo.NSSATranslatorRole)
						{
							case OSPF_NSSA_ROLE_NEVER:
								SYS_MEM_CLEAR(tmpStr);
								osal_sprintf(tmpStr, " translate-never");
								osal_strcat(cmd, tmpStr);
								break;
							case OSPF_NSSA_ROLE_ALWAYS:
								SYS_MEM_CLEAR(tmpStr);
								osal_sprintf(tmpStr, " translate-always");
								osal_strcat(cmd, tmpStr);
								break;
							case OSPF_NSSA_ROLE_CANDIDATE:
								default:

							SYS_MEM_CLEAR(tmpStr);
							osal_sprintf(tmpStr, " translate-candidate");
							osal_strcat(cmd, tmpStr);
						}
				    }

					if (areaInfo.bNoSummary)
					{
						SYS_MEM_CLEAR(tmpStr);
						osal_sprintf(tmpStr, " no-summary");
						osal_strcat(cmd, tmpStr);
					}

					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, "\n");
					osal_strcat(cmd, tmpStr);
					CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
				}

				virIntfInfo.bFirst = TRUE;					
				virIntfInfo.instance = process;
				while(1)
				{
				    
					rc = sal_l3_ospf_virtual_link_info_get(DATA_OP_GETNEXT, &virIntfInfo);
					if (SYS_ERR_OK != rc)
					{
						break;
					}
					
					if (virIntfInfo.areaId == areaInfo.area_id)
					{
						SYS_MEM_CLEAR(cmd);
						SYS_MEM_CLEAR(ipv4);
						IPV42STR(virIntfInfo.prefix, ipv4);
						if (virIntfInfo.hello_time != OSPF_HELLO_INTERVAL_DEFAULT ||
							virIntfInfo.dead_time != OSPF_ROUTER_DEAD_INTERVAL_DEFAULT ||
							virIntfInfo.transmit_delay != OSPF_TRANSMIT_DELAY_DEFAULT ||
							virIntfInfo.retransmit_interval != OSPF_RETRANSMIT_INTERVAL_DEFAULT)
						{

							osal_sprintf(cmd, "virtual-link %s hello-interval %d retransmit-interval %d transmit-delay %d dead-interval %d\n",
								ipv4, virIntfInfo.hello_time, virIntfInfo.retransmit_interval, virIntfInfo.transmit_delay, virIntfInfo.dead_time);
							CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
						}
						else
						{
							osal_sprintf(cmd, "virtual-link %s\n", ipv4);
							CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
						}
						
						if (virIntfInfo.auth_key[0] != '\0')
						
				{
							if (virIntfInfo.auth_type == OSPF_AUTH_SIMPLE)
							{
								osal_sprintf(cmd, "virtual-link %s authentication-key %s\n", ipv4, virIntfInfo.auth_key);
								CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
							}
							else if (virIntfInfo.auth_type == OSPF_AUTH_CRYPTOGRAPHIC)
							{
								osal_sprintf(cmd, "virtual-link %s message-digest-key %d md5 %s\n", ipv4, virIntfInfo.key_id, virIntfInfo.auth_key);
								CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
							}
                        }
					}
				}

				if (areaInfo.default_cost != 1)
				{
					osal_sprintf(cmd, "default-cost %d\n", areaInfo.default_cost);
					CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
				}

				if (areaInfo.export_name[0] != 0)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, "export-list %s\n", areaInfo.export_name);
					osal_strcat(cmd, tmpStr);
					CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
				}

				if (areaInfo.import_name[0] != 0)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, "import-list %s\n", areaInfo.import_name);
					osal_strcat(cmd, tmpStr);
					CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
				}

				if (areaInfo.filter_in_name[0] != 0)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, "filter-list prefix %s in\n", areaInfo.filter_in_name);
					osal_strcat(cmd, tmpStr);
					CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
				}

				if (areaInfo.filter_out_name[0] != 0)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, "filter-list prefix %s out\n", areaInfo.filter_out_name);
					osal_strcat(cmd, tmpStr);
					CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
				}
				
	        	osal_sprintf(cmd, "exit\n");
	        	CMD_WRITE_TO_FILE_INDENT2(fp, cmd);
		    }
		}
		else
		{
			break;
		}
	}

    return SYS_ERR_OK;
}

int32 cmd_l3_ospf_interface_save(FILE *fp, sys_vid_t vid)
{
	char cmd[MAX_CMD_STRLEN];
    char tmpStr[SYS_BUF32_LEN];
	OSPF_INTF_INFO_T intfInfo;
	sys_l3_intfId_t intf;
    char ifStr[CAPA_NAME_STR_LEN + 1];
	char ipv4[32];
	int rc;

    SYS_MEM_CLEAR(intf);

    intf.type = SYS_L3_INTFTYPE_VLAN;
    intf.id = vid;

	if (!IS_L3_INTF_EXIST(&intf))
	{
		return SYS_ERR_FAILED;
	}

	sys_l3_intfId2Str(&intf, ifStr);

	SYS_MEM_CLEAR(intfInfo);
	intfInfo.bFirst = TRUE;
	intfInfo.instance = 1;
	strcpy(intfInfo.ifname, ifStr);
	
	{
		rc = sal_l3_ospf_intf_info_get(DATA_OP_GETNEXT, &intfInfo);
		if (VOS_ERR_NONE != rc)
		{
			
			return SYS_ERR_FAILED;
		}

		if (intfInfo.prefix)
		{
			SYS_MEM_CLEAR(ipv4);
			IPV42STR(intfInfo.prefix, ipv4);
		}

		if ((intfInfo.type != OSPF_IFTYPE_LOOPBACK) && (intfInfo.type != OSPF_IFTYPE_BROADCAST))
		{
			SYS_MEM_CLEAR(cmd);
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "ospf network %s", ospf_int_type_str[intfInfo.type]);
			osal_strcat(cmd, tmpStr);
			#if 0
			if (intfInfo.prefix)
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " %s", ipv4);
				osal_strcat(cmd, tmpStr);
			}
			#endif
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		if (intfInfo.auth_type != OSPF_AUTH_NOTSET)
		{
			const char *auth_str;

			switch (intfInfo.auth_type)
			{
				case OSPF_AUTH_NULL:
					auth_str = " null";
					break;
				case OSPF_AUTH_SIMPLE:
					auth_str = "simple";
					break;
				case OSPF_AUTH_CRYPTOGRAPHIC:
					auth_str = " message-digest";
					break;
				default:
					auth_str = "";
					break;
			}
			SYS_MEM_CLEAR(cmd);
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "ospf authentication%s", auth_str);
			osal_strcat(cmd, tmpStr);
			#if 0
			if (intfInfo.prefix)
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " %s", ipv4);
				osal_strcat(cmd, tmpStr);
			}
			#endif
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		if (intfInfo.auth_key[0] != '\0')
		{

			if (intfInfo.auth_type == OSPF_AUTH_SIMPLE)
			{
				SYS_MEM_CLEAR(cmd);
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, "ospf authentication-key %s", intfInfo.auth_key);
				osal_strcat(cmd, tmpStr);
				#if 0
				if (intfInfo.prefix)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " %s", ipv4);
					osal_strcat(cmd, tmpStr);
				}
				#endif
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, "\n");
				osal_strcat(cmd, tmpStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}
			else if (intfInfo.auth_type == OSPF_AUTH_CRYPTOGRAPHIC)
			{
				SYS_MEM_CLEAR(cmd);
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, "ospf message-digest-key %d md5 %s", intfInfo.key_id, intfInfo.auth_key);
				osal_strcat(cmd, tmpStr);
				#if 0
				if (intfInfo.prefix)
				{
					SYS_MEM_CLEAR(tmpStr);
					osal_sprintf(tmpStr, " %s", ipv4);
					osal_strcat(cmd, tmpStr);
				}
				#endif
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, "\n");
				osal_strcat(cmd, tmpStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

		}

		if (intfInfo.output_cost && (intfInfo.output_cost != OSPF_OUTPUT_COST_DEFAULT))
		{
			SYS_MEM_CLEAR(cmd);
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "ospf cost %u", intfInfo.output_cost);
			osal_strcat(cmd, tmpStr);
			#if 0
			if (intfInfo.prefix)
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " %s", ipv4);
				osal_strcat(cmd, tmpStr);
			}
			#endif
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		if (intfInfo.hello_time && (intfInfo.hello_time != OSPF_HELLO_INTERVAL_DEFAULT))
		{
			SYS_MEM_CLEAR(cmd);
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "ospf hello-interval %u", intfInfo.hello_time);
			osal_strcat(cmd, tmpStr);
			#if 0
			if (intfInfo.prefix)
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " %s", ipv4);
				osal_strcat(cmd, tmpStr);
			}
			#endif
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		if (intfInfo.dead_time && (intfInfo.dead_time != OSPF_ROUTER_DEAD_INTERVAL_DEFAULT))
		{
			SYS_MEM_CLEAR(cmd);
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "ospf dead-interval %u", intfInfo.dead_time);
			osal_strcat(cmd, tmpStr);
			#if 0
			if (intfInfo.prefix)
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " %s", ipv4);
				osal_strcat(cmd, tmpStr);
			}
			#endif
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		if (intfInfo.retransmit_interval && (intfInfo.retransmit_interval != OSPF_RETRANSMIT_INTERVAL_DEFAULT))
		{
			SYS_MEM_CLEAR(cmd);
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "ospf retransmit-interval %u", intfInfo.retransmit_interval);
			osal_strcat(cmd, tmpStr);

			#if 0
			if (intfInfo.prefix)
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " %s", ipv4);
				osal_strcat(cmd, tmpStr);
			}
			#endif

			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		if (intfInfo.transmit_delay && (intfInfo.transmit_delay != OSPF_TRANSMIT_DELAY_DEFAULT))
		{
			SYS_MEM_CLEAR(cmd);
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "ospf transmit-delay %u", intfInfo.transmit_delay);
			osal_strcat(cmd, tmpStr);

			#if 0
			if (intfInfo.prefix)
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " %s", ipv4);
				osal_strcat(cmd, tmpStr);
			}
			#endif

			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}

		if (intfInfo.priority && (intfInfo.priority != OSPF_ROUTER_PRIORITY_DEFAULT))
		{
			SYS_MEM_CLEAR(cmd);
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "ospf priority %u", intfInfo.priority);
			osal_strcat(cmd, tmpStr);

			#if 0
			if (intfInfo.prefix)
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " %s", ipv4);
				osal_strcat(cmd, tmpStr);
			}
			#endif

			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}
		#if 0
		
		if (OSPF_IF_PARAM_CONFIGURED (params, if_area))
		  {
			if (ospf->instance)
			  vty_out (vty, " ip ospf %d area %s%s", ospf->instance,
				   inet_ntoa (params->if_area), VTY_NEWLINE);
			else
			  vty_out (vty, " ip ospf area %s%s",
				   inet_ntoa (params->if_area), VTY_NEWLINE);

		  }
		#endif
		if (intfInfo.mtu_ignore != OSPF_MTU_IGNORE_DEFAULT)
		{
			SYS_MEM_CLEAR(cmd);
			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "ospf mtu-ignore");
			osal_strcat(cmd, tmpStr);

			#if 0
			if (intfInfo.prefix)
			{
				SYS_MEM_CLEAR(tmpStr);
				osal_sprintf(tmpStr, " %s", ipv4);
				osal_strcat(cmd, tmpStr);
			}
			#endif

			SYS_MEM_CLEAR(tmpStr);
			osal_sprintf(tmpStr, "\n");
			osal_strcat(cmd, tmpStr);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}
	}

	return SYS_ERR_OK;
}

