#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <libipc/vosMemory.h>
void *vosCalloc(ulong_t ulElemNum, ulong_t ulElemSize)
{
    return calloc(ulElemNum, ulElemSize);
}
void *vosMalloc(ulong_t ulSize)
{
    return malloc(ulSize);
}
void vosFree(void *ptr)
{
    free(ptr);
}
void *vosRealloc(void *ptr, ulong_t ulSize)
{
    return realloc(ptr, ulSize);
}
void *vosMemoryShareGet(char *name, uchar_t id, ulong_t ulSize, ulong_t ulFlags)
{
    int           iShmId, iShmflg;
    key_t         key;
    void         *pShm = NULL;

    key = ftok(name, id);
    if ( -1 == key )
    {
        
		printf("%s:ftok failed errno:%d(%s).", __func__, errno, strerror(errno));
        return NULL;
    }

    iShmflg = 0;
    if ( ulFlags & VOS_MEM_SHARE_FLAG_CREATE )
    {
        iShmflg |= IPC_CREAT;
    }
    if ( ulFlags & VOS_MEM_SHARE_FLAG_RONLY )
    {
        iShmflg |= S_IRUSR;
    }
    else
    {
        iShmflg |= (S_IRUSR |S_IWGRP); 
    }
    iShmId = shmget(key, ulSize, iShmflg);
    if ( -1 == iShmId )
    {
        
        printf("%s:shmget failed errno:%d(%s).", __func__, errno, strerror(errno));
        return NULL;
    }

    iShmflg = 0;
    if ( ulFlags & VOS_MEM_SHARE_FLAG_RONLY )
    {
        iShmflg |= SHM_RDONLY;
    }
    
    pShm = shmat(iShmId, NULL, iShmflg);
    if ( (void *)-1 == pShm )
    {
        
       	printf("%s:shmat failed errno:%d(%s).",  __func__, errno, strerror(errno)); 
        return NULL;
    }

    return pShm;
}
void *vosDmaMalloc(ulong_t ulSize, ulong_t ulFlags)
{
    return malloc(ulSize);
}
void vosDmaFree(void *ptr)
{
    free(ptr);
}

