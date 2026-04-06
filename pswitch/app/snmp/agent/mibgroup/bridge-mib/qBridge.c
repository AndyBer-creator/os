
/*
* Copyright (C) 2010 Realtek Semiconductor Corp.
* All Rights Reserved.
*
* This program is the proprietary software of Realtek Semiconductor
* Corporation and/or its licensors, and only be used, duplicated,
* modified or distributed under the authorized license from Realtek.
*
* ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
* THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
*
* $Revision: 10018 $
* $Date: 2010-06-04 13:57:15 +0800 (Fri, 04 Jun 2010) $
*
* Purpose : Export the public APIs in lower layer module in the Realtek Protocols.
*
* Feature : Export the public APIs in lower layer module
*
*/
/*
* Include Files
*/
#include "qBridge.h"

/*
* Symbol Definition
*/

static fdb_entry_t *fdbList = NULL, *fdbScanPtr = NULL;

static tpfdb_entry_t *tpfdbList = NULL, *tpfdbScanPtr = NULL, *tpfdbTail = NULL;
static tpfdb_entry_t *staticUcastList = NULL, *staticUcastScanPtr = NULL;

static tpgroup_entry_t *mcastList = NULL, *mcastScanPtr = NULL;
static tpgroup_entry_t *stGroupList = NULL, *stGroupScanPtr = NULL;

static fwdAll_entry_t *fwdList = NULL;

static vlan_cuurent_entry_t *curVlanList = NULL, *curVlanScanPtr = NULL;
static vlan_static_entry_t *stVlanList = NULL, *stVlanScanPtr = NULL;

static portVlan_entry_t *portVlanList = NULL;

uint32 vlanMac[DFLT_VLAN_RESERVED_VLAN];

int32 qBridge_vlanMacNum_get(sys_vlanmask_t *vmlist)
{
    uint32 table_size = 0;
    uint32 i;
    sys_l2_entry_t *pEntry = NULL;
    char zeroMac[6] = {0,0,0,0,0,0};
    sys_logic_port_t port;
    uint32 numMac=0;
    sys_vlanmask_t tmpVmlist;
    uint8 isFound;
    sys_mac_t cpuMac;
    sys_vid_t vid;
    fdb_entry_t  fdbEntry;

    osal_memset(vlanMac, 0, sizeof(vlanMac));
    osal_memset(&tmpVmlist,0,sizeof(sys_vlanmask_t));
    sal_l2_sort_start(SYS_L2_SORTMETH_VLANMAC, &table_size);
    sal_sys_macAddr_get(&cpuMac);

    for(i = 0; i < table_size; i++)
    {
        pEntry = L2_SORT_ENTRY_PTR(i);

        if (0 == pEntry->vid)
            continue;

        if (SYS_L2_TYPE_STATIC == pEntry->type)
            continue;

        if (0 == osal_memcmp(&pEntry->mac, zeroMac, CAPA_MAC_BYTE_NUM))
            continue;

        if (0 == osal_memcmp(&pEntry->mac, cpuMac.octet, CAPA_MAC_BYTE_NUM))
            continue;

        vlanMac[pEntry->vid]++;
        if (!VLANMASK_IS_VIDSET(tmpVmlist, pEntry->vid))
            VLANMASK_SET_VID(tmpVmlist, pEntry->vid);
    }
    VLANMASK_COPY(*vmlist, tmpVmlist);

    FOR_EACH_VID_IN_VLANMASK(vid, tmpVmlist)
    {
        SYS_MEM_CLEAR(fdbEntry);
        fdbEntry.vlanId = vid;
        fdbEntry.dynCnt = vlanMac[vid];
        qBridge_fdb_creatEntry(&fdbEntry);
    }

    return SYS_ERR_OK;
}

void qBridge_fdb_removeAll(void)
{
    fdb_entry_t *pTmp = NULL;
    fdb_entry_t *prev = NULL;
    pTmp = fdbList;
    while (pTmp)
    {
        prev = pTmp;
        pTmp = pTmp->next;
        SNMP_FREE(prev);
    }
    fdbList = NULL;
}

fdb_entry_t *qbridge_fdb_getFirst(void)
{
    sys_vlanmask_t vlanMask;
    static u_long startTime = 0;
    u_long getTime = 0, interval = 0;

    osal_time_sysUptime_get(&getTime);
    interval = getTime - startTime;
    if ((getTime - startTime) >= DOT1D_ADDR_TABLE_UPDATA_TIMER *  SEC_TO_TENMSEC)
    {
        qBridge_fdb_removeAll();
        qBridge_vlanMacNum_get(&vlanMask);
    }
    startTime = getTime;

    return fdbList;
}

fdb_entry_t *qbridge_fdb_getNext(fdb_entry_t *preEntry)
{
    if (preEntry == NULL)
        return NULL;

    fdb_entry_t *pTmp = fdbList;

    while (pTmp)
    {
        if (pTmp->vlanId == preEntry->vlanId)
            return pTmp->next;

        pTmp = pTmp->next;
    }

    return NULL;
}


int32 qBridge_fdb_creatEntry(fdb_entry_t *pEntry)
{
    fdb_entry_t *entry;
    fdb_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(fdb_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    osal_memcpy(entry, pEntry,sizeof(fdb_entry_t));
    entry->next = NULL;

    if (fdbList == NULL)
    {
        fdbList = entry;
    }
    else
    {
        pTmp = fdbList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
    }

    return SYS_ERR_OK;
}


int32 qBridge_tpfdb_creatEntry(tpfdb_entry_t *pEntry)
{
    tpfdb_entry_t *entry;
    tpfdb_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(tpfdb_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    osal_memcpy(entry, pEntry,sizeof(tpfdb_entry_t));
    entry->next = NULL;

    if (tpfdbList == NULL)
    {
        tpfdbList = entry;
        tpfdbTail = entry;
    }
    else
    {
        #if 0
        pTmp = tpfdbList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
        #endif

        tpfdbTail->next = entry;
        tpfdbTail = entry;
    }

    return SYS_ERR_OK;
}


void qBridge_tpfdb_removeAll(void)
{
    tpfdb_entry_t *pTmp = NULL;
    tpfdb_entry_t *prev = NULL;
    pTmp = tpfdbList;
    while (pTmp)
    {
        prev = pTmp;
        pTmp = pTmp->next;
        SNMP_FREE(prev);
    }
    tpfdbList = NULL;
}

uint8 is_tpfdbMacExist(char *pMac)
{
    tpfdb_entry_t *pTmp;

    if (NULL == pMac)
        return FALSE;

    pTmp = tpfdbList;
    while(pTmp)
    {
        if (0 == osal_memcmp(pMac,pTmp->mac, CAPA_MAC_BYTE_NUM))
            return TRUE;

        pTmp = pTmp->next;
    }
}

int32 qbridge_tpfdb_entry_get(uint32 *pEntryNum, uint8 isRmSameMac)
{
    sys_mac_t cpuMac;
    uint32 table_size = 0;
    uint32 i;
    sys_l2_entry_t *pEntry = NULL;
    char zeroMac[6] = {0,0,0,0,0,0};
    tpfdb_entry_t tpfdb;
    sys_logic_port_t port;
    uint8 IsCpuMac = 0;
    uint32 numMac=0;

    SYS_PARAM_CHK((NULL == pEntryNum), SYS_ERR_NULL_POINTER);
    *pEntryNum = 0;

    qBridge_tpfdb_removeAll();

    /*get cpu mac address here*/
    sal_sys_macAddr_get(&cpuMac);
    #if 1
    sal_l2_sort_start(SYS_L2_SORTMETH_VLANMAC, &table_size);
    #else
    table_size = 1*1024 ;
    #endif

    for(i = 0; i < table_size; i++)
    {
        #if 1
        pEntry = L2_SORT_ENTRY_PTR(i);
        #else
        macEntry.vid = 1;
        macEntry.type = MAC_UNICAST;
        macEntry.mac[1] = (i+1)/256;
        macEntry.mac[0] = (i+1)%256;
        #endif

       // snmp_log(LOG_DEBUG, "mac: 0x%x:%x \n", macEntry.mac[1], macEntry.mac[0]);

        if( (0 != pEntry->vid)    &&
            (memcmp(&pEntry->mac, zeroMac, CAPA_MAC_BYTE_NUM) != 0))
        {
            osal_memset(&tpfdb,0,sizeof(tpfdb));
            tpfdb.vlanId = pEntry->vid;

            if(memcmp(&pEntry->mac , cpuMac.octet, CAPA_MAC_BYTE_NUM) == 0)
                IsCpuMac = 1;
            else
                IsCpuMac = 0;

            tpfdb.port = LP2UP(pEntry->port);

            osal_memcpy(tpfdb.mac, &pEntry->mac, CAPA_MAC_BYTE_NUM);

            if(IsCpuMac)
                tpfdb.status = DOT1D_STATUS_SELF;
            else if(SYS_L2_TYPE_SUSPEND == pEntry->type)
                tpfdb.status = DOT1D_STATUS_INVALID;
            else if(SYS_L2_TYPE_DYNAMIC == pEntry->type)
                tpfdb.status = DOT1D_STATUS_LEARNED;
            else if(SYS_L2_TYPE_STATIC == pEntry->type)
                tpfdb.status = DOT1D_STATUS_MGMT;
            else
                tpfdb.status = DOT1D_STATUS_OTHERS;

            if (isRmSameMac)
            {
                if (is_tpfdbMacExist(tpfdb.mac))
                {
                    SYS_ERR_CONTINUE(qBridge_tpfdb_creatEntry(&tpfdb));
                    numMac++;
                }
            }
            else
            {
                SYS_ERR_CONTINUE(qBridge_tpfdb_creatEntry(&tpfdb));
                numMac++;
            }

        }
    }

    *pEntryNum = numMac;

    return SYS_ERR_OK;
}


/*function define*/

void qbridge_tpfdb_scanAccessInit(void)
{
    tpfdbScanPtr  = tpfdbList;
}

tpfdb_entry_t *qbridge_tpfdb_scanFirst(void)
{
    return tpfdbList;
}

tpfdb_entry_t *qbridge_tpfdb_scanAccessNext(void)
{
    tpfdb_entry_t *returnval = tpfdbScanPtr;
    if (tpfdbScanPtr)
        tpfdbScanPtr  = tpfdbScanPtr->next;
    return returnval;
}

tpfdb_entry_t *qbridge_getTpfdbEntry_byMac(char *pMac)
{
    if (NULL == pMac)
        return NULL;

    tpfdbScanPtr  = tpfdbList;
    while(tpfdbScanPtr)
    {
        if (0 == osal_memcmp(pMac,tpfdbScanPtr->mac, CAPA_MAC_BYTE_NUM))
            return tpfdbScanPtr;

        tpfdbScanPtr = tpfdbScanPtr->next;
    }

    return NULL;
}

tpfdb_entry_t *qbridge_getTpfdbEntry(uint32 vlanId, char *pMac)
{
    if (NULL == pMac)
        return NULL;

    tpfdbScanPtr  = tpfdbList;
    while(tpfdbScanPtr)
    {
        if ((vlanId == tpfdbScanPtr->vlanId) &&
            (0 == osal_memcmp(pMac,tpfdbScanPtr->mac, CAPA_MAC_BYTE_NUM)) )
            return tpfdbScanPtr;

        tpfdbScanPtr = tpfdbScanPtr->next;
    }

    return NULL;
}

tpfdb_entry_t *qbridge_tpfdb_getFirst(void)
{
    uint32 entryNum;
    static u_long startTime = 0;
    u_long getTime = 0, interval = 0;

    osal_time_sysUptime_get(&getTime);
    interval = getTime - startTime;
    if ((getTime - startTime) >= DOT1D_ADDR_TABLE_UPDATA_TIMER *  SEC_TO_TENMSEC)
    {
        qbridge_tpfdb_entry_get(&entryNum, 1);
    }
    startTime = getTime;

    return qbridge_tpfdb_scanFirst();
}

tpfdb_entry_t *qbridge_tpfdb_getNext(tpfdb_entry_t *preEntry)
{
    tpfdb_entry_t *pTmp;

    if (preEntry == NULL)
        return NULL;

    pTmp = qbridge_tpfdb_scanFirst();
    while (pTmp)
    {
        if ((pTmp->vlanId == preEntry->vlanId) && (0 == osal_memcmp(pTmp->mac, preEntry->mac,6)))
            return pTmp->next;

        pTmp = pTmp->next;
    }

    return NULL;
}

/*dot1qTpGroupTable*/
int32 qBridge_group_creatEntry(tpgroup_entry_t *pEntry)
{
    tpgroup_entry_t *entry;
    tpgroup_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(tpgroup_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    osal_memcpy(entry, pEntry, sizeof(tpgroup_entry_t));
    entry->next = NULL;

    if (mcastList == NULL)
        mcastList = entry;
    else
    {
        pTmp = mcastList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
    }

    return SYS_ERR_OK;
}


void qBridge_group_removeAll(void)
{
    tpgroup_entry_t *pTmp = NULL;
    tpgroup_entry_t *prev = NULL;
    pTmp = mcastList;
    while (pTmp)
    {
        prev = pTmp;
        pTmp = pTmp->next;
        SNMP_FREE(prev);
    }
    mcastList = NULL;
}



void qbridge_group_scanAccessInit(void)
{
    mcastScanPtr = mcastList;
}

tpgroup_entry_t *qbridge_group_scanFirst(void)
{
    return mcastList;
}

tpgroup_entry_t *qbridge_group_scanAccessNext(void)
{
    tpgroup_entry_t *returnval = mcastScanPtr;
    if (mcastScanPtr)
        mcastScanPtr  = mcastScanPtr->next;
        return returnval;
}

uint8 is_groupExist(uint32 vlanId, char *pMac)
{
    tpgroup_entry_t *pTmp;

    if (NULL == pMac)
        return FALSE;

    pTmp = mcastList;
    while(pTmp)
    {
        if ((vlanId == pTmp->vlanId) &&
            (0 == osal_memcmp(pMac,pTmp->mac, CAPA_MAC_BYTE_NUM)) )
            return TRUE;

        pTmp = pTmp->next;
    }

    return FALSE;
}



tpgroup_entry_t *qbridge_getGroupEntry(uint32 vlanId, char *pMac)
{
    if (NULL == pMac)
        return NULL;

    mcastScanPtr  = mcastList;
    while(mcastScanPtr)
    {
        if ((vlanId == mcastScanPtr->vlanId) &&
            (0 == osal_memcmp(pMac,mcastScanPtr->mac, CAPA_MAC_BYTE_NUM)) )
            return mcastScanPtr;

        mcastScanPtr = mcastScanPtr->next;
    }

    return NULL;
}


int32 qbridge_group_entry_get(uint32 *pEntryNum)
{
    uint32 dbSize;
    uint32 i;
    sys_igmp_db_t db;
    tpgroup_entry_t mcast;
    tpgroup_entry_t *pMcast = NULL;
    igmp_lookup_mode_t igmpMode = IGMP_DMAC_VID_MODE, mldMode =IGMP_DMAC_VID_MODE;

    uint32 numGroup=0;

    SYS_PARAM_CHK((NULL == pEntryNum), SYS_ERR_NULL_POINTER);
    *pEntryNum = 0;

    qBridge_group_removeAll();

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    sal_igmp_lookupMode_get(&igmpMode);
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    sal_mld_lookupMode_get(&mldMode);
    #endif

    if (IGMP_DMAC_VID_MODE != igmpMode && IGMP_DMAC_VID_MODE != mldMode)
    {
        *pEntryNum = 0;
        return SYS_ERR_OK;
    }

    if (SYS_ERR_OK != sal_mcast_databaseSize_get(&dbSize))
    {
        *pEntryNum = 0;
        return SYS_ERR_FAILED;
    }

    for (i = 0; i < dbSize; i++)
    {
        osal_memset(&db,0,sizeof(sys_igmp_db_t));
        db.indx = i;
        SYS_ERR_CONTINUE(sal_mcast_database_get(&db));

        if(0 == db.dip)
            continue;

        if (MULTICAST_TYPE_IPV4 == db.ipType)
        {
            if (IGMP_DMAC_VID_MODE != igmpMode)
                continue;
        }
        else
        {
            if (IGMP_DMAC_VID_MODE != mldMode)
                continue;
        }

        if (!is_groupExist(db.vid, db.mac))
        {
            osal_memset(&mcast,0,sizeof(tpgroup_entry_t));
            mcast.vlanId = (uint32)db.vid;
            osal_memcpy(mcast.mac, db.mac, CAPA_MAC_BYTE_NUM);
            //LOGIC_PORTMASK_COPY(mcast.stMbr, db.staticInMbr);
            //LOGIC_PORTMASK_ANDNOT(mcast.learnMbr, db.portmask, db.staticInMbr);

            LOGIC_PORTMASK_COPY(mcast.mbr, db.portmask);
            if (IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            {
                LOGIC_PORTMASK_COPY(mcast.learnMbr, db.portmask);
                LPM2UPM(&mcast.learnMbr, &mcast.upmsk_dyn);
                userPortMask_reverse(&mcast.upmsk_dyn,SYS_USER_PORTMASK_LEN);
                osal_memcpy(&mcast.upmsk_mbr, &mcast.upmsk_dyn, SYS_USER_PORTMASK_LEN);
            }
            else
            {
                LOGIC_PORTMASK_COPY(mcast.stMbr, db.portmask);
                LPM2UPM(&mcast.stMbr, &mcast.upmsk_st);
                userPortMask_reverse(&mcast.upmsk_st,SYS_USER_PORTMASK_LEN);
                osal_memcpy(&mcast.upmsk_mbr, &mcast.upmsk_st, SYS_USER_PORTMASK_LEN);
            }
            SYS_ERR_CONTINUE(qBridge_group_creatEntry(&mcast));
                numGroup++;
        }
        else
        {
            pMcast = qbridge_getGroupEntry(db.vid, db.mac);
            if (pMcast != NULL)
            {
                LOGIC_PORTMASK_OR(pMcast->mbr, pMcast->mbr, db.portmask);
                LPM2UPM(&pMcast->learnMbr, &pMcast->mbr);
                userPortMask_reverse(&pMcast->upmsk_mbr,SYS_USER_PORTMASK_LEN);

                if (IGMP_GROUP_ENTRY_DYNAMIC == db.form)
                {
                    LOGIC_PORTMASK_OR(pMcast->learnMbr, pMcast->learnMbr, db.portmask);
                    LPM2UPM(&pMcast->learnMbr, &pMcast->upmsk_dyn);
                    userPortMask_reverse(&pMcast->upmsk_dyn,SYS_USER_PORTMASK_LEN);
                }
                else
                {
                    LOGIC_PORTMASK_OR(pMcast->stMbr, pMcast->stMbr, db.portmask);
                    LPM2UPM(&pMcast->stMbr, &pMcast->upmsk_st);
                    userPortMask_reverse(&pMcast->upmsk_st,SYS_USER_PORTMASK_LEN);
                }
            }
        }
    }
#endif
    *pEntryNum = numGroup;

    return SYS_ERR_OK;
}

tpgroup_entry_t *qbridge_group_getFirst(void)
{
    uint32 entryNum;
    static u_long startTime = 0;
    u_long getTime = 0, interval = 0;

    osal_time_sysUptime_get(&getTime);
    interval = getTime - startTime;
    if ((getTime - startTime) >= RECREATE_TABLE_UPDATA_TIMER *  SEC_TO_TENMSEC)
    {
        qbridge_group_entry_get(&entryNum);
    }
    startTime = getTime;

    return qbridge_group_scanFirst();
}

tpgroup_entry_t *qbridge_group_getNext(tpgroup_entry_t *preEntry)
{
    tpgroup_entry_t *pTmp;

    if (preEntry == NULL)
        return NULL;

    pTmp = qbridge_group_scanFirst();
    while (pTmp)
    {
        if ((pTmp->vlanId == preEntry->vlanId) && (0 == osal_memcmp(pTmp->mac, preEntry->mac,6)))
            return pTmp->next;

        pTmp = pTmp->next;
    }

    return NULL;
}


/*dot1qStaticUnicastTable*/
int32 qBridge_staticUcast_creatEntry(tpfdb_entry_t*pEntry)
{
    tpfdb_entry_t *entry;
    tpfdb_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(tpfdb_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    osal_memcpy(entry, pEntry, sizeof(tpfdb_entry_t));
    entry->next = NULL;

    if (staticUcastList == NULL)
        staticUcastList = entry;
    else
    {
        pTmp = staticUcastList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
    }

    return SYS_ERR_OK;
}



void qBridge_staticUcast_removeAll(void)
{
    tpfdb_entry_t *pTmp = NULL;
    tpfdb_entry_t *prev = NULL;
    pTmp = staticUcastList;
    while (pTmp)
    {
        prev = pTmp;
        pTmp = pTmp->next;
        SNMP_FREE(prev);
    }
    staticUcastList = NULL;
}


void qbridge_staticUcast_scanAccessInit(void)
{
    staticUcastScanPtr  = staticUcastList;
}

tpfdb_entry_t *qbridge_staticUcast_scanFirst(void)
{
    return staticUcastList;
}

tpfdb_entry_t *qbridge_staticUcast_scanAccessNext(void)
{
    tpfdb_entry_t *returnval = staticUcastScanPtr;
    if (staticUcastScanPtr)
        staticUcastScanPtr  = staticUcastScanPtr->next;
    return returnval;
}

tpfdb_entry_t *qbridge_getStaticUcastEntry(uint32 vlanId, char *pMac)
{
   if (NULL == pMac)
        return NULL;

    staticUcastScanPtr  = staticUcastList;
    while(staticUcastScanPtr)
    {
        if ((vlanId == staticUcastScanPtr->vlanId) &&
            (0 == osal_memcmp(pMac,staticUcastScanPtr->mac, CAPA_MAC_BYTE_NUM)) )
            return staticUcastScanPtr;

        staticUcastScanPtr = staticUcastScanPtr->next;
    }

    return NULL;
}

tpfdb_entry_t *qbridge_getStaticUcastEntry_byMac(char *pMac)
{
   if (NULL == pMac)
        return NULL;

    staticUcastScanPtr  = staticUcastList;
    while(staticUcastScanPtr)
    {
        if ((0 == osal_memcmp(pMac,staticUcastScanPtr->mac, CAPA_MAC_BYTE_NUM)) )
            return staticUcastScanPtr;

        staticUcastScanPtr = staticUcastScanPtr->next;
    }

    return NULL;
}



int32 qbridge_staticUnicast_entry_get(uint32 *pEntryNum)
{
    uint32 i;
    tpfdb_entry_t tpfdb;
    sys_logic_port_t port;
    sys_l2_entry_t *pPrev = NULL;
    sys_l2_entry_t *pCurr = NULL;

    uint32 numMac =0;
    uint8 isFindPort;

    SYS_PARAM_CHK((NULL == pEntryNum), SYS_ERR_NULL_POINTER);
    *pEntryNum = 0;

    qBridge_staticUcast_removeAll();

    while (NULL != (pCurr = sal_l2_staticMacNext_ret(pPrev)))
    {
        osal_memset(&tpfdb,0,sizeof(tpfdb));
        tpfdb.vlanId = pCurr->vid;
        osal_memcpy(tpfdb.mac, &pCurr->mac,CAPA_MAC_BYTE_NUM);

        tpfdb.port = pCurr->port;

        tpfdb.status =  3;

        #if 0
        if (SYS_L2_STATIC_MODE_PERMANENT & pCurr->staticMode)
        {
            tpfdb.status =  3;
        }
        else if (RSD_L2_UCAST_MODE_REBOOT & macEntry.mode)
        {
            tpfdb.status =  4;
        }
        else if (RSD_L2_UCAST_MODE_TIMEOUT & macEntry.mode)
        {
            tpfdb.status =  5;
        }
        else if (RSD_L2_UCAST_MODE_SECURE & macEntry.mode)
        {
            tpfdb.status =  1;
        }
        else
            continue;
        #endif

        pPrev = pCurr;

        SYS_ERR_CONTINUE(qBridge_staticUcast_creatEntry(&tpfdb));
        numMac++;
    }

#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    pPrev = NULL;
    while (NULL != (pCurr = sal_l2_blacklistMacNext_ret(pPrev)))
    {
        tpfdb.vlanId = pCurr->vid;
        osal_memcpy(tpfdb.mac, &pCurr->mac,CAPA_MAC_BYTE_NUM);
        tpfdb.status =  1;
        tpfdb.port = 0;

        SYS_ERR_CONTINUE(qBridge_staticUcast_creatEntry(&tpfdb));
        numMac++;

        pPrev = pCurr;
    }
#endif
    *pEntryNum = numMac;

    return SYS_ERR_OK;
}

tpfdb_entry_t *qbridge_staticUcast_getFirst(void)
{
    uint32 entryNum;
    static u_long startTime = 0;
    u_long getTime = 0, interval = 0;

    osal_time_sysUptime_get(&getTime);
    interval = getTime - startTime;
    if ((getTime - startTime) >= RECREATE_TABLE_UPDATA_TIMER *  SEC_TO_TENMSEC)
    {
        qbridge_staticUnicast_entry_get(&entryNum);
    }
    startTime = getTime;

    return qbridge_staticUcast_scanFirst();
}

tpfdb_entry_t *qbridge_staticUcast_getNext(tpfdb_entry_t *preEntry)
{
    tpfdb_entry_t *pTmp;

    if (preEntry == NULL)
        return NULL;

    pTmp = qbridge_staticUcast_scanFirst();
    while (pTmp)
    {
         if ((pTmp->vlanId == preEntry->vlanId) &&
            (0 == osal_memcmp(pTmp->mac, preEntry->mac, CAPA_MAC_BYTE_NUM)) )
            return pTmp->next;

        pTmp = pTmp->next;
    }

    return NULL;
}

/*dot1qStaticMulticastTable*/

int32 qBridge_stGroup_creatEntry(tpgroup_entry_t *pEntry)
{
    tpgroup_entry_t *entry;
    tpgroup_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(tpgroup_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    osal_memcpy(entry, pEntry, sizeof(tpgroup_entry_t));
    entry->next = NULL;

    if (stGroupList == NULL)
        stGroupList = entry;
    else
    {
        pTmp = stGroupList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
    }

    return SYS_ERR_OK;
}


void qBridge_stGroup_removeAll(void)
{
    tpgroup_entry_t *pTmp = NULL;
    tpgroup_entry_t *prev = NULL;
    pTmp = stGroupList;
    while (pTmp)
    {
        prev = pTmp;
        pTmp = pTmp->next;
        SNMP_FREE(prev);
    }
    stGroupList = NULL;
}


void qbridge_stGroup_scanAccessInit(void)
{
    stGroupScanPtr = stGroupList;
}

tpgroup_entry_t *qbridge_stGroup_scanFirst(void)
{
    return stGroupList;
}

tpgroup_entry_t *qbridge_stGroup_scanAccessNext(void)
{
    tpgroup_entry_t *returnval = stGroupScanPtr;
    if (stGroupScanPtr)
        stGroupScanPtr  = stGroupScanPtr->next;
    return returnval;
}

tpgroup_entry_t *qbridge_getStGroupEntry(uint32 vlanId, char *pMac)
{
    if (NULL == pMac)
        return NULL;

    stGroupScanPtr  = stGroupList;
    while(stGroupScanPtr)
    {
        if ((vlanId == stGroupScanPtr->vlanId) &&
            (0 == osal_memcmp(pMac,stGroupScanPtr->mac, CAPA_MAC_BYTE_NUM)) )
            return stGroupScanPtr;

        stGroupScanPtr = stGroupScanPtr->next;
    }

    return NULL;
}


uint8 is_stGroupExist(uint32 vlanId, char *pMac)
{
    tpgroup_entry_t *pTmp;

    if (NULL == pMac)
        return FALSE;

    pTmp = stGroupList;
    while(pTmp)
    {
        if ((vlanId == pTmp->vlanId) &&
            (0 == osal_memcmp(pMac,pTmp->mac, CAPA_MAC_BYTE_NUM)) )
            return TRUE;

        pTmp = pTmp->next;
    }

    return FALSE;
}

int32 qbridge_stGroup_entry_get(uint32 *pEntryNum)
{
    uint32 dbSize;
    uint32 i;
    sys_igmp_db_t db;
    tpgroup_entry_t stGroup;
    tpgroup_entry_t *pStGrp = NULL;
    uint32 numGroup=0;

    SYS_PARAM_CHK((NULL == pEntryNum), SYS_ERR_NULL_POINTER);
    *pEntryNum = 0;

   // if (stGroupList)
    qBridge_stGroup_removeAll();
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));

    for (i = 0; i < dbSize; i++)
    {
        osal_memset(&db,0,sizeof(sys_igmp_db_t));
        db.indx = i;
        SYS_ERR_CHK(sal_mcast_database_get(&db));

        if(0 == db.dip)
            continue;

        if (IGMP_GROUP_ENTRY_STATIC != db.form)
            continue;

        if(!is_stGroupExist(db.vid, db.mac))
        {
            osal_memset(&stGroup,0,sizeof(stGroup));
            stGroup.vlanId = db.vid;
            LOGIC_PORTMASK_COPY(stGroup.stMbr, db.staticInMbr);
            LPM2UPM(&stGroup.stMbr, &stGroup.upmsk_st);
            userPortMask_reverse(&stGroup.upmsk_st,SYS_USER_PORTMASK_LEN);

            osal_memcpy(stGroup.mac, db.mac, CAPA_MAC_BYTE_NUM);
            SYS_ERR_CONTINUE(qBridge_stGroup_creatEntry(&stGroup));
            numGroup++;
        }
        else
        {
            pStGrp = qbridge_getStGroupEntry(db.vid, db.mac);
            if (pStGrp != NULL)
            {
                LOGIC_PORTMASK_OR(pStGrp->stMbr, pStGrp->stMbr, db.staticInMbr);
                LPM2UPM(&pStGrp->stMbr, &pStGrp->upmsk_st);
                userPortMask_reverse(&pStGrp->upmsk_st,SYS_USER_PORTMASK_LEN);
            }
        }

    }
#endif
    *pEntryNum = numGroup;

    return SYS_ERR_OK;
}

tpgroup_entry_t *qbridge_stGroup_getFirst(void)
{
    uint32 entryNum;
    static u_long startTime = 0;
    u_long getTime = 0, interval = 0;

    osal_time_sysUptime_get(&getTime);
    interval = getTime - startTime;
    if ((getTime - startTime) >= RECREATE_TABLE_UPDATA_TIMER *  SEC_TO_TENMSEC)
    {
        qbridge_stGroup_entry_get(&entryNum);
    }
    startTime = getTime;

    return qbridge_stGroup_scanFirst();
}

tpgroup_entry_t *qbridge_stGroup_getNext(tpgroup_entry_t *preEntry)
{
    tpgroup_entry_t *pTmp;

    if (preEntry == NULL)
        return NULL;

    pTmp = qbridge_stGroup_scanFirst();
    while (pTmp)
    {
         if ((pTmp->vlanId == preEntry->vlanId) &&
            (0 == osal_memcmp(pTmp->mac, preEntry->mac, CAPA_MAC_BYTE_NUM)) )
            return pTmp->next;

        pTmp = pTmp->next;
    }

    return NULL;
}

/*dot1qVlanCurrentTable*/

int32 qBridge_curVlan_creatEntry(vlan_cuurent_entry_t *pEntry)
{
    vlan_cuurent_entry_t *entry;
    vlan_cuurent_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(vlan_cuurent_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    osal_memcpy(entry, pEntry, sizeof(vlan_cuurent_entry_t));
    entry->next = NULL;

    if (curVlanList == NULL)
        curVlanList = entry;
    else
    {
        pTmp = curVlanList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
    }

    return SYS_ERR_OK;
}



void qBridge_curVlan_removeAll(void)
{
    vlan_cuurent_entry_t *pTmp = NULL;
    vlan_cuurent_entry_t *prev = NULL;

    pTmp = curVlanList;
    while (pTmp)
    {
        prev = pTmp;
        pTmp = pTmp->next;
        SNMP_FREE(prev);
    }
    curVlanList = NULL;

}

void qbridge_curVlan_scanAccessInit(void)
{
    curVlanScanPtr = curVlanList;
}

vlan_cuurent_entry_t *qbridge_curVlan_scanFirst(void)
{
    return curVlanList;
}

vlan_cuurent_entry_t *qbridge_curVlan_scanAccessNext(void)
{
    vlan_cuurent_entry_t *returnval = curVlanScanPtr;
    if (curVlanScanPtr)
        curVlanScanPtr  = curVlanScanPtr->next;
    return returnval;
}

vlan_cuurent_entry_t *qbridge_getCurVlanEntry(uint32 vlanId, uint32 timeMark)
{
    curVlanScanPtr  = curVlanList;
    while(curVlanScanPtr)
    {
        if ((vlanId == curVlanScanPtr->vlanId) &&
            (timeMark == curVlanScanPtr->timeMark) )
            return curVlanScanPtr;

        curVlanScanPtr = curVlanScanPtr->next;
    }

    return NULL;
}

int32 qbridge_curVlan_entry_get(uint32 *pEntryNum)
{
    vlan_cuurent_entry_t curVlan;
    sys_vid_t vid;
    uint32 numCurVlan = 0;
    sys_vlanmask_t vmList;
    sys_vlan_operEntry_t operEntry;

    SYS_PARAM_CHK((NULL == pEntryNum), SYS_ERR_NULL_POINTER);
    *pEntryNum = 0;

    //if (curVlanList)
        qBridge_curVlan_removeAll();


    SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vmList));

    FOR_EACH_VID_IN_VLANMASK(vid, vmList)
    {
        if (!IS_VLAN_OPER_ENTRY_EXIST(vid))
            continue;

         osal_memset(&curVlan, 0, sizeof(curVlan));

         SYS_MEM_CLEAR(operEntry);
         SYS_ERR_CONTINUE(sal_vlan_operEntry_get(vid, &operEntry));
         if (SYS_VLAN_ENTRY_TYPE_STATIC == operEntry.type)
            curVlan.status = 2; /*permanent*/
         else if (SYS_VLAN_ENTRY_TYPE_DYNAMIC== operEntry.type)
            curVlan.status = 3; /*dynamicGvrp*/
         else
            curVlan.status = 1; /*other*/

        curVlan.vlanId = vid;
        curVlan.timeMark = 0;
        curVlan.fdbId = vid;
        SYS_ERR_CONTINUE(sal_vlan_mibDot1qVlanCreationTime_get(vid, &curVlan.createTime));
        //curVlan.createTime = curVlan.createTime;

        LOGIC_PORTMASK_COPY(curVlan.egressMbr, operEntry.lpmMember);
        LOGIC_PORTMASK_COPY(curVlan.untagMbr, operEntry.lpmUntag);

        LPM2UPM(&curVlan.egressMbr, &curVlan.upmsk_mbr);
        userPortMask_reverse(&curVlan.upmsk_mbr,SYS_USER_PORTMASK_LEN);
        LPM2UPM(&curVlan.untagMbr, &curVlan.upmsk_untagmbr);
        userPortMask_reverse(&curVlan.upmsk_untagmbr,SYS_USER_PORTMASK_LEN);

        SYS_ERR_CONTINUE(qBridge_curVlan_creatEntry(&curVlan));
        numCurVlan++;
    }

    *pEntryNum = numCurVlan;

    return SYS_ERR_OK;
}

vlan_cuurent_entry_t *qbridge_curVlan_getFirst(void)
{
    uint32 entryNum;
    static u_long startTime = 0;
    u_long getTime = 0, interval = 0;

    osal_time_sysUptime_get(&getTime);
    interval = getTime - startTime;
    if ((getTime - startTime) >= RECREATE_TABLE_UPDATA_TIMER *  SEC_TO_TENMSEC)
    {
        qbridge_curVlan_entry_get(&entryNum);
    }
    startTime = getTime;

    return qbridge_curVlan_scanFirst();
}

vlan_cuurent_entry_t *qbridge_curVlan_getNext(vlan_cuurent_entry_t *preEntry)
{
    vlan_cuurent_entry_t *pTmp;

    if (preEntry == NULL)
        return NULL;

    pTmp = qbridge_curVlan_scanFirst();
    while (pTmp)
    {
         if ((pTmp->vlanId == preEntry->vlanId) && (pTmp->timeMark == preEntry->timeMark))
            return pTmp->next;

        pTmp = pTmp->next;
    }

    return NULL;
}

/*dot1qVlanStaticTable*/

int32 qBridge_stVlan_creatEntry(vlan_static_entry_t *pEntry)
{
    vlan_static_entry_t *entry;
    vlan_static_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(vlan_static_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    osal_memcpy(entry, pEntry, sizeof(vlan_static_entry_t));
    entry->next = NULL;

    if (stVlanList == NULL)
        stVlanList = entry;
    else
    {
        pTmp = stVlanList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
    }

    return SYS_ERR_OK;
}



void qBridge_stVlan_removeAll(void)
{
    vlan_static_entry_t *pTmp = NULL;
    vlan_static_entry_t *prev = NULL;

    pTmp = stVlanList;
    while (pTmp)
    {
        prev = pTmp;
        pTmp = pTmp->next;
        SNMP_FREE(prev);
    }
    stVlanList = NULL;

}

void qbridge_stVlan_scanAccessInit(void)
{
    stVlanScanPtr = stVlanList;
}

vlan_static_entry_t *qbridge_stVlan_scanFirst(void)
{
    return stVlanList;
}

vlan_static_entry_t *qbridge_stVlan_scanAccessNext(void)
{
    vlan_static_entry_t *returnval = stVlanScanPtr;
    if (stVlanScanPtr)
        stVlanScanPtr  = stVlanScanPtr->next;
    return returnval;
}

vlan_static_entry_t *qbridge_getStVlanEntry(uint32 vlanId)
{
    stVlanScanPtr  = stVlanList;
    while(stVlanScanPtr)
    {
        if (vlanId == stVlanScanPtr->vlanId )
            return stVlanScanPtr;

        stVlanScanPtr = stVlanScanPtr->next;
    }

    return NULL;
}


int32 qbridge_stVlan_entry_get(uint32 *pEntryNum)
{
    vlan_static_entry_t stVlan;
    uint32 numStVlan = 0;
    sys_vlanmask_t vmList;
    sys_vid_t vid;
    sys_vlan_adminEntry_t adminEntry;

    SYS_PARAM_CHK((NULL == pEntryNum), SYS_ERR_NULL_POINTER);
    *pEntryNum = 0;

    //if (stVlanList)
        qBridge_stVlan_removeAll();

    SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vmList));

    FOR_EACH_VID_IN_VLANMASK(vid, vmList)
    {
        if (!IS_VLAN_OPER_ENTRY_EXIST(vid))
            continue;

        SYS_MEM_CLEAR(adminEntry);
        SYS_ERR_CONTINUE(sal_vlan_adminEntry_get(vid, &adminEntry));

        osal_memset(&stVlan,0,sizeof(stVlan));
        stVlan.vlanId = vid;
        stVlan.rowStatus= 1; /*active*/
        osal_memcpy(stVlan.name, adminEntry.name.str, osal_strlen(adminEntry.name.str));
        LOGIC_PORTMASK_COPY(stVlan.egressMbr, adminEntry.lpmMember);
        LOGIC_PORTMASK_COPY(stVlan.untagMbr, adminEntry.lpmUntag);
        LOGIC_PORTMASK_COPY(stVlan.forbidMbr, adminEntry.lpmForbidden);

        LPM2UPM(&stVlan.egressMbr, &stVlan.upmsk_mbr);
        userPortMask_reverse(&stVlan.upmsk_mbr,SYS_USER_PORTMASK_LEN);

        LPM2UPM(&stVlan.untagMbr, &stVlan.upmsk_untagmbr);
        userPortMask_reverse(&stVlan.upmsk_untagmbr,SYS_USER_PORTMASK_LEN);

        LPM2UPM(&stVlan.forbidMbr, &stVlan.upmsk_fbmbr);
        userPortMask_reverse(&stVlan.upmsk_fbmbr,SYS_USER_PORTMASK_LEN);

        SYS_ERR_CONTINUE(qBridge_stVlan_creatEntry(&stVlan));
        numStVlan++;
    }

    *pEntryNum = numStVlan;

    return SYS_ERR_OK;
}

vlan_static_entry_t *qbridge_stVlan_getFirst(void)
{
    uint32 entryNum;
    static u_long startTime = 0;
    u_long getTime = 0, interval = 0;

    osal_time_sysUptime_get(&getTime);
    interval = getTime - startTime;
    if ((getTime - startTime) >= RECREATE_TABLE_UPDATA_TIMER *  SEC_TO_TENMSEC)
    {
        qbridge_stVlan_entry_get(&entryNum);
    }
    startTime = getTime;

    return qbridge_stVlan_scanFirst();
}

vlan_static_entry_t *qbridge_stVlan_getNext(vlan_static_entry_t *preEntry)
{
    vlan_static_entry_t *pTmp;

    if (preEntry == NULL)
        return NULL;

    pTmp = qbridge_stVlan_scanFirst();
    while (pTmp)
    {
         if (pTmp->vlanId == preEntry->vlanId)
            return pTmp->next;

        pTmp = pTmp->next;
    }

    return NULL;
}

//dot1qForwardAllTable
void qBridge_fwdAll_removeAll(void)
{
    fwdAll_entry_t *pTmp = NULL;
    fwdAll_entry_t *prev = NULL;
    pTmp = fwdList;
    while (pTmp)
    {
        prev = pTmp;
        pTmp = pTmp->next;
        SNMP_FREE(prev);
    }
    fwdList = NULL;
}

fwdAll_entry_t *qbridge_fwdAll_getFirst(void)
{
    static u_long startTime = 0;
    u_long getTime = 0, interval = 0;

    osal_time_sysUptime_get(&getTime);
    interval = getTime - startTime;
    if ((getTime - startTime) >= DOT1D_ADDR_TABLE_UPDATA_TIMER *  SEC_TO_TENMSEC)
    {
        qBridge_fwdAll_removeAll();
        qBridge_fwdAll_get();
    }
    startTime = getTime;

    return fwdList;
}

fwdAll_entry_t *qbridge_fwdAll_getNext(fwdAll_entry_t *preEntry)
{
    fwdAll_entry_t *pTmp = fwdList;

    if (preEntry == NULL)
        return NULL;

    while (pTmp)
    {
        if (pTmp->vlanId == preEntry->vlanId)
            return pTmp->next;

        pTmp = pTmp->next;
    }

    return NULL;
}

int32 qBridge_fwdAll_creatEntry(fwdAll_entry_t *pEntry)
{
    fwdAll_entry_t *entry;
    fwdAll_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(fwdAll_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    osal_memcpy(entry, pEntry,sizeof(fwdAll_entry_t));
    entry->next = NULL;

    if (fwdList == NULL)
    {
        fwdList = entry;
    }
    else
    {
        pTmp = fwdList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
    }

    return SYS_ERR_OK;
}

int32 qBridge_fwdAll_get(void)
{
    mcast_vlan_entry_t igmpVlan;
    static sys_vlanmask_t vmList;
    sys_vid_t vid;
    fwdAll_entry_t fwdEntry;

    qBridge_fwdAll_removeAll();

    SYS_MEM_CLEAR(vmList);

    if (SYS_ERR_OK !=  sal_vlan_adminEntryExist_get(&vmList))
       return SYS_ERR_FAILED;

    FOR_EACH_VID_IN_VLANMASK(vid, vmList)
    {
        SYS_MEM_CLEAR(fwdEntry);
        SYS_MEM_CLEAR(igmpVlan);
        igmpVlan.vid = vid;
        igmpVlan.ipType = MULTICAST_TYPE_IPV4;
        SYS_ERR_CONTINUE(sal_mcast_vlan_get(&igmpVlan));
        fwdEntry.vlanId = vid;
        LOGIC_PORTMASK_COPY(fwdEntry.mbr, igmpVlan.staticMbr);
        LOGIC_PORTMASK_COPY(fwdEntry.stMbr, igmpVlan.staticMbr);
        LOGIC_PORTMASK_COPY(fwdEntry.fbMbr, igmpVlan.forbidMbr);

        LPM2UPM(&fwdEntry.mbr, &fwdEntry.upmsk_mbr);
        userPortMask_reverse(&fwdEntry.upmsk_mbr,SYS_USER_PORTMASK_LEN);
        osal_memcpy(&fwdEntry.upmsk_stmbr, &fwdEntry.upmsk_mbr, SYS_USER_PORTMASK_LEN);

        LPM2UPM(&fwdEntry.fbMbr, &fwdEntry.upmsk_fbmbr);
        userPortMask_reverse(&fwdEntry.upmsk_fbmbr,SYS_USER_PORTMASK_LEN);

        qBridge_fwdAll_creatEntry(&fwdEntry);
    }

    return SYS_ERR_OK;
}

//dot1qPortVlanTable
void qBridge_portVlan_removeAll(void)
{
    portVlan_entry_t *pTmp = NULL;
    portVlan_entry_t *prev = NULL;
    pTmp = portVlanList;
    while (pTmp)
    {
        prev = pTmp;
        pTmp = pTmp->next;
        SNMP_FREE(prev);
    }
    portVlanList = NULL;
}

portVlan_entry_t *qbridge_portVlan_getFirst(void)
{
    static u_long startTime = 0;
    u_long getTime = 0, interval = 0;

    osal_time_sysUptime_get(&getTime);
    interval = getTime - startTime;
    if ((getTime - startTime) >= DOT1D_ADDR_TABLE_UPDATA_TIMER *  SEC_TO_TENMSEC)
    {
        qBridge_portVlan_get();
    }
    startTime = getTime;

    return portVlanList;
}

portVlan_entry_t *qbridge_portVlan_getNext(portVlan_entry_t *preEntry)
{
    portVlan_entry_t *pTmp = portVlanList;

    if (preEntry == NULL)
        return NULL;

    while (pTmp)
    {
        if (pTmp->userPort == preEntry->userPort)
            return pTmp->next;

        pTmp = pTmp->next;
    }

    return NULL;
}

int32 qBridge_portVlan_creatEntry(portVlan_entry_t *pEntry)
{
    portVlan_entry_t *entry;
    portVlan_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(portVlan_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    osal_memcpy(entry, pEntry,sizeof(portVlan_entry_t));
    entry->next = NULL;

    if (portVlanList == NULL)
    {
        portVlanList = entry;
    }
    else
    {
        pTmp = portVlanList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
    }

    return SYS_ERR_OK;
}

int32 qBridge_portVlan_get(void)
{
    sys_vid_t pvid;
    sys_logic_port_t lp;
    portVlan_entry_t portVlanEntry;
    sys_vlan_frameType_t frmType;
    sys_enable_t enable = DISABLED;

    qBridge_portVlan_removeAll();

    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(portVlanEntry);
        portVlanEntry.lp = lp;
        portVlanEntry.userPort = LP2UP(lp);

        sal_vlan_portAdminPvid_get(lp, &pvid);
        portVlanEntry.pvid = pvid;

        sal_vlan_portAdminAcceptFrameType_get(lp, &frmType);
        portVlanEntry.frameType = frmType + 1;

        portVlanEntry.gvrpStatus = (enable == DISABLED) ? 2: 1;
        portVlanEntry.portRegVlan = (enable == DISABLED) ? 2: 1;

        sal_vlan_portAdminIgrFilterEnable_get(lp, &enable);
        portVlanEntry.filter = (enable == DISABLED) ? 2: 1;

        #ifdef CONFIG_SYS_PROTO_GVRP
        gvrp_error_statistics_t statis;
        sal_gvrp_port_enable_get(lp, &enable);
        portVlanEntry.gvrpStatus = (enable == DISABLED) ? 2: 1;
        sal_gvrp_port_forbidVlan_get(lp, &enable);
        portVlanEntry.portRegVlan = (enable == DISABLED) ? 2: 1;
        SYS_MEM_CLEAR(statis);
        sal_gvrp_port_error_statistics_get(lp, &statis);
        portVlanEntry.gvrpErrStats = statis.invAlen + statis.invAtyp + statis.invAval + statis.invEvent + statis.invProt;
        #endif
        qBridge_portVlan_creatEntry(&portVlanEntry);
    }

    return SYS_ERR_OK;
}

