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

#include "mib_util.h"
#include <common/sys_def.h>
#include <libsal/sal_util.h>
#include <libsal/sal_vlan.h>

#if 0
mib_port_entry_t  portEntry[SYS_LOGICPORT_NUM_MAX];
static  mib_port_entry_t *portList = NULL, *portScanList = NULL;
uint8 portEntryInit = 0;
#endif

int32 userPortMask_reverse(sys_user_portmask_t *pUserPmsk, int32 portMaskSize)
{
    int i,j,k, n = 4;
    char *pTmp;
    char t;
    sys_user_portmask_t tmpUserPmsk;

    SYS_PARAM_CHK((NULL == pUserPmsk), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((portMaskSize%4 != 0), SYS_ERR_FAILED);
    SYS_MEM_CLEAR(tmpUserPmsk);
    osal_memcpy(&tmpUserPmsk, pUserPmsk, portMaskSize);

    for (i = 0; i < portMaskSize/4 ; i++)
    {
        if (i > 1 && i < portMaskSize/4 -1)
            continue;

        pTmp = (char *)&tmpUserPmsk.bits[i];
        {
            //one uint32 4 byte reversal
            for (j = 0; j < n/2; j++)
            {
                t = pTmp[j];
                pTmp[j] = pTmp[n-1-j];
                pTmp[n-1-j] = t;
            }

            for (k = 0; k < n; k++)
            {
                t = 0;
                for (j = 0; j < 8; j++)
                {
                    t += (((pTmp[k] >>(7-j))& 0x01) << j);
                }
                pTmp[k] = t;
            }
        }
    }
    osal_memcpy( pUserPmsk, &tmpUserPmsk,portMaskSize);
    return SYS_ERR_OK;
}

int32 charstr2userPortMask(u_char *pvar_val, uint32 val_len, sys_user_portmask_t *pUserPmsk)
{
    int i, j;
    uint32 tmpVal = 0;

    SYS_PARAM_CHK((NULL == pvar_val), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pUserPmsk), SYS_ERR_NULL_POINTER);

    for (i = 0; i < val_len/4; i++)
    {
        pUserPmsk->bits[i] =  *((uint32 *)(pvar_val + i));
    }

    if (val_len%4)
    {
        if (1 == val_len%4)
        {
            tmpVal +=  ((*(pvar_val+val_len-1)) << 24);
        }
        else if (2 == val_len%4)
        {
            tmpVal +=  ((*(pvar_val+val_len-2)) << 24) + ((*(pvar_val+val_len-1)) << 16);
        }
        else if (3 == val_len%4)
        {
            tmpVal +=  ((*(pvar_val+val_len-3)) << 24) + ((*(pvar_val+val_len-2)) << 16) + ((*(pvar_val+val_len-1)) << 16);
        }

        pUserPmsk->bits[val_len/4] = tmpVal;
    }

    return SYS_ERR_OK;
}
int header_logicPort(long *index, struct variable *vp, oid *name, size_t  *length)
{
    sys_user_port_t userPort;
    userPort = name[vp->namelen];

    while (IS_LOGIC_PORT_INVALID(UP2LP(userPort)))
    {
       if (userPort >= LP2UP(LOGIC_PORT_END))
            return -1;

        if (userPort > LP2UP(NORMAL_LOGIC_PORT_END))
        {
            userPort = LP2UP(TRUNK_LOGIC_PORT_START);
        }
        else
            userPort++;
    }
    *index = UP2LP(userPort);
    name[vp->namelen] = userPort;
    *length = vp->namelen + 1;

    return 0;
}

uint32 userPort_getNext(sys_user_port_t preUserPort)
{
    sys_logic_port_t preLp = UP2LP(preUserPort);
    sys_logic_port_t nextLp = 0;

    if (preLp < NORMAL_LOGIC_PORT_END)
    {
        nextLp = preLp + 1;
    }
    else if (preLp == NORMAL_LOGIC_PORT_END)
    {
        nextLp = TRUNK_LOGIC_PORT_START;
    }
    else if (preLp >=  TRUNK_LOGIC_PORT_START && preLp < TRUNK_LOGIC_PORT_END)
    {
        nextLp = preLp + 1;
    }
    else if (preLp == TRUNK_LOGIC_PORT_END)
    {
        return 0; /*invalid user port*/
    }

    return LP2UP(nextLp);
}

/*-1 failue,   0 , ok,  1 first vlan*/
int header_vlan(long *index, struct variable *vp, oid *name, size_t  *length, sys_vlanmask_t vlanMask)
{
    sys_vid_t curVlan;

    curVlan = *index;
    while (!VLANMASK_IS_VIDSET(vlanMask, curVlan))
    {
        if (curVlan > BOND_VLAN_MAX)
        {
            *index = curVlan;
            name[vp->namelen] = curVlan;
            *length = vp->namelen + 1;
            return -1;
        }
        curVlan++;
    }

    *index = curVlan;
    name[vp->namelen] = curVlan;
    *length = vp->namelen + 1;

    return 0;
}

/*-1 failue,   0 , ok,  1 first vlan*/
int header_exist_vlan(long *index, struct variable *vp, oid *name, size_t  *length, sys_vlanmask_t vlanMask)
{
    sys_vid_t curVlan;

    curVlan = *index;
    while (VLANMASK_IS_VIDSET(vlanMask, curVlan))
    {
        if (curVlan > BOND_VLAN_MAX)
        {
            *index = curVlan;
            name[vp->namelen] = curVlan;
            *length = vp->namelen + 1;
            return -1;
        }
        curVlan++;
    }

    *index = curVlan;
    name[vp->namelen] = curVlan;
    *length = vp->namelen + 1;

    return 0;
}

#if 0
void mib_port_entry_init(void)
{
    sys_logic_port_t port;
    mib_port_entry_t *pTmp;

    if (0 == portEntryInit)
    {
        osal_memset(portEntry, 0, sizeof(portEntry));

        FOR_EACH_LOGIC_PORT(port)
        {
            portEntry[port].uport = LP2UP(port);

            if (portList == NULL)
                portList = &portEntry[port];
            else
            {
                pTmp = portList;
                while(pTmp->next)
                {
                    pTmp = pTmp->next;
                }
                pTmp->next = &portEntry[port];
            }
        }

        portEntryInit = 1;
    }
}


/*function define*/
void port_scanAccessInit(void)
{
    PORT_ENTRY_SEM_LOCK();
    portScanList  = portList;
    PORT_ENTRY_SEM_UNLOCK();
}

mib_port_entry_t *port_scanFirst(void)
{
    return portList;
}

mib_port_entry_t *port_scanAccessNext(void)
{
    mib_port_entry_t *returnval;

    PORT_ENTRY_SEM_LOCK();

    returnval = portScanList;
    if (portScanList)
        portScanList  = portScanList->next;

    PORT_ENTRY_SEM_UNLOCK();
    return returnval;
}

mib_port_entry_t *portEntry_get(uint32 uPort)
{
    if(IS_LOGIC_PORT_INVALID(UP2LP(uPort)))
        return NULL;

    PORT_ENTRY_SEM_LOCK();

    portScanList  = portList;
    while(portScanList)
    {
        if (uPort ==  portScanList->uport)
         {
            PORT_ENTRY_SEM_UNLOCK();
            return portScanList;
         }

        portScanList = portScanList->next;
    }

    PORT_ENTRY_SEM_UNLOCK();
    return NULL;
}
#endif

int
AGUTIL_advance_index_name(struct variable *vp, oid * name,
                          size_t * length, int exact)
{
    int             result;

    if (exact)
        return 0;

    if (*length <= vp->namelen) {
        result =
            snmp_oid_compare(name, *length, vp->name, (int) vp->namelen);
        memcpy((char *) name, (char *) vp->name,
               ((int) vp->namelen) * sizeof(oid));
        *length = vp->namelen;
    } else {
        /*
         * If the name is given with indexes - compare only the oids.
         */
        result =
            snmp_oid_compare(name, (int) vp->namelen, vp->name,
                             (int) vp->namelen);
        /*
         * If it's not the same oid - change name to the new oid
         */
        if (result < 0) {
            memcpy((char *) name, (char *) vp->name,
                   ((int) vp->namelen) * sizeof(oid));
            *length = vp->namelen;
        }
    }

    if (result > 0) {
        snmp_log(LOG_DEBUG,"*length=%d result=%d !!!", (int) *length, (int) result);
        return -1;
    }
    return 0;
}

int32 isprint_check(char * var_val, uint32 var_val_len)
{
    char *cp = NULL;
    int32 count;

    if (NULL == var_val)
        return SYS_ERR_OK;

    for (cp = var_val, count = 0; count < (int) var_val_len; count++, cp++)
    {
       if (!isprint(*cp))
        {
            return SYS_ERR_FAILED;
        }

        if (isspace(*cp))
        {
            return SYS_ERR_FAILED;
        }
    }

    return SYS_ERR_OK;
}

char *
strchr_get (char *s, const char begin, const char end, char **lasts)
{
    char *tok;
    char *pStrEnd = NULL;
    tok = s;

    if (s == NULL && (s = *lasts) == NULL)
		return (NULL);

    tok = strchr(s, begin);
    if (NULL == tok)
        return NULL;
    tok++;

    pStrEnd = strchr(tok, end);
    if (NULL == pStrEnd)
        return NULL;
    *pStrEnd = '\0';

    pStrEnd++;
    *lasts = pStrEnd;

    return tok;
}
