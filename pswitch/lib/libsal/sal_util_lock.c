
#include <sys/mman.h>
#include <netdb.h>
#include <dirent.h>
#include <libgen.h>
#include <libsal/sal_util.h>
#include <libsal/sal_sys.h>

static int32 g_locksem[SYS_USER_LOCK_END] = {-1};
static int32 g_processInit = -1;
struct sembuf _lock = {0, -1, SEM_UNDO};
struct sembuf _unlock = {0, 1, SEM_UNDO};

static void _sal_util_processLock_init(void)
{
    uint32 i = 0;

    for (i = 0; i < SYS_USER_LOCK_END; i++)
    {
        g_locksem[i] = -1;
    }

    g_processInit = 1;
}

int32 sal_util_lock_init(void)
{
    union semun sem_val;
    int32 ret;
    uint32 i = 0;

    for (i = 0; i < SYS_USER_LOCK_END; i++)
    {
        g_locksem[i] = semget((USER_LOCK_ID_BASE + i), 1, IPC_CREAT | IPC_EXCL | 0777);

        if (g_locksem[i] < 0)
        {
            SYS_PRINTF("[SEM] sem open fail error=%s\n", strerror(errno));
            continue;
        }

        SYS_MEM_CLEAR(sem_val);

        sem_val.val = 1;
        ret = semctl(g_locksem[i], 0, SETVAL, sem_val);

        if (ret < 0)
        {
            SYS_PRINTF("[SEM] sem ctrl fail error=%s\n", strerror(errno));
            continue;
        }
    }

    return SYS_ERR_OK;
}

int32 sal_util_sem_lock(sys_userlock_type_t type)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK((type >= SYS_USER_LOCK_END), SYS_ERR_INPUT);

    if (g_processInit < 0)
        _sal_util_processLock_init();

    if (g_locksem[type] < 0)
        g_locksem[type] = semget((USER_LOCK_ID_BASE + type), 1, IPC_CREAT | 0777);

    ret = semop(g_locksem[type], &_lock, 1);

    if (ret < 0)
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

int32 sal_util_sem_unlock(sys_userlock_type_t type)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK((type >= SYS_USER_LOCK_END), SYS_ERR_INPUT);

    if (g_processInit < 0)
        _sal_util_processLock_init();

    if (g_locksem[type] < 0)
        g_locksem[type] = semget((USER_LOCK_ID_BASE + type), 1, IPC_CREAT | 0777);

    ret = semop(g_locksem[type], &_unlock, 1);

    if (ret < 0)
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

