#ifndef MIB_UTIL_H
#define MIB_UTIL_H

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include <common/sys_portview.h>

//#define SYS_GIGA_PORT_NUM     LPTYPENUM_1000M()

typedef struct mib_port_entry_s{
    uint32  uport;
    struct mib_port_entry_s *next;
}mib_port_entry_t;

//osal_mutex_t            port_sem;
//#define PORT_ENTRY_SEM_LOCK()    \
//do {\
//    if (osal_sem_mutex_take(port_sem, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)\
//    {\
  //      SYS_PRINTF("MIB port entry semaphore lock failed");\
   // }\
//} while(0)

//#define PORT_ENTRY_SEM_UNLOCK() \
//do {\
 //   if (osal_sem_mutex_give(port_sem) != SYS_ERR_OK)\
   // {\
     //   SYS_PRINTF("MIB port entry semaphore unlock failed");\
   // }\
//} while(0)


extern int32 charstr2userPortMask(u_char *pvar_val, uint32 val_len, sys_user_portmask_t *pUserPmsk);
extern int32 userPortMask_reverse(sys_user_portmask_t *pUserPmsk, int32 portMaskSize);
extern int header_logicPort(long *index, struct variable *vp, oid *name, size_t  *length);
extern uint32 userPort_getNext(sys_user_port_t preUserPort);
extern int header_vlan(long *index, struct variable *vp, oid *name, size_t  *length, sys_vlanmask_t vlanMask);

extern void mib_port_entry_init(void);
extern void port_scanAccessInit(void);
extern mib_port_entry_t *port_scanFirst(void);
extern mib_port_entry_t *port_scanAccessNext(void);
extern mib_port_entry_t *portEntry_get(uint32 uPort);

int             AGUTIL_advance_index_name(struct variable *vp, oid * name,
                                          size_t * length, int exact);

extern int32 isprint_check(char * var_val, uint32 var_val_len);

extern char *strchr_get (char *s, const char begin, const char end, char **lasts);

#endif /*MIB_UTIL_H*/

