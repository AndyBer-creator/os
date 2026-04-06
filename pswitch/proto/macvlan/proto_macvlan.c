#include <common/sys_def.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_acl.h>
#include <macvlan/proto_macvlan.h>
#define _BITMASK_SET(mask, bits)        \
do{ uint8 __i;                          \
    for (__i = 0 ; __i < bits ; __i++)  \
            mask[__i/8] |= 1 << (7 - __i%8);  \
} while(0)
static int16 _gPortBindCnt[SYS_LOGICPORT_NUM_MAX];
static sys_mac_vlan_group_t _gGroups[CAPA_VLAN_MAC_GROUP_NUM_MAX];
static sys_mac_vlan_mapping_t _gMapping[SYS_LOGICPORT_NUM_MAX][CAPA_VLAN_MAC_GROUP_NUM_MAX];
static int32 _macvlan_last_bind_entry_check(sys_mac_vlan_group_t *pmacGroup)
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 cnt = 0;
    sys_logic_port_t lp;

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (_gPortBindCnt[lp] <= 0)
            continue;

        for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
        {
            
            if (0 == _gMapping[lp][i].groupId || (_gMapping[lp][i].groupId != pmacGroup->groupId))
                continue;

            for (j = 0; j < CAPA_VLAN_MAC_GROUP_NUM ; j++)
            {
                if (_gGroups[j].groupId == pmacGroup->groupId)
                    cnt++;
            }

            if (1 == cnt)
                return SYS_ERR_VLAN_MAC_LAST_DEL;
            else
                return SYS_ERR_OK;
        }
    }

    return SYS_ERR_OK;
}

static int32 _macvlan_byMapping_chip_apply(sys_logic_port_t lp, uint32 mapidx)
{
    uint32 i = 0;
    int32 aclResource = 0;
    sys_acl_entryKey_t key;
    sys_acl_action_t action;
    sys_acl_field_t field;
    sys_err_code_t ret = SYS_ERR_OK;

    SYS_ERR_CHK(rsd_acl_typeAvailable_get(ACL_PROTOTYPE_MACVLAN, &aclResource));

    for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        if (( 0 != _gGroups[i].groupId) && (_gGroups[i].groupId == _gMapping[lp][mapidx].groupId))
        {
            if (--aclResource <= 0)
            {
                SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_RESOURCE_LACK);

                return SYS_ERR_VLAN_MAC_NO_RESOURCE;
            }
	    }
    }

    SYS_MEM_CLEAR(action);

    action.actionMask |= ACL_ACTIONMASK_SVID;
    action.svid = _gMapping[lp][mapidx].vid;

    for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        if (( 0 == _gGroups[i].groupId) || (_gGroups[i].groupId != _gMapping[lp][mapidx].groupId))
            continue;

        SYS_MEM_CLEAR(key);
        SYS_MEM_CLEAR(field);

        RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SMAC);
        osal_memcpy(field.smac.octet, _gGroups[i].mac.octet, CAPA_MAC_BYTE_NUM);
        _BITMASK_SET(field.smacMask.octet, _gGroups[i].mask);

        RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SPM);
        LOGIC_PORTMASK_SET_PORT(field.spm, lp);

        RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_STAG_STATUS);
        field.stagStatus = SYS_ACL_TAGTYPE_UNTAG_PRIOTAG;

        key.macVlan.lp = lp;
        osal_memcpy(key.macVlan.mac.octet, field.smac.octet, CAPA_MAC_BYTE_NUM);
        osal_memcpy(key.macVlan.macMask.octet, field.smacMask.octet, CAPA_MAC_BYTE_NUM);

        MV_DBG("Add mac group ACL idx %u gid %u mask %u %x:%x:%x:%x:%x:%x vid %u lp %s",
                i, _gGroups[i].groupId, _gGroups[i].mask,
                _gGroups[i].mac.octet[0], _gGroups[i].mac.octet[1], _gGroups[i].mac.octet[2],
                _gGroups[i].mac.octet[3], _gGroups[i].mac.octet[4], _gGroups[i].mac.octet[5],
                _gMapping[lp][mapidx].vid, LPSTR(lp));

        ret = rsd_acl_entry_add(ACL_PROTOTYPE_MACVLAN, &key, &action, &field, ENABLED);

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_BIND_FAIL);

            return ret;
        }
    }

    return SYS_ERR_OK;
}

static int32 _macvlan_byMapping_chip_unapply(sys_logic_port_t lp, uint32 mapidx)
{
    uint32 i = 0;
    sys_acl_entryKey_t key;
    sys_err_code_t ret = SYS_ERR_OK;

    for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        if (( 0 == _gGroups[i].groupId) || (_gGroups[i].groupId != _gMapping[lp][mapidx].groupId))
            continue;

        MV_DBG("Del mac group ACL idx %u gid %u mask %u %x:%x:%x:%x:%x:%x vid %u lp %s",
                i, _gGroups[i].groupId, _gGroups[i].mask,
                _gGroups[i].mac.octet[0], _gGroups[i].mac.octet[1], _gGroups[i].mac.octet[2],
                _gGroups[i].mac.octet[3], _gGroups[i].mac.octet[4], _gGroups[i].mac.octet[5],
                _gMapping[lp][mapidx].vid, LPSTR(lp));

        SYS_MEM_CLEAR(key);

        key.macVlan.lp = lp;
        osal_memcpy(key.macVlan.mac.octet, _gGroups[i].mac.octet, CAPA_MAC_BYTE_NUM);
        _BITMASK_SET(key.macVlan.macMask.octet, _gGroups[i].mask);

        ret = rsd_acl_entry_del(ACL_PROTOTYPE_MACVLAN, &key);

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_UNBIND_FAIL);

            return ret;
        }
    }

    return SYS_ERR_OK;
}

static int32 _macvlan_byMapping_chip_action_update(sys_logic_port_t lp, uint32 mapidx)
{
    uint32 i = 0;
    sys_acl_entryKey_t key;
    sys_acl_action_t action;
    sys_err_code_t ret = SYS_ERR_OK;

    SYS_MEM_CLEAR(action);

    action.actionMask |= ACL_ACTIONMASK_SVID;
    action.svid = _gMapping[lp][mapidx].vid;

    for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        if (_gPortBindCnt[lp] <= 0)
            continue;

        if (( 0 == _gGroups[i].groupId) || (_gGroups[i].groupId != _gMapping[lp][mapidx].groupId))
            continue;

        MV_DBG("Modify mac group ACL action idx %u gid %u mask %u %x:%x:%x:%x:%x:%x vid %u lp %s",
                i, _gGroups[i].groupId, _gGroups[i].mask,
                _gGroups[i].mac.octet[0], _gGroups[i].mac.octet[1], _gGroups[i].mac.octet[2],
                _gGroups[i].mac.octet[3], _gGroups[i].mac.octet[4], _gGroups[i].mac.octet[5],
                _gMapping[lp][mapidx].vid, LPSTR(lp));

        SYS_MEM_CLEAR(key);

        key.macVlan.lp = lp;
        osal_memcpy(key.macVlan.mac.octet, _gGroups[i].mac.octet, CAPA_MAC_BYTE_NUM);
        _BITMASK_SET(key.macVlan.macMask.octet, _gGroups[i].mask);

        ret = rsd_acl_entryAction_set(ACL_PROTOTYPE_MACVLAN, &key, &action);

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_UPDATE_FAIL);

            return ret;
        }
    }

    return SYS_ERR_OK;
}

static int32 _macvlan_byGroup_chip_apply(uint32 gid)
{
    uint32 i = 0;
    int32 aclResource = 0;
    sys_acl_entryKey_t key;
    sys_acl_action_t action;
    sys_acl_field_t field;
    sys_logic_port_t lp;
    sys_err_code_t ret = SYS_ERR_OK;

    SYS_ERR_CHK(rsd_acl_typeAvailable_get(ACL_PROTOTYPE_MACVLAN, &aclResource));

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (_gPortBindCnt[lp] <= 0)
            continue;

        for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
        {
            if (( 0 != _gGroups[gid].groupId) && (_gGroups[gid].groupId == _gMapping[lp][i].groupId))
            {
                if (--aclResource <= 0)
                {
                    SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_RESOURCE_LACK);

                    return SYS_ERR_VLAN_MAC_NO_RESOURCE;
                }
	        }
        }
    }

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (_gPortBindCnt[lp] <= 0)
            continue;

        for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
        {
            if (( 0 == _gGroups[gid].groupId) || (_gGroups[gid].groupId != _gMapping[lp][i].groupId))
                continue;

            MV_DBG("Add mac group ACL idx %u gid %u mask %u %x:%x:%x:%x:%x:%x vid %u lp %s",
                gid, _gGroups[gid].groupId, _gGroups[gid].mask,
                _gGroups[gid].mac.octet[0], _gGroups[gid].mac.octet[1], _gGroups[gid].mac.octet[2],
                _gGroups[gid].mac.octet[3], _gGroups[gid].mac.octet[4], _gGroups[gid].mac.octet[5],
                _gMapping[lp][i].vid, LPSTR(lp));

            SYS_MEM_CLEAR(action);
            SYS_MEM_CLEAR(field);
            SYS_MEM_CLEAR(key);

            action.actionMask |= ACL_ACTIONMASK_SVID;
            action.svid = _gMapping[lp][i].vid;

            RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SMAC);
            osal_memcpy(field.smac.octet, _gGroups[gid].mac.octet, CAPA_MAC_BYTE_NUM);
            _BITMASK_SET(field.smacMask.octet, _gGroups[gid].mask);

            RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SPM);
            LOGIC_PORTMASK_SET_PORT(field.spm, lp);

            RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_STAG_STATUS);
            field.stagStatus = SYS_ACL_TAGTYPE_UNTAG_PRIOTAG;

            key.macVlan.lp = lp;
            osal_memcpy(key.macVlan.mac.octet, field.smac.octet, CAPA_MAC_BYTE_NUM);
            osal_memcpy(key.macVlan.macMask.octet, field.smacMask.octet, CAPA_MAC_BYTE_NUM);

            ret = rsd_acl_entry_add(ACL_PROTOTYPE_MACVLAN, &key, &action, &field, ENABLED);

            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_BIND_FAIL);

                return ret;
            }
        }
    }

    return SYS_ERR_OK;
}

static int32 _macvlan_byGroup_chip_unapply(uint32 gid)
{
    uint32 i = 0;
    sys_acl_entryKey_t key;
    sys_logic_port_t lp;
    sys_err_code_t ret = SYS_ERR_OK;

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (_gPortBindCnt[lp] <= 0)
            continue;

        for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
        {
            if (( 0 == _gGroups[gid].groupId) || (_gGroups[gid].groupId != _gMapping[lp][i].groupId))
                continue;

            SYS_MEM_CLEAR(key);

            MV_DBG("Del mac group ACL idx %u gid %u mask %u %x:%x:%x:%x:%x:%x vid %u lp %s",
                    gid, _gGroups[gid].groupId, _gGroups[gid].mask,
                    _gGroups[gid].mac.octet[0], _gGroups[gid].mac.octet[1], _gGroups[gid].mac.octet[2],
                    _gGroups[gid].mac.octet[3], _gGroups[gid].mac.octet[4], _gGroups[gid].mac.octet[5],
                    _gMapping[lp][i].vid, LPSTR(lp));

            key.macVlan.lp = lp;
            osal_memcpy(key.macVlan.mac.octet, _gGroups[gid].mac.octet, CAPA_MAC_BYTE_NUM);
            _BITMASK_SET(key.macVlan.macMask.octet, _gGroups[gid].mask);

            ret = rsd_acl_entry_del(ACL_PROTOTYPE_MACVLAN, &key);

            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_UNBIND_FAIL);

                return ret;
            }
        }
    }

    return SYS_ERR_OK;
}

static int32 _macvlan_byPort_chip_apply(sys_logic_port_t lp)
{
    uint32 i = 0;
    uint32 j = 0;
    int32 aclResource = 0;
    sys_acl_entryKey_t key;
    sys_acl_action_t action;
    sys_acl_field_t field;
    sys_err_code_t ret = SYS_ERR_OK;

    if (_gPortBindCnt[lp] <= 0)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_acl_typeAvailable_get(ACL_PROTOTYPE_MACVLAN, &aclResource));

    for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        
        if (_gMapping[lp][i].groupId == 0)
            continue;

        for (j = 0 ; j < CAPA_VLAN_MAC_GROUP_NUM ; j++)
        {
            if (_gGroups[j].groupId == _gMapping[lp][i].groupId)
            {
                if (--aclResource <= 0)
                {
                    SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_RESOURCE_LACK);

                    return SYS_ERR_VLAN_MAC_NO_RESOURCE;
                }
            }
        }
    }

    for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        
        if (_gMapping[lp][i].groupId == 0)
            continue;

        SYS_MEM_CLEAR(action);
        action.actionMask |= ACL_ACTIONMASK_SVID;
        action.svid = _gMapping[lp][i].vid;

        for (j = 0 ; j < CAPA_VLAN_MAC_GROUP_NUM ; j++)
        {
            if (_gGroups[j].groupId != _gMapping[lp][i].groupId)
                continue;

            MV_DBG("Add mac group ACL idx %u gid %u mask %u %x:%x:%x:%x:%x:%x vid %u lp %s",
                j, _gGroups[j].groupId, _gGroups[j].mask,
                _gGroups[j].mac.octet[0], _gGroups[j].mac.octet[1], _gGroups[j].mac.octet[2],
                _gGroups[j].mac.octet[3], _gGroups[j].mac.octet[4], _gGroups[j].mac.octet[5],
                _gMapping[lp][i].vid, LPSTR(lp));

            SYS_MEM_CLEAR(field);
            SYS_MEM_CLEAR(key);

            RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SMAC);
            osal_memcpy(field.smac.octet, _gGroups[j].mac.octet, CAPA_MAC_BYTE_NUM);
            _BITMASK_SET(field.smacMask.octet, _gGroups[j].mask);

            RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SPM);
            LOGIC_PORTMASK_SET_PORT(field.spm, lp);

            RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_STAG_STATUS);
            field.stagStatus = SYS_ACL_TAGTYPE_UNTAG_PRIOTAG;

            key.macVlan.lp = lp;
            osal_memcpy(key.macVlan.mac.octet, field.smac.octet, CAPA_MAC_BYTE_NUM);
            osal_memcpy(key.macVlan.macMask.octet, field.smacMask.octet, CAPA_MAC_BYTE_NUM);

            ret = rsd_acl_entry_add(ACL_PROTOTYPE_MACVLAN, &key, &action, &field, ENABLED);

            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_BIND_FAIL);

                return ret;
            }
        }
    }

    return SYS_ERR_OK;
}

static int32 _macvlan_byPort_chip_unapply(sys_logic_port_t lp)
{
    uint32 i = 0;
    uint32 j = 0;
    sys_acl_entryKey_t key;
    sys_err_code_t ret = SYS_ERR_OK;

    if (_gPortBindCnt[lp] <= 0)
        return SYS_ERR_OK;

    for (i = 0 ; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        
        if (_gMapping[lp][i].groupId == 0)
            continue;

        for (j = 0 ; j < CAPA_VLAN_MAC_GROUP_NUM ; j++)
        {
            if (_gGroups[j].groupId != _gMapping[lp][i].groupId)
                continue;

            SYS_MEM_CLEAR(key);

            MV_DBG("Del mac group ACL idx %u gid %u mask %u %x:%x:%x:%x:%x:%x vid %u",
                    j, _gGroups[j].groupId, _gGroups[j].mask,
                    _gGroups[j].mac.octet[0], _gGroups[j].mac.octet[1], _gGroups[j].mac.octet[2],
                    _gGroups[j].mac.octet[3], _gGroups[j].mac.octet[4], _gGroups[j].mac.octet[5],
                    _gMapping[lp][i].vid);

            key.macVlan.lp = lp;
            osal_memcpy(key.macVlan.mac.octet, _gGroups[j].mac.octet, CAPA_MAC_BYTE_NUM);
            _BITMASK_SET(key.macVlan.macMask.octet, _gGroups[j].mask);

            ret = rsd_acl_entry_del(ACL_PROTOTYPE_MACVLAN, &key);

            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_MAC_UNBIND_FAIL);

                return ret;
            }
        }
    }

    return SYS_ERR_OK;
}

static int32 _macvlan_trkEvt_handler(uint32 event, char *pData)
{
    sys_trunk_event_t *pTrunkEvent = (sys_trunk_event_t *)pData;
    sys_logic_port_t lp;

    switch (event)
    {
        case SYS_NT_TRK_MBR_JOIN:
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, pTrunkEvent->portmask)
            {
                MV_DBG("<SYS_NT_TRK_MBR_JOIN> port %s to trunkid %u", LPSTR(lp), pTrunkEvent->trunkIdx);
                SYS_ERR_CHK(_macvlan_byPort_chip_unapply(lp));
                SYS_ERR_CHK(_macvlan_byPort_chip_apply(TRK2LP(pTrunkEvent->trunkIdx)));
            }
            break;

        case SYS_NT_TRK_MBR_LEAVE:
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, pTrunkEvent->portmask)
            {
                MV_DBG("<SYS_NT_TRK_MBR_LEAVE> port %s from trunkid %u", LPSTR(lp), pTrunkEvent->trunkIdx);
                SYS_ERR_CHK(_macvlan_byPort_chip_unapply(TRK2LP(pTrunkEvent->trunkIdx)));
                SYS_ERR_CHK(_macvlan_byPort_chip_apply(lp));
            }
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}

static int32 _macvlan_acl_entryPrio_cmp(sys_acl_entry_t *entry, sys_acl_entry_t *cmp)
{
    
    return 0;
}
static int32 _macvlan_acl_entryInfo_dump(sys_acl_entry_t *entry)
{
    SYS_PARAM_CHK((NULL == entry), SYS_ERR_NULL_POINTER);

    ACLDUMP("  lp %s vid %u mac %x:%x:%x:%x:%x:%x", LPSTR(entry->key.macVlan.lp), entry->action.svid,
        entry->key.macVlan.mac.octet[0], entry->key.macVlan.mac.octet[1], entry->key.macVlan.mac.octet[2],
        entry->key.macVlan.mac.octet[3], entry->key.macVlan.mac.octet[4], entry->key.macVlan.mac.octet[5]);

    return 0;
}

static int32 _macvlan_acl_entry2Str_get(sys_acl_entry_t *entry, char *buf, uint32 maxlen)
{
    SYS_PARAM_CHK((NULL == entry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == buf), SYS_ERR_NULL_POINTER);

    osal_sprintf(buf, "lp %s vid %u mac %x:%x:%x:%x:%x:%x", LPSTR(entry->key.macVlan.lp), entry->action.svid,
        entry->key.macVlan.mac.octet[0], entry->key.macVlan.mac.octet[1], entry->key.macVlan.mac.octet[2],
        entry->key.macVlan.mac.octet[3], entry->key.macVlan.mac.octet[4], entry->key.macVlan.mac.octet[5]);

    if (osal_strlen(buf) >= maxlen)
        buf[maxlen-1] = '\0';

    return SYS_ERR_OK;
}
int32 macvlan_group_get(uint32 idx, sys_mac_vlan_group_t *pmacGroup)
{
    SYS_PARAM_CHK((idx >= CAPA_VLAN_MAC_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pmacGroup), SYS_ERR_NULL_POINTER);

    osal_memcpy(pmacGroup, &_gGroups[idx], sizeof(sys_mac_vlan_group_t));

    return SYS_ERR_OK;
}
int32 macvlan_group_set(sys_mac_vlan_group_t *pmacGroup)
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 bits = 0;
    uint32 oldGID = 0;
    uint32 result = 0;
    uint32 emptyIdx = 0xffff;
    sys_err_code_t ret = SYS_ERR_OK;

    SYS_PARAM_CHK((NULL == pmacGroup), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        
        if (0xffff == emptyIdx && _gGroups[i].mask == 0)
            emptyIdx = i;

        if (_gGroups[i].mask == 0)
            continue;

        bits = (_gGroups[i].mask <= pmacGroup->mask) ? _gGroups[i].mask : pmacGroup->mask;

        result = 1;
        for (j = 0 ; j < bits ; j++)
        {
            if (((1 << j%8) & _gGroups[i].mac.octet[j/8]) != ((1 << j%8) & pmacGroup->mac.octet[j/8]))
            {
                result = 0;
                break;
            }
        }

        if (0 == result)
            continue;
        else if ((_gGroups[i].mask == pmacGroup->mask)
                && (0 == osal_memcmp(_gGroups[i].mac.octet, pmacGroup->mac.octet, CAPA_MAC_BYTE_NUM)))
        {
            
            SYS_ERR_CHK(_macvlan_last_bind_entry_check(&_gGroups[i]));

            MV_DBG("Modify mac group idx %u gid %u mask %u %x:%x:%x:%x:%x:%x",
                i, pmacGroup->groupId, pmacGroup->mask,
                pmacGroup->mac.octet[0], pmacGroup->mac.octet[1], pmacGroup->mac.octet[2],
                pmacGroup->mac.octet[3], pmacGroup->mac.octet[4], pmacGroup->mac.octet[5]);

            SYS_ERR_CHK(_macvlan_byGroup_chip_unapply(i));

            oldGID = _gGroups[i].groupId;
            _gGroups[i].groupId = pmacGroup->groupId;

            ret = _macvlan_byGroup_chip_apply(i);

            if (SYS_ERR_VLAN_MAC_NO_RESOURCE == ret)
            {
                _gGroups[i].groupId = oldGID;
                _macvlan_byGroup_chip_apply(i);

                return ret;
            }

            return SYS_ERR_OK;
        }
        else
            return SYS_ERR_VLAN_MAC_OVERLAP;

    }

    if (emptyIdx == 0xffff)
        return SYS_ERR_VLAN_MAC_MAX;

    MV_DBG("Set mac group idx %u gid %u mask %u %x:%x:%x:%x:%x:%x",
        emptyIdx, pmacGroup->groupId, pmacGroup->mask,
        pmacGroup->mac.octet[0], pmacGroup->mac.octet[1], pmacGroup->mac.octet[2],
        pmacGroup->mac.octet[3], pmacGroup->mac.octet[4], pmacGroup->mac.octet[5]);

    osal_memcpy(&_gGroups[emptyIdx], pmacGroup, sizeof(sys_mac_vlan_group_t));

    ret = _macvlan_byGroup_chip_apply(emptyIdx);

    if (SYS_ERR_VLAN_MAC_NO_RESOURCE == ret)
        SYS_MEM_CLEAR(_gGroups[emptyIdx]);

    return ret;
}
int32 macvlan_group_del(sys_mac_vlan_group_t *pmacGroup)
{
    uint32 i = 0;

    SYS_PARAM_CHK((NULL == pmacGroup), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        if (_gGroups[i].mask == pmacGroup->mask
            && !osal_memcmp(_gGroups[i].mac.octet, pmacGroup->mac.octet, CAPA_MAC_BYTE_NUM))
        {
            SYS_ERR_CHK(_macvlan_last_bind_entry_check(&_gGroups[i]));

            MV_DBG("Delete mac group idx %u gid %u mask %u %x:%x:%x:%x:%x:%x",
                i, pmacGroup->groupId, pmacGroup->mask,
                pmacGroup->mac.octet[0], pmacGroup->mac.octet[1], pmacGroup->mac.octet[2],
                pmacGroup->mac.octet[3], pmacGroup->mac.octet[4], pmacGroup->mac.octet[5]);

            SYS_ERR_CHK(_macvlan_byGroup_chip_unapply(i));

            SYS_MEM_CLEAR(_gGroups[i]);

            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_OK;
}
int32 macvlan_mapping_get(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == mapping), SYS_ERR_NULL_POINTER);

    osal_memcpy(mapping, &(_gMapping[port]), sizeof(_gMapping[port]));

    return SYS_ERR_OK;
}
int32 macvlan_mapping_set(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping)
{
    uint32 i = 0;
    uint32 emptyIdx = 0xffff;
    sys_err_code_t ret = SYS_ERR_OK;

    SYS_PARAM_CHK((NULL == mapping), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        if (_gGroups[i].groupId == mapping->groupId)
            break;
    }

    if (CAPA_VLAN_MAC_GROUP_NUM == i)
        return SYS_ERR_VLAN_MAC_NO_GID;

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        
        if (emptyIdx == 0xffff && _gMapping[port][i].groupId == 0)
            emptyIdx = i;

        if (_gMapping[port][i].groupId == mapping->groupId)
        {
            MV_DBG("modify mapping idx %u port %s gid %u vid %u",
                i, LPSTR(port), mapping->groupId, mapping->vid);

            _gMapping[port][i].vid = mapping->vid;

            SYS_ERR_CHK(_macvlan_byMapping_chip_action_update(port, i));

            return SYS_ERR_OK;
        }
    }

    if (emptyIdx == 0xffff)
        return SYS_ERR_VLAN_MAC_NO_RESOURCE;

    MV_DBG("set mapping idx %u port %s gid %u vid %u",
                emptyIdx, LPSTR(port), mapping->groupId, mapping->vid);

    _gMapping[port][emptyIdx].groupId = mapping->groupId;
    _gMapping[port][emptyIdx].vid = mapping->vid;
    _gPortBindCnt[port]++;

    ret = _macvlan_byMapping_chip_apply(port, emptyIdx);

    if (SYS_ERR_VLAN_MAC_NO_RESOURCE == ret)
    {
        SYS_MEM_CLEAR(_gMapping[port][emptyIdx]);
        _gPortBindCnt[port]--;
    }

    return ret;
}
int32 macvlan_mapping_del(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping)
{
    uint32 i = 0;

    SYS_PARAM_CHK((NULL == mapping), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        if (((0 != mapping->groupId) && (mapping->groupId == _gMapping[port][i].groupId)) ||
             ((0 == mapping->groupId) && (0 != _gMapping[port][i].groupId))  )
        {
            MV_DBG("Delete mapping idx %u port %s gid %u vid %u",
                i, LPSTR(port), mapping->groupId, mapping->vid);

            SYS_ERR_CHK(_macvlan_byMapping_chip_unapply(port, i));

            SYS_MEM_CLEAR(_gMapping[port][i]);
            _gPortBindCnt[port]--;

            if (0 != mapping->groupId)
                return SYS_ERR_OK;
        }
    }

    return SYS_ERR_OK;
}
int32 mac_vlan_init(void)
{
    sys_acl_cb_t cb_info;

    SYS_MEM_CLEAR(_gGroups);
    SYS_MEM_CLEAR(_gPortBindCnt);
    SYS_MEM_CLEAR(_gMapping);

    SYS_ERR_CHK(sys_notifier_observer_register("macvlan", SYS_NOTIFIER_SUBJECT_TRUNK,
        (SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE), _macvlan_trkEvt_handler, SYS_NOTIFIER_PRI_MEDIUM));

    osal_memset(&cb_info, 0, sizeof(sys_acl_cb_t));
    cb_info.entry_prio_cmp = _macvlan_acl_entryPrio_cmp;
    cb_info.entry_info_dump = _macvlan_acl_entryInfo_dump;
    cb_info.entry_str_get = _macvlan_acl_entry2Str_get;

    SYS_ERR_CHK(rsd_acl_cb_register(ACL_PROTOTYPE_MACVLAN, &cb_info));

    return SYS_ERR_OK;
}

