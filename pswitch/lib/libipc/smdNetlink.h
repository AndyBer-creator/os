
#ifndef _INCLUDE_SMDNETLINK_H_
#define _INCLUDE_SMDNETLINK_H_

#include <libipc/vos.h>

int smdMsgNetlinkRequest(uchar_t *pStream, ulong_t ulStremLen, ulong_t ulTimeOut);

int smdMsgNetlinkHeaderLen(ulong_t *pulHeaderLen);
#endif 

