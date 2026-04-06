
#include <common/sys_list.h>
#include <common/sys_vlan.h>
#include <libcfg/cfg.h>
#include <libsal/sal_util.h>
#include <libsal/sal_vlan.h>
#ifdef CONFIG_SYS_PROTO_GVRP
#include <libsal/sal_gvrp.h>
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
#include <libsal/sal_igmp.h>
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
#include <libsal/sal_authmgr.h>
#endif
#ifdef CONFIG_SYS_VOICEVLAN
#include <libsal/sal_vlan_voice.h>
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
#include <libsal/sal_vlan_proto.h>
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
#include <libsal/sal_vlan_surveillance.h>
#endif
#ifdef CONFIG_SYS_MACVLAN
#include <libsal/sal_vlan_mac.h>
#endif

const sys_text_t text_vlan_portMode[] =
{
    {       "hybrid" },
    {        "access" },
    {         "trunk" },
    {   "dot1q-tunnel" },
};

const sys_text_t text_vlan_membership[] =
{
    {                "excluded" },
    {               "forbidden" },
    {                  "tagged" },
    {                "untagged" },
};

const sys_text_t text_vlan_tagMode[] =
{
    {           "original" },
    {        "keep-format" },
    {                "priority-tag" },
};

const sys_text_t text_vlan_acceptFrameType[] =
{
    {            "all" },
    {       "tag-only" },
    {     "untag-only" },
};

int32 sal_vlan_exclusive_chk(sys_vlan_category_t subject, sys_vid_t vid)
{
#define _CHK(_api, _err)         \
do {                             \
    SYS_ERR_CHK(_api(&tmpVid));  \
    if (vid == tmpVid)           \
        return _err;             \
} while(0)

    sys_vid_t tmpVid;
    tmpVid = 0;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    if (0) {}
#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    else if (SYS_VLAN_CATE_MGMT == subject)    
    {
  #ifdef CONFIG_SYS_PROTO_GVRP
        if (IS_DYNAMIC_VLAN_EXIST(vid))
            return SYS_ERR_VLAN_NOT_EXIST;
  #endif
  #ifdef CONFIG_SYS_PROTO_AUTHMGR
        _CHK(sal_vlan_guestVid_get, SYS_ERR_DOT1X_MGMT_VLAN_GUEST_VLAN);
  #endif
    }
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    else if (SYS_VLAN_CATE_GUEST == subject)    
    {
        if (!IS_STATIC_VLAN_EXIST(vid))
            return SYS_ERR_VLAN_NOT_EXIST;
        _CHK(sal_vlan_defaultVid_get, SYS_ERR_VLAN_VID_GUESTVLAN);
  #ifdef CONFIG_SYS_MANAGEMENT_VLAN
        _CHK(sal_vlan_mgmtVid_get, SYS_ERR_DOT1X_GUEST_VLAN_MGMT_VLAN);
  #endif
  #ifdef CONFIG_SYS_VOICEVLAN
        _CHK(sal_vlan_voiceVid_get, SYS_ERR_DOT1X_GUEST_VLAN_VOICE_VLAN);
  #endif
  #ifdef CONFIG_SYS_SURVEILLANCEVLAN
        _CHK(sal_surveillance_vlanVid_get, SYS_ERR_DOT1X_GUEST_VLAN_SURVEILLANCE_VLAN);
  #endif
    }
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    else if (SYS_VLAN_CATE_VOICE_MANUAL == subject)   
    {
        _CHK(sal_vlan_defaultVid_get, SYS_ERR_VLAN_VOICEVLAN_OUI_DFLT_VID);

  #ifdef CONFIG_SYS_PROTO_GVRP
        if (IS_DYNAMIC_VLAN_EXIST(vid))
            return SYS_ERR_VLAN_NOT_EXIST;
  #endif
  #ifdef CONFIG_SYS_PROTO_AUTHMGR
        _CHK(sal_vlan_guestVid_get, SYS_ERR_VLAN_VOICEVLAN_GUEST_VLAN);
  #endif
  #ifdef CONFIG_SYS_SURVEILLANCEVLAN
        _CHK(sal_surveillance_vlanVid_get, SYS_ERR_VLAN_VOICEVLAN_SURVEILLANCE_VLAN);
  #endif
    }
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    else if (SYS_VLAN_CATE_SURVEILLANCE == subject)   
    {
        _CHK(sal_vlan_defaultVid_get, SYS_ERR_VLAN_SV_EQ_DEFAULT);

  #ifdef CONFIG_SYS_PROTO_GVRP
        if (IS_DYNAMIC_VLAN_EXIST(vid))
            return SYS_ERR_VLAN_NOT_EXIST;
  #endif
  #ifdef CONFIG_SYS_PROTO_AUTHMGR
        _CHK(sal_vlan_guestVid_get, SYS_ERR_VLAN_SV_EQ_GUEST);
  #endif
  #ifdef CONFIG_SYS_VOICEVLAN
        _CHK(sal_vlan_voiceVid_get, SYS_ERR_VLAN_SV_EQ_VOICE);
  #endif
    }
#endif

    return SYS_ERR_OK;
}

int32 sal_vlan_nameStr_get(sys_vid_t vid, char *pPrefix, sys_namestr_t *pName)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pPrefix), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    osal_sprintf(pName->str, "%s%04d", pPrefix, vid);

    return SYS_ERR_OK;
}

int32 sal_vlan_name_set(sys_vid_t vid, sys_namestr_t *pName)
{
    sys_vid_t vidCheck;
    sys_vlan_cfgEntry_t cfgEntry;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    if (0 == osal_strcmp(pName->str, ""))
    {
        return SYS_ERR_VLAN_NAME_NULL;
    }

    if (SYS_ERR_OK == cfg_vlan_vidByName_get(pName, &vidCheck))
    {
        return SYS_ERR_VLAN_NAME_EXIST;
    }

    SYS_ERR_CHK(rsd_vlan_name_set(vid, pName));

    SYS_MEM_CLEAR(cfgEntry);
    SYS_ERR_CHK(cfg_vlan_list_get(vid, &cfgEntry));
    cfgEntry.name = *pName;
    SYS_ERR_CHK(cfg_vlan_list_update(vid, &cfgEntry));

    return SYS_ERR_OK;
}

int32 sal_vlan_vidByName_get(sys_namestr_t *pName, sys_vid_t *pVid)
{
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    return cfg_vlan_vidByName_get(pName, pVid);
}

int32 sal_vlan_adminEntryNum_ret(void)
{
    return cfg_vlan_listPtr_ret();
}

int32 sal_vlan_adminEntryExist_get(sys_vlanmask_t *pVlanmask)
{
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_adminEntryExist_get(pVlanmask));

    return SYS_ERR_OK;
}

int32 sal_vlan_adminEntryExist_ret(sys_vid_t vid)
{
    return cfg_vlan_list_vidExist_ret(vid);
}

int32 sal_vlan_adminEntry_create(sys_vid_t vid, sys_namestr_t *pName)
{
    sys_err_code_t ret = SYS_ERR_OK;
    sys_vid_t defaultVid;
    sys_vid_t vidCheck;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    if (CAPA_VLAN_ADMIN_NUM < CAPA_VLAN_NUM)
    {
        if (CAPA_VLAN_ADMIN_NUM == sal_vlan_operEntryNum_ret())
            return SYS_ERR_VLAN_NUM_NOT_SUPPORT;
    }

    if (SYS_ERR_OK == cfg_vlan_vidByName_get(pName, &vidCheck))
    {
        return SYS_ERR_VLAN_NAME_EXIST;
    }

    SYS_ERR_HDL(sal_vlan_defaultVid_get(&defaultVid), ERR, ret);

    if (vid == defaultVid)
    {
        
        SYS_ERR_HDL(rsd_vlan_name_set(vid, pName), ERR, ret);
    }
    else
    {
        
        SYS_ERR_HDL(rsd_vlan_adminEntry_create(vid, pName), ERR, ret);
    }

    SYS_ERR_HDL(cfg_vlan_list_add(vid, pName), ERR, ret);
#ifdef CONFIG_SYS_PROTO_GARP
    gmrp_vlan_set(vid, ENABLED);
#endif

    SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_CREATE, vid, pName);

ERR:
    if (SYS_ERR_FAILED == ret)
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_CREATE_FAIL, vid);

    }

    return ret;
}

int32 sal_vlan_adminEntry_destroy(sys_vid_t vid)
{
    sys_err_code_t ret = SYS_ERR_OK;
    sys_vid_t defaultVid;

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    sys_vid_t mgmtVid;
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    sys_vid_t guestVlan;
#endif

#ifdef CONFIG_SYS_VOICEVLAN
    sys_vid_t voiceVid;
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    sys_vid_t surveillanceVid;
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    sys_vid_t mvrVlan;
    sys_enable_t mvrEnable;
#endif
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_ERR_HDL(sal_vlan_defaultVid_get(&defaultVid), ERR, ret);

    if (defaultVid == vid)
        return SYS_ERR_VLAN_DFLTVLAN_DEL;

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    SYS_ERR_HDL(sal_vlan_mgmtVid_get(&mgmtVid), ERR, ret);
    if (mgmtVid == vid)
        return SYS_ERR_VLAN_MGMTVLAN_DEL;
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_ERR_HDL(sal_authmgr_guestVlan_get(&guestVlan), ERR, ret);
    if (guestVlan == vid)
        return SYS_ERR_DOT1X_GUEST_VLAN_DEL;
#endif

#ifdef CONFIG_SYS_VOICEVLAN
    SYS_ERR_HDL(sal_vlan_voiceVid_get(&voiceVid), ERR, ret);
    if(voiceVid == vid)
        return SYS_ERR_VLAN_VOICEVLAN_VID_DEL;
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    SYS_ERR_HDL(sal_surveillance_vlanVid_get(&surveillanceVid), ERR, ret);
    if(surveillanceVid == vid)
        return SYS_ERR_VLAN_SV_VID_DEL;
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    SYS_ERR_HDL(sal_igmp_mvr_enable_get(&mvrEnable), ERR, ret);
    if (ENABLED == mvrEnable)
    {
        SYS_ERR_HDL(sal_igmp_mvr_vlanId_get(&mvrVlan), ERR, ret);
        if (mvrVlan == vid)
            return SYS_ERR_VLAN_MVR_VID_DEL;
    }
#endif

#ifdef CONFIG_SYS_L3_ROUTE
    {
        sys_l3_intfId_t l3intf;

        SYS_MEM_CLEAR(l3intf);
        l3intf.type = SYS_L3_INTFTYPE_VLAN;
        l3intf.id = vid;
        if (IS_L3_INTF_EXIST(&l3intf))
            return SYS_ERR_L3_INTF_EXIST;
    }
#endif

    SYS_ERR_HDL(rsd_vlan_adminEntry_destroy(vid), ERR, ret);
    SYS_ERR_HDL(cfg_vlan_list_del(vid), ERR, ret);

    SYS_ERR_HDL(sal_l2_dynamicMac_clearByVlan(vid), ERR, ret);
		
#ifdef CONFIG_SYS_PROTO_GARP
    gmrp_vlan_set(vid, DISABLED);
#endif

    SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_DESTROY, vid);
    sal_snmp_trap_static_vlan_destroy(getpid(), vid, SYS_ERR_OK);

ERR:
    if (SYS_ERR_FAILED == ret)
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_DESTROY_FAIL, vid);
        sal_snmp_trap_static_vlan_destroy(getpid(), vid, SYS_ERR_FAILED);
    }

    return ret;
}

int32 sal_vlan_adminEntry_set(sys_vid_t vid, sys_vlan_adminEntry_t *pEntry)
{
    char memberStrPortmask[CAPA_PORTMASK_STR_LEN];
    char untagStrPortmask[CAPA_PORTMASK_STR_LEN];
    sys_vlan_adminEntry_t adminEntry;
    sys_vlan_cfgEntry_t cfgEntry;

    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    lp = 0;
    SYS_MEM_CLEAR(lpm);

#ifdef CONFIG_SYS_VOICEVLAN
    sys_vid_t voiceVid;
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    sys_vid_t surveillanceVid;
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    sys_logic_port_t port;
    sys_enable_t mvrEnable;
    sys_vid_t mvrVlan;
    mvr_portType_t portType;
#endif

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    sys_vid_t mgmtVid;
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    sys_vid_t guestVlan;
    sys_logic_portmask_t guestVlanEnPm;
    sys_vlan_adminEntry_t oldEntry;
    sys_logic_portmask_t lpmModifyMember;
    sys_logic_portmask_t lpmModifyUntag;
#endif

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

#ifdef CONFIG_SYS_VOICEVLAN
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
    if (vid == voiceVid)
    {
        LOGIC_PORTMASK_CLEAR_ALL(lpm);
        osal_memset(&adminEntry, 0, sizeof(sys_vlan_adminEntry_t));
        SYS_ERR_CHK(sal_vlan_adminEntry_get(vid,&adminEntry));
        LOGIC_PORTMASK_XOR(lpm, adminEntry.lpmMember, pEntry->lpmMember);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            if(SYS_ERR_OK != sal_vlan_portVoiceStatus_chk(lp))
                return SYS_ERR_VLAN_VOICEVLAN_AUTO_PORT;
        }
    }
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&surveillanceVid));
    if (vid == surveillanceVid)
    {
        LOGIC_PORTMASK_CLEAR_ALL(lpm);
        osal_memset(&adminEntry, 0, sizeof(sys_vlan_adminEntry_t));
        SYS_ERR_CHK(sal_vlan_adminEntry_get(vid,&adminEntry));
        LOGIC_PORTMASK_XOR(lpm, adminEntry.lpmMember, pEntry->lpmMember);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            if(IS_PORT_USE_BY_SURVEILLANCE_VLAN(lp))
                return SYS_ERR_VLAN_SV_USED_PORT;
        }
    }
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&mvrEnable));
    if (ENABLED == mvrEnable)
    {
        SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlan));
        if (mvrVlan == vid)
        {
            FOR_EACH_LOGIC_PORT(port)
            {
                SYS_ERR_CHK(sal_igmp_mvr_portType_get(port, &portType));
                if (MVR_TYPE_SOURCE == portType)
                {
                    if (! IS_LOGIC_PORTMASK_PORTSET(pEntry->lpmMember, port))
                        return SYS_ERR_MVR_VLAN_PORT_SOURCE;
                }
            }
        }
    }
#endif

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    SYS_ERR_CHK(sal_vlan_mgmtVid_get(&mgmtVid));
    if (mgmtVid == vid)
    {
        if (IS_LOGIC_PORTMASK_CLEAR(pEntry->lpmMember))
            return SYS_ERR_VLAN_MGMTVLAN_EMPTY;
    }
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_ERR_CHK(sal_authmgr_guestVlan_get(&guestVlan));
    if (guestVlan == vid)
    {
        SYS_ERR_CHK(sal_authmgr_guestVlanPortmask_get(&guestVlanEnPm));
        SYS_ERR_CHK(sal_vlan_adminEntry_get(vid, &oldEntry));
        LOGIC_PORTMASK_XOR(lpmModifyMember, oldEntry.lpmMember, pEntry->lpmMember);
        LOGIC_PORTMASK_XOR(lpmModifyUntag, oldEntry.lpmUntag, pEntry->lpmUntag);
        LOGIC_PORTMASK_AND(lpmModifyMember, lpmModifyMember, guestVlanEnPm);
        LOGIC_PORTMASK_AND(lpmModifyUntag, lpmModifyUntag, guestVlanEnPm);
        if (!IS_LOGIC_PORTMASK_CLEAR(lpmModifyMember))
            return SYS_ERR_DOT1X_GUEST_VLAN_PORT_VLAN_CONFIG;
        if (!IS_LOGIC_PORTMASK_CLEAR(lpmModifyUntag))
            return SYS_ERR_DOT1X_GUEST_VLAN_PORT_VLAN_CONFIG;
    }
#endif

    SYS_ERR_CHK(rsd_vlan_adminEntry_get(vid, &adminEntry));
    adminEntry.name = pEntry->name;
    adminEntry.lpmForbidden = pEntry->lpmForbidden;
    adminEntry.lpmMember = pEntry->lpmMember;
    adminEntry.lpmUntag = pEntry->lpmUntag;
    SYS_ERR_CHK(rsd_vlan_adminEntry_set(vid, &adminEntry));

    cfgEntry.vid = vid;
    cfgEntry.name = pEntry->name;
    SYS_ERR_CHK(cfg_vlan_list_update(vid, &cfgEntry));

    osal_memset(memberStrPortmask, 0, sizeof(memberStrPortmask));
    LPM2STR(&pEntry->lpmMember, memberStrPortmask);
    osal_memset(untagStrPortmask, 0, sizeof(untagStrPortmask));
    LPM2STR(&pEntry->lpmUntag, untagStrPortmask);

    return SYS_ERR_OK;
}

int32 sal_vlan_adminEntry_get(sys_vid_t vid, sys_vlan_adminEntry_t *pEntry)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_adminEntry_get(vid, pEntry));

    return SYS_ERR_OK;
}

int32 sal_vlan_adminEntrySort_get(uint32 idx, sys_vid_t *pVid, sys_vlan_adminEntry_t *pEntry)
{
    sys_vlan_cfgEntry_t cfgEntry;

    SYS_ERR_CHK(cfg_vlan_list_sortId_get(idx, &cfgEntry));

    if (NULL != pVid)
    {
        *pVid = cfgEntry.vid;
    }

    if (NULL != pEntry)
    {
        SYS_ERR_CHK(rsd_vlan_adminEntry_get(cfgEntry.vid, pEntry));
    }

    return SYS_ERR_OK;
}

int32 sal_vlan_defaultVid_set(sys_vid_t vid)
{
    sys_vid_t origDefaultVid;
    sys_vlan_adminEntry_t adminEntry;
    sys_namestr_t origVlanName;
    sys_namestr_t vlanName;
    sys_logic_port_t logicPort;
    sys_vlan_membership_t membership;
    sys_vid_t portPvid;
    char *pVlanStrPrefix = DFLT_VLAN_PREFIX_NAME;
    sys_namestr_t name;

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    sys_vid_t mgmtVid;
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    sys_vid_t guestVid;
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    sys_vid_t voiceVid;
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    sys_vid_t surveillanceVid;
#endif

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    SYS_ERR_CHK(cfg_vlan_defaultVid_get(&origDefaultVid));
    if (origDefaultVid == vid)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_vlan_exclusive_chk(SYS_VLAN_CATE_DEFAULT, vid));

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    SYS_ERR_CHK(sal_vlan_mgmtVid_get(&mgmtVid));
    if (mgmtVid == vid)
        return SYS_ERR_VLAN_VID_MGMTVLAN;
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_ERR_CHK(sal_authmgr_guestVlan_get(&guestVid));
    if (guestVid == vid)
        return SYS_ERR_VLAN_VID_GUESTVLAN;
#endif

#ifdef CONFIG_SYS_VOICEVLAN
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
    if(voiceVid == vid)
        return SYS_ERR_VLAN_VID_VOICEVLAN;
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&surveillanceVid));
    if (vid == surveillanceVid)
        return SYS_ERR_VLAN_VID_SURVEILLANCEVLAN;
#endif

    if (IS_STATIC_VLAN_EXIST(vid))
        SYS_ERR_CHK(sal_vlan_adminEntry_destroy(vid));

    SYS_MEM_CLEAR(adminEntry);
    SYS_ERR_CHK(sal_vlan_adminEntry_get(origDefaultVid, &adminEntry));
    SYS_ERR_CHK(sal_vlan_name_get(origDefaultVid, &origVlanName));
    SYS_MEM_CLEAR(vlanName);
    vlanName.str[0] = '\n'; 
    SYS_ERR_CHK(sal_vlan_name_set(origDefaultVid, &vlanName));
    SYS_ERR_CHK(sal_vlan_adminEntry_create(vid, &origVlanName));

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_get(logicPort, origDefaultVid, &membership));
        SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_set(logicPort, vid, membership));
        SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_set(logicPort, origDefaultVid, SYS_VLAN_EXCLUDED));
        SYS_ERR_CHK(sal_vlan_portAdminPvid_get(logicPort, &portPvid));

        if (portPvid == origDefaultVid)
        {
            SYS_ERR_TRK_CHK(sal_vlan_portAdminPvid_set(logicPort, vid));
        }

        if (SYS_VLAN_FORBIDDEN == membership)
            SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_set(logicPort, vid, membership));
    }

    SYS_ERR_CHK(rsd_vlan_defaultVid_set(vid));
    SYS_ERR_CHK(cfg_vlan_defaultVid_set(vid));

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    
    if (mgmtVid == origDefaultVid)
        SYS_ERR_CHK(sal_vlan_mgmtVid_set(vid));
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    if (guestVid == origDefaultVid)
        SYS_ERR_CHK(sal_authmgr_guestVlan_set(vid));
#endif

#if CONFIG_SYS_VOICEVLAN
    if(voiceVid == origDefaultVid)
        SYS_ERR_CHK(sal_vlan_voiceVid_set(vid));
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    if (surveillanceVid == origDefaultVid )
        SYS_ERR_CHK(sal_surveillance_vlanVid_set(vid));
#endif

    SYS_ERR_CHK(rsd_vlan_adminEntry_destroy(origDefaultVid));
    SYS_ERR_CHK(cfg_vlan_list_del(origDefaultVid));

    SYS_ERR_CHK(sal_vlan_nameStr_get(origDefaultVid, pVlanStrPrefix, &name));
    SYS_ERR_CHK(sal_vlan_adminEntry_create(origDefaultVid, &name));

    SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_DEFAULT_VLAN, vid);

    return SYS_ERR_OK;
}

int32 sal_vlan_defaultVid_get(sys_vid_t *pVid)
{
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_defaultVid_get(pVid));

    return SYS_ERR_OK;
}

int32 sal_vlan_nameExist_ret(sys_namestr_t *pName)
{
    return cfg_vlan_listNameExist_ret(pName);
}

int32 sal_vlan_name_get(sys_vid_t vid, sys_namestr_t *pName)
{
    sys_vlan_cfgEntry_t cfgEntry;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(cfgEntry);

    if (SYS_ERR_OK == cfg_vlan_list_get(vid, &cfgEntry))
    {
        *pName = cfgEntry.name;
    }
    else
    {
        SYS_MEM_CLEAR(pName->str);  
    }

    return SYS_ERR_OK;
}

int32 sal_vlan_operEntryNum_get(uint32 *pNum)
{
    SYS_PARAM_CHK((NULL == pNum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_operEntryNum_get(pNum));

    return SYS_ERR_OK;
}

int32 sal_vlan_operEntryNum_ret(void)
{
    uint32 num;

    if (SYS_ERR_OK == rsd_vlan_operEntryNum_get(&num))
    {
        return num;
    }

    return SYS_ERR_FAILED;
}

int32 sal_vlan_operEntryExist_get(sys_vlanmask_t *pVlanmask)
{
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_operEntryExist_get(pVlanmask));

    return SYS_ERR_OK;
}

int32 sal_vlan_operEntryExist_ret(sys_vid_t vid)
{
    sys_vlanmask_t vmExist;

    if (SYS_ERR_OK == rsd_vlan_operEntryExist_get(&vmExist))
    {
        if (VLANMASK_IS_VIDSET(vmExist, vid))
            return TRUE;
        else
            return FALSE;
    }

    return FALSE;
}

int32 sal_vlan_operEntry_get(sys_vid_t vid, sys_vlan_operEntry_t *pEntry)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_operEntry_get(vid, pEntry));

    return SYS_ERR_OK;
}

int32 sal_vlan_operEntrySort_get(uint32 idx, sys_vid_t *pVid, sys_vlan_operEntry_t *pEntry)
{
    SYS_PARAM_CHK((idx >= SYS_VLAN_NUM_MAX), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_operEntrySort_get(idx, pVid, pEntry));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminMode_set(sys_logic_port_t port, sys_vlan_portMode_t mode)
{
    sys_enable_t portModeChk = DISABLED;

    sys_vlan_portMode_t portMode;
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    uint32 groupIdx = 0;
    sys_enable_t portEnable;;
    sys_vlan_protoVlanCfg_t vlan_cfg;
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    voice_vlan_state_t state;
    sys_enable_t active;
    sys_enable_t status;
    voice_vlan_port_mode_t pmode;
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    sys_enable_t svPortEnable;
    surveillance_vlan_port_mode_t svPortMode;
#endif

#ifdef CONFIG_SYS_MACVLAN
    uint32 i = 0;
    sys_mac_vlan_mapping_t mapping[CAPA_VLAN_MAC_GROUP_NUM_MAX];
#endif

#ifdef CONFIG_SYS_PROTO_GVRP
    sys_enable_t enable;
#endif
#ifdef CONFIG_SYS_PROTO_MVR
    mvr_portType_t type;
    sys_vid_t mvrVid;
    sys_vlan_operEntry_t vlanEntry;
#endif
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_VLAN_PORT_MODE_INVALID(mode), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_vlan_portAdminMode_get(port, &portMode));
    if (portMode == mode)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_vlan_isChkVlanPortMode_get(&portModeChk));

    if (ENABLED == portModeChk)
    {
#ifdef CONFIG_SYS_PROTOCOL_VLAN
        for (groupIdx = 0; groupIdx < CAPA_VLAN_PROTO_GROUP_NUM; groupIdx++)
        {
            SYS_MEM_CLEAR(vlan_cfg);
            SYS_ERR_CHK(sal_vlan_portProtoVlan_get(port, groupIdx, &vlan_cfg,&portEnable));

            if ((ENABLED == portEnable) && (SYS_VLAN_PORT_MODE_GENERAL != mode))
                return SYS_ERR_VLAN_PROVLAN_ENABLED_PORT;
        }
#endif

#ifdef CONFIG_SYS_VOICEVLAN
        SYS_ERR_CHK(sal_vlan_voiceVlanState_get(&state));
        SYS_ERR_CHK(sal_vlan_portVoiceVlanActive_get(port,&active));
        SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(port,&status));
        SYS_ERR_CHK(sal_vlan_portVoiceVlanMode_get(port,&pmode));

        if((VOICE_VLAN_AUTO_ENABLED == state) && (ENABLED == active))
        {
            if((SYS_VLAN_PORT_MODE_TRUNK != mode) && (SYS_VLAN_PORT_MODE_GENERAL!= mode))
                return SYS_ERR_VLAN_VOICEVLAN_PORT_MODE;
        }

        if((ENABLED == status) && (VOICE_VLAN_MODE_AUTO == pmode))
        {
            if((SYS_VLAN_PORT_MODE_TRUNK != mode)&&(SYS_VLAN_PORT_MODE_GENERAL != mode))
                return SYS_ERR_VLAN_VOICEVLAN_PORT_MODE;
        }
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
        SYS_ERR_CHK(sal_surveillance_vlan_portEnable_get(port,&svPortEnable));
        SYS_ERR_CHK(sal_surveillance_vlan_portMode_get(port,&svPortMode));

        if((ENABLED == svPortEnable) && (SURVEILLANCE_VLAN_PORT_MODE_AUTO == svPortMode))
        {
            if((SYS_VLAN_PORT_MODE_TRUNK != mode)&&(SYS_VLAN_PORT_MODE_GENERAL != mode))
                return SYS_ERR_VLAN_SV_PORT_MODE;
        }
#endif

#ifdef CONFIG_SYS_MACVLAN
        if (SYS_VLAN_PORT_MODE_GENERAL != mode)
        {
            SYS_ERR_CHK(sal_macvlan_mapping_get(port, mapping));

            for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM; i++)
            {
                if (mapping[i].groupId != 0)
                    return SYS_ERR_VLAN_MAC_PORT_MODE;
            }
        }
#endif

#ifdef CONFIG_SYS_PROTO_GVRP
        SYS_ERR_CHK(sal_gvrp_port_enable_get(port, &enable));
        if ((ENABLED == enable) && (SYS_VLAN_PORT_MODE_TRUNK != mode))
        {
            return SYS_ERR_GVRP_PORT_MODE;
        }
#endif

#ifdef CONFIG_SYS_PROTO_MVR
        SYS_ERR_CHK(sal_igmp_mvr_portType_get(port, &type));
        if (MVR_TYPE_RECEIVER == type)
        {
            if (SYS_VLAN_PORT_MODE_ACCESS != mode)
                return SYS_ERR_MVR_PORT_MODE;
        }

        if (MVR_TYPE_SOURCE == type)
        {
            SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVid));
            SYS_ERR_CHK(sal_vlan_operEntry_get(mvrVid,&vlanEntry));
            if (IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmMember, port))
                return SYS_ERR_MVR_VLAN_PORT_SOURCE;
        }
#endif
    }

    SYS_ERR_TRK_CHK(sal_vlan_portAdminRoleUplinkEnable_set(port, DISABLED));
    SYS_ERR_TRK_CHK(sal_vlan_portDot1qTunnelTpid_set(port, DFLT_VLAN_PORT_TPID));
    SYS_ERR_TRK_CHK(rsd_vlan_portAdminMode_set(port, mode));
    SYS_ERR_CHK(cfg_vlan_portAdminMode_set(port, mode));

    SYS_ERR_CHK(sal_l2_dynamicMac_clearByPort(port));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminMode_get(sys_logic_port_t port, sys_vlan_portMode_t *pMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_portAdminMode_get(port, pMode));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminMode_chk(sys_logic_port_t port, uint32 allowMask)
{
    sys_enable_t enable;
    sys_vlan_portMode_t mode;

    SYS_ERR_CHK(sal_vlan_isChkVlanPortMode_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(cfg_vlan_portAdminMode_get(port, &mode));

    if (!(allowMask & (1 << mode)))
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminRoleUplinkEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_vlan_portAdminRoleUplinkEnable_set(port, enable));
    SYS_ERR_CHK(cfg_vlan_portAdminRoleUplinkEnable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminRoleUplinkEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_portAdminRoleUplinkEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminAcceptFrameType_set(sys_logic_port_t port, sys_vlan_frameType_t type)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_VLAN_FRAME_TYPE_INVALID(type), SYS_ERR_VLAN_FRAMETYPE);

    SYS_ERR_TRK_CHK(rsd_vlan_portAdminAcceptFrameType_set(port, type));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminAcceptFrameType_get(sys_logic_port_t port, sys_vlan_frameType_t *pType)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pType), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portAdminAcceptFrameType_get(port, pType));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminIgrFilterEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_vlan_portAdminIgrFilterEnable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminIgrFilterEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portAdminIgrFilterEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminPvid_set(sys_logic_port_t port, sys_vid_t pvid)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_SYSTEM_VID_INVALID(pvid), SYS_ERR_VLAN_ID);

#ifdef CONFIG_SYS_VOICEVLAN
    int32 ret = -1;
    sys_vid_t voiceVid;

    ret = sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_TRUNK));
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));

    if((SYS_ERR_OK == ret)&&(pvid == voiceVid))
    {
        if(SYS_ERR_OK != sal_vlan_portVoiceStatus_chk(port))
            return SYS_ERR_VLAN_VOICEVLAN_AUTO_PORT;
    }
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    sys_vid_t surveillanceVid;

    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&surveillanceVid));

    if((SYS_ERR_OK == sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_TRUNK))) &&
        (pvid == surveillanceVid))
    {
        if(IS_PORT_USE_BY_SURVEILLANCE_VLAN(port))
            return SYS_ERR_VLAN_SV_USED_PORT;
    }
#endif

    SYS_ERR_TRK_CHK(rsd_vlan_portAdminPvid_set(port, pvid));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminPvid_get(sys_logic_port_t port, sys_vid_t *pPvid)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPvid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portAdminPvid_get(port, pPvid));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminVlanmask_add(sys_logic_port_t port, sys_vlan_membership_t membership, sys_vlanmask_t *pVlanmask)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((membership >= SYS_VLAN_MEMBERSHIP_END), SYS_ERR_VLAN_MEMBERSHIP);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

#ifdef CONFIG_SYS_VOICEVLAN
    int32 ret = 0;
    sys_vid_t voiceVid;
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));

    if (SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
    {
        if(VLANMASK_IS_VIDSET(*pVlanmask, voiceVid))
        {
            ret = sal_vlan_portVoiceStatus_chk(port);

            if (SYS_ERR_OK != ret)
                VLANMASK_CLEAR_VID(*pVlanmask, voiceVid);
        }
    }
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    uint8 svClear = FALSE;
    sys_vid_t surveillanceVid;
    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&surveillanceVid));

    if (SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
    {
        if(VLANMASK_IS_VIDSET(*pVlanmask, surveillanceVid))
        {
            if(IS_PORT_USE_BY_SURVEILLANCE_VLAN(port))
            {
                svClear = TRUE;
                VLANMASK_CLEAR_VID(*pVlanmask, surveillanceVid);
            }
        }
    }
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    sys_enable_t mvrEn;
    sys_vid_t mvrVid;
    mvr_portType_t portType;
    uint8 mvrClear= FALSE;

    if (SYS_VLAN_EXCLUDED == membership || SYS_VLAN_FORBIDDEN == membership)
    {
        SYS_ERR_CHK(sal_igmp_mvr_enable_get(&mvrEn));
        if (ENABLED == mvrEn)
        {
            SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVid));
            SYS_ERR_CHK(sal_igmp_mvr_portType_get(port, &portType));
            if (VLANMASK_IS_VIDSET(*pVlanmask, mvrVid))
            {
                if (MVR_TYPE_SOURCE == portType)
                {
                    mvrClear = TRUE;
                    VLANMASK_CLEAR_VID(*pVlanmask, mvrVid);
                }
            }
        }
    }
#endif

#ifdef CONFIG_SYS_PROTO_GARP
    sys_vlanmask_t pVlanmaskTmp;
    sys_vlanmask_t pVlanmaskTmp1;
    sys_vid_t vid;
				
    if (SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
    {
        SYS_MEM_CLEAR(pVlanmaskTmp);
        SYS_MEM_CLEAR(pVlanmaskTmp1);
        sal_vlan_portAdminVlanmask_get(port, membership, &pVlanmaskTmp);
        VLANMASK_XOR(pVlanmaskTmp1, *pVlanmask, pVlanmaskTmp);
        FOR_EACH_VID(vid)
        {
			if(VLANMASK_IS_VIDSET(pVlanmaskTmp1, vid))
			{
				gmrp_vlan_port_set(port, vid, ENABLED);
			}
        }
    }
#endif

    SYS_ERR_CHK(rsd_vlan_portAdminVlanmask_add(port, membership, pVlanmask));

#ifdef CONFIG_SYS_VOICEVLAN
    if (SYS_ERR_OK != ret)
        return SYS_ERR_VLAN_VOICEVLAN_AUTO_PORT;
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    if (svClear)
        return SYS_ERR_VLAN_SV_USED_PORT;
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    if (mvrClear)
        return SYS_ERR_MVR_VLAN_PORT_SOURCE;
#endif

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminVlanmask_del(sys_logic_port_t port, sys_vlan_membership_t membership, sys_vlanmask_t *pVlanmask)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((membership >= SYS_VLAN_MEMBERSHIP_END), SYS_ERR_VLAN_MEMBERSHIP);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);
		
#ifdef CONFIG_SYS_PROTO_GARP
    sys_vlanmask_t pVlanmaskTmp;
    sys_vlanmask_t pVlanmaskTmp1;
    sys_vid_t vid;
		
    if (SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
    {
		SYS_MEM_CLEAR(pVlanmaskTmp);
		SYS_MEM_CLEAR(pVlanmaskTmp1);
		sal_vlan_portAdminVlanmask_get(port, membership, &pVlanmaskTmp);
		VLANMASK_AND(pVlanmaskTmp1, *pVlanmask, pVlanmaskTmp);
		FOR_EACH_VID(vid)
		{
			if(VLANMASK_IS_VIDSET(pVlanmaskTmp1, vid))
			{
				gmrp_vlan_port_set(port, vid, DISABLED);
			}
		}
    }
#endif

    SYS_ERR_CHK(rsd_vlan_portAdminVlanmask_del(port, membership, pVlanmask));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminVlanmask_get(sys_logic_port_t port, sys_vlan_membership_t membership, sys_vlanmask_t *pVlanmask)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((membership >= SYS_VLAN_MEMBERSHIP_END), SYS_ERR_VLAN_MEMBERSHIP);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portAdminVlanmask_get(port, membership, pVlanmask));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminVlanMember_get(sys_logic_port_t port, sys_vlan_portMember_t *pVlanMember)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVlanMember), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portAdminVlanMember_get(port, pVlanMember));

    return SYS_ERR_OK;
}

static int32 _sal_vlan_portAdminVlanMembership_set(sys_logic_port_t port, sys_vid_t vid, sys_vlan_membership_t membership)
{
    sys_vlanmask_t vmTarget;
#ifdef CONFIG_SYS_PROTO_MVR
    mvr_portType_t portType;
    sys_vid_t mvrVlan;
    sys_enable_t mvrEnable;
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&mvrEnable));
    if (ENABLED == mvrEnable)
    {
        SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlan));
        SYS_ERR_CHK(sal_igmp_mvr_portType_get(port, &portType));
        if (vid == mvrVlan)
        {
            if (MVR_TYPE_RECEIVER == portType)
            {
                if (SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
                    return SYS_ERR_MVR_VLAN_PORT_TYPE;
            }
            else if (MVR_TYPE_SOURCE == portType)
            {
                if (SYS_VLAN_FORBIDDEN == membership || SYS_VLAN_EXCLUDED == membership)
                    return SYS_ERR_MVR_VLAN_PORT_TYPE;
            }
        }
        #if 0 
        else 
        {
            if (MVR_TYPE_SOURCE == portType)
            {
                if (SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
                    return SYS_ERR_MVR_VLAN_PORT_SOURCE;
            }
        }
        #endif
    }
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    sys_vid_t voiceVid;
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));

    if (SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
    {
        if(vid == voiceVid)
        {
            if(SYS_ERR_OK != sal_vlan_portVoiceStatus_chk(port))
                return SYS_ERR_VLAN_VOICEVLAN_AUTO_PORT;
        }
    }
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    sys_vid_t surveillanceVid;
    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&surveillanceVid));

    if (SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
    {
        if(vid == surveillanceVid)
        {
            if(IS_PORT_USE_BY_SURVEILLANCE_VLAN(port))
                return SYS_ERR_VLAN_SV_USED_PORT;
        }
    }
#endif

    VLANMASK_CLEAR_ALL(vmTarget);
    VLANMASK_SET_VID(vmTarget, vid);
    switch (membership)
    {
    case SYS_VLAN_FORBIDDEN:
        SYS_ERR_CHK(rsd_vlan_portAdminVlanmask_add(port, SYS_VLAN_FORBIDDEN, &vmTarget));
        SYS_ERR_CHK(rsd_vlan_portAdminVlanmask_add(port, SYS_VLAN_EXCLUDED, &vmTarget));
        break;

    case SYS_VLAN_TAGGED:
        SYS_ERR_CHK(rsd_vlan_portAdminVlanmask_add(port, SYS_VLAN_TAGGED, &vmTarget));
        break;

    case SYS_VLAN_UNTAGGED:
        SYS_ERR_CHK(rsd_vlan_portAdminVlanmask_add(port, SYS_VLAN_UNTAGGED, &vmTarget));
        break;

    case SYS_VLAN_EXCLUDED:
    default:
        SYS_ERR_CHK(rsd_vlan_portAdminVlanmask_add(port, SYS_VLAN_EXCLUDED, &vmTarget));
        break;
    }

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminVlanMembership_set(sys_logic_port_t port, sys_vid_t vid, sys_vlan_membership_t membership)
{
    sys_vlan_portMode_t portMode;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_SYSTEM_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((membership >= SYS_VLAN_MEMBERSHIP_END), SYS_ERR_VLAN_MEMBERSHIP);

    SYS_ERR_CHK(sal_vlan_portAdminMode_get(port, &portMode));
    SYS_ERR_TRK_CHK(_sal_vlan_portAdminVlanMembership_set(port, vid, membership));

    return SYS_ERR_OK;
}

int32 sal_vlan_portAdminVlanMembership_get(sys_logic_port_t port, sys_vid_t vid, sys_vlan_membership_t *pMembership)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_SYSTEM_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pMembership), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portAdminVlanMembership_get(port, vid, pMembership));

    return SYS_ERR_OK;
}

int32 sal_vlan_portTrunkAllowedVlan_set(sys_logic_port_t port, sys_vlanmask_t *pVlanmask)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

#ifdef CONFIG_SYS_VOICEVLAN
    int32 ret = 0;
    uint8 vvClear = FALSE;
    sys_vid_t voiceVid;
    sys_vlan_portMember_t voperMbr;
    SYS_ERR_CHK(sal_vlan_portOperVlanMember_get(port, &voperMbr));
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
    if(VLANMASK_IS_VIDSET(*pVlanmask,voiceVid) ||
        (!VLANMASK_IS_VIDSET(*pVlanmask,voiceVid) && VLANMASK_IS_VIDSET(voperMbr.vmMember, voiceVid)))
    {
        ret = sal_vlan_portVoiceStatus_chk(port);
        if (SYS_ERR_OK != ret)
        {
            vvClear = TRUE;
            VLANMASK_CLEAR_VID(*pVlanmask, voiceVid);
        }
    }
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    uint8 svClear = FALSE;
    sys_vid_t surveillanceVid;
    sys_vlan_portMember_t soperMbr;
    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&surveillanceVid));
    SYS_ERR_CHK(sal_vlan_portOperVlanMember_get(port, &soperMbr));

    if(VLANMASK_IS_VIDSET(*pVlanmask, surveillanceVid) ||
        (!VLANMASK_IS_VIDSET(*pVlanmask,surveillanceVid) && VLANMASK_IS_VIDSET(soperMbr.vmMember, surveillanceVid)))
    {
        if(IS_PORT_USE_BY_SURVEILLANCE_VLAN(port))
        {
            svClear = TRUE;
            VLANMASK_CLEAR_VID(*pVlanmask, surveillanceVid);
        }
    }
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    sys_enable_t mvrEn;
    sys_vid_t mvrVid;
    mvr_portType_t portType;
    uint8 mvrSet = FALSE;
    sys_vlanmask_t trunkVlanMask;

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&mvrEn));
    if (ENABLED == mvrEn)
    {
        SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVid));
        SYS_ERR_CHK(cfg_vlan_portTrunkAllowedVlan_get(port, &trunkVlanMask));

        if (VLANMASK_IS_VIDSET(trunkVlanMask, mvrVid))
        {
            SYS_ERR_CHK(sal_igmp_mvr_portType_get(port, &portType));

            if (!VLANMASK_IS_VIDSET(*pVlanmask, mvrVid))
            {
                if (MVR_TYPE_SOURCE == portType)
                {
                    mvrSet = TRUE;
                    VLANMASK_SET_VID(*pVlanmask, mvrVid);
                }
            }
          }
    }
#endif

    SYS_ERR_CHK(cfg_vlan_portTrunkAllowedVlan_set(port, pVlanmask));
    SYS_ERR_CHK(rsd_vlan_portTrunkAllowedVlan_set(port, pVlanmask));

#ifdef CONFIG_SYS_VOICEVLAN
    if (vvClear)
        return SYS_ERR_VLAN_VOICEVLAN_AUTO_PORT;
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    if (svClear)
        return SYS_ERR_VLAN_SV_USED_PORT;
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    if (mvrSet)
        return SYS_ERR_MVR_VLAN_PORT_SOURCE;
#endif

    return SYS_ERR_OK;
}

int32 sal_vlan_portTrunkAllowedVlan_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_portTrunkAllowedVlan_get(port, pVlanmask));

    return SYS_ERR_OK;
}

int32 sal_vlan_portOperPvid_get(sys_logic_port_t port, sys_vid_t *pPvid)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPvid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portOperPvid_get(port, pPvid));

    return SYS_ERR_OK;
}

int32 sal_vlan_portOperVlanMember_get(sys_logic_port_t port, sys_vlan_portMember_t *pVlanMember)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVlanMember), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portOperVlanMember_get(port, pVlanMember));

    return SYS_ERR_OK;
}

int32 sal_vlan_portOperVlanMembership_get(sys_logic_port_t port, sys_vid_t vid, sys_vlan_membership_t *pMembership)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_SYSTEM_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pMembership), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portOperVlanMembership_get(port, vid, pMembership));

    return SYS_ERR_OK;
}

int32 sal_vlan_dot1qTunnelTpid_set(uint32 index, uint16 tpid)
{
    SYS_ERR_CHK(rsd_vlan_dot1qTunnelTpid_set(index, tpid));

    return SYS_ERR_OK;
}

int32 sal_vlan_dot1qTunnelTpid_get(uint32 index, uint16 *pTpid)
{
    SYS_ERR_CHK(rsd_vlan_dot1qTunnelTpid_get(index, pTpid));

    return SYS_ERR_OK;
}

int32 sal_vlan_portDot1qTunnelTpid_set(sys_logic_port_t port, uint16 tpid)
{
    SYS_ERR_CHK(rsd_vlan_portDot1qTunnelTpid_set(port, tpid));

    return SYS_ERR_OK;
}

int32 sal_vlan_portDot1qTunnelTpid_get(sys_logic_port_t port, uint16 *pTpid)
{
    SYS_ERR_CHK(rsd_vlan_portDot1qTunnelTpid_get(port, pTpid));

    return SYS_ERR_OK;
}

int32 sal_vlan_mibDot1qVlanNumDeletes_get(uint32 *pVlanNumDeletes)
{
    SYS_PARAM_CHK((NULL == pVlanNumDeletes), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_mibDot1qVlanNumDeletes_get(pVlanNumDeletes));

    return SYS_ERR_OK;
}

int32 sal_vlan_mibDot1qVlanCreationTime_get(sys_vid_t vid, uint32 *pTime)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_mibDot1qVlanCreationTime_get(vid, pTime));

    return SYS_ERR_OK;
}

int32 sal_vlan_isChkVlanPortMode_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_vlan_isChkVlanPortMode_set(enable));

    return SYS_ERR_OK;
}

int32 sal_vlan_isChkVlanPortMode_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_isChkVlanPortMode_get(pEnable));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MANAGEMENT_VLAN

int32 sal_vlan_mgmtVid_set(sys_vid_t vid)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    SYS_ERR_CHK(sal_vlan_exclusive_chk(SYS_VLAN_CATE_MGMT, vid));
    SYS_ERR_CHK(rsd_vlan_mgmtVid_set(vid));
    SYS_ERR_CHK(cfg_vlan_mgmtVid_set(vid));

    return SYS_ERR_OK;
}

int32 sal_vlan_mgmtVid_get(sys_vid_t *pVid)
{
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_mgmtVid_get(pVid));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

int32 sal_vlan_guestVid_set(sys_vid_t vid)
{
    
    if(0 != vid)
    {
        SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

        if (!IS_VLAN_OPER_ENTRY_EXIST(vid))
            return SYS_ERR_VLAN_NOT_EXIST;

        SYS_ERR_CHK(sal_vlan_exclusive_chk(SYS_VLAN_CATE_GUEST, vid));
    }

    SYS_ERR_CHK(rsd_vlan_guestVid_set(vid));
    SYS_ERR_CHK(cfg_vlan_guestVid_set(vid));

    return SYS_ERR_OK;
}

int32 sal_vlan_guestVid_get(sys_vid_t *pVid)
{
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_guestVid_get(pVid));

    return SYS_ERR_OK;
}
#endif

