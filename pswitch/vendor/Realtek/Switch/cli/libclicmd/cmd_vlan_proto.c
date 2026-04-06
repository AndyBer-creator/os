
#include <libcmd/cmd.h>

int32 cmd_vlan_proto_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlan_protoGroup_t  protoGroup;
    uint32 groupIdx;

    osal_memset(&protoGroup, 0, sizeof(sys_vlan_protoGroup_t));

    XMORE("\n");
    XMORE("   Group ID    |     Status    |      Type      |        value     \n");
    XMORE("---------------+---------------+----------------+------------------\n");

    if (1 == argc)
    {
        groupIdx = SYS_STR2UINT(argv[0]);

        SYS_ERR_CHK(sal_vlan_protoGroup_get(groupIdx - 1, &protoGroup));

        XMORE("      %u        |", groupIdx);

        if (protoGroup.frametype == FRAME_TYPE_END)
        {
            XMORE("  Disabled     |       --       |         --       \n");
        }
        else
        {
            XMORE("  Enabled      |");

            switch (protoGroup.frametype)
            {
                case FRAME_TYPE_ETHERNET:
                    XMORE("    Ethernet    |");
                    break;

                case FRAME_TYPE_RFC1042:
                    XMORE("     SNAP_1024  |");
                    break;

                case FRAME_TYPE_LLCOTHER:
                    XMORE("   LLC other    |");
                    break;

                default:
                    XMORE("   Not support  |");
                    break;
            }

            XMORE("       0x%04X     \n", protoGroup.framevalue);
        }
    }
    else
    {
        for (groupIdx = 0; groupIdx < CAPA_VLAN_PROTO_GROUP_NUM; groupIdx++)
        {
            SYS_ERR_CHK(sal_vlan_protoGroup_get(groupIdx, &protoGroup));

            XMORE("      %u        |", groupIdx + 1);

            if (protoGroup.frametype == FRAME_TYPE_END)
            {
                XMORE("  Disabled     |       --       |         --       \n");
            }
            else
            {
                XMORE("  Enabled      |");
                switch (protoGroup.frametype)
                {
                    case FRAME_TYPE_ETHERNET:
                        XMORE("    Ethernet    |");
                        break;

                    case FRAME_TYPE_RFC1042:
                        XMORE("     SNAP_1024  |");
                        break;

                    case FRAME_TYPE_LLCOTHER:
                        XMORE("   LLC other    |");
                        break;

                    default:
                        XMORE("   Not support  |");
                        break;
                }

                XMORE("       0x%04X     \n", protoGroup.framevalue);
            }
        }
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_vlan_portProto_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char strPort[CAPA_PORT_STR_LEN] = {0};
    sys_logic_port_t logicPort = 0;
    sys_logic_portmask_t logicPortmask;
    sys_vlan_protoVlanCfg_t vlanCfg;
    uint32 groupIdx = 0;
    sys_enable_t enable;

    osal_memset(&vlanCfg, 0, sizeof(sys_vlan_protoVlanCfg_t));

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE("\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        LP2STR(logicPort, strPort);

        XMORE("Port %s : \n", strPort);

        for (groupIdx = 0; groupIdx < CAPA_VLAN_PROTO_GROUP_NUM; groupIdx++)
        {

            XMORE("  Group %u \n", groupIdx + 1);

            SYS_ERR_CONTINUE(sal_vlan_portProtoVlan_get(logicPort, groupIdx, &vlanCfg, &enable));

            if (enable == ENABLED)
            {
                XMORE("    Status \t: Enabled\n");
                XMORE("    VLAN ID\t: %u\n", vlanCfg.vid);
            }
            else
            {
                XMORE("    Status \t: Disabled\n");
            }
        }

        XMORE("\n");
    }
    return SYS_ERR_OK;
}

int32 cmd_vlan_pvlanGroup_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlan_protoGroup_t   protoGroup;
    uint32 groupIdx = 0;

    osal_memset(&protoGroup, 0, sizeof(sys_vlan_protoGroup_t));

    groupIdx = SYS_STR2UINT(argv[0]) - 1;

    if ('e' == argv[1][0])
        protoGroup.frametype = FRAME_TYPE_ETHERNET;
    else if ('l' == argv[1][0])
        protoGroup.frametype = FRAME_TYPE_LLCOTHER;
    else
        protoGroup.frametype = FRAME_TYPE_RFC1042;

    if (osal_strlen(argv[2]) > osal_strlen("0xfffe"))
        return SYS_ERR_OUT_OF_RANGE;

    protoGroup.framevalue = SYS_STR2UINT(argv[2]);

    if ((protoGroup.framevalue < 0x600) || (protoGroup.framevalue > 0xFFFE))
        return SYS_ERR_OUT_OF_RANGE;

    SYS_ERR_CHK(sal_vlan_protoGroup_set(groupIdx, &protoGroup));

    return SYS_ERR_OK;
}

int32 cmd_vlan_pvlanGroup_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 groupIdx = 0;
    sys_logic_port_t port;
    sys_enable_t  enable;
    sys_vlan_protoGroup_t protoGroup;
    sys_vlan_protoVlanCfg_t vlan_cfg;

    groupIdx = SYS_STR2UINT(argv[0]) - 1;

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_vlan_portProtoVlan_get(port, groupIdx, &vlan_cfg, &enable));

        if (ENABLED == enable)
            return SYS_ERR_VLAN_PROVLAN_PORT_REF;
    }

    SYS_MEM_CLEAR(protoGroup);
    protoGroup.frametype = FRAME_TYPE_END;
    SYS_ERR_CHK(sal_vlan_protoGroup_set(groupIdx, &protoGroup));

    return SYS_ERR_OK;
}

int32 cmd_vlan_pvlanVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_vlan_protoVlanCfg_t vlanCfg;
    uint32 groupIdx = 0;

    SYS_MEM_CLEAR(vlanCfg);

    groupIdx = SYS_STR2UINT(argv[0]) - 1;
    vlanCfg.vid = SYS_STR2UINT(argv[1]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_vlan_portProtoVlan_set(logicPort, groupIdx, &vlanCfg, ENABLED), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_pvlanVlan_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_vlan_protoVlanCfg_t vlanCfg;
    uint32 groupIdx = 0;
    sys_vid_t pvid;

    SYS_MEM_CLEAR(vlanCfg);

    groupIdx = SYS_STR2UINT(argv[0]) - 1;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        SYS_ERR_CONTINUE(sal_vlan_portOperPvid_get(logicPort, &pvid));

        vlanCfg.vid = pvid;

        CMD_ERR_CONTINUE_PORT(sal_vlan_portProtoVlan_set(logicPort, groupIdx, &vlanCfg, DISABLED), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_proto_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 groupIdx = 0;
    sys_vlan_protoGroup_t  protoGroup;
    uint32 typeIdx = 0;
    char * strType[] = {"ethernet_ii","llc_other", "snap_1042"};

    for (groupIdx = 0; groupIdx < CAPA_VLAN_PROTO_GROUP_NUM; groupIdx++)
    {
        osal_memset(&protoGroup, 0, sizeof(sys_vlan_protoGroup_t));

        SYS_ERR_CONTINUE(sal_vlan_protoGroup_get(groupIdx, &protoGroup));

        if (protoGroup.frametype != FRAME_TYPE_END)
        {
            switch (protoGroup.frametype)
            {
                case FRAME_TYPE_ETHERNET:
                    typeIdx = 0;
                    break;

                case FRAME_TYPE_LLCOTHER:
                    typeIdx = 1;
                    break;

                case FRAME_TYPE_RFC1042:
                    typeIdx = 2;
                    break;

                default:
                    continue;
            }

            osal_memset(cmd, 0, sizeof(MAX_CMD_STRLEN));
            osal_sprintf(cmd, "vlan protocol-vlan group %u frame-type %s protocol-value 0x%x\n",
                groupIdx+1, strType[typeIdx], protoGroup.framevalue);

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_proto_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 groupIdx = 0;
    sys_enable_t enable;
    sys_vlan_protoVlanCfg_t vlan_cfg;

    for (groupIdx = 0; groupIdx < CAPA_VLAN_PROTO_GROUP_NUM; groupIdx++)
    {
        SYS_MEM_CLEAR(vlan_cfg);

        SYS_ERR_CONTINUE(sal_vlan_portProtoVlan_get(port, groupIdx, &vlan_cfg, &enable));

        if (ENABLED == enable)
        {
            SYS_MEM_CLEAR(cmd);

            if (vlan_cfg.pri)
                osal_sprintf(cmd, "vlan protocol-vlan group %u vlan %u priority %u\n",
                             groupIdx + 1, vlan_cfg.vid, vlan_cfg.pri);
            else
                osal_sprintf(cmd, "vlan protocol-vlan group %u vlan %u\n",
                             groupIdx + 1, vlan_cfg.vid);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

