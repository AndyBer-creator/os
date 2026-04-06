#ifndef __COMMON_SYS_PORTVIEW_H__
#define __COMMON_SYS_PORTVIEW_H__
#include <common/sys_type.h>
#include <common/sys_vlanmask.h>
#include <board/board.h>
#define SYS_PORT_TYPE_LAG_IDNUM_BASE    1000
#define SYS_TRUNK_INVALID_ID            0xFFFFFFFF
#define SYS_PORT_INVALID_ID             0xFFFFFFFF
#define SYS_CPU_LP                      0xFF 

extern sys_portStr_t g_portStr[];
typedef enum sys_trunk_portRole_e
{
    SYS_PORT_NORMAL = 0,           
    SYS_PORT_TRUNK_MEMBER,         
    SYS_PORT_TRUNK_BACKUP_MEMBER,  
    SYS_PORT_TRUNK_EMPTY,          
    SYS_PORT_TRUNK_LACP,           
    SYS_PORT_TRUNK_STATIC,         
    SYS_PORT_END
} sys_trunk_portRole_t;

typedef struct sys_logic_portInfo_s
{
    sys_user_port_t userPort[SYS_LOGICPORT_NUM_MAX];
    sys_port_type_t portType[SYS_LOGICPORT_NUM_MAX];
    sys_logic_port_t portTypeStartPort[PORT_TYPE_END];
    uint32           portNum[PORT_TYPE_END];
    uint8            isPoePort[SYS_LOGICPORT_NUM_MAX];
    uint8            isComboFiberPort[SYS_LOGICPORT_NUM_MAX];
} sys_logic_portInfo_t;

typedef struct sys_physic_portInfo_s
{
    uint32 dev;
    uint32 portNum;
}sys_physic_portInof_t;

typedef enum sys_port_mediaType_e
{
    SYS_PORT_MEDIA_COPPER,
    SYS_PORT_MEDIA_FIBER,
    SYS_PORT_MEDIA_COMBO,
    SYS_PORT_MEDIA_END,
} sys_port_mediaType_t;

typedef enum sys_port_strFormat_e
{
    SYS_PORT_STRFMT_BRIEF = 0,
    SYS_PORT_STRFMT_FULL,
    SYS_PORT_STRFMT_WEB,
    SYS_PORT_STRFMT_END
} sys_port_strFormat_t;

typedef struct sys_port_prefixName_s
{
    char briefPrefix[CAPA_PORT_STR_LEN]; 
    char fullPrefix[CAPA_PORT_STR_LEN];  
    char webPrefix[CAPA_PORT_STR_LEN];   
} sys_port_prefixName_t;
#ifdef __KERNEL__

#define LPM2STR(pLPMsk, str) (sys_port_lPortmask2uStr(pLPMsk, SYS_PORT_STRFMT_BRIEF, FALSE, str))
#define LPM2MLP(lPMsk) (sys_port_lPortmaskLowestPort_ret(lPMsk))

#else
#define STR2LPM(str, pLPMsk) (sys_port_uStr2lPortmask(str, pLPMsk))
#define LPM2STR(pLPMsk, str) (sys_port_lPortmask2uStr(pLPMsk, SYS_PORT_STRFMT_BRIEF, FALSE, str))
#define LPM2FULLSTR(pLPMsk, str) (sys_port_lPortmask2uStr(pLPMsk, SYS_PORT_STRFMT_FULL, FALSE, str))
#define LPM2WEBSTR(pLPMsk, str) (sys_port_lPortmask2uStr(pLPMsk, SYS_PORT_STRFMT_WEB, FALSE, str))
#endif
#define LPSTR(lport) (sys_port_lPortStr_ret(lport))
#define LPM2TRKLPM(pSrcPm, pDstPm) sys_trunk_member2TrunklogicPortmask_get(pSrcPm, pDstPm)
#define LPMEXCLMBR(pLpm) sys_trunk_member2TrunklogicPortmask_get(pLpm, NULL)
#define LP2STR(lport, str) (sys_port_lPort2uStr(lport, SYS_PORT_STRFMT_BRIEF, str))
#define LP2FULLSTR(lport, str) (sys_port_lPort2uStr(lport, SYS_PORT_STRFMT_FULL, str))
#define LP2WEBSTR(lport, str) (sys_port_lPort2uStr(lport, SYS_PORT_STRFMT_WEB, str))
#define LP2UP(lport) (sys_port_lport2uport_ret(lport))
#define UP2LP(uport) (sys_port_uport2lport_ret(uport))
#define LP2TYPE(lport) (sys_port_lPort2Type_ret(lport))
#define LPTYPENUM(type)   (sys_port_lPortTypeNum_ret(type))
#define LPTYPENUM_100M()  (LPTYPENUM(PORT_TYPE_100M))
#define LPTYPENUM_1000M() (LPTYPENUM(PORT_TYPE_1000M))
#define LPTYPENUM_2500M() (LPTYPENUM(PORT_TYPE_2500M))
#define LPTYPENUM_10G() (LPTYPENUM(PORT_TYPE_10G))
#define LPTYPENUM_LAG()   (LPTYPENUM(PORT_TYPE_LAG))
#define STR2LP(str, pPort) (sys_port_uStr2lPort(str, pPort))
#define LP2TRKID(lport) (sys_trunk_portTrunkInfo_ret(lport, TRUE))
#define LP2TRKLP(lport) (sys_trunk_portTrunkInfo_ret(lport, FALSE))
#define LP2TRKUID(lport) (TRK2UID(LP2TRKID(lport)))
#define TRK2UID(trkId) (trkId + 1)
#define UID2TRK(uTrkId) (uTrkId - 1)
#define TRK2LP(trkId) (trkId + TRUNK_LOGIC_PORT_START)
#define TRK2MBRLPM(trkId, pMbrPm) (sys_trunk_member_get(trkId, pMbrPm))
#define LP2TRKMBRLPM(lport, pMbrPm) (sys_trunk_member_get(LP2TRKID(lport) ,pMbrPm))
#define TRK2ACTMBRLPM(trkId, pMbrPm) (sys_trunk_activeMember_get(trkId, pMbrPm))
#define LP2TRKACTMBRLPM(lport, pMbrPm) (sys_trunk_activeMember_get(LP2TRKID(lport) ,pMbrPm))
#define LPM2UPM(pLPMsk, pUPMsk) (sys_port_lPortmask2UPortmask(pLPMsk, pUPMsk))
#define UPM2LPM(pUPMsk, pLPMsk) (sys_port_uPortmask2LPortmask(pUPMsk, pLPMsk))
#define LP2MEDIA(lport) (sys_port_lPortMedia_ret(lport))

#define IS_CPU_LP(port) (SYS_CPU_LP == port)
#define IS_TRK_EMPTY(trunkId) (SYS_PORT_TRUNK_EMPTY == sys_trunk_portRole_ret(TRK2LP(trunkId)))
#define IS_TRK_STATIC(trunkId) (SYS_PORT_TRUNK_STATIC == sys_trunk_portRole_ret(TRK2LP(trunkId)))
#define IS_TRK_LACP(trunkId) (SYS_PORT_TRUNK_LACP == sys_trunk_portRole_ret(TRK2LP(trunkId)))
#define IS_TRKLP_EMPTY(port) (SYS_PORT_TRUNK_EMPTY == sys_trunk_portRole_ret(port))
#define IS_TRKLP_STATIC(port) (SYS_PORT_TRUNK_STATIC == sys_trunk_portRole_ret(port))
#define IS_TRKLP_LACP(port) (SYS_PORT_TRUNK_LACP == sys_trunk_portRole_ret(port))
#define IS_TRKLP(port) ((port >= TRUNK_LOGIC_PORT_START) && (port <= TRUNK_LOGIC_PORT_END))

#define IS_LP_NML(port) (SYS_PORT_NORMAL == sys_trunk_portRole_ret(port))
#define IS_LP_TRKMBR(port) (SYS_PORT_TRUNK_MEMBER == sys_trunk_portRole_ret(port))
#define IS_LP_TRKBAKMBR(port) (SYS_PORT_TRUNK_BACKUP_MEMBER == sys_trunk_portRole_ret(port))
#define IS_LP_TRKMBR_STATIC(port) (IS_LP_TRKMBR(port) && IS_TRKLP_STATIC(LP2TRKLP(port)))
#define IS_LP_TRKMBR_LACP(port) (IS_LP_TRKMBR(port) && IS_TRKLP_LACP(LP2TRKLP(port)))

#define IS_LP_LINKUP(port) (sys_port_linkStatus_ret(port))
#define IS_COMBO_PORT(port) (SYS_PORT_MEDIA_COMBO == LP2MEDIA(port))
#define IS_COPPER_PORT(port) (SYS_PORT_MEDIA_COPPER == LP2MEDIA(port))
#define IS_FIBER_PORT(port) (SYS_PORT_MEDIA_FIBER == LP2MEDIA(port))
#define IS_TGE_PORT(port) (PORT_TYPE_10G == LP2TYPE(port))
#define IS_POE_PORT(port) (TRUE == sys_port_isLPortPoe_ret(port))
#define IS_NOT_POE_PORT(port) (FALSE == sys_port_isLPortPoe_ret(port))
#define IS_COMBO_FIBER_PORT(port) (TRUE == sys_port_isLPortComboFiberMode_ret(port))
#ifdef __KERNEL__
extern int32 sys_physic_portview_init(void);
extern int32 sys_board_portConf_get(sys_port_conf_t **ppPortConf);
extern int32 sys_logic_to_physic_port(
    sys_logic_port_t logicPort,
    sys_physic_port_t *pPhysicPort);
extern int32 sys_physic_to_logic_port(
    sys_physic_port_t physicPort,
    sys_logic_port_t *pLogicPort);
extern int32 sys_physic_to_logic_portmask(
    sys_physic_portmask_t *pPhysicPortmask,
    sys_logic_portmask_t *pLogicPortmask);
extern int32 sys_logic_to_physic_portmask(
    sys_logic_portmask_t *pLogicPortmask,
    sys_physic_portmask_t *pPhysicPortmask);
extern int32 sys_logic_port_to_physic_portmask(
    sys_logic_port_t port,
    sys_physic_portmask_t *pPhysicPortmask);
extern sys_logic_port_t sys_port_lPortmaskLowestPort_ret(sys_logic_portmask_t lPortmask);

#else

extern int32 sys_user_portview_init(uint32 unit);
extern int32 sys_port_uStr2lPortmask(const char *str, sys_logic_portmask_t *pLPortmask);

#endif 
extern int32 sys_port_uStr2lPort(char *str, sys_logic_port_t *pLPort);
extern int32 sys_port_lPort2uStr(sys_logic_port_t logicPort, sys_port_strFormat_t fmt, char *str);
extern char *sys_port_lPortStr_ret(sys_logic_port_t port);
extern sys_user_port_t sys_port_lport2uport_ret(sys_logic_port_t logicPort);
extern sys_logic_port_t sys_port_uport2lport_ret(sys_user_port_t userPort);
extern sys_port_type_t sys_port_lPort2Type_ret(sys_logic_port_t logicPort);
extern uint32 sys_port_lPortTypeNum_ret(sys_port_type_t type);
extern sys_port_mediaType_t sys_port_lPortMedia_ret(sys_logic_port_t logicPort);
extern uint8 sys_port_isLPortPoe_ret(sys_logic_port_t logicPort);
extern int32 sys_port_lPortmask2uStr(
    sys_logic_portmask_t *pLPortmask,
    sys_port_strFormat_t fmt,
    uint32               useAllKey,
    char                 *str);
extern int32 sys_port_lPortmask2UPortmask(
    sys_logic_portmask_t *pLPortmask,
    sys_user_portmask_t *pUPortmask);
extern int32 sys_port_uPortmask2LPortmask(
    sys_user_portmask_t *pUPortmask,
    sys_logic_portmask_t *pLPortmask);
extern uint32 sys_port_linkStatus_ret(sys_logic_port_t port);
extern int32 sys_trunk_member2TrunklogicPortmask_get(
    sys_logic_portmask_t *pSrcPortmask,
    sys_logic_portmask_t *pDstPortmask);
extern int32 sys_trunk_member_get(uint32 trunkId, sys_logic_portmask_t *pMemberPm);
extern int32 sys_trunk_activeMember_get(uint32 trunkId, sys_logic_portmask_t *pMemberPm);
extern sys_trunk_portRole_t sys_trunk_portRole_ret(sys_logic_port_t port);
extern uint32 sys_trunk_portTrunkInfo_ret(sys_logic_port_t port, uint32 isRetTrkId);

extern int32 sys_port_lPortmask2lstrPortmask(sys_logic_portmask_t *pLPortmask, sys_logic_portmask_t *pLstrPortmask);

extern uint8 sys_port_isLPortComboFiberMode_ret(sys_logic_port_t logicPort);
#endif 

