
#ifndef __SCHED_H__
#define __SCHED_H__
#include <common/sys_def.h>
typedef enum sys_task_scheduler_rec_type_e
{
    SCHED_REC_TYPE_INVALID = 0,
    SCHED_REC_TYPE_SINGLE,
    SCHED_REC_TYPE_LIST,
    SCHED_REC_TYPE_END,
} sys_task_scheduler_rec_type_t;

#define SCHED_SINGLE_SIZE 6
#define SCHED_LIST_SIZE 11

typedef struct sys_task_scheduler_rec_s
{
    union
    {
        struct
        {
            uint8 ts_rec_wday_s;    
            uint8 ts_rec_hour_s;    
            uint8 ts_rec_min_s;     

            uint8 ts_rec_wday_e;      
            uint8 ts_rec_hour_e;      
            uint8 ts_rec_min_e;       

            int16 ts_rec_s;
            int16 ts_rec_e;
        }single;

        struct
        {
            uint8 ts_rec_wday[7];   
            uint8 ts_rec_hour_s;    
            uint8 ts_rec_min_s;     

            uint8 ts_rec_hour_e;      
            uint8 ts_rec_min_e;       

            uint8 reverse[7];
            int16 ts_rec_s[7];
            int16 ts_rec_e[7];
        }list;
    };

    sys_task_scheduler_rec_type_t type;
    sys_namestr_t *name;
    struct sys_task_scheduler_rec_s *prev;
    struct sys_task_scheduler_rec_s *next;
} sys_task_scheduler_rec_t;

typedef struct sys_task_scheduler_rec_list_s
{

    sys_namestr_t *name;
    struct sys_task_scheduler_rec_list_s *prev;
    struct sys_task_scheduler_rec_list_s *next;
} sys_task_scheduler_rec_list_t;

typedef struct sys_task_scheduler_s
{
    sys_namestr_t name;
    uint8 isAbsOn;
    uint8 isRecOn;
    uint8 isOn;
    uint8 usedCnt;
    uint32 ts_abs_start;
    uint32 ts_abs_end;

    uint16 ts_abs_year_s;   
    uint8 ts_abs_month_s;   
    uint8 ts_abs_date_s;    
    uint8 ts_abs_hour_s;    
    uint8 ts_abs_min_s;     

    uint16 ts_abs_year_e;   
    uint8 ts_abs_month_e;   
    uint8 ts_abs_date_e;    
    uint8 ts_abs_hour_e;    
    uint8 ts_abs_min_e;     

    struct sys_task_scheduler_s *prev;
    struct sys_task_scheduler_s *next;
    sys_task_scheduler_rec_t *tsRecHead;
} sys_task_scheduler_t;

typedef struct sys_task_scheduler_stat_s
{
    sys_namestr_t name;
    uint8 isOn;
} sys_task_scheduler_stat_t;
extern int32 sched_entry_create(sys_namestr_t name);
extern int32 sched_entry_del(sys_namestr_t name);
extern int32 sched_entry_get(sys_task_scheduler_t *ts);
extern int32 sched_entry_stat_get(sys_task_scheduler_stat_t *stat);
extern int32 sched_entry_register(sys_namestr_t name);
extern int32 sched_entry_unregister(sys_namestr_t name);
extern int32 sched_entry_set(sys_task_scheduler_t *ts);
extern int32 sched_entry_rec_set(sys_namestr_t *tsname, sys_task_scheduler_rec_t *rec);
extern int32 sched_entry_rec_del(sys_namestr_t *tsname, sys_task_scheduler_rec_t *rec);
extern int32 sched_entry_rec_get(sys_namestr_t *tsname, sys_task_scheduler_rec_t *rec);
extern int32 sched_entryNameList_get(sys_namestr_t *names);
extern int32 sched_time_tz_set(int32 diff);
extern int32 sched_time_dls_set(uint32 start, uint32 end, int32 diff);
extern int32 scheduler_init(void);
#endif  

