
#ifndef _INCLUDE_VOSPROCESS_H_
#define _INCLUDE_VOSPROCESS_H_
#define GLN_PROCESS_MODE 

#define GLN_PROC_DIR "/sbin/"
#define GLN_MODULE_DIR "/lib/modules"
#define GLN_PROC_NAME_MAX_LEN 32 

#define GLN_PROCESS(entryname) main(int argc, char **argv)

#define VOS_PROCESS_STACKSIZE_DEFAULT 0 

#define PROCESS_PRIORITY_MIN             1
#define PROCESS_PRIORITY_MAX             99

#ifdef GLN_SYSTEM_LINUX
extern char *optarg;
extern int optind, opterr, optopt;
#define vosOpterr opterr
#define vosOptind optind
#define vosOptopt optopt
#define vosOptarg optarg
#endif

#define VOS_PID_SELF                    0

int vosProcessCreate(char *name, int iPriority, int iStackSize, char *args);

int vosProcessCreateWithNewFd(char *name, int iPriority, int iStackSize, int iNewIn, int iNewOut, int iNewErr);

int vosProcessDestroy(int iPid);

int vosProcessSched(int iPid, int iPriority, int iStackSize);

int vosProcessGetId(void);

int vosProcessGetNameByPid(int iPid, char *szName);

char *vosProcessGetName(void);

int vosProcessInit(char *pProcName);

typedef int (*vosProcessTerminateProc) (ulong_t ulValue);

int vosProcessRegTerminateProc(vosProcessTerminateProc proc);

int vosProcessCheck(int iPid);

int vosProcessDescGet(int iPid, int *piPrio, char szDesc[128]);

int vosGetopt(int argc, char *const argv[], const char *optstring);
#endif 

