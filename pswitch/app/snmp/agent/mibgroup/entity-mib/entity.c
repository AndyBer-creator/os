
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
#include "entity.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>






#include <common/sys_portview.h>


/*
* Symbol Definition
*/


static physicall_entry_t *phyList = NULL, *phyScanPtr = NULL;
static physicall_entry_t *aliasList = NULL, *aliasScanPtr = NULL;
static physicall_entry_t *containList = NULL, *containScanPtr = NULL;

physicall_entry_t  phyDescr[] =
{
   {64, PHY_STACK_NAME,  0,  11, -1,  PHY_STACK_NAME},
   {67108992,  PHY_DESCR,  64,  3, 1,  PHY_NAME},
   {68157952,  PHY_SLOT_NAME,  67108992,  5, 1,  PHY_SLOT_NAME},
   {68420352,  PHY_NAME,  68157952,  9, 1,  PHY_NAME},
   {68424704,  PHY_SERIAL_NUM,  68420352,  1, 1,  PHY_SERIAL_NUM}
};


int32 entity_physical_creatEntry(physicall_entry_t *pEntry)
{
    physicall_entry_t *entry;
    physicall_entry_t *pTmp;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    entry = SNMP_MALLOC_TYPEDEF(physicall_entry_t);
    if(!entry)
        return SYS_ERR_FAILED;

    entry->index = pEntry->index;
    osal_memcpy(entry->descr, pEntry->descr, sizeof(pEntry->descr));
    entry->containedIn = pEntry->containedIn;
    entry->phyclass = pEntry->phyclass;
    entry->parentRelPos = pEntry->parentRelPos;
    osal_memcpy(entry->physicalName, pEntry->physicalName, sizeof(pEntry->physicalName));
    entry->next = NULL;

    if (phyList == NULL)
        phyList = entry;
    else
    {
        pTmp = phyList;
        while(pTmp->next)
        {
            pTmp = pTmp->next;
        }
        pTmp->next = entry;
    }

    return SYS_ERR_OK;
}


/*function define*/

/*entPhysicalTable*/
void entity_physical_scanAccessInit(void)
{
    phyScanPtr  = phyList;
}

physicall_entry_t *entity_physical_scanFirst(void)
{
    return phyList;
}

physicall_entry_t *entity_physical_scanAccessNext(void)
{
    physicall_entry_t *returnval = phyScanPtr;
    if (phyScanPtr)
        phyScanPtr  = phyScanPtr->next;
    return returnval;
}



physicall_entry_t *entity_getPhysicalEntry(uint32 index)
{
    physicall_entry_t *pTmp;

    pTmp  = phyList;
    while(pTmp)
    {
        if (pTmp->index == index)
            return pTmp;

        pTmp = pTmp->next;
    }

    return pTmp;
}


/*entAliasMappingTable*/

void entity_aliasMap_scanAccessInit(void)
{
    aliasScanPtr = entity_getPhysicalEntry(ENTITY_START_INDEX);
}

physicall_entry_t *entity_aliasMap_scanFirst(void)
{
    return entity_getPhysicalEntry(ENTITY_START_INDEX);
}

physicall_entry_t *entity_aliasMap_scanAccessNext(void)
{
    physicall_entry_t *returnval = aliasScanPtr;
    if (aliasScanPtr)
        aliasScanPtr  = aliasScanPtr->next;
    return returnval;
}


/*entPhysicalContainsTable*/
void entity_contains_scanAccessInit(void)
{
    if (phyList)
        containScanPtr = phyList->next;
}

physicall_entry_t *entity_contains_scanFirst(void)
{
    entity_contains_scanAccessInit();
    return containScanPtr;
}

physicall_entry_t *entity_contains_scanAccessNext(void)
{
    physicall_entry_t *returnval = containScanPtr;
    if (containScanPtr)
        containScanPtr  = containScanPtr->next;
    return returnval;
}


void entity_physical_data_init(void)
{
    int i;
    sys_logic_port_t  lport;
    sys_physic_port_t phyPort;
    physicall_entry_t phyEntry;
    char port_str[CAPA_PORT_STR_LEN];
    uint32 phyPortList[64];
    char sysModel[CAPA_SYS_NAME_LEN + 1];

    sal_sys_bdinfoVar_get("modelname", sysModel);

    for(i = 0; i < ENTITY_PHY_DESCR_PORT_NUM; i++)
    {
        if (1 == i || 3 == i)
        {
            osal_memset(phyDescr[i].descr, 0, sizeof(phyDescr[i].descr));
            osal_memset(phyDescr[i].physicalName, 0, sizeof(phyDescr[i].physicalName));
            if (1 == i)
                osal_memcpy(phyDescr[i].descr, sysModel, osal_strlen(sysModel));
            else
                osal_memcpy(phyDescr[i].descr, sysModel, osal_strlen(sysModel));

            osal_memcpy(phyDescr[i].physicalName, sysModel, osal_strlen(sysModel));
        }

        if (4 == i)
        {
            osal_memset(phyDescr[i].descr, 0, sizeof(phyDescr[i].descr));
            osal_memset(phyDescr[i].physicalName, 0, sizeof(phyDescr[i].physicalName));
            osal_strcpy(phyDescr[i].descr, ENTITY_PHY_CHIP_DESC);
        }

        SYS_ERR_CONTINUE(entity_physical_creatEntry(&phyDescr[i]));
    }

    SYS_MEM_CLEAR(phyPortList);
    FOR_EACH_NORMAL_LOGIC_PORT(lport)
    {
        sal_port_logic2Physic_get(lport, &phyPort);
        phyPortList[phyPort.port] = phyPort.port + 1;
    }

    phyPort.dev = SYS_DEV_ID_DEFAULT;
    for (i = 0; i < 64; i++)
    {
        if (0 != phyPortList[i])
        {
            osal_memset(&phyEntry, 0, sizeof(phyEntry));
            phyEntry.index = phyPortList[i]-1 + ENTITY_START_INDEX;
            phyEntry.containedIn = phyDescr[ENTITY_PHY_DESCR_PORT_NUM -1].index;
            phyEntry.phyclass = ENTITY_PHY_CLASS;
            phyEntry.parentRelPos = phyPortList[i];

            phyPort.port = phyPortList[i]-1;
            sal_port_physic2Logic_get(phyPort, &lport);
            SYS_MEM_CLEAR(port_str);
            LP2STR(lport, port_str);
            osal_strcpy(phyEntry.physicalName, port_str);
            SYS_ERR_CONTINUE(entity_physical_creatEntry(&phyEntry));
        }
    }

}



