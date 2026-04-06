
#ifndef _INCLUDE_VOSMEMORY_H_
#define _INCLUDE_VOSMEMORY_H_

#include <libipc/vos.h>

typedef enum {
    VOS_MEM_SHARE_FLAG_RONLY              = 0x00000001,
    VOS_MEM_SHARE_FLAG_CREATE             = 0x00000002
}VOS_MEM_SHARE_FLAG_ENT;

typedef enum {
    MEM_FLAG_NONE                          = 0x00000000,  
    MEM_FLAG_DMA                           = 0x000000001, 
    MEM_FLAG_SLEEP                         = 0x000000002, 
    MEM_FLAG_MAX
}MEM_FLAG_ENT;

void *vosCalloc(ulong_t ulElemNum, ulong_t ulElemSize);

void *vosMalloc(ulong_t ulSize);

void vosFree(void *ptr);

void *vosRealloc(void *ptr, ulong_t ulSize);

void *vosDmaMalloc(ulong_t ulSize, ulong_t ulFlags);

void vosDmaFree(void *ptr);

void *vosMemoryShareGet(char *name, uchar_t id, ulong_t ulSize, ulong_t ulFlags);
#endif 

