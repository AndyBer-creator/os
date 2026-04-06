
#ifndef __CMD_TIME_H__
#define __CMD_TIME_H__

#define GET_MONTH(str , mon)                \
do{                                         \
    switch(str[0])                          \
    {                                       \
        case 'j':                           \
            if ('a' == str[1])              \
                mon = 0;                    \
            else if ('n' == str[2])         \
                mon = 5;                    \
            else if ('l' == str[2])         \
                mon = 6;                    \
            break;                          \
        case 'f':                           \
            mon = 1;                        \
            break;                          \
        case 'm':                           \
            if ('r' == str[2])              \
                mon = 2;                    \
            else if ('y' == str[2])         \
                mon = 4;                    \
            break;                          \
        case 'a':                           \
            if ('r' == str[2])              \
                mon = 3;                    \
            else if ('g' == str[2])         \
                mon = 7;                    \
            break;                          \
        case 's':                           \
            mon = 8;                        \
            break;                          \
        case 'o':                           \
            mon = 9;                        \
            break;                          \
        case 'n':                           \
            mon = 10;                       \
            break;                          \
        case 'd':                           \
            mon = 11;                       \
            break;                          \
        default:                            \
            break;                          \
    }                                       \
}while(0);

#define GET_WEEKDAY(str , wd)               \
do{                                         \
    switch(str[0])                          \
    {                                       \
        case 's':                           \
            if ('u' == str[1])              \
                wd = 0;                     \
            else if ('a' == str[1])         \
                wd = 6;                     \
            break;                          \
        case 'm':                           \
            wd = 1;                         \
            break;                          \
        case 't':                           \
            if ('u' == str[1])              \
                wd = 2;                     \
            else if ('h' == str[1])         \
                wd = 4;                     \
            break;                          \
        case 'w':                           \
                wd = 3;                     \
            break;                          \
        case 'f':                           \
            wd = 5;                         \
            break;                          \
        default:                            \
            break;                          \
    }                                       \
}while(0);

#define GET_HM(str , min, hour)         \
do{                                     \
    uint8 __i = 0;                      \
    char *__tmp = NULL;                 \
    char *__ridx = NULL;                \
    __tmp = str;                        \
    __ridx = rindex(__tmp, ':');        \
    if (NULL == __ridx)                 \
        return SYS_ERR_INPUT;           \
    if (__ridx == str)                  \
        return SYS_ERR_INPUT;           \
    *__ridx = '\0';                     \
    __ridx++;                           \
    for (__i = 0; __i < osal_strlen(__ridx) ; __i++)    \
    {                                   \
        if (!isdigit(__ridx[__i]))      \
            return SYS_ERR_INPUT;        \
    }                                    \
    min = SYS_STR2UINT(__ridx);          \
    if ( min > 59 )                      \
        return SYS_ERR_INPUT;            \
    for (__i = 0; __i < osal_strlen(__tmp) ; __i++)     \
    {                                    \
        if (!isdigit(__tmp[__i]))        \
            return SYS_ERR_INPUT;        \
    }                                    \
    hour = SYS_STR2UINT(__tmp);          \
    if ( hour > 23 )                     \
        return SYS_ERR_INPUT;            \
}while(0);

extern int32 cmd_time_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_time_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_APP_MSNTP

extern int32 cmd_time_sntp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_APP_MSNTP

extern int32 cmd_time_sntpEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_time_sntp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_time_timezone_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_time_dls_date_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_time_dls_rec_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_time_save(FILE *fp);

#endif 

