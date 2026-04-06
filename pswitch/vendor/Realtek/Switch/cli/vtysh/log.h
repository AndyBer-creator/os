
#ifndef _ZEBRA_LOG_H
#define _ZEBRA_LOG_H

#include <syslog.h>

typedef enum
{
  ZLOG_NONE,
  ZLOG_DEFAULT,
  ZLOG_ZEBRA,
  ZLOG_RIP,
  ZLOG_BGP,
  ZLOG_OSPF,
  ZLOG_RIPNG,
  ZLOG_OSPF6,
  ZLOG_ISIS,
  ZLOG_MASC
} zlog_proto_t;

#define ZLOG_DISABLED    (LOG_EMERG-1)

typedef enum
{
  ZLOG_DEST_SYSLOG = 0,
  ZLOG_DEST_STDOUT,
  ZLOG_DEST_MONITOR,
  ZLOG_DEST_FILE
} zlog_dest_t;
#define ZLOG_NUM_DESTS        (ZLOG_DEST_FILE+1)

struct zlog
{
  const char *ident;    
  zlog_proto_t protocol;
  int maxlvl[ZLOG_NUM_DESTS];    
  int default_lvl;    
  FILE *fp;
  char *filename;
  int facility;        
  int record_priority;    
  int syslog_options;    
  int timestamp_precision;    
};

struct message
{
  int key;
  const char *str;
};

extern struct zlog *zlog_default;

extern struct zlog *openzlog (const char *progname, zlog_proto_t protocol,
                      int syslog_options, int syslog_facility);

extern void closezlog (struct zlog *zl);

#ifdef __GNUC__
#define PRINTF_ATTRIBUTE(a,b) __attribute__ ((__format__ (__printf__, a, b)))
#else
#define PRINTF_ATTRIBUTE(a,b)
#endif 

extern void zlog (struct zlog *zl, int priority, const char *format, ...)
  PRINTF_ATTRIBUTE(3, 4);

extern void zlog_err (const char *format, ...) PRINTF_ATTRIBUTE(1, 2);
extern void zlog_warn (const char *format, ...) PRINTF_ATTRIBUTE(1, 2);
extern void zlog_info (const char *format, ...) PRINTF_ATTRIBUTE(1, 2);
extern void zlog_notice (const char *format, ...) PRINTF_ATTRIBUTE(1, 2);
extern void zlog_debug (const char *format, ...) PRINTF_ATTRIBUTE(1, 2);

extern void plog_err (struct zlog *, const char *format, ...)
  PRINTF_ATTRIBUTE(2, 3);
extern void plog_warn (struct zlog *, const char *format, ...)
  PRINTF_ATTRIBUTE(2, 3);
extern void plog_info (struct zlog *, const char *format, ...)
  PRINTF_ATTRIBUTE(2, 3);
extern void plog_notice (struct zlog *, const char *format, ...)
  PRINTF_ATTRIBUTE(2, 3);
extern void plog_debug (struct zlog *, const char *format, ...)
  PRINTF_ATTRIBUTE(2, 3);

extern void zlog_set_level (struct zlog *zl, zlog_dest_t, int log_level);

extern int zlog_set_file (struct zlog *zl, const char *filename, int log_level);

extern int zlog_reset_file (struct zlog *zl);

extern int zlog_rotate (struct zlog *);

#define LOOKUP(x, y) mes_lookup(x, x ## _max, y, "(no item found)")

extern const char *lookup (const struct message *, int);
extern const char *mes_lookup (const struct message *meslist,
                               int max, int index,
                               const char *no_item);

extern const char *zlog_priority[];
extern const char *zlog_proto_names[];

extern const char *safe_strerror(int errnum);

extern void zlog_signal(int signo, const char *action
#ifdef SA_SIGINFO
            , siginfo_t *siginfo, void *program_counter
#endif
               );

extern void zlog_backtrace(int priority);

extern void zlog_backtrace_sigsafe(int priority, void *program_counter);

extern size_t quagga_timestamp(int timestamp_precision ,
                   char *buf, size_t buflen);

struct timestamp_control {
   size_t len;        
   int precision;    
   int already_rendered; 
   char buf[40];    
};

#define LOG_LEVELS "(emergencies|alerts|critical|errors|warnings|notifications|informational|debugging)"

#define LOG_LEVEL_DESC \
  "System is unusable\n" \
  "Immediate action needed\n" \
  "Critical conditions\n" \
  "Error conditions\n" \
  "Warning conditions\n" \
  "Normal but significant conditions\n" \
  "Informational messages\n" \
  "Debugging messages\n"

#define LOG_FACILITIES "(kern|user|mail|daemon|auth|syslog|lpr|news|uucp|cron|local0|local1|local2|local3|local4|local5|local6|local7)"

#define LOG_FACILITY_DESC \
       "Kernel\n" \
       "User process\n" \
       "Mail system\n" \
       "System daemons\n" \
       "Authorization system\n" \
       "Syslog itself\n" \
       "Line printer system\n" \
       "USENET news\n" \
       "Unix-to-Unix copy system\n" \
       "Cron/at facility\n" \
       "Local use\n" \
       "Local use\n" \
       "Local use\n" \
       "Local use\n" \
       "Local use\n" \
       "Local use\n" \
       "Local use\n" \
       "Local use\n"

#endif 
