
#include <include/cgi.h>
#include <libsal/sal_dos.h>

static int32 cgi_dos_prop_get(void);
static int32 cgi_dos_port_get(void);
static int32 cgi_dos_portEdit_get(void);

 const cgi_cmdmap_t cmdGetDosTbl[] =
 {
     {CGI_CMD_DOS_PROP,             cgi_dos_prop_get},
     {CGI_CMD_DOS_PORT,             cgi_dos_port_get},
     {CGI_CMD_DOS_PORTEDIT,         cgi_dos_portEdit_get},
     {NULL, NULL}
 };

static int32 cgi_dos_prop_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    sys_enable_t            enable;
    uint32                  length;

    CGI_GET_JSON_CREATE(root, data);

    sal_dos_typeEnable_get(DOS_SYN_SPORTL1024, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_TCPSYNSPORT1024, enable);
    sal_dos_typeEnable_get(DOS_TCP_FRAG_OFF_MIN, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_TCPFRAGOFFMINCHK, enable);
    sal_dos_typeEnable_get(DOS_XMA, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_XMA, enable);
    sal_dos_typeEnable_get(DOS_NULLSCAN, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_NULLSCAN, enable);
    sal_dos_typeEnable_get(DOS_ICMP_FRAG_PKTS, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_ICMPFRAGPKT, enable);
    sal_dos_typeEnable_get(DOS_POD, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_POD, enable);
    sal_dos_typeEnable_get(DOS_TCPBLAT, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_TCPBLAT, enable);
    sal_dos_typeEnable_get(DOS_UDPBLAT, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_UDPBLAT, enable);
    sal_dos_typeEnable_get(DOS_LAND, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_LAND, enable);
    sal_dos_typeEnable_get(DOS_DAEQSA, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_DAEQSA, enable);
    sal_dos_typeEnable_get(DOS_SYNFIN, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_SYNFIN, enable);
    sal_dos_typeEnable_get(DOS_SYNRST, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_SYNRST, enable);

    sal_dos_typeEnable_get(DOS_TCPHDR_MIN, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_TCPHDRMINCHK, enable);
    sal_dos_minTCPHdrLen_get(&length);
    cJSON_AddNumberToObject(data, CGI_DOS_TCPHDRMINCHKLENGTH, length);
    cJSON_AddNumberToObject(data, CGI_DOS_TCPHDRMINCHKLENGTH_MIN, BOND_DOS_TCPHDRMINCHKLENGTH_MIN);
    cJSON_AddNumberToObject(data, CGI_DOS_TCPHDRMINCHKLENGTH_MAX, BOND_DOS_TCPHDRMINCHKLENGTH_MAX);
    cJSON_AddNumberToObject(data, CGI_DOS_TCPHDRMINCHKLENGTH_DFLT, DFLT_DOS_MINTCPHDRLEN);

    sal_dos_typeEnable_get(DOS_IPV4_PING_MAX, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_V4PINGMAX, enable);
    sal_dos_typeEnable_get(DOS_IPV6_PING_MAX, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_V6PINGMAX, enable);
    sal_dos_maxPingLen_get(&length);
    cJSON_AddNumberToObject(data, CGI_DOS_PINGMAXLENGTH, length);
    cJSON_AddNumberToObject(data, CGI_DOS_PINGMAXLENGTH_MIN, BOND_DOS_PINGMAXLENGTH_MIN);
    cJSON_AddNumberToObject(data, CGI_DOS_PINGMAXLENGTH_MAX, BOND_DOS_PINGMAXLENGTH_MAX);
    cJSON_AddNumberToObject(data, CGI_DOS_PINGMAXLENGTH_DFLT, DFLT_DOS_MAXPINGLEN);

    sal_dos_typeEnable_get(DOS_SMURF, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_SMURF, enable);
    sal_dos_smurfNetmaskLen_get(&length);
    cJSON_AddNumberToObject(data, CGI_DOS_SMURFLENGTH, length);
    cJSON_AddNumberToObject(data, CGI_DOS_SMURFLENGTH_MIN, BOND_DOS_SMURFLENGTH_MIN);
    cJSON_AddNumberToObject(data, CGI_DOS_SMURFLENGTH_MAX, BOND_DOS_SMURFLENGTH_MAX);
    cJSON_AddNumberToObject(data, CGI_DOS_SMURFLENGTH_DFLT, DFLT_DOS_SMURFNETMASKLEN);

    sal_dos_typeEnable_get(DOS_IPV6_MIN_FRAG_SIZE, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_IPV6MINFRAG, enable);
    sal_dos_minIPv6FragLen_get(&length);
    cJSON_AddNumberToObject(data, CGI_DOS_IPV6MINFRAGLENGTH, length);
    cJSON_AddNumberToObject(data, CGI_DOS_IPV6MINFRAGLENGTH_MIN, BOND_DOS_IPV6MINFRAGLENGTH_MIN);
    cJSON_AddNumberToObject(data, CGI_DOS_IPV6MINFRAGLENGTH_MAX, BOND_DOS_IPV6MINFRAGLENGTH_MAX);
    cJSON_AddNumberToObject(data, CGI_DOS_IPV6MINFRAGLENGTH_DFLT, DFLT_DOS_MINIPV6FRAGLEN);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_dos_port_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    sys_enable_t enable = DISABLED;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_dos_portAttackPreventEnable_get(lp, &enable);
        cJSON_AddBoolToObject(port, CGI_DOS_STATUS, enable);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_dos_portEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_logic_port_t lp = 0;
    sys_enable_t enable = DISABLED;
    char *value = NULL;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_dos_portAttackPreventEnable_get(lp, &enable);
    cJSON_AddBoolToObject(data, CGI_DOS_STATUS, enable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

