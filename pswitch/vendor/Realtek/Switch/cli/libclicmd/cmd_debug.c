
#include <libcmd/cmd.h>
#include <libclicmd/cmd_desc.h>

const sys_text_t text_dbg_aclmgr_desc[] =
{
    {              "common acl API"    },
#ifdef CONFIG_SYS_MGMT_ACL
    {             "management acl"    },
#endif
    {              "reserve acl"       },
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    {         "user-defined acl"  },
#endif
};

#ifdef CONFIG_SYS_PROTO_AUTHMGR
const sys_text_t text_dbg_authmgr_desc[] =
{
    {      "dot1x"     },
    {      "mac auth"  },
    {      "web auth"  },
};
#endif

#ifdef CONFIG_SYS_L3_ROUTE
const sys_text_t text_dbg_l3_desc[] =
{
    {   "intf"        },
    {   "route"       },
    {   "fib"         },
    {   "fpm"         },
};
#endif

#ifdef CONFIG_SYS_PROTO_LLDP
const sys_text_t text_dbg_lldp_desc[] =
{
    {   "common"    },
    {   "packet"    },
    {   "poe"       },
    {   "rx"        },
    {   "tx"        },
};
#endif

const sys_text_t text_dbg_vlan_desc[] =
{
    {   "vlan common"           },
#ifdef CONFIG_SYS_PROTO_GVRP
    {   "gvrp protocol"         },
#endif
#ifdef CONFIG_SYS_MACVLAN
    {   "mac vlan"              },
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {   "protocol vlan"         },
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {   "surveillance vlan"     },
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    {   "voice vlan"            },
#endif
};

#ifdef CONFIG_SYS_DHCP_SERVER
const sys_text_t text_dbg_dhcp_server_desc[] =
{
    {   "common"    },
    {   "packet"    },
    {   "cmd"       },
    {   "rx"        },
    {   "tx"        },
};
#endif

#define CMD_DBG_DESC_INITIALIZER(_cat, _text, _extdesc) \
[_cat]  =                                               \
{                                                       \
    .text    = _text,                                   \
    .extdesc = _extdesc,                                \
}

const cmd_dbg_desc_t cmd_dbg_desc[DBG_CAT_ALL] =
{
    CMD_DBG_DESC_INITIALIZER(DBG_ACLMGR,        "ACLMGR",
                                                text_dbg_aclmgr_desc),
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    CMD_DBG_DESC_INITIALIZER(DBG_AUTHMGR,       "AUTHMGR",
                                                text_dbg_authmgr_desc),
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    CMD_DBG_DESC_INITIALIZER(DBG_DAI,           "DAI", NULL),
#endif
#ifdef CONFIG_SYS_PROTO_DHCP_SNOOPING
    CMD_DBG_DESC_INITIALIZER(DBG_DHCP_SNOOPING, "DHCP Snooping", NULL),
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    CMD_DBG_DESC_INITIALIZER(DBG_IPSG,          "IP Source Guard", NULL),
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    CMD_DBG_DESC_INITIALIZER(DBG_L3,            "L3", text_dbg_l3_desc),
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    CMD_DBG_DESC_INITIALIZER(DBG_LLDP,          "LLDP",
                                                text_dbg_lldp_desc),
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
    CMD_DBG_DESC_INITIALIZER(DBG_PSECURE,       "PSECURE", NULL),
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    CMD_DBG_DESC_INITIALIZER(DBG_RMON,          "RMON", NULL),
#endif
    CMD_DBG_DESC_INITIALIZER(DBG_TIME,          "TIME", NULL),
#ifdef CONFIG_SYS_PROTO_STP
    CMD_DBG_DESC_INITIALIZER(DBG_STP,           "Spanning Tree", NULL),
#endif
    CMD_DBG_DESC_INITIALIZER(DBG_LAG,          "lag", NULL),
#ifdef CONFIG_SYS_PROTO_UDLD
    CMD_DBG_DESC_INITIALIZER(DBG_UDLD,          "UDLD", NULL),
#endif
    CMD_DBG_DESC_INITIALIZER(DBG_VLAN,          "VLAN",
                                                text_dbg_vlan_desc),
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    CMD_DBG_DESC_INITIALIZER(DBG_MCAST,         "MCAST", NULL),
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
	CMD_DBG_DESC_INITIALIZER(DBG_ERPS,			"ERPS", NULL),
#endif

    CMD_DBG_DESC_INITIALIZER(DBG_PLATFORM,         "PLATFORM", NULL),
#ifdef CONFIG_SYS_DHCP_SERVER
    CMD_DBG_DESC_INITIALIZER(DBG_DHCP_SERVER,    "DHCP SERVER", 
    											text_dbg_dhcp_server_desc),
#endif
};

static int32 _cmd_debug_booting_set(sys_enable_t booting)
{
    uint32 i = 0;
    char *token = NULL;
    char buf[SYS_BUF4096_LEN];
    FILE *fp;

    sys_enable_t enable;

    SYS_MEM_CLEAR(buf);

    fp = fopen(SKI_CORE_PARAM_FILE_NAME, "w+");

    if (NULL != fp)
    {
        fgets(buf, sizeof(buf), fp);
        rewind(fp);
        buf[osal_strlen(buf)] = 0;
        token = strtok(buf, " ");

        while (NULL != token)
        {
            
            if (NULL == strstr(token, SYS_DBG_MASK_STR))
            {
                if (NULL != index(token, '\n'))
                    *(index(token, '\n')) = 0;
                fprintf(fp, "%s ", token);
            }

            token = strtok(NULL, " ");
        }

       if (booting)
        {
            fprintf(fp, "%s=", SYS_DBG_MASK_STR);

            for (i = 0; i < DBG_CAT_ALL; i++)
            {
                enable = DISABLED;

                sys_dbg_enable_get(i, &enable);

                fprintf(fp, "%u,", ENABLED == enable ? TRUE : FALSE);
            }

            fprintf(fp, " ");
        }

        fclose(fp);
    }

    return SYS_ERR_OK;
}

int32 cmd_debug_init(void)
{
    _cmd_debug_booting_set(FALSE);

    return SYS_ERR_OK;
}

int32 cmd_debug_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char buf[SYS_BUF4096_LEN];
    char *t;
    char *p;
    FILE *fp = NULL;

    int32 cat = 0;
    int32 ext = 0;
    int32 base = 0;

    sys_enable_t enable;

    for (cat = 0; cat < DBG_CAT_ALL; cat++)
    {
        enable = DISABLED;

        SYS_ERR_CONTINUE(sys_dbg_enable_get(cat, &enable));

        if (DISABLED == enable || !cmd_dbg_desc[cat].text || 0 == strlen(cmd_dbg_desc[cat].text))
            continue;

        XMORE("%s is on\n", cmd_dbg_desc[cat].text);

        if (!cmd_dbg_desc[cat].extdesc)
            continue;

        base = DBG_EXT_BASE(cat);

        for (ext = 0; ext < DBG_EXT_MAX; ext++)
        {
            SYS_ERR_CONTINUE(sys_dbg_enable_get(ext + base, &enable));

            if (ENABLED == enable)
                XMORE("  %s debugging is on\n", cmd_dbg_desc[cat].extdesc[ext].text);
        }
    }

    SYS_MEM_CLEAR(buf);

    fp = fopen(SKI_CORE_PARAM_FILE_NAME, "r");

    if (NULL != fp)
    {
        fgets(buf, sizeof(buf), fp);

        fclose(fp);

        t = strtok(buf, " ");

        while (NULL != t)
        {
            p = strstr(t, SYS_DBG_MASK_STR);

            if (NULL != p)
            {
                XMORE("\nBooting print is on for above category\n");
                break;
            }

            t = strtok(NULL, " ");
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_debug_booting_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    _cmd_debug_booting_set(!CMD_IS_NO_FORM_EXIST());

    return SYS_ERR_OK;
}

int32 cmd_acl_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbg;
    sys_enable_t enable;

    enable = CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED;

    if (0 == argc)
    {
        dbg = DBG_ACLMGR;
    }
    else if (1 == argc)
    {
        if ('a' == argv[0][0])
            dbg = DBG_ACLMGR_ALL;
        else if ('c' == argv[0][0])
            dbg = DBG_ACLMGR_API;
#ifdef CONFIG_SYS_MGMT_ACL
        else if ('m' == argv[0][0])
            dbg = DBG_ACLMGR_MGMT;
#endif
        else if ('r' == argv[0][0])
            dbg = DBG_ACLMGR_RSV;
#ifdef CONFIG_SYS_USER_DEFINED_ACL
        else if ('u' == argv[0][0])
            dbg = DBG_ACLMGR_USER_DEF;
#endif
        else
            return SYS_ERR_INPUT;
    }
    else
    {
        return SYS_ERR_INPUT;
    }

    SYS_ERR_CHK(sys_dbg_enable_set(dbg, enable));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_AUTHMGR

int32 cmd_auth_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbg;;
    sys_enable_t enable;

    enable = CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED;

    if (0 == argc)
    {
        dbg = DBG_AUTHMGR;
    }
    else if (1 == argc)
    {
        if ('a' == argv[0][0])
            dbg = DBG_AUTHMGR_ALL;
        else if ('d' == argv[0][0])
            dbg = DBG_AUTHMGR_DOT1X;
        else if ('m' == argv[0][0])
            dbg = DBG_AUTHMGR_MACAUTH;
        else if ('w' == argv[0][0])
            dbg = DBG_AUTHMGR_WEBAUTH;
        else
            return SYS_ERR_INPUT;
    }
    else
    {
        return SYS_ERR_INPUT;
    }

    SYS_ERR_CHK(sys_dbg_enable_set(dbg, enable));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_DAI

int32 cmd_dai_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_DAI, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

int32 cmd_ds_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_DHCP_SNOOPING, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_ISG

int32 cmd_isg_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_IPSG, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}
#endif

int32 cmd_l2_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbg = DBG_L3;
    uint32 token = 0;
    uint32 beg = 0;
    sys_enable_t enable = ENABLED;

    if (CMD_IS_NO_FORM_EXIST())
    {
        beg++;
        enable = DISABLED;
    }

    token = CMD_TOKENNUM_GET(cmd->vline) - beg;

    if (2 == token)
    {
        dbg = DBG_L2;
    }
    else
    {
        return SYS_ERR_INPUT;
    }

    SYS_ERR_CHK(sys_dbg_enable_set(dbg, enable));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_L3_ROUTE

int32 cmd_l3_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbg = DBG_L3;
    uint32 token = 0;
    uint32 beg = 0;
    char *ridx = NULL;
    sys_enable_t enable = ENABLED;

    if (CMD_IS_NO_FORM_EXIST())
    {
        beg++;
        enable = DISABLED;
    }

    token = CMD_TOKENNUM_GET(cmd->vline) - beg;

    if (2 == token)
    {
        dbg = DBG_L3;
    }
    else if (3 == token)
    {
        ridx = CMD_TOKENSTR_GET(cmd->vline, token + beg - 1);

        if ('a' == ridx[0])
            dbg = DBG_L3_ALL;
        else if ('i' == ridx[0])
            dbg = DBG_L3_INTF;
        else if ('r' == ridx[0])
            dbg = DBG_L3_ROUTE;
        else if ('f' == ridx[0] && 'i' == ridx[1])
            dbg = DBG_L3_FIB;
        else if ('f' == ridx[0] && 'p' == ridx[1])
            dbg = DBG_L3_FPM;
        else
            return SYS_ERR_INPUT;
    }
    else
    {
        return SYS_ERR_INPUT;
    }

    SYS_ERR_CHK(sys_dbg_enable_set(dbg, enable));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_LLDP

int32 cmd_lldp_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbg = DBG_LLDP;
    uint32 token = 0;
    uint32 beg = 0;
    char *ridx = NULL;
    sys_enable_t enable = ENABLED;

    if (CMD_IS_NO_FORM_EXIST())
    {
        beg++;
        enable = DISABLED;
    }

    token = CMD_TOKENNUM_GET(cmd->vline) - beg;

    if (2 == token)
    {
        dbg = DBG_LLDP;
    }
    else if (3 == token)
    {
        ridx = CMD_TOKENSTR_GET(cmd->vline, token + beg - 1);

        if ('a' == ridx[0])
            dbg = DBG_LLDP_ALL;
        else if ('c' == ridx[0])
            dbg = DBG_LLDP_COMMON;
        else if ('p' == ridx[0] && 'a' == ridx[1])
            dbg = DBG_LLDP_PKT;
        else if ('p' == ridx[0] && 'o' == ridx[1])
            dbg = DBG_LLDP_POE;
        else if ('r' == ridx[0])
            dbg = DBG_LLDP_RX;
        else if ('t' == ridx[0])
            dbg = DBG_LLDP_TX;
        else
            return SYS_ERR_INPUT;
    }
    else
    {
        return SYS_ERR_INPUT;
    }

    SYS_ERR_CHK(sys_dbg_enable_set(dbg, enable));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PORT_SECURITY

int32 cmd_psecure_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_PSECURE, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI

int32 cmd_rmon_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_RMON, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}
#endif

int32 cmd_time_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbg;
    sys_enable_t enable;

    enable = CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED;

    if (0 == argc)
    {
        dbg = DBG_TIME;
    }
    else
    {
        return SYS_ERR_INPUT;
    }

    SYS_ERR_CHK(sys_dbg_enable_set(dbg, enable));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_UDLD

int32 cmd_udld_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_UDLD, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}
#endif

int32 cmd_vlan_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbg;
    sys_enable_t enable;

    enable = CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED;

    if (0 == argc)
    {
        dbg = DBG_VLAN;
    }
    else if (1 == argc)
    {
        if ('a' == argv[0][0])
            dbg = DBG_VLAN_ALL;
        else if ('c' == argv[0][0])
            dbg = DBG_VLAN;
#ifdef CONFIG_SYS_MACVLAN
        else if ('m' == argv[0][0])
            dbg = DBG_VLAN_MAC;
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
        else if ('p' == argv[0][0])
            dbg = DBG_VLAN_PROTOCOL;
#endif
#ifdef CONFIG_SYS_VOICEVLAN
        else if ('v' == argv[0][0])
            dbg = DBG_VLAN_VOICE;
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
        else if ('s' == argv[0][0])
            dbg = DBG_VLAN_SURVEILLANCE;
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
        else if ('g' == argv[0][0])
            dbg = DBG_VLAN_GVRP;
#endif
        else
            return SYS_ERR_INPUT;
    }
    else
    {
        return SYS_ERR_INPUT;
    }

    SYS_ERR_CHK(sys_dbg_enable_set(dbg, enable));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP

int32 cmd_debug_stp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_STP, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}
#endif

int32 cmd_debug_lag_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_LAG, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_debug_mcast_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_MCAST, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_ERPS

int32 cmd_debug_erps_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_ERPS, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}
#endif

int32 cmd_debug_platform_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sys_dbg_enable_set(DBG_PLATFORM, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_DHCP_SERVER

int32 cmd_dhcp_server_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbg = DBG_LLDP;
    uint32 token = 0;
    uint32 beg = 0;
    char *ridx = NULL;
    sys_enable_t enable = ENABLED;

    if (CMD_IS_NO_FORM_EXIST())
    {
        beg++;
        enable = DISABLED;
    }

    token = CMD_TOKENNUM_GET(cmd->vline) - beg;

    if (2 == token)
    {
        dbg = DBG_DHCP_SERVER;
    }
    else if (3 == token)
    {
        ridx = CMD_TOKENSTR_GET(cmd->vline, token + beg - 1);
        if ('a' == ridx[0])
            dbg = DBG_DHCP_SERVER_ALL;
        else if ('p' == ridx[0])
            dbg = DBG_DHCP_SERVER_PKT;
        else if ('c' == ridx[0] && 'o' == ridx[1])
            dbg = DBG_DHCP_SERVER_COMMON;
        else if ('c' == ridx[0] && 'm' == ridx[1])
            dbg = DBG_DHCP_SERVER_CMD;
        else if ('r' == ridx[0])
            dbg = DBG_DHCP_SERVER_RX;
        else if ('t' == ridx[0])
            dbg = DBG_DHCP_SERVER_TX;
        else
            return SYS_ERR_INPUT;
    }
    else
    {
        return SYS_ERR_INPUT;
    }
    SYS_ERR_CHK(sys_dbg_enable_set(dbg, enable));

    return SYS_ERR_OK;
}
#endif
