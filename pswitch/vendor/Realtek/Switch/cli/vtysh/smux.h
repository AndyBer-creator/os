
#ifndef _ZEBRA_SNMP_H
#define _ZEBRA_SNMP_H

#define SMUX_PORT_DEFAULT 199

#define SMUXMAXPKTSIZE    1500
#define SMUXMAXSTRLEN      256

#define SMUX_OPEN       (ASN_APPLICATION | ASN_CONSTRUCTOR | 0)
#define SMUX_CLOSE      (ASN_APPLICATION | ASN_PRIMITIVE | 1)
#define SMUX_RREQ       (ASN_APPLICATION | ASN_CONSTRUCTOR | 2)
#define SMUX_RRSP       (ASN_APPLICATION | ASN_PRIMITIVE | 3)
#define SMUX_SOUT       (ASN_APPLICATION | ASN_PRIMITIVE | 4)

#define SMUX_GET        (ASN_CONTEXT | ASN_CONSTRUCTOR | 0)
#define SMUX_GETNEXT    (ASN_CONTEXT | ASN_CONSTRUCTOR | 1)
#define SMUX_GETRSP     (ASN_CONTEXT | ASN_CONSTRUCTOR | 2)
#define SMUX_SET    (ASN_CONTEXT | ASN_CONSTRUCTOR | 3)
#define SMUX_TRAP    (ASN_CONTEXT | ASN_CONSTRUCTOR | 4)

#define SMUX_MAX_FAILURE 3

#define MATCH_FAILED     (-1)
#define MATCH_SUCCEEDED  0

#define SNMP_TRUE  1
#define SNMP_FALSE 2

#define SNMP_VALID  1
#define SNMP_INVALID 2

#define IN_ADDR_SIZE sizeof(struct in_addr)

struct variable;

#define REGISTER_MIB(descr, var, vartype, theoid)        \
    smux_register_mib(descr, (struct variable *)var, sizeof(struct vartype), \
    sizeof(var)/sizeof(struct vartype),            \
    theoid, sizeof(theoid)/sizeof(oid))

typedef int (WriteMethod)(int action,
              u_char  *var_val,
              u_char   var_val_type,
              size_t   var_val_len,
              u_char  *statP,
              oid     *name,
              size_t   length,
              struct variable *v);

typedef u_char *(FindVarMethod)(struct variable *v,
                oid     *name,
                size_t  *length,
                int      exact,
                size_t  *var_len,
                WriteMethod   **write_method);

struct variable
{
  
  u_char magic;

  char type;

  u_short acl;

  FindVarMethod *findVar;

  int namelen;
  oid name[MAX_OID_LEN];
};

struct subtree
{
  
  oid name[MAX_OID_LEN];
  u_char name_len;

  struct variable *variables;

  int variables_num;

  int variables_width;

  int registered;
};

struct trap_object
{
  FindVarMethod *findVar;
  int namelen;
  oid name[MAX_OID_LEN];
};

#define SNMP_LOCAL_VARIABLES \
  static long snmp_int_val; \
  static struct in_addr snmp_in_addr_val;

#define SNMP_INTEGER(V) \
  ( \
    *var_len = sizeof (snmp_int_val), \
    snmp_int_val = V, \
    (u_char *) &snmp_int_val \
  )

#define SNMP_IPADDRESS(V) \
  ( \
    *var_len = sizeof (struct in_addr), \
    snmp_in_addr_val = V, \
    (u_char *) &snmp_in_addr_val \
  )

extern void smux_init (struct thread_master *tm);
extern void smux_start (void);
extern void smux_register_mib(const char *, struct variable *,
                              size_t, int, oid [], size_t);
extern int smux_header_generic (struct variable *, oid [], size_t *,
                                int, size_t *, WriteMethod **);
extern int smux_trap (const oid *, size_t, const oid *, size_t,
              const struct trap_object *,
                      size_t, unsigned int, u_char);
extern int oid_compare (oid *, int, oid *, int);
extern void oid2in_addr (oid [], int, struct in_addr *);
extern void *oid_copy (void *, const void *, size_t);
extern void oid_copy_addr (oid [], struct in_addr *, int);

#endif 
