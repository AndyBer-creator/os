
#ifndef _INCLUDE_SMDLINK_H_
#define _INCLUDE_SMDLINK_H_

#include <h/glnTypes.h>
#include <h/vos/lstLib.h>
#include <h/vos/vosSystem.h>
#include <h/switch/smd/smdVDPort.h>

typedef struct {
    BOOL               bLinkUp; 
    BOOL               bLDEnabled; 
    BOOL               bLDBeing; 
    ulong_t            ulLDTimeCfg; 
    vos_jiffies_t      ulLDTimeout; 
}SMD_LINK_STC;

typedef struct {
    NODE               lstNode;
    ulong_t            ulPort; 
    ulong_t            ulLink;
}SMD_LINK_MSG;

int smdLinkInit(void);

int smdLinkHwChg(unsigned int uiPort, unsigned int uiLink);

int smdLinkStart(void);

int smdLinkAGBindChgNotify(ulong_t ulPort);

int smdLinkStatusGet(SMD_AGAP_TYPE portType, 
    ulong_t ulPort, VD_PORT_LINK_STATUS *pLinkState);

#endif 

