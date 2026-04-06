
#ifndef __DEFDB_H__
#define __DEFDB_H__

#include <common/sys_def.h>
#include <defdb/defdb_data.h>

#define DEFDB_MAJOR 126
#define DEFDB_NAME  "defdb"
#define DEFDB_DEV   "/dev/"DEFDB_NAME

typedef enum def_type_e
{
    DEF_TYPE_INT = 0,
    DEF_TYPE_UINT,
    DEF_TYPE_STR,
    DEF_TYPE_END
} def_type_t;

typedef struct defdb_tok_s
{
    uint16 type; 
    uint16 size;
    uint32 offset;
} defdb_tok_t;

#ifdef __KERNEL__
#define DEFDB_VAL(key) defdb_val_ret(key)
#define DEFDB_STR(key) defdb_str_ret(key)
#else
#define DEFDB_VAL(key) sal_util_defdbVal_ret(key)
#define DEFDB_STR(key) sal_util_defdbStr_ret(key)
#endif

extern uint32 defdb_val_ret(def_t key);
extern char *defdb_str_ret(def_t key);

#endif  

