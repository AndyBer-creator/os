
#include <libsal/sal_mirror.h>
#include <libsal/sal_lldp.h>
#include <libsal/sal_lacp.h>
#ifdef CONFIG_SYS_PROTO_GVRP
#include <libsal/sal_gvrp.h>
#endif
#include <libcmd/cmd.h>

int32 cmd_mirror_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_gid_t gid;
    sys_logic_port_t destPort;
    sys_mirror_state_t state;
    sys_logic_portmask_t rxPortmask;
    sys_logic_portmask_t txPortmask;
    sys_enable_t enable;
    char port_str[8];
    char ingportmask_str[64];
    char egportmask_str[64];

    for(gid = 0; gid < CAPA_MIRROR_SESSION_NUM; gid ++)
    {
        XMORE("\nSession %d Configuration\n", GID2UGID(gid));

            SYS_ERR_CHK(sal_mirror_group_state_get(gid,&state));

            if(PORTBASED_ENABLED == state)
            {
                SYS_ERR_CHK(sal_mirror_group_sourcePort_get(gid,&rxPortmask, &txPortmask));
                SYS_ERR_CHK(LPM2STR(&rxPortmask, ingportmask_str));
                SYS_ERR_CHK(LPM2STR(&txPortmask, egportmask_str));

                XMORE("Source RX Port    : %s\n", ingportmask_str);
                XMORE("Source TX Port    : %s\n", egportmask_str);

            }
            else
            {
                XMORE("Mirrored source   : Not Config\n");
            }

            SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid, &destPort, &enable));
            if (ENABLED == enable)
            {
                SYS_ERR_CHK(LP2STR(destPort, port_str));
                XMORE("Destination port  : %s\n", port_str);

                SYS_ERR_CHK(sal_mirror_group_igrState_get(gid,&enable));

                XMORE("Ingress State: %s\n",text_enable[enable].text);
            }
            else
            {
                XMORE("Destination port  : Not Config\n");
            }

        XMORE("\n");
    }
    return SYS_ERR_OK;
}

int32 cmd_mirror_session_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_gid_t gid;
    sys_logic_port_t destPort;
    sys_mirror_state_t state;
    sys_logic_portmask_t rxPortmask;
    sys_logic_portmask_t txPortmask;
    sys_enable_t enable;
    char port_str[8];
    char ingportmask_str[64];
    char egportmask_str[64];

    gid = UGID2GID(SYS_STR2UINT(argv[0]));
    XMORE("\nSession %d Configuration\n", GID2UGID(gid));

        SYS_ERR_CHK(sal_mirror_group_state_get(gid,&state));

        if(PORTBASED_ENABLED == state)
        {
            SYS_ERR_CHK(sal_mirror_group_sourcePort_get(gid,&rxPortmask, &txPortmask));
            SYS_ERR_CHK(LPM2STR(&rxPortmask, ingportmask_str));
            SYS_ERR_CHK(LPM2STR(&txPortmask, egportmask_str));

            XMORE("Source RX Port    : %s\n", ingportmask_str);
            XMORE("Source TX Port    : %s\n", egportmask_str);

        }
        else
        {
            XMORE("Mirrored source   : Not Config\n");
        }

        SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid, &destPort, &enable));
        if (ENABLED == enable)
        {
            SYS_ERR_CHK(LP2STR(destPort, port_str));
            XMORE("Destination port  : %s\n", port_str);

            SYS_ERR_CHK(sal_mirror_group_igrState_get(gid,&enable));

            XMORE("Ingress State: %s\n",text_enable[enable].text);
        }
        else
        {
            XMORE("Destination port  : Not Config\n");
        }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_mirror_group_sport_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int32 ret;
    sys_gid_t gid;
    sys_logic_portmask_t rxPortmask,oldrxPm;
    sys_logic_portmask_t txPortmask,oldtxPm;

    gid = UGID2GID(SYS_STR2UINT(argv[0]));

    osal_memset(&rxPortmask, 0, sizeof(rxPortmask));
    osal_memset(&txPortmask, 0, sizeof(txPortmask));

    if ('r' == argv[2][0]) 
    {
        CMD_PORTLIST_GET(1, &rxPortmask);
    }
    else if ('t' == argv[2][0]) 
    {
        CMD_PORTLIST_GET(1, &txPortmask);
    }
    else if ('b' == argv[2][0]) 
    {
        CMD_PORTLIST_GET(1, &rxPortmask);
        CMD_PORTLIST_GET(1, &txPortmask);
    }

    if(CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_mirror_group_sourcePort_get(gid,&oldrxPm,&oldtxPm));
        LOGIC_PORTMASK_ANDNOT(rxPortmask,oldrxPm,rxPortmask);
        LOGIC_PORTMASK_ANDNOT(txPortmask,oldtxPm,txPortmask);
    }
    else
    {
        ret = sal_mirror_group_sourcePort_get(gid,&oldrxPm,&oldtxPm);
        if(ret == SYS_ERR_OK)
        {
            LOGIC_PORTMASK_OR(rxPortmask,oldrxPm,rxPortmask);
            LOGIC_PORTMASK_OR(txPortmask,oldtxPm,txPortmask);
        }
        else
        {
            if(SYS_ERR_MIRROR_NOT_PORTBASED != ret)
                return ret;
        }
    }
    SYS_ERR_CHK(sal_mirror_group_sourcePort_set(gid,rxPortmask, txPortmask));

    return SYS_ERR_OK;
}

int32 cmd_mirror_group_dport_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_gid_t gid;
    sys_enable_t enable;
    uint32 tokenCnt = 0;
    sys_logic_port_t mirroring_port = 0;
    sys_logic_port_t originalPort = 0;

    gid = UGID2GID(SYS_STR2UINT(argv[0]));
    SYS_ERR_CHK(STR2LP(argv[1], &mirroring_port));

    SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid,&originalPort,&enable));

    if(CMD_IS_NO_FORM_EXIST())
    {
        if(DISABLED == enable)
            return SYS_ERR_OK;
        else if(mirroring_port != originalPort)
            return SYS_ERR_MIRROR_TARGET;
        else
            SYS_ERR_CHK(sal_mirror_group_targetPort_set(gid,originalPort,DISABLED));

        return SYS_ERR_OK;
    }

    if(ENABLED == enable)
        SYS_ERR_CHK(sal_mirror_group_targetPort_set(gid,originalPort,DISABLED));

    SYS_ERR_CHK(sal_mirror_group_targetPort_set(gid,mirroring_port,ENABLED));

    tokenCnt = CMD_TOKENNUM_GET(cmd->vline);
    if(7 == tokenCnt)
        SYS_ERR_CHK(sal_mirror_group_igrState_set(gid,ENABLED));
    else
        SYS_ERR_CHK(sal_mirror_group_igrState_set(gid,DISABLED));

    return SYS_ERR_OK;
}

int32 cmd_mirror_group_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_gid_t delGid;

    if('a' != argv[0][0])
    {
        delGid = UGID2GID(SYS_STR2UINT(argv[0]));
        SYS_ERR_CHK(sal_mirror_group_del(delGid));
    }
    else
    {
        for(delGid = 0; delGid < CAPA_MIRROR_SESSION_NUM; delGid ++)
        {
            SYS_ERR_CHK(sal_mirror_group_del(delGid));
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_mirror_session_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char port_str[CAPA_PORT_STR_LEN];
    char portmask_str[CAPA_PORTMASK_STR_LEN];
    sys_gid_t gid;
    sys_enable_t enable = DISABLED;
    sys_enable_t igrState = DISABLED;
    sys_logic_port_t port;
    sys_mirror_state_t state;
    sys_logic_portmask_t rxPortmask;
    sys_logic_portmask_t txPortmask;

    osal_memset(&rxPortmask, 0, sizeof(rxPortmask));
    osal_memset(&txPortmask, 0, sizeof(txPortmask));

    FOR_EACH_GID(gid)
    {
        
        SYS_ERR_CHK(sal_mirror_group_state_get(gid, &state));

        if (PORTBASED_ENABLED == state)
        {
            SYS_ERR_CHK(sal_mirror_group_sourcePort_get(gid,&rxPortmask, &txPortmask));

            if (!IS_LOGIC_PORTMASK_CLEAR(rxPortmask))
            {
                SYS_ERR_CHK(LPM2STR(&rxPortmask, portmask_str));

                osal_memset(cmd, 0, sizeof(cmd));
                osal_sprintf(cmd, "mirror session %u source interfaces %s rx\n", GID2UGID(gid), portmask_str);
                CMD_WRITE_TO_FILE(fp, cmd);
            }

            if (!IS_LOGIC_PORTMASK_CLEAR(txPortmask))
            {
                SYS_ERR_CHK(LPM2STR(&txPortmask, portmask_str));

                osal_memset(cmd, 0, sizeof(cmd));
                osal_sprintf(cmd, "mirror session %u source interfaces %s tx\n", GID2UGID(gid), portmask_str);
                CMD_WRITE_TO_FILE(fp, cmd);
            }
        }
        
        SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid, &port, &enable));
        SYS_ERR_CHK(sal_mirror_group_igrState_get(gid, &igrState));

        if (ENABLED == enable)
        {
            SYS_ERR_CHK(LP2STR(port, port_str));
            osal_memset(cmd, 0, sizeof(cmd));

            osal_sprintf(cmd, "mirror session %u destination interface %s %s\n",
                GID2UGID(gid),
                port_str,
                (ENABLED == igrState)? "allow-ingress":"");

            CMD_WRITE_TO_FILE(fp, cmd);
        }

    }

    return SYS_ERR_OK;
}

int32 cmd_mirror_save(FILE *fp)
{
    SYS_ERR_CHK(_cmd_mirror_session_save(fp));

    return SYS_ERR_OK;
}

