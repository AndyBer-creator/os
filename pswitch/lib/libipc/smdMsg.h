
#ifndef _INCLUDE_SMDMSG_H_
#define _INCLUDE_SMDMSG_H_

#include <libipc/vos.h>
#include <libipc/smdMsgRequest.h>

int smdMsgStreamToArgs(uchar_t *pStream, ulong_t ulStremLen, SMD_ARG args[], ulong_t *pulArgsNum);
#endif 

