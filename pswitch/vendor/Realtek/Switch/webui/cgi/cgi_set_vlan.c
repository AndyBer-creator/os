
#include <include/cgi.h>

static int32 cgi_vlan_create_set(void);
static int32 cgi_vlan_edit_set(void);
static int32 cgi_vlan_del_set(void);
static int32 cgi_vlan_conf_set(void);
static int32 cgi_vlan_portEdit_set(void);
static int32 cgi_vlan_membershipEdit_set(void);

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
static int32 cgi_vlan_mgmt_set(void);
#endif

const cgi_cmdmap_t cmdSetVlanTbl[] =
{

    {CGI_CMD_VLAN_CREATE,           cgi_vlan_create_set},
    {CGI_CMD_VLAN_EDIT,             cgi_vlan_edit_set},
    {CGI_CMD_VLAN_DEL,              cgi_vlan_del_set},
    {CGI_CMD_VLAN_CONF,             cgi_vlan_conf_set},
    {CGI_CMD_VLAN_PORTEDIT,         cgi_vlan_portEdit_set},
    {CGI_CMD_VLAN_MEMBERSHIPEDIT,   cgi_vlan_membershipEdit_set},

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    {CGI_CMD_VLAN_MGMT,             cgi_vlan_mgmt_set},
#endif
    {NULL, NULL}
};

static int32 cgi_vlan_create_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_vid_t vid;
    sys_vlanmask_t adminVM;
    sys_vlanmask_t newVM;
    sys_vlanmask_t createdVM;
    sys_vlanmask_t deletedVM;
    sys_namestr_t name;
    uint32 i = 0;

    SYS_MEM_CLEAR(adminVM);
    SYS_MEM_CLEAR(newVM);
    SYS_MEM_CLEAR(createdVM);
    SYS_MEM_CLEAR(deletedVM);
    sal_vlan_adminEntryExist_get(&adminVM);

    if (CGI_IS_PARAM_EXIST(CGI_VLANLIST, value))
        STR2VMSK(value, &newVM);
    else
        goto cgi_end;

    VLANMASK_ANDNOT(createdVM, newVM, adminVM);
    VLANMASK_ANDNOT(deletedVM, adminVM, newVM);

    if (!IS_VLANMASK_CLEAR(deletedVM))
    {
        FOR_EACH_VID_IN_VLANMASK(vid, deletedVM)
        {
            i++;
            CGI_SET_VID_ERR_CONTINUE(sal_vlan_adminEntry_destroy(vid), vid);
        }
    }

    if (!IS_VLANMASK_CLEAR(createdVM))
    {
        FOR_EACH_VID_IN_VLANMASK(vid, createdVM)
        { i++;
            SYS_MEM_CLEAR(name);
            CGI_SET_VID_ERR_CONTINUE(sal_vlan_nameStr_get(vid, DFLT_VLAN_PREFIX_NAME, &name), vid);
            CGI_SET_VID_ERR_CONTINUE(sal_vlan_adminEntry_create(vid, &name),vid);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_edit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_vid_t vid;

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        vid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_NAME, value))
    {
        CGI_SET_ERR_HDL(sal_vlan_name_set(vid, (sys_namestr_t *)value), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_del_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_vid_t vid;
    sys_vlanmask_t vm;

    if (CGI_IS_PARAM_EXIST(CGI_VLANLIST, value))
        STR2VMSK(value, &vm);
    else
        goto cgi_end;

    FOR_EACH_VID_IN_VLANMASK(vid, vm)
    {
        CGI_SET_VID_ERR_CONTINUE(sal_vlan_adminEntry_destroy(vid), vid);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_conf_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value;
    char tmp[64];
    sys_vid_t selvid = DFLT_VLAN_DFLT_VLAN;
    sys_logic_port_t lp;
    sys_vlan_portMode_t portMode = SYS_VLAN_PORT_MODE_END;
    sys_vlan_membership_t membership = SYS_VLAN_MEMBERSHIP_END;
    sys_vlan_membership_t ori_membership;
    sys_vlan_adminEntry_t vlanEntry;
    sys_vlanmask_t vmTrunkAllowed;
    sys_vlanmask_t vmForbidden;

    SYS_MEM_CLEAR(tmp);
    SYS_MEM_CLEAR(vmTrunkAllowed);
    SYS_MEM_CLEAR(vmForbidden);
    SYS_MEM_CLEAR(vlanEntry);

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
    {
        selvid = SYS_STR2UINT(value);
        VLANMASK_SET_VID(vmForbidden, selvid);
        sal_vlan_adminEntry_get(selvid, &vlanEntry);
    }
    else
        goto cgi_end;

    FOR_EACH_LOGIC_PORT(lp)
    {
        membership = SYS_VLAN_MEMBERSHIP_END;
        sal_vlan_portAdminMode_get(lp, &portMode);
        sal_vlan_portOperVlanMembership_get(lp, selvid, &ori_membership);

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%s_%d", "forbidden", lp);

        if (CGI_IS_PARAM_EXIST(tmp, value))
        {
            
            if (!IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmForbidden, lp))
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminVlanmask_add(lp, SYS_VLAN_FORBIDDEN, &vmForbidden), lp);
            }
        }
        else
        {
            
            if (IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmForbidden, lp))
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminVlanmask_del(lp, SYS_VLAN_FORBIDDEN, &vmForbidden), lp);
            }
        }

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%s_%d", "membership", lp);

        if (CGI_IS_PARAM_EXIST(tmp, value))
        {
            membership = SYS_STR2UINT(value);

            if (membership == ori_membership)
                continue;
        }

        switch (portMode)
        {
            case SYS_VLAN_PORT_MODE_GENERAL:
                if (SYS_VLAN_MEMBERSHIP_END != membership)
                {
                    CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminVlanMembership_set(lp, selvid, membership), lp);
                }
                break;

            case SYS_VLAN_PORT_MODE_TRUNK:
                CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portTrunkAllowedVlan_get(lp, &vmTrunkAllowed), lp);

                if (SYS_VLAN_UNTAGGED == membership)
                {
                    
                    CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminPvid_set(lp, selvid), lp);
                    break;
                }
                else if (SYS_VLAN_TAGGED == membership)
                {
                    VLANMASK_SET_VID(vmTrunkAllowed, selvid);
                    CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portTrunkAllowedVlan_set(lp, &vmTrunkAllowed), lp);
                }
                else if (SYS_VLAN_EXCLUDED == membership)
                {
                    VLANMASK_CLEAR_VID(vmTrunkAllowed, selvid);
                    CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portTrunkAllowedVlan_set(lp, &vmTrunkAllowed), lp);
                }

                break;

            case SYS_VLAN_PORT_MODE_ACCESS:
            case SYS_VLAN_PORT_MODE_DOT1QTUNNEL:
                if (SYS_VLAN_UNTAGGED == membership)
                {
                    CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminPvid_set(lp, selvid), lp);
                }
                else if (SYS_VLAN_MEMBERSHIP_END != membership)
                {
                    CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminVlanMembership_set(lp, selvid, membership), lp);
                }
                break;

             default:
                break;
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_portEdit_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
#ifdef CONFIG_SYS_VLAN_QINQ
    uint16 tpid = 0x8100;
    sys_enable_t uplink;
#endif
    sys_vid_t pvid;
    sys_enable_t ingressFilter = DISABLED;
    sys_vlan_portMode_t mode;
    sys_vlan_frameType_t type = 0;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VLAN_MODE, value))
        mode = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VLAN_PVID, value))
        pvid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (SYS_VLAN_PORT_MODE_GENERAL == mode)
    {
        if (CGI_IS_PARAM_EXIST(CGI_VLAN_ACCFRAMETYPE, value))
            type = SYS_STR2UINT(value);
        else
            goto cgi_end;

        if (CGI_IS_PARAM_EXIST(CGI_VLAN_INGRESS_FILTER, value))
            ingressFilter = ENABLED;
        else
            ingressFilter = DISABLED;
    }

#ifdef CONFIG_SYS_VLAN_QINQ
    if (CGI_IS_PARAM_EXIST(CGI_VLAN_UPLINK, value))
        uplink = ENABLED;
    else
        uplink = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_VLAN_TPID, value))
        tpid = SYS_STR2UINT(value);
#endif

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        
        CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminMode_set(lp, mode), lp);

        if (SYS_VLAN_PORT_MODE_GENERAL == mode)
        {
            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminPvid_set(lp, pvid), lp);
            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminAcceptFrameType_set(lp, type), lp);
            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminIgrFilterEnable_set(lp, ingressFilter), lp);
        }
        else
        {

#ifdef CONFIG_SYS_VLAN_QINQ
            if (SYS_VLAN_PORT_MODE_TRUNK == mode)
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminRoleUplinkEnable_set(lp, uplink), lp);

                if (ENABLED == uplink)
                    CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portDot1qTunnelTpid_set(lp, tpid), lp);
            }
#endif
             
            if (!IS_STATIC_VLAN_EXIST(pvid))
            {
                ret = SYS_ERR_VLAN_WAS_NOT_CREATED_BY_USER;
                goto cgi_end;
            }

            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portAdminPvid_set(lp, pvid), lp);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_membershipEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 strLen = 0;
    uint32 i = 0;
    char *value = NULL;
    char **selVlans = NULL;
    sys_logic_port_t lp;
    sys_vlan_portMode_t mode = 0;
    sys_vid_t vid = 0;
    sys_vid_t pvid = 0;
    sys_vlanmask_t forbiddenVM;
    sys_vlanmask_t untaggedVM;
    sys_vlanmask_t taggedVM;

    if (CGI_IS_PARAM_EXIST(CGI_PORT, value))
        lp = SYS_STR2UINT(value);
    else
        goto cgi_end;

    SYS_MEM_CLEAR(forbiddenVM);
    SYS_MEM_CLEAR(untaggedVM);
    SYS_MEM_CLEAR(taggedVM);
    sal_vlan_portAdminVlanmask_get(lp, SYS_VLAN_FORBIDDEN, &forbiddenVM);
    sal_vlan_portAdminVlanmask_del(lp, SYS_VLAN_FORBIDDEN, &forbiddenVM);

    sal_vlan_portTrunkAllowedVlan_set(lp, &taggedVM);
    sal_vlan_portAdminVlanmask_get(lp, SYS_VLAN_UNTAGGED, &untaggedVM);
    sal_vlan_portAdminVlanmask_del(lp, SYS_VLAN_UNTAGGED, &untaggedVM);
    SYS_MEM_CLEAR(forbiddenVM);
    SYS_MEM_CLEAR(untaggedVM);
    SYS_MEM_CLEAR(taggedVM);

    if (CGI_GET_PARAM_ARRAY_DATA("A", selVlans, &len))
    {
        for (i = 0 ; i < len ; i++)
        {
            strLen = strlen(selVlans[i]);
            strLen--;
            vid = atoi(selVlans[i]);

            if (vid > SYS_VLAN_NUM_MAX)
                continue;

            if ('P' == (selVlans[i][strLen]))
            {
                pvid = vid;
                strLen--;
            }

            if ('U' == (selVlans[i][strLen]))
                VLANMASK_SET_VID(untaggedVM, vid);
            else if ('T' == (selVlans[i][strLen]))
                VLANMASK_SET_VID(taggedVM, vid);
            else if ('F' == (selVlans[i][strLen]))
                VLANMASK_SET_VID(forbiddenVM, vid);
        }
    }

    sal_vlan_portAdminMode_get(lp, &mode);
    CGI_SET_ERR_HDL(sal_vlan_portAdminPvid_set(lp, pvid), ret);

    if (!IS_VLANMASK_CLEAR(forbiddenVM))
        CGI_SET_ERR_HDL(sal_vlan_portAdminVlanmask_add(lp, SYS_VLAN_FORBIDDEN, &forbiddenVM), ret);

    if (!IS_VLANMASK_CLEAR(untaggedVM))
        CGI_SET_ERR_HDL(sal_vlan_portAdminVlanmask_add(lp, SYS_VLAN_UNTAGGED, &untaggedVM), ret);

    if (!IS_VLANMASK_CLEAR(taggedVM))
    {
        if (SYS_VLAN_PORT_MODE_TRUNK == mode)
            CGI_SET_ERR_HDL(sal_vlan_portTrunkAllowedVlan_set(lp, &taggedVM), ret);
        else
            CGI_SET_ERR_HDL(sal_vlan_portAdminVlanmask_add(lp, SYS_VLAN_TAGGED, &taggedVM), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);
    if (NULL != selVlans)
        osal_free(selVlans);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
static int32 cgi_vlan_mgmt_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    sys_vid_t vid;

    if (CGI_IS_PARAM_EXIST(CGI_VLAN_MGMT_VLAN, value))
         vid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_vlan_mgmtVid_set(vid), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

