
#include <include/cgi.h>
#include <libsal/sal_vlan.h>

static int32 cgi_vlan_create_get(void);
static int32 cgi_vlan_conf_get(void);
static int32 cgi_vlan_port_get(void);
static int32 cgi_vlan_portEdit_get(void);
static int32 cgi_vlan_membership_get(void);
static int32 cgi_vlan_membershipEdit_get(void);

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
static int32 cgi_vlan_mgmt_get(void);
#endif

const cgi_cmdmap_t cmdGetVlanTbl[] =
{
    {CGI_CMD_VLAN_CREATE,           cgi_vlan_create_get},
    {CGI_CMD_VLAN_CONF,             cgi_vlan_conf_get},
    {CGI_CMD_VLAN_PORT,             cgi_vlan_port_get},
    {CGI_CMD_VLAN_PORTEDIT,         cgi_vlan_portEdit_get},
    {CGI_CMD_VLAN_MEMBERSHIP,       cgi_vlan_membership_get},
    {CGI_CMD_VLAN_MEMBERSHIPEDIT,   cgi_vlan_membershipEdit_get},

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    {CGI_CMD_VLAN_MGMT,             cgi_vlan_mgmt_get},
#endif
    {NULL, NULL}
};

static int32 cgi_vlan_create_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *selVlans = NULL;
    cJSON *adminVlans = NULL;
    cJSON *vlan = NULL;
    char tmp[SYS_BUF16_LEN];
    uint8 hasSelVlan = FALSE;
    sys_vid_t vid ;
    sys_vlanmask_t vm;
    sys_vlanmask_t adminVM;
    sys_vlan_operEntry_t entry;
	sys_l3_intfId_t l3intf;

    SYS_MEM_CLEAR(adminVM);
    SYS_MEM_CLEAR(vm);

	CGI_GET_JSON_CREATE(root, data);
	cJSON_AddNumberToObject(data, CGI_MAXLEN, CAPA_NAME_STR_LEN);
	cJSON_AddNumberToObject(data, CGI_VLAN_MAX_VLANS, CAPA_VLAN_ADMIN_NUM);

	selVlans = cJSON_CreateArray();
	adminVlans = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_VLAN_ADMIN_VLANS, adminVlans);

    sal_vlan_operEntryExist_get(&adminVM);

    FOR_EACH_VID(vid)
    {
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "VLAN %u", vid);

        if (VLANMASK_IS_VIDSET(adminVM, vid))
        {
            SYS_MEM_CLEAR(entry);
            sal_vlan_operEntry_get(vid, &entry);
            cJSON_AddItemToArray(selVlans, cJSON_CreateNumber(vid));
            hasSelVlan = TRUE;

            vlan = cJSON_CreateObject();
            cJSON_AddItemToArray(adminVlans, vlan);
            cJSON_AddNumberToObject(vlan, CGI_VLAN, vid);
            cJSON_AddStringToObject(vlan, CGI_NAME, entry.name.str);
            cJSON_AddNumberToObject(vlan, CGI_VLAN_TYPE, entry.type);
            if(entry.type == SYS_VLAN_ENTRY_TYPE_DEFAULT)
            {
				cJSON_AddBoolToObject(vlan, CGI_VLAN_INTF_STAT, TRUE);
            }
            else 
            {
	            SYS_MEM_CLEAR(l3intf);
	            l3intf.type = SYS_L3_INTFTYPE_VLAN;
	            l3intf.id = vid;
	            if (IS_L3_INTF_IPV4ENBL(&l3intf))
	            {
					cJSON_AddBoolToObject(vlan, CGI_VLAN_INTF_STAT, TRUE);
	            }
	            else 
	            {
					cJSON_AddBoolToObject(vlan, CGI_VLAN_INTF_STAT, FALSE);
	            }
            }
        }
    }

    if (hasSelVlan)
        cJSON_AddItemToObject(data, CGI_VLAN_SELECT_VLANS, selVlans);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_conf_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vlans = NULL;
    cJSON *vlan = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    char *value = NULL;
    sys_vid_t vid;
    sys_vid_t selvid;
    sys_vid_t defvid;
    sys_vid_t pvid;
    sys_vlanmask_t adminVM;
    sys_logic_port_t lp;
    sys_vlan_portMode_t portMode;
    sys_namestr_t name;
    sys_vlan_adminEntry_t entry;

    SYS_MEM_CLEAR(adminVM);

    SYS_ERR_CHK(sal_vlan_defaultVid_get(&defvid));

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        selvid = SYS_STR2UINT(value);
    else
        selvid = defvid;

	CGI_GET_JSON_CREATE(root, data);

	vlans = cJSON_CreateArray();
	ports = cJSON_CreateArray();

    cJSON_AddNumberToObject(data, CGI_VLAN, selvid);
    cJSON_AddItemToObject(data, CGI_VLANS, vlans);
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    sal_vlan_adminEntryExist_get(&adminVM);
    sal_vlan_adminEntry_get(selvid, &entry);

    FOR_EACH_VID_IN_VLANMASK(vid, adminVM)
    {
        SYS_MEM_CLEAR(name);
        sal_vlan_name_get(vid, &name);

        vlan = cJSON_CreateObject();
        cJSON_AddItemToArray(vlans, vlan);
        cJSON_AddNumberToObject(vlan, CGI_VALUE, vid);
        cJSON_AddStringToObject(vlan, CGI_NAME, name.str);
    }

    FOR_EACH_LOGIC_PORT(lp)
    {
        sal_vlan_portAdminMode_get(lp, &portMode);
        sal_vlan_portAdminPvid_get(lp, &pvid);

        port = cJSON_CreateObject();
        cJSON_AddItemToArray(ports, port);
        cJSON_AddNumberToObject(port, CGI_VLAN_MODE, portMode);

        if (!IS_LOGIC_PORTMASK_PORTSET(entry.lpmMember, lp))
            cJSON_AddNumberToObject(port, CGI_VLAN_MEMBERSHIP, SYS_VLAN_EXCLUDED);
        else
        {
            if (IS_LOGIC_PORTMASK_PORTSET(entry.lpmUntag, lp))
                cJSON_AddNumberToObject(port, CGI_VLAN_MEMBERSHIP, SYS_VLAN_UNTAGGED);
            else
                cJSON_AddNumberToObject(port, CGI_VLAN_MEMBERSHIP, SYS_VLAN_TAGGED);
        }

        if (IS_LOGIC_PORTMASK_PORTSET(entry.lpmForbidden, lp))
            cJSON_AddBoolToObject(port, CGI_VLAN_FORBIDDEN, TRUE);
        else
            cJSON_AddBoolToObject(port, CGI_VLAN_FORBIDDEN, FALSE);
        cJSON_AddBoolToObject(port, CGI_VLAN_PVID, (selvid == pvid));
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_port_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    char tmp[SYS_BUF16_LEN];
#ifdef CONFIG_SYS_VLAN_QINQ
    uint16 tpid;
    sys_enable_t uplink;
#endif
    sys_vid_t pvid;
    sys_enable_t ingressFilter;
    sys_logic_port_t lp;
    sys_vlan_portMode_t mode;
    sys_vlan_frameType_t type;

    SYS_MEM_CLEAR(tmp);

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        sal_vlan_portOperPvid_get(lp, &pvid);
        sal_vlan_portAdminMode_get(lp, &mode);
        sal_vlan_portAdminAcceptFrameType_get(lp, &type);
        sal_vlan_portAdminIgrFilterEnable_get(lp, &ingressFilter);

        port = cJSON_CreateObject();
        cJSON_AddItemToArray(ports, port);

        cJSON_AddNumberToObject(port, CGI_VLAN_MODE, mode);
        cJSON_AddNumberToObject(port, CGI_VLAN_PVID, pvid);
        cJSON_AddNumberToObject(port, CGI_VLAN_ACCFRAMETYPE, type);
        cJSON_AddBoolToObject(port, CGI_VLAN_INGRESS_FILTER, ingressFilter);

#ifdef CONFIG_SYS_VLAN_QINQ
        sal_vlan_portAdminRoleUplinkEnable_get(lp, &uplink);
        sal_vlan_portDot1qTunnelTpid_get(lp, &tpid);

        osal_sprintf(tmp, "0x%04x", tpid);
        cJSON_AddBoolToObject(port, CGI_VLAN_UPLINK, uplink);
        cJSON_AddStringToObject(port, CGI_VLAN_TPID, tmp);
#endif
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_portEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
#ifdef CONFIG_SYS_VLAN_QINQ
    cJSON *tpids = NULL;
    cJSON *tpid = NULL;
#endif
    char *value = NULL;
    char tmp[SYS_BUF16_LEN];
#ifdef CONFIG_SYS_VLAN_QINQ
    uint16 portTpid;
    uint32 i = 0;
    sys_enable_t uplink;
#endif
    sys_vid_t pvid;
    sys_enable_t ingressFilter;
    sys_logic_port_t lp;
    sys_vlan_portMode_t mode;
    sys_vlan_frameType_t type;

    SYS_MEM_CLEAR(tmp);

    CGI_GET_JSON_CREATE(root, data);

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);
    else
        goto cgi_end;

    sal_vlan_portOperPvid_get(lp, &pvid);
    sal_vlan_portAdminMode_get(lp, &mode);
    sal_vlan_portAdminAcceptFrameType_get(lp, &type);
    sal_vlan_portAdminIgrFilterEnable_get(lp, &ingressFilter);

    cJSON_AddNumberToObject(data, CGI_VLAN_MODE, mode);
    cJSON_AddNumberToObject(data, CGI_VLAN_PVID, pvid);
    cJSON_AddNumberToObject(data, CGI_VLAN_ACCFRAMETYPE, type);
    cJSON_AddBoolToObject(data, CGI_VLAN_INGRESS_FILTER, ingressFilter);

#ifdef CONFIG_SYS_VLAN_QINQ
    sal_vlan_portAdminRoleUplinkEnable_get(lp, &uplink);
    sal_vlan_portDot1qTunnelTpid_get(lp, &portTpid);

    osal_sprintf(tmp, "0x%04x", portTpid);
    cJSON_AddBoolToObject(data, CGI_VLAN_UPLINK, uplink);
    cJSON_AddStringToObject(data, CGI_VLAN_TPID, tmp);

    tpids = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_VLAN_TPIDS, tpids);

    for (i = 0; i < 4 ; i++)
    {
        sal_vlan_dot1qTunnelTpid_get(i, &portTpid);

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "0x%04x", portTpid);
        tpid = cJSON_CreateObject();
        cJSON_AddItemToArray(tpids, tpid);
        cJSON_AddStringToObject(tpid, CGI_NAME, tmp);
        cJSON_AddStringToObject(tpid, CGI_VALUE, tmp);
    }
#endif

cgi_end:
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#define MAX_VLAN_STR_LEN 256
static sys_vlanmask_t vmMember;
static sys_vlan_adminEntry_t adminVlan[SYS_VLAN_NUM_MAX];
static sys_vlan_operEntry_t operVlan[SYS_VLAN_NUM_MAX];
static uint8 firstGet = TRUE;
#ifdef CONFIG_SYS_SOFTWARE_GUEST_VLAN
static sys_vid_t guest_vid;
#endif

static char _vlanMemberStrGet(sys_logic_port_t port, char *strVlanMember, uint8 isAdmin, uint8 isFullStr)
{
    char buf[8];
    uint32 lenBuf = 0;
    uint32 lenStr = 0;
#ifdef CONFIG_SYS_SOFTWARE_GUEST_VLAN
    uint32 inGuestVlan;
#endif
    sys_vid_t vid;
    sys_vid_t pvid;
    sys_vlan_membership_t membership;

    if (firstGet)
    {
        SYS_MEM_CLEAR(vmMember);
        sal_vlan_operEntryExist_get(&vmMember);

        FOR_EACH_VID_IN_VLANMASK(vid, vmMember)
        {
            sal_vlan_operEntry_get(vid, &operVlan[vid]);

            if (sal_vlan_adminEntryExist_ret(vid))
                sal_vlan_adminEntry_get(vid, &adminVlan[vid]);
        }

#ifdef CONFIG_SYS_SOFTWARE_GUEST_VLAN
        sal_vlan_guestVid_get(&guest_vid);
#endif

        firstGet = FALSE;
    }

    sal_vlan_portAdminPvid_get(port, &pvid);

    SYS_BITMAP_SET_BIT((vmMember).bits, pvid);

    FOR_EACH_SYSTEM_VID_IN_VLANMASK(vid, vmMember)
    {
        membership = SYS_VLAN_EXCLUDED;

        if (isAdmin)
        {
            if (IS_LOGIC_PORTMASK_PORTSET(adminVlan[vid].lpmMember, port))
            {
                
                if (IS_LOGIC_PORTMASK_PORTSET(adminVlan[vid].lpmUntag, port))
                    membership = SYS_VLAN_UNTAGGED;
                else
                    membership = SYS_VLAN_TAGGED;
            }
            else
            {
                
                if (IS_LOGIC_PORTMASK_PORTSET(adminVlan[vid].lpmForbidden, port))
                    membership = SYS_VLAN_FORBIDDEN;
                else if (vid != pvid)
                    continue;
            }
        }
        else
        {
            if (IS_LOGIC_PORTMASK_PORTSET(operVlan[vid].lpmMember, port))
            {
                
                if (IS_LOGIC_PORTMASK_PORTSET(operVlan[vid].lpmUntag, port))
                    membership = SYS_VLAN_UNTAGGED;
                else
                    membership = SYS_VLAN_TAGGED;
            }
            else
            {
                
                if (IS_LOGIC_PORTMASK_PORTSET(operVlan[vid].lpmForbidden, port))
                    membership = SYS_VLAN_FORBIDDEN;
                else if (vid != pvid)
                    continue;
            }
        }

#ifdef CONFIG_SYS_SOFTWARE_GUEST_VLAN
        sal_dot1x_guestVlanPortState_get(port, &inGuestVlan);

        if (vid == guest_vid && inGuestVlan)
        {
            lenBuf = sprintf(buf, "%u%s%s, ", vid,       \
                             (SYS_VLAN_TAGGED == membership)? "T" :     \
                             (SYS_VLAN_UNTAGGED == membership)? "G" :   \
                             (SYS_VLAN_FORBIDDEN == membership)? "F" : "", \
                             (pvid == vid)? "P" : "");
        }
        else
#endif
        {
            lenBuf = sprintf(buf, "%u%s%s, ", vid,       \
                             (SYS_VLAN_TAGGED == membership)? "T" :     \
                             (SYS_VLAN_UNTAGGED == membership)? "U" :   \
                             (SYS_VLAN_FORBIDDEN == membership)? "F" : "", \
                             (pvid == vid)? "P" : "");
        }

        if (isFullStr && (lenStr + lenBuf + 3) < MAX_VLAN_STR_LEN)
        {
            lenBuf = sprintf(strVlanMember + lenStr, "%s", buf);
            lenStr += lenBuf;
        }
        else
        {
            
            lenStr += sprintf(strVlanMember + lenStr, "...");
            break;
        }
    }

    if (',' == strVlanMember[strlen(strVlanMember)-2])
        strVlanMember[strlen(strVlanMember)-2] = '\0';

    if (pvid == DFLT_VLAN_RESERVED_VLAN)
        SYS_BITMAP_CLEAR_BIT((vmMember).bits, (DFLT_VLAN_RESERVED_VLAN));

    return SYS_ERR_OK;
}

static int32 cgi_vlan_membership_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    char adminVlanStr[MAX_VLAN_STR_LEN];
    char operVlanStr[MAX_VLAN_STR_LEN];
    sys_vlan_portMode_t mode = 0;
    sys_logic_port_t lp;

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        sal_vlan_portAdminMode_get(lp, &mode);

        port = cJSON_CreateObject();

        cJSON_AddItemToArray(ports, port);
        cJSON_AddNumberToObject(port, CGI_VLAN_MODE, mode);

        SYS_MEM_CLEAR(adminVlanStr);
        SYS_MEM_CLEAR(operVlanStr);

        _vlanMemberStrGet(lp, adminVlanStr, TRUE, TRUE);
        _vlanMemberStrGet(lp, operVlanStr, FALSE, TRUE);

        cJSON_AddStringToObject(port, CGI_VLAN_ADMIN_VLANS, adminVlanStr);
        cJSON_AddStringToObject(port, CGI_VLAN_OPER_VLANS, operVlanStr);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_membershipEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vlans = NULL;
    cJSON *selVlans = NULL;
    char *value = NULL;
    char buf[SYS_BUF8_LEN];
    int8 hasVlan = FALSE;
    int8 hasSelVlan = FALSE;
    sys_vid_t vid;
    sys_vid_t pvid;
    sys_vlanmask_t vmMember;
    sys_logic_port_t lp;
    sys_vlan_membership_t membership = SYS_VLAN_EXCLUDED;
    sys_vlan_adminEntry_t adminEntry;
    sys_vlan_portMode_t mode = 0;

    if (CGI_IS_PARAM_EXIST(CGI_PORT, value))
        lp = SYS_STR2UINT(value);
    else
        goto cgi_end;

    CGI_GET_JSON_CREATE(root, data);

    sal_vlan_portAdminMode_get(lp, &mode);
    cJSON_AddNumberToObject(data, CGI_VLAN_MODE, mode);

    vlans = cJSON_CreateArray();
    selVlans = cJSON_CreateArray();

    SYS_MEM_CLEAR(vmMember);
    sal_vlan_adminEntryExist_get(&vmMember);
    sal_vlan_portAdminPvid_get(lp, &pvid);

    SYS_BITMAP_SET_BIT((vmMember).bits, pvid);

    FOR_EACH_SYSTEM_VID_IN_VLANMASK(vid, vmMember)
    {
        membership = SYS_VLAN_EXCLUDED;
        sal_vlan_adminEntry_get(vid, &adminEntry);

        if (vid == DFLT_VLAN_RESERVED_VLAN)
        {
            SYS_MEM_CLEAR(buf);
            sprintf(buf, "4095P");
        }
        else
        {
            if (IS_LOGIC_PORTMASK_PORTSET(adminEntry.lpmMember, lp))
            {
                
                if (IS_LOGIC_PORTMASK_PORTSET(adminEntry.lpmUntag, lp))
                    membership = SYS_VLAN_UNTAGGED;
                else
                    membership = SYS_VLAN_TAGGED;
            }
            else
            {
                
                if (IS_LOGIC_PORTMASK_PORTSET(adminEntry.lpmForbidden, lp))
                    membership = SYS_VLAN_FORBIDDEN;
                else
                {
                    if (pvid != vid)
                    {
                        cJSON_AddItemToArray(vlans, cJSON_CreateNumber(vid));
                        hasVlan = TRUE;
                        continue;
                    }
                }
            }
        }

        SYS_MEM_CLEAR(buf);
        sprintf(buf, "%u%s%s", vid,       \
                         (SYS_VLAN_TAGGED == membership)? "T" :     \
                         (SYS_VLAN_UNTAGGED == membership)? "U" :   \
                         (SYS_VLAN_FORBIDDEN == membership)? "F" : "", \
                         (pvid == vid)? "P" : "");

        cJSON_AddItemToArray(selVlans, cJSON_CreateString(buf));
        hasSelVlan = TRUE;
    }

    if (hasVlan)
        cJSON_AddItemToObject(data, CGI_VLANS, vlans);

    if (hasSelVlan)
        cJSON_AddItemToObject(data, CGI_VLAN_SELECT_VLANS, selVlans);

cgi_end:
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
static int32 cgi_vlan_mgmt_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
	cJSON *vlans = NULL;
    cJSON *vlan = NULL;

    char tmp[SYS_BUF64_LEN];
    sys_vid_t vid;
    sys_vlanmask_t vm;
    sys_vlan_adminEntry_t entry;

    sal_vlan_mgmtVid_get(&vid);

	CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_VLAN_MGMT_VLAN, vid);

	vlans = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_VLAN_ADMIN_VLANS, vlans);

    SYS_MEM_CLEAR(vm);
    sal_vlan_adminEntryExist_get(&vm);

    FOR_EACH_VID_IN_VLANMASK(vid, vm)
    {
        vlan = cJSON_CreateObject();

        SYS_MEM_CLEAR(entry);
        SYS_MEM_CLEAR(tmp);

        sal_vlan_adminEntry_get(vid, &entry);
        osal_sprintf(tmp, "%u - %s", vid, entry.name.str);
        cJSON_AddStringToObject(vlan, CGI_NAME, tmp);
    	cJSON_AddNumberToObject(vlan, CGI_VALUE, vid);
	    cJSON_AddItemToArray(vlans, vlan);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

