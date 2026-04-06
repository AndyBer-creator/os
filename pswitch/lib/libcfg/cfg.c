
#include <sys/shm.h>
#include <sys/sem.h>

#include <libcfg/cfg.h>
#include <libcfgtbl/cfgtbl.h>

#define CFG_TBL_SIZE ((cfgtbl_size_ret() / sizeof(cfg_tbl_t) - 1))
#define CFG_ARRD_MAP_SIZE (CFG_TBL_SIZE * sizeof(cfg_map_t))

cfg_addr glb_shmptr = NULL;
uint32 glb_unit = 0;
int32 glb_semid[CFG_ID_END] = {-1};
struct sembuf lock = {0, -1, SEM_UNDO};
struct sembuf unlock = {0, 1, SEM_UNDO};

static int32 _cfg_totalStrutSize_get(uint32 *size);
static int32 _cfg_sem_create(cfg_id_t cfgid);
static int32 _cfg_sem_lock(cfg_id_t cfgid);
static int32 _cfg_sem_unlock(cfg_id_t cfgid);
static int32 _cfg_shm_alloc(void);
static int32 _cfg_shm_open(void);
#if 0

static int32 _cfg_sem_unlink(cfg_id_t cfgid)
static int32 _cfg_shm_unlink(void);
#endif

int32 cfg_shm_init(void)
{
    uint32 i;
    cfg_map_t *cfgmap;
    cfg_addr map_offset;
    cfg_addr strut_offset;

    for(i = 0; i < CFG_TBL_SIZE; i++)
    {
        SYS_ERR_CHK(_cfg_sem_create(i));
    }
    
    SYS_ERR_CHK(_cfg_shm_alloc());

    map_offset = glb_shmptr;
    strut_offset = map_offset + CFG_ARRD_MAP_SIZE;
    
    osal_memset(strut_offset, 0, cfgtbl_size_ret());
    for(i = 0; i < CFG_TBL_SIZE; i++)
    {
        cfgmap = map_offset;
        cfgmap->addr = strut_offset - glb_shmptr;
        map_offset += sizeof(cfg_map_t);
        strut_offset += cfgTbl[i].size;
    }

    return SYS_ERR_OK;
}

int32 cfg_currUnitId_set(uint32 unit)
{
    glb_unit = unit;

    return SYS_ERR_OK;
}

uint32 cfg_currUnitId_ret(void)
{
    return glb_unit;
}

int32 cfg_open(cfg_id_t id, cfg_addr *addr)
{
    cfg_map_t *cfgmap;

    if(NULL == glb_shmptr)
    {
        SYS_ERR_CHK(_cfg_shm_open());
    }

    SYS_ERR_CHK(_cfg_sem_lock(id));

    cfgmap = glb_shmptr + (sizeof(cfg_map_t) * id);
    *addr = glb_shmptr + cfgmap->addr;

    return SYS_ERR_OK;
}

int32 cfg_close(cfg_id_t id)
{
    
    SYS_ERR_CHK(_cfg_sem_unlock(id));

    return SYS_ERR_OK;
}

int32 cfg_shm_dump(void)
{
    uint32 count, i;
    uint32 structsize;
    uint8 *tmp;
    uint32 cfgid;
    cfg_map_t *cfgmap;
    cfg_addr map_offset;

    cfg_addr strut_offset;
    cfg_addr end_addr;

    if (glb_shmptr == NULL)
    {
        SYS_ERR_CHK(_cfg_shm_open());
    }

    map_offset = glb_shmptr;
    
    strut_offset = map_offset + CFG_ARRD_MAP_SIZE;
    _cfg_totalStrutSize_get(&structsize);

    SYS_PRINTF("======DUMP ADDR MAP======\n");
    cfgid = 0;
    while((map_offset - glb_shmptr) < CFG_ARRD_MAP_SIZE)
    {
        cfgmap = map_offset;
        SYS_PRINTF("CFG_ID_%d OFFSET=%x\n", cfgid, cfgmap->addr);
        map_offset += sizeof(cfg_map_t);
        cfgid++;
    }

    SYS_PRINTF("======DUMP STRUCTURE BY ID======\n");
    for(i = 0; i < CFG_TBL_SIZE; i++)
    {

        SYS_ERR_CHK(_cfg_sem_lock(i));
        
        SYS_PRINTF("~~~~~~ CFG_ID_%d ADDR=%p ~~~~~~\n", cfgTbl[i].id, strut_offset);
        end_addr = strut_offset + cfgTbl[i].size;
        count = 0;
        while(strut_offset < end_addr)
        {
            count++;
            tmp = strut_offset;
            SYS_PRINTF("%02x ", *(tmp));
            if((count % 30) == 0)
            {
                SYS_PRINTF("\n");
            }
            strut_offset++;
        }
        SYS_PRINTF("\n");
        
        SYS_ERR_CHK(_cfg_sem_unlock(i));
    }
#if 0
    
    SYS_PRINTF("======DUMP STRUCTURE ALL======\n");
    count = 0;
    strut_offset = strut_start;
    while(strut_offset <= strut_end)
    {
        count++;
        tmp = strut_offset;
        SYS_PRINTF("%02x ", *(tmp));
        if((count % 30) == 0)
        {
            SYS_PRINTF("\n");
        }
        strut_offset++;
    }
    SYS_PRINTF("\n");
#endif

    return SYS_ERR_OK;
}

static int32 _cfg_totalStrutSize_get(uint32 *size)
{
    uint32 i;

    *size = 0;
    for (i = 0; i < CFG_TBL_SIZE; i++)
        *size += (cfgTbl[i].size);

    return SYS_ERR_OK;
}

static int32 _cfg_sem_create(cfg_id_t cfgid)
{
    union semun sem_val;
    int32 ret;

    glb_semid[cfgid] = semget(cfgid + 1, 1, IPC_CREAT | IPC_EXCL | 0777);
    if (glb_semid[cfgid] < 0)
    {
        SYS_PRINTF("[SEM] sem open fail error=%s\n", strerror(errno));
        return SYS_ERR_FAILED;
    }

    sem_val.val = 1;
    ret = semctl(glb_semid[cfgid], 0, SETVAL, sem_val);
    if (ret < 0)
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

static int32 _cfg_sem_lock(cfg_id_t cfgid)
{
    int32 ret;

    if (glb_semid[cfgid] < 0)
        glb_semid[cfgid] = semget(cfgid + 1, 1, IPC_CREAT | 0777);

    ret = semop(glb_semid[cfgid], &lock, 1);

    if (ret < 0)
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

static int32 _cfg_sem_unlock(cfg_id_t cfgid)
{
    int32 ret;

    if (glb_semid[cfgid] < 0)
        glb_semid[cfgid] = semget(cfgid + 1, 1, IPC_CREAT | 0777);

    ret = semop(glb_semid[cfgid], &unlock, 1);

    if (ret < 0)
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

static int32 _cfg_shm_alloc(void)
{
    int32 shmid;
    uint32 size;
    uint32 structsize;

    _cfg_totalStrutSize_get(&structsize);
    size = CFG_ARRD_MAP_SIZE + structsize ;

    shmid = shmget(ftok(CFG_SHM_PATH, 1), size, (IPC_CREAT | IPC_EXCL | 0777));
    if(shmid < 0)
    {
        SYS_PRINTF("[SHM] shm open fail error=%s\n", strerror(errno));
        return SYS_ERR_FAILED;
    }
    glb_shmptr = shmat(shmid, NULL, 0);
    osal_memset(glb_shmptr, 0, size);

    return SYS_ERR_OK;
}

static int32 _cfg_shm_open(void)
{
    int32 shmid;

    shmid = shmget(ftok(CFG_SHM_PATH, 1), 0, (IPC_CREAT | 0777));
    if(shmid < 0)
    {
        SYS_PRINTF("[SHM] shm open fail error=%s\n", strerror(errno));
        return SYS_ERR_FAILED;
    }
    glb_shmptr = shmat(shmid, NULL, 0);
    if(glb_shmptr == NULL)
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

#if 0

static int32 _cfg_sem_unlink(cfg_id_t cfgid)
{
    if (glb_semid[cfgid] < 0)
        glb_semid[cfgid] = semget(cfgid + 1, 1, IPC_CREAT | 0777);

    semctl(glb_semid[cfgid], 0, IPC_RMID);
    return SYS_ERR_OK;
}

static int32 _cfg_shm_unlink(void)
{
    int shmid;

    shmid = shmget(ftok(CFG_SHM_PATH, 1), 0, (IPC_CREAT | 0777));
    shmctl(shmid, IPC_RMID, NULL);

    return SYS_ERR_OK;
}
#endif

