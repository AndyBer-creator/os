
#ifndef _INCLUDE_VOS_H_
#define _INCLUDE_VOS_H_
typedef unsigned char         uchar_t;
typedef unsigned short        ushort_t;
typedef unsigned int          uint_t;
typedef unsigned int         ulong_t; 
typedef unsigned int         BOOL;
typedef unsigned int         ULONG;
typedef unsigned short       USHORT; 
typedef unsigned char         UCHAR;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

typedef enum VOS_RET_tag{
  VOS_ERR_NONE          = 0,       
  VOS_ERR_FAIL          = -1,      
  VOS_ERR_PARAM        = -2,      
  VOS_ERR_NOTSUPPORT   = -3,      
  VOS_ERR_TIMEOUT      = -4,      
  VOS_ERR_NOMEMORY     = -5,      
  VOS_ERR_EXISTS       = -6,      
  VOS_ERR_INIT         = -7,      
  VOS_ERR_TOOLONG      = -8,       
  VOS_ERR_SYSTEM       = -9,       
  VOS_ERR_NOT_EXISTS  = -10,     
  VOS_ERR_STOP         = -11,     
  VOS_ERR_VALID         = -12,    
  VOS_ERR_TYPE         = -13,     
  VOS_ERR_CRC          = -14,     
  VOS_OSPF_AREA_NETWORK_EXSIT = 30,
  VOS_OSPF_AREA_AUTH_EXSIT,
  VOS_OSPF_IS_CLOSE,
  VOS_RIP_IS_CLOSE     = 50,
  VOS_ERR_MAX          = 10000
}VOS_RET;

typedef enum {
    DATA_OP_ADD     = 0x0,
    DATA_OP_DELETE,
    DATA_OP_GET,
    DATA_OP_GETNEXT
}DATA_OP_ENT;

typedef struct Vos_To_Sys_Err_s
{
    int   vos_err;
    int   sys_err;
} VosToSysErr_t;

#endif 

