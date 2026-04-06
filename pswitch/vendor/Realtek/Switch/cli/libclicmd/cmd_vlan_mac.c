
#include <libcmd/cmd.h>
#include <libsal/sal_vlan_mac.h>

int32 cmd_vlan_mac_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char tmp[CAPA_MAC_STR_LEN];
    uint32 i = 0;
    uint32 cnt = 0;
    sys_mac_vlan_group_t group;

    XMORE("\n");
    XMORE("   Mac Address    Mask   Group Id   \n");
    XMORE("----------------- ---- ------------ \n");

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        SYS_ERR_CHK(sal_macvlan_group_get(i, &group));

        if (group.mask == 0)
            continue;

        SYS_ERR_CHK(sys_util_mac2Str(group.mac.octet, tmp));
        XMORE("%15s %4u %11u\n", tmp, group.mask, group.groupId);
        cnt++;
    }

    XMORE("\n");
    XMORE("Total %u %s\n\n", cnt, (cnt > 1) ? "Entries": "Entry");

    return SYS_ERR_OK;
}

int32 cmd_vlan_mac_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i = 0;
    char portStr[CAPA_PORT_STR_LEN];
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_mac_vlan_mapping_t mapping[CAPA_VLAN_MAC_GROUP_NUM_MAX];

    SYS_MEM_CLEAR(lpm);
    SYS_MEM_CLEAR(mapping);

    if (1 == argc)
        CMD_NORMALPORTLIST_GET(0, &lpm);
    else
        LOGIC_PORTMASK_SET_ALL(lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        SYS_MEM_CLEAR(mapping);
        SYS_MEM_CLEAR(portStr);
        SYS_ERR_CHK(sal_macvlan_mapping_get(lp, mapping));

        SYS_ERR_CHK(LP2STR(lp, portStr));

        XMORE("\n");
        XMORE("Interface %s Mac based VLANs:\n", portStr);
        XMORE("  Group ID    Vlan ID\n");
        XMORE("------------  -------\n");

        for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM; i++)
        {
            if (mapping[i].groupId != 0)
                XMORE("%11u  %4u\n", mapping[i].groupId, mapping[i].vid);
        }

        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_mac_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_mac_vlan_group_t group;

    SYS_MEM_CLEAR(group);

    if (CMD_IS_NO_FORM_EXIST())
    {
        group.mask = SYS_STR2UINT(argv[1]);
        SYS_ERR_CHK(sys_util_str2Mac(argv[0], group.mac.octet));
        SYS_ERR_CHK(sal_macvlan_group_del(&group));

    }
    else
    {
        group.groupId = SYS_STR2UINT(argv[0]);
        group.mask = SYS_STR2UINT(argv[2]);
        SYS_ERR_CHK(sys_util_str2Mac(argv[1], group.mac.octet));
        SYS_ERR_CHK(sal_macvlan_group_set(&group));
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_mac_mapping_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_mac_vlan_mapping_t mapping;

    SYS_MEM_CLEAR(mapping);

    if (!CMD_IS_NO_FORM_EXIST())
    {
        mapping.vid = SYS_STR2UINT(argv[1]);
        mapping.groupId = SYS_STR2UINT(argv[0]);
    }
    else
    {
        
        if (1 == argc)
            mapping.groupId = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        if (CMD_IS_NO_FORM_EXIST())
        {
            CMD_ERR_CONTINUE_PORT(sal_macvlan_mapping_del(lp, &mapping), lp);
        }
        else
        {
            CMD_ERR_CONTINUE_PORT(sal_macvlan_mapping_set(lp, &mapping), lp);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_mac_save(FILE *fp)
{
    char tmp[CAPA_MAC_STR_LEN];
    char cmd[MAX_CMD_STRLEN];
    uint32 i = 0;
    sys_mac_vlan_group_t group;

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        SYS_ERR_CHK(sal_macvlan_group_get(i, &group));

        if (group.mask == 0)
            continue;

        SYS_ERR_CHK(sys_util_mac2Str(group.mac.octet, tmp));

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "vlan mac-vlan group %u %s mask %u\n", group.groupId, tmp, group.mask);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_mac_port_save(FILE *fp, sys_logic_port_t port)
{
    uint32 i = 0;
    char cmd[MAX_CMD_STRLEN];
    sys_mac_vlan_mapping_t mapping[CAPA_VLAN_MAC_GROUP_NUM_MAX];

    SYS_MEM_CLEAR(mapping);
    SYS_ERR_CHK(sal_macvlan_mapping_get(port, mapping));

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM; i++)
    {
        if (mapping[i].groupId == 0)
            continue;

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "vlan mac-vlan group %u vlan %u\n", mapping[i].groupId, mapping[i].vid);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

