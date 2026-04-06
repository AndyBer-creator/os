
#ifndef __CLI_MAIN_H__
#define __CLI_MAIN_H__

#include <common/sys_def.h>

#define CLI_ERR_HDL(op)                     \
do {                                        \
    int32 err;                              \
    if ((err = (int32)(op)) != SYS_ERR_OK) \
        cli_cmd_errHandle(err);             \
} while(0)

extern void cli_cmd_dump(void);

extern int32 cli_cmd_init(void);

extern int32 cli_cmd_errHandle(int cmdErr);

#endif 

