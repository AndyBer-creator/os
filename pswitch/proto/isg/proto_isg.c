#include <common/sys_def.h>
#include <isg/proto_isg.h>
#include <ksi/ksi_core.h>
#include <ksi_isg.h>

#include <rsd/rsd_acl.h>
#include <rsd/rsd_trunk.h>

#include <common/sys_def.h>

#include <rsd/rsd_sys.h>
#include <rsd/rsd_port.h>
#define ISG_ACL_KEY_RSV_DHCP_SNP_CLIENT     0
#define ISG_ACL_KEY_RSV_DHCP_SNP_SERVER     1
#define ISG_ACL_KEY_RSV_NUM                 2
static int32 g_portMaxEntryNum[SYS_LOGICPORT_NUM_MAX]; 
static uint32 g_portCurrentEntryNum[SYS_LOGICPORT_NUM_MAX];
static uint32 g_maxEntryNum;
static sys_mac_t zeroMac;
static sys_logic_portmask_t g_portEnablePM;
static sys_isg_verify_type_t g_portVerifyType[SYS_LOGICPORT_NUM_MAX];
static sys_isg_entry_t *g_bindTbl = NULL;
static sys_isg_db_t database;

static osal_mutex_t  g_isg_sem;
static osal_thread_t g_isg_thread;
static uint8  isDBModify = FALSE;
static uint16 dbTimer = 0;
#define MSG " Type %u, %s, VLAN %u,IP %u.%u.%u.%u, MAC %2x:%2x:%2x:%2x:%2x:%2x, Time %d"
#define DUMP_ENTRY(act, __be)  SYS_DBG(DBG_IPSG, act MSG,                \
            __be->type, LPSTR(__be->port), __be->vid,(__be->ip >> 24) & 0xff,  \
            (__be->ip >> 16) & 0xff,(__be->ip >> 8) & 0xff, (__be->ip >> 0) & 0xff, \
            __be->mac.octet[0]& 0xff, __be->mac.octet[1]& 0xff,__be->mac.octet[2]& 0xff,  \
            __be->mac.octet[3]& 0xff, __be->mac.octet[4]& 0xff, __be->mac.octet[5]& 0xff,   \
            __be->leaseTime);    \

#define ISG_LOCK()    \
do{                     \
    if (SYS_ERR_OK != osal_sem_mutex_take(g_isg_sem, OSAL_SEM_WAIT_FOREVER)) {  \
        SYS_DBG(DBG_IPSG, "IPSG Lock Error");    \
        return SYS_ERR_SEM_LOCK;}           \
}while(0);

#define ISG_UNLOCK()    \
do{                     \
    if (SYS_ERR_OK != osal_sem_mutex_give(g_isg_sem))    \
        SYS_DBG(DBG_IPSG, "IPSG unLock Error");     \
}while(0);
static void _isg_timer_thread(void *pInput)
{
    uint32 i = 0;

    for (;;)
    {
        
        osal_time_usleep(1 * 1000 * 1000);

        for (i = 0 ; i < CAPA_IPSG_ENTRY_NUM; i++ )
        {
            if ((TRUE == g_bindTbl[i].isUsed) && (ISG_ENTRY_DYNAMIC == g_bindTbl[i].type))
            {
                if (g_bindTbl[i].leaseTime > 0)
                    g_bindTbl[i].leaseTime --;
                else if (g_bindTbl[i].leaseTime == 0)
                    isg_bindingEntry_del(&g_bindTbl[i]);
            }
        }

        if (ISG_DB_TYPE_NONE != database.dbType && (TRUE == isDBModify))
        {
            if (0 == --dbTimer)
            {
                KSI_NOTIFY_MSG(KSI_ISG_DB_MODIFY);
                isDBModify = FALSE;
                dbTimer = database.writeDelay;
            }
        }
    }

    osal_thread_exit(0);
}
int32 _isg_allDeny_set(uint32 i, sys_isg_entry_t *pIpFilterEntry, sys_logic_portmask_t pm)
{
    sys_acl_protoType_t blkType = ACL_PROTOTYPE_IPSG;
    sys_acl_entryKey_t key;
    sys_acl_action_t action;
    sys_acl_field_t field;
    sys_logic_port_t lp;

    SYS_PARAM_CHK((i > CAPA_IPSG_ENTRY_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pIpFilterEntry), SYS_ERR_NULL_POINTER);

    SYS_DBG(DBG_IPSG, "update all deny entry %u to pm %x", i, pm.bits[0]);

    SYS_MEM_CLEAR(key);
    SYS_MEM_CLEAR(action);
    SYS_MEM_CLEAR(field);

    key.idx = i + ISG_ACL_KEY_RSV_NUM;
    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_IP4);

    if (IS_LOGIC_PORTMASK_CLEAR(pm))
    {
        SYS_ERR_CHK(rsd_acl_entry_del(blkType, &key));

        return SYS_ERR_OK;
    }

    action.actionMask = ACL_ACTIONMASK_DROP;

    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SPM);
    LOGIC_PORTMASK_COPY(field.spm, pm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, field.spm)
    {
        if (IS_LP_TRKMBR(lp))
        {
            LOGIC_PORTMASK_CLEAR_PORT(field.spm, lp);
        }
    }

    SYS_ERR_CHK(rsd_acl_entry_add(blkType, &key, &action, &field, ENABLED));

    return SYS_ERR_OK;
}
static int32 _isg_entry_set(uint32 entryIdx, sys_isg_entry_t *pIpFilterEntry, sys_isg_verify_type_t verifyType)
{
    sys_acl_protoType_t blkType = ACL_PROTOTYPE_IPSG;
    sys_acl_entryKey_t key;
    sys_acl_action_t action;
    sys_acl_field_t field;

    SYS_PARAM_CHK((entryIdx > CAPA_IPSG_ENTRY_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pIpFilterEntry), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(key);
    SYS_MEM_CLEAR(action);
    SYS_MEM_CLEAR(field);

    key.idx = entryIdx + ISG_ACL_KEY_RSV_NUM;
    action.actionMask = ACL_ACTIONMASK_PERMIT;

    osal_memset(&field, 0, sizeof(sys_acl_field_t));
    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_IP4);
    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SPM);
    LOGIC_PORTMASK_SET_PORT(field.spm, pIpFilterEntry->port);

    if(0 != pIpFilterEntry->ip)
    {
        RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SIP);
        field.sip = pIpFilterEntry->ip;
        field.sipMask = pIpFilterEntry->mask;
    }

    if((ISG_VERIFY_IP_MAC == verifyType) && (TRUE == pIpFilterEntry->isCareMac))
    {
        RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SMAC);
        osal_memcpy(&field.smac, &pIpFilterEntry->mac, CAPA_MAC_BYTE_NUM);
        osal_memset(&field.smacMask, 0xFF, CAPA_MAC_BYTE_NUM);
    }
    else if(ISG_VERIFY_IP == verifyType)
    {
        RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SMAC);
    }

    if(0 != pIpFilterEntry->vid)
    {
        
        RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_VID);
        field.vid = pIpFilterEntry->vid;
    }

    SYS_ERR_CHK(rsd_acl_entry_add(blkType, &key, &action, &field, ENABLED));

    return SYS_ERR_OK;
}
static int32 _isg_entry_del(uint32 entryIdx)
{
    sys_acl_entryKey_t  key;

    SYS_PARAM_CHK((entryIdx > (CAPA_IPSG_ENTRY_NUM - 1)), SYS_ERR_OUT_OF_RANGE);

    osal_memset(&key, 0, sizeof(sys_acl_entryKey_t));
    key.idx = entryIdx + ISG_ACL_KEY_RSV_NUM;

    SYS_ERR_CHK(rsd_acl_entry_del(ACL_PROTOTYPE_IPSG, &key));

    return SYS_ERR_OK;
}

static int32 _isg_bindingEntry_update(sys_logic_port_t port)
{
    uint32 i = 0;

    for (i = 0 ; i < CAPA_IPSG_ENTRY_NUM; i++ )
    {
        if ((TRUE == g_bindTbl[i].isUsed) && (port == g_bindTbl[i].port))
        {
            if (ISG_ENTRY_ALL_DENY == g_bindTbl[i].type)
                break;

            DUMP_ENTRY("Re-configure for verify type", (&g_bindTbl[i]));
            _isg_entry_set(i, &g_bindTbl[i], g_portVerifyType[port]);
        }
    }

    return SYS_ERR_OK;
}
static int32 _isg_rsvDhcpEntry_set(sys_enable_t enable)
{
    sys_acl_protoType_t blkType = ACL_PROTOTYPE_IPSG;
    sys_acl_entryKey_t  key;
    sys_acl_action_t    action;
    sys_acl_field_t     field;

    osal_memset(&key, 0, sizeof(sys_acl_entryKey_t));
    osal_memset(&action, 0, sizeof(sys_acl_action_t));
    osal_memset(&field, 0, sizeof(sys_acl_field_t));

    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_IP4);
    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_IPPROTO);
    field.protol = 0x11;

    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_L4DPORT);
    field.dstPort = 67;

    key.idx = ISG_ACL_KEY_RSV_DHCP_SNP_CLIENT;
    if (ENABLED == enable)
        SYS_ERR_CHK(rsd_acl_entry_add(blkType, &key, &action, &field, ENABLED));
    else
        SYS_ERR_CHK(rsd_acl_entry_del(blkType, &key));

    key.idx = ISG_ACL_KEY_RSV_DHCP_SNP_SERVER;
    field.dstPort = 68;
    if (ENABLED == enable)
        SYS_ERR_CHK(rsd_acl_entry_add(blkType, &key, &action, &field, ENABLED));
    else
        SYS_ERR_CHK(rsd_acl_entry_del(blkType, &key));

    return SYS_ERR_OK;
}

static int32 _isg_acl_entry_dump(sys_acl_entry_t *entry)
{
    SYS_PARAM_CHK((NULL == entry), SYS_ERR_NULL_POINTER);

    if (entry->key.idx == ISG_ACL_KEY_RSV_DHCP_SNP_CLIENT)
        SYS_DBG(DBG_IPSG, "hw-idx %u use for ISG_ACL_KEY_RSV_DHCP_SNP_CLIENT", entry->idx);
    else if (entry->key.idx == ISG_ACL_KEY_RSV_DHCP_SNP_SERVER)
        SYS_DBG(DBG_IPSG, "hw-idx %u use for ISG_ACL_KEY_RSV_DHCP_SNP_SERVER", entry->idx);
    else if (entry->key.idx == (g_maxEntryNum - 1 + ISG_ACL_KEY_RSV_NUM))
        SYS_DBG(DBG_IPSG, "hw-idx %u use for All deny", entry->idx);
    else
        DUMP_ENTRY("acl ", (&g_bindTbl[entry->key.idx - ISG_ACL_KEY_RSV_NUM]));

    return SYS_ERR_OK;
}

static int32 _isg_acl_entry2Str_get(sys_acl_entry_t *entry, char *buf, uint32 maxlen)
{
    sys_isg_entry_t *be = NULL;

    SYS_PARAM_CHK((NULL == entry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == buf), SYS_ERR_NULL_POINTER);

    if (entry->key.idx == ISG_ACL_KEY_RSV_DHCP_SNP_CLIENT)
        osal_sprintf(buf, "ISG_ACL_KEY_RSV_DHCP_SNP_CLIENT");
    else if (entry->key.idx == ISG_ACL_KEY_RSV_DHCP_SNP_SERVER)
        osal_sprintf(buf, "ISG_ACL_KEY_RSV_DHCP_SNP_SERVER");
    else if (entry->key.idx == (g_maxEntryNum - 1 + ISG_ACL_KEY_RSV_NUM))
        osal_sprintf(buf, "All Deny");
    else
    {
        be = &g_bindTbl[entry->key.idx - ISG_ACL_KEY_RSV_NUM];

        osal_sprintf(buf, "Type %u, lp %u, VLAN %u,IP %u.%u.%u.%u, MAC %2x:%2x:%2x:%2x:%2x:%2x",
            be->type, be->port, be->vid,(be->ip >> 24) & 0xff,
            (be->ip >> 16) & 0xff,(be->ip >> 8) & 0xff, (be->ip >> 0) & 0xff,
            be->mac.octet[0]& 0xff, be->mac.octet[1]& 0xff,be->mac.octet[2]& 0xff,
            be->mac.octet[3]& 0xff, be->mac.octet[4]& 0xff, be->mac.octet[5]& 0xff);

        if (osal_strlen(buf) >= maxlen)
            buf[maxlen-1] = '\0';
    }

    return SYS_ERR_OK;
}

static int32 _isg_acl_entryPrio_cmp(sys_acl_entry_t *entry, sys_acl_entry_t *cmp)
{
    SYS_PARAM_CHK((NULL == entry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == cmp), SYS_ERR_NULL_POINTER);

    if (entry->key.idx < cmp->key.idx)
        return 1;

    return 0;
}

static int32 _isg_bindingEntry_del_dyn_by_port(sys_logic_port_t port)
{
	int i;

    ISG_LOCK();

	for (i = 0 ; i < CAPA_IPSG_ENTRY_NUM; i++ )
	{
		if ((TRUE == g_bindTbl[i].isUsed)
			&& (port == g_bindTbl[i].port)
			&& (ISG_ENTRY_DYNAMIC == g_bindTbl[i].type))
		{
				
				osal_printf("%s():%d - port %d ip %x\n", __FUNCTION__, __LINE__, port, g_bindTbl[i].ip);
				_isg_entry_del(i);
				g_portCurrentEntryNum[g_bindTbl[i].port]--;
				osal_memset(&g_bindTbl[i], 0, sizeof(sys_isg_entry_t));

		}
	}

    ISG_UNLOCK();

	return SYS_ERR_OK;
}

static int32 _isg_linkevent_handler(uint32 event, char *pData)
{
	sys_port_event_t *pPortEvent = NULL;

	SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

	pPortEvent = (sys_port_event_t *)pData;

	SYS_DBG(DBG_IPSG, "ISG link event port:%s up %u",  LPSTR(pPortEvent->lport), pPortEvent->new_linkStatus);

	if (PORT_LINKDOWN == pPortEvent->new_linkStatus)
	{
		_isg_bindingEntry_del_dyn_by_port(LP2TRKLP(pPortEvent->lport));
	}

	return SYS_ERR_OK;
}

static int32 _isg_trunkEvent_update(uint32 event, char *pData)
{
    sys_logic_port_t lp;
    sys_trunk_event_t *pTrunkEvent;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pTrunkEvent = (sys_trunk_event_t *)pData;

    ISG_LOCK();

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, pTrunkEvent->portmask)
    {
        _isg_bindingEntry_update(lp);
    }

    lp = TRK2LP(pTrunkEvent->trunkIdx);
    _isg_bindingEntry_update(lp);
    _isg_allDeny_set(g_maxEntryNum-1, &g_bindTbl[g_maxEntryNum-1], g_portEnablePM);

    ISG_UNLOCK();

    return SYS_ERR_OK;
}
int32 isg_bindingMaxEntryNum_get(uint32 *pMaxEntryNum)
{
    *pMaxEntryNum = CAPA_IPSG_ENTRY_NUM;

    return SYS_ERR_OK;
}
int32 isg_port_enable_set(sys_logic_port_t port, sys_enable_t enable, sys_isg_verify_type_t verifyType)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if (ENABLED == enable)
    {
        
        if ((IS_LOGIC_PORTMASK_PORTSET(g_portEnablePM, port)) && (verifyType == g_portVerifyType[port]))
            return SYS_ERR_OK;

        if (IS_LOGIC_PORTMASK_CLEAR(g_portEnablePM))
            _isg_rsvDhcpEntry_set(enable);

        LOGIC_PORTMASK_SET_PORT(g_portEnablePM, port);

        g_portVerifyType[port] = verifyType;

        _isg_bindingEntry_update(port);
    }
    else
    {
        if (IS_LOGIC_PORTMASK_PORTCLEAR(g_portEnablePM, port))
            return SYS_ERR_OK;

        LOGIC_PORTMASK_CLEAR_PORT(g_portEnablePM, port);
        g_portVerifyType[port] = ISG_VERIFY_IP;

        if (IS_LOGIC_PORTMASK_CLEAR(g_portEnablePM))
            _isg_rsvDhcpEntry_set(enable);
    }

    SYS_DBG(DBG_IPSG, "ISG set %s to status %u verify type %u", LPSTR(port), enable, verifyType);

    SYS_ERR_CHK(_isg_allDeny_set(g_maxEntryNum-1, &g_bindTbl[g_maxEntryNum-1], g_portEnablePM));

    return SYS_ERR_OK;
}
int32 isg_port_maxEntryNum_set(sys_logic_port_t port, int32 num)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    g_portMaxEntryNum[port] = num;

    SYS_DBG(DBG_IPSG, "ISG set %s entry num to %d", LPSTR(port), num);

    return SYS_ERR_OK;
}
int32 isg_port_currentEntryNum_get(sys_logic_port_t port, uint32 *pNum)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pNum), SYS_ERR_NULL_POINTER);

    *pNum = g_portCurrentEntryNum[port];

    return SYS_ERR_OK;
}
int32 isg_bindingEntryByIndex_get(uint16 entryIdx, sys_isg_entry_t *pEntry)
{
    SYS_PARAM_CHK((entryIdx >= CAPA_IPSG_ENTRY_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    osal_memcpy(pEntry, &g_bindTbl[entryIdx], sizeof(sys_isg_entry_t));

    return SYS_ERR_OK;
}
int32 isg_bindingEntry_set(sys_isg_entry_t *pEntry, char isFromDhcp)
{
    uint8 macStr[CAPA_MAC_STR_LEN];
    uint32 i = 0;
    sys_isg_entry_t filterEntry;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    ISG_LOCK();

    osal_memset(&filterEntry, 0, sizeof(sys_isg_entry_t));

    for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
    {
        
        if (TRUE == pEntry->isCareMac)
        {
            if ((g_bindTbl[i].vid == pEntry->vid) &&
                (0 == osal_memcmp(&g_bindTbl[i].mac, &pEntry->mac, CAPA_MAC_BYTE_NUM)))
            {
                if ((ISG_ENTRY_STATIC == g_bindTbl[i].type) && (ISG_ENTRY_STATIC != pEntry->type))
                {
                    ISG_UNLOCK();
                    SYS_MEM_CLEAR(macStr);
                    SYS_ERR_CHK(sys_util_mac2Str(pEntry->mac.octet, macStr));
                    SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_BINDING_COLLISION, pEntry->vid, macStr);

                    return SYS_ERR_ISG_SAME_ENTRY;
                }
                else
                    break;
            }
        }
        else 
        {
            if ((g_bindTbl[i].ip == pEntry->ip) && (g_bindTbl[i].mask == pEntry->mask))
                break;
        }
    }

    if ((ISG_ENTRY_DYNAMIC == pEntry->type) && (FALSE == isFromDhcp) && (CAPA_IPSG_ENTRY_NUM != i))
    {
        DUMP_ENTRY("Same entry exist", pEntry);

        ISG_UNLOCK();
        SYS_MEM_CLEAR(macStr);
        SYS_ERR_CHK(sys_util_mac2Str(pEntry->mac.octet, macStr));
        SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_BINDING_COLLISION, pEntry->vid, macStr);

        return SYS_ERR_OK;
    }

    DUMP_ENTRY("insert", pEntry);

    if ((IPSG_ENTRY_UNLIMITED != g_portMaxEntryNum[pEntry->port]) && (g_portCurrentEntryNum[pEntry->port] >= g_portMaxEntryNum[pEntry->port]))
    {
        ISG_UNLOCK();
        SYS_MEM_CLEAR(macStr);
        SYS_ERR_CHK(sys_util_mac2Str(pEntry->mac.octet, macStr));
        SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_ENTRY_FULL, pEntry->vid, macStr);

        return SYS_ERR_ISG_MAX_ENTRY_REACH;
    }

    if (CAPA_IPSG_ENTRY_NUM == i)
    {
        SYS_DBG(DBG_IPSG, "Looking for new entry");

        for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
        {
            if (FALSE == g_bindTbl[i].isUsed)
            {
                g_portCurrentEntryNum[pEntry->port]++;
                break;
            }
        }
    }
    else
    {
        
        if (g_bindTbl[i].port != pEntry->port)
        {
            g_portCurrentEntryNum[g_bindTbl[i].port]--;
            g_portCurrentEntryNum[pEntry->port]++;
        }
    }

    if (CAPA_IPSG_ENTRY_NUM == i)
    {
        ISG_UNLOCK();
        SYS_MEM_CLEAR(macStr);
        SYS_ERR_CHK(sys_util_mac2Str(pEntry->mac.octet, macStr));
        SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_FULL, pEntry->vid, macStr);

        return SYS_ERR_ISG_DB_FULL;
    }

    SYS_DBG(DBG_IPSG, "ISG entry found %u", i);
    g_bindTbl[i].isUsed = TRUE;
    osal_memcpy(&g_bindTbl[i].mac, &pEntry->mac, CAPA_MAC_BYTE_NUM);
    g_bindTbl[i].vid = pEntry->vid;
    g_bindTbl[i].port = pEntry->port;
    g_bindTbl[i].ip = pEntry->ip;
    g_bindTbl[i].mask = pEntry->mask;
    g_bindTbl[i].type = pEntry->type;
    g_bindTbl[i].leaseTime= pEntry->leaseTime;
    g_bindTbl[i].isCareMac = pEntry->isCareMac;

    _isg_entry_set(i, &g_bindTbl[i], g_portVerifyType[g_bindTbl[i].port]);

    if ((ISG_ENTRY_DYNAMIC == g_bindTbl[i].type) && (TRUE == isFromDhcp))
        isDBModify = TRUE;

    ISG_UNLOCK();

    return SYS_ERR_OK;
}
int32 isg_bindingEntry_del(sys_isg_entry_t *pEntry)
{
    uint32 i = 0;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    DUMP_ENTRY("Remove", pEntry);

    ISG_LOCK();

    for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
    {
        if (TRUE == g_bindTbl[i].isUsed)
        {
            if ((g_bindTbl[i].ip == pEntry->ip) && (g_bindTbl[i].vid == pEntry->vid)
                && (0 == osal_memcmp(&g_bindTbl[i].mac, &pEntry->mac, CAPA_MAC_BYTE_NUM)))
            {
                if (pEntry->type != g_bindTbl[i].type)
                {
                    ISG_UNLOCK();
                    return SYS_ERR_ENTRY_NOT_FOUND;
                }
                else
                    break;
            }
        }
    }

    if (i != CAPA_IPSG_ENTRY_NUM)
    {
        if (ISG_ENTRY_DYNAMIC == g_bindTbl[i].type)
            isDBModify = TRUE;

        _isg_entry_del(i);
        g_portCurrentEntryNum[g_bindTbl[i].port]--;
        osal_memset(&g_bindTbl[i], 0, sizeof(sys_isg_entry_t));

        ISG_UNLOCK();

        return SYS_ERR_OK;
    }

    ISG_UNLOCK();

    return SYS_ERR_ENTRY_NOT_FOUND;
}
int32 isg_bindingEntryDhcp_del(sys_isg_entry_filter_del_t filterType, uint32 val)
{
    uint16 i = 0;
    sys_isg_entry_t *pEntry = NULL;

    SYS_DBG(DBG_IPSG, "delete type %u val %u", filterType, val);

    ISG_LOCK();

    for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
    {
        pEntry = &g_bindTbl[i];

        if ((FALSE == pEntry->isUsed) || (ISG_ENTRY_DYNAMIC != pEntry->type))
            continue;

        if ((filterType == ISG_ENTRY_DEL_ALL) ||
            ((filterType == ISG_ENTRY_DEL_VID) && (pEntry->vid == val)) ||
            ((filterType == ISG_ENTRY_DEL_IP) && (pEntry->ip == val)) ||
            ((filterType == ISG_ENTRY_DEL_PORT) && (pEntry->port == val)))
        {
            
            DUMP_ENTRY("Remove", pEntry);
            SYS_ERR_CHK(_isg_entry_del(i));
            g_portCurrentEntryNum[pEntry->port]--;
            osal_memset(pEntry, 0, sizeof(sys_isg_entry_t));
            isDBModify = TRUE;
        }
    }

    ISG_UNLOCK();

    return SYS_ERR_OK;
}
int32 isg_bindingEntry_exist(sys_isg_entry_t *pEntry, uint8 isCarePort)
{
    uint32 i = 0;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    DUMP_ENTRY("Check exist", pEntry);

    for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
    {
        if (TRUE == g_bindTbl[i].isUsed)
        {
            if ((g_bindTbl[i].ip == pEntry->ip) && (g_bindTbl[i].vid == pEntry->vid)
                && (0 == osal_memcmp(&g_bindTbl[i].mac, &pEntry->mac, CAPA_MAC_BYTE_NUM)))
            {
                if (TRUE == isCarePort)
                {
                    if (g_bindTbl[i].port != pEntry->port)
                        return SYS_ERR_FAILED;
                    else
                        return SYS_ERR_OK;
                }
                else
                    return SYS_ERR_OK;
            }
        }
    }

    return SYS_ERR_ISG_ENTRY_NOT_EXIST;
}
int32 isg_db_set(sys_isg_db_t *pDB)
{
    SYS_PARAM_CHK((NULL == pDB), SYS_ERR_NULL_POINTER);

    database.dbType = pDB->dbType;
    database.timeout = pDB->timeout;
    database.writeDelay = pDB->writeDelay;
    dbTimer = pDB->writeDelay;

    return SYS_ERR_OK;
}
int32 isg_db_timer_set(uint32 timer)
{
    if (0 == timer)
    {
            dbTimer = database.writeDelay;
            isDBModify = FALSE;
    }
    else
    {
        dbTimer = timer;
        isDBModify = TRUE;
    }

    return SYS_ERR_OK;
}
int32 isg_db_timer_get(uint32 *pTimer)
{
    SYS_PARAM_CHK((NULL == pTimer), SYS_ERR_NULL_POINTER);

    *pTimer = dbTimer;

    return SYS_ERR_OK;
}
int32 isg_init(void)
{
    sys_acl_cb_t cb_info;

    SYS_MEM_CLEAR(g_portMaxEntryNum);
    SYS_MEM_CLEAR(g_portCurrentEntryNum);
    SYS_MEM_CLEAR(g_portEnablePM);
    SYS_MEM_CLEAR(g_portVerifyType);
    SYS_MEM_CLEAR(zeroMac);
    SYS_MEM_CLEAR(database);
    SYS_MEM_CLEAR(cb_info);

    g_bindTbl = osal_alloc(CAPA_IPSG_ENTRY_NUM * sizeof(sys_isg_entry_t));
    osal_memset(g_bindTbl, 0, CAPA_IPSG_ENTRY_NUM * sizeof(sys_isg_entry_t));

    g_isg_sem = osal_sem_mutex_create();
    g_isg_thread = osal_thread_create("ISG Thread", 32768, DFLT_THREAD_PRI_IPSG_TIMER, (void *)_isg_timer_thread, NULL);

    if ((osal_thread_t)NULL == g_isg_thread)
        return SYS_ERR_FAILED;

    SYS_ERR_CHK(isg_bindingMaxEntryNum_get(&g_maxEntryNum));

    SYS_DBG(DBG_IPSG, "max num %u", g_maxEntryNum);

    g_bindTbl[g_maxEntryNum -1].type = ISG_ENTRY_ALL_DENY;
    g_bindTbl[g_maxEntryNum -1].isUsed = TRUE;

    cb_info.entry_info_dump = _isg_acl_entry_dump;
    cb_info.entry_str_get = _isg_acl_entry2Str_get;
    cb_info.entry_prio_cmp = _isg_acl_entryPrio_cmp;
    SYS_ERR_CHK(rsd_acl_cb_register(ACL_PROTOTYPE_IPSG, &cb_info));

	SYS_ERR_CHK(sys_notifier_observer_register("ipsg", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG, _isg_linkevent_handler, SYS_NOTIFIER_PRI_MEDIUM));

    SYS_ERR_CHK(sys_notifier_observer_register("ipsg", SYS_NOTIFIER_SUBJECT_TRUNK,
            (SYS_NT_TRK_ACT_MBR_JOIN | SYS_NT_TRK_ACT_MBR_LEAVE | SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE),
            _isg_trunkEvent_update, SYS_NOTIFIER_PRI_MEDIUM));

    return SYS_ERR_OK;
}
int32 isg_exit(void)
{
    osal_sem_mutex_destroy(g_isg_sem);

    if ((osal_thread_t)NULL != g_isg_thread)
        osal_thread_destroy(g_isg_thread);

    return SYS_ERR_OK;
}

