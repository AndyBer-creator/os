
#include <libcfg/cfg.h>

int32 cfg_vlan_list_init(sys_list_key_t key)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_LIST_KEY_INVALID(key), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    pVlan->vlanlist.key = key;
    LIST_INIT(&pVlan->vlanlist);

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_list_destroy(void)
{
    cfg_vlan_t *pVlan = NULL;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    LIST_DESTROY(&pVlan->vlanlist);
    SYS_MEM_CLEAR(pVlan->cfgEntry);

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_list_add(sys_vid_t vid, sys_namestr_t *pName)
{
    cfg_vlan_t *pVlan = NULL;
    sys_list_node_t node;
    int32 idx;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    LIST_NODE_INIT(&node);
    node.id = vid;
    node.name = *pName;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    LIST_ADD_NODE(&pVlan->vlanlist, &node);

    if (SYS_LIST_NODE_FOUND == LIST_SEARCH_NODE(&pVlan->vlanlist, &node, &idx))
    {
        idx = pVlan->vlanlist.sortId[idx].ptr;
        
        SYS_MEM_CLEAR(pVlan->cfgEntry[idx]);
        pVlan->cfgEntry[idx].vid = node.id;
        pVlan->cfgEntry[idx].name = node.name;
    }

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_list_update(sys_vid_t vid, sys_vlan_cfgEntry_t *pEntry)
{
    cfg_vlan_t *pVlan = NULL;
    sys_list_node_t node;
    int32 idx;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    LIST_NODE_INIT(&node);
    node.id = vid;
    node.name = pEntry->name;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    LIST_UPDATE_NODE(&pVlan->vlanlist, &node);

    if (SYS_LIST_NODE_FOUND == LIST_SEARCH_NODE(&pVlan->vlanlist, &node, &idx))
    {
        idx = pVlan->vlanlist.sortId[idx].ptr;
        pVlan->cfgEntry[idx] = *pEntry;

        CFG_CLOSE(CFG_ID_VLAN);
    }
    else
        CFG_ERR_CLOSE(CFG_ID_VLAN, SYS_ERR_FAILED);

}

int32 cfg_vlan_list_del(sys_vid_t vid)
{
    cfg_vlan_t *pVlan = NULL;
    sys_list_node_t node;
    int32 idx;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    LIST_NODE_INIT(&node);
    node.id = vid;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    if (SYS_LIST_NODE_FOUND == LIST_SEARCH_NODE(&pVlan->vlanlist, &node, &idx))
    {
        idx = pVlan->vlanlist.sortId[idx].ptr;
        SYS_MEM_CLEAR(pVlan->cfgEntry[idx]);
    }

    if (SYS_LIST_NODE_EMPTY == LIST_DEL_NODE(&pVlan->vlanlist, &node))
        SYS_MEM_CLEAR(pVlan->cfgEntry);

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_list_vidExist_ret(sys_vid_t vid)
{
    int32 idx;
    uint32 exist;
    cfg_vlan_t *pVlan = NULL;
    sys_list_node_t node;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    LIST_NODE_INIT(&node);
    node.id = vid;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    if (SYS_LIST_NODE_FOUND == LIST_SEARCH_NODE(&pVlan->vlanlist, &node, &idx))
        exist = TRUE;
    else
        exist = FALSE;

    CFG_NORET_CLOSE(CFG_ID_VLAN);

    return exist;
}

int32 cfg_vlan_list_get(sys_vid_t vid, sys_vlan_cfgEntry_t *pEntry)
{
    cfg_vlan_t *pVlan = NULL;
    sys_list_node_t node;
    int32 idx;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    LIST_NODE_INIT(&node);

    node.id = vid;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    if (SYS_LIST_NODE_FOUND == LIST_SEARCH_NODE(&pVlan->vlanlist, &node, &idx))
    {
        idx = pVlan->vlanlist.sortId[idx].ptr;
        *pEntry = pVlan->cfgEntry[idx];

        CFG_CLOSE(CFG_ID_VLAN);
    }
    else
        CFG_ERR_CLOSE(CFG_ID_VLAN, SYS_ERR_ENTRY_NOT_FOUND);

}

int32 cfg_vlan_list_sortId_get(sys_nodeid_t idx, sys_vlan_cfgEntry_t *pEntry)
{
    cfg_vlan_t *pVlan = NULL;
    uint32 idx2;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((idx >= CAPA_VLAN_NUM), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    pEntry->vid = pVlan->vlanlist.sortId[idx].id;

    idx2 = pVlan->vlanlist.sortId[idx].ptr;
    *pEntry = pVlan->cfgEntry[idx2];

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_listPtr_ret(void)
{
    cfg_vlan_t *pVlan = NULL;
    sys_nodeid_t ptr;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    ptr = pVlan->vlanlist.ptr;

    CFG_NORET_CLOSE(CFG_ID_VLAN);

    return ptr;
}

int32 cfg_vlan_listNameExist_ret(sys_namestr_t *pName)
{
    int32 idx;
    uint32 exist;
    cfg_vlan_t *pVlan = NULL;
    sys_list_node_t node;
    sys_list_key_t key;

    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    LIST_NODE_INIT(&node);
    node.name = *pName;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    key = pVlan->vlanlist.key;
    pVlan->vlanlist.key = SYS_LIST_KEY_NAME;

    if (SYS_LIST_NODE_FOUND == LIST_SEARCH_NODE(&pVlan->vlanlist, &node, &idx))
        exist = TRUE;
    else
        exist = FALSE;

    pVlan->vlanlist.key = key;

    CFG_NORET_CLOSE(CFG_ID_VLAN);

    return exist;
}

int32 cfg_vlan_vidByName_get(sys_namestr_t *pName, sys_vid_t *pVid)
{
    int32 idx;
    int32 ret = SYS_ERR_OK;
    cfg_vlan_t *pVlan = NULL;
    sys_list_node_t node;
    sys_list_key_t key;

    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    LIST_NODE_INIT(&node);
    node.name = *pName;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    key = pVlan->vlanlist.key;
    pVlan->vlanlist.key = SYS_LIST_KEY_NAME;

    if (SYS_LIST_NODE_FOUND == LIST_SEARCH_NODE(&pVlan->vlanlist, &node, &idx))
    {
        idx = pVlan->vlanlist.sortName[idx].ptr;
        *pVid = pVlan->cfgEntry[idx].vid;
    }
    else
    {
        ret = SYS_ERR_FAILED;
    }

    pVlan->vlanlist.key = key;

    CFG_NORET_CLOSE(CFG_ID_VLAN);

    return ret;
}

int32 cfg_vlan_init(void)
{
    cfg_vlan_t *pVlan = NULL;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    pVlan->defaultVlan.vid = DFLT_VLAN_DFLT_VLAN;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_defaultVid_set(sys_vid_t vid)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    pVlan->defaultVlan.vid = vid;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_defaultVid_get(sys_vid_t *pVid)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    *pVid = pVlan->defaultVlan.vid;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_portAdminMode_set(sys_logic_port_t port, sys_vlan_portMode_t mode)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_VLAN_PORT_MODE_INVALID(mode), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    pVlan->adminVlan.port[port].mode = mode;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_portAdminMode_get(sys_logic_port_t port, sys_vlan_portMode_t *pMode)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    *pMode = pVlan->adminVlan.port[port].mode;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_portAdminRoleUplinkEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_vlan_t *cfgVlan = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&cfgVlan);

    cfgVlan->adminVlan.port[port].uplink = enable;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_portAdminRoleUplinkEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_vlan_t *cfgVlan = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&cfgVlan);

    *pEnable = cfgVlan->adminVlan.port[port].uplink;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_portTrunkAllowedVlan_set(sys_logic_port_t port, sys_vlanmask_t *pVlanmask)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    osal_memcpy(&pVlan->adminVlan.port[port].vmTrunkAllowed, pVlanmask, sizeof(sys_vlanmask_t));

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_portTrunkAllowedVlan_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    osal_memcpy(pVlanmask, &pVlan->adminVlan.port[port].vmTrunkAllowed, sizeof(sys_vlanmask_t));

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_adminVlanMaxEntryNum_get(uint32 *pMaxEntryNum)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK((NULL == pMaxEntryNum), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    *pMaxEntryNum = pVlan->maxAdminVlanNum;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_adminVlanMaxEntryNum_set(uint32 maxEntryNum)
{
    cfg_vlan_t *pVlan = NULL;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    pVlan->maxAdminVlanNum = maxEntryNum ;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_isChkVlanPortMode_set(sys_enable_t enable)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    pVlan->isChkVlanPortMode = enable;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_isChkVlanPortMode_get(sys_enable_t *pEnable)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    *pEnable = pVlan->isChkVlanPortMode;

    CFG_CLOSE(CFG_ID_VLAN);
}

#ifdef CONFIG_SYS_MANAGEMENT_VLAN

int32 cfg_vlan_mgmtVid_set(sys_vid_t vid)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    pVlan->mgmtVlan.vid = vid;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_mgmtVid_get(sys_vid_t *pVid)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    *pVid = pVlan->mgmtVlan.vid;

    CFG_CLOSE(CFG_ID_VLAN);
}
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

int32 cfg_vlan_guestVid_set(sys_vid_t vid)
{
    cfg_vlan_t *pVlan = NULL;

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    pVlan->guestVlan.vid = vid;

    CFG_CLOSE(CFG_ID_VLAN);
}

int32 cfg_vlan_guestVid_get(sys_vid_t *pVid)
{
    cfg_vlan_t *pVlan = NULL;

    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN, (cfg_addr)&pVlan);

    *pVid = pVlan->guestVlan.vid;

    CFG_CLOSE(CFG_ID_VLAN);
}
#endif

