
#include <include/cgi.h>

static int32 cgi_lldp_prop_get(void);
static int32 cgi_lldp_port_get(void);
static int32 cgi_lldp_portEdit_get(void);
#ifdef CONFIG_SYS_PROTO_LLDP_MED
static int32 cgi_lldp_medPolicy_get(void);
static int32 cgi_lldp_medPolicyAdd_get(void);
static int32 cgi_lldp_medPolicyEdit_get(void);
static int32 cgi_lldp_medPort_get(void);
static int32 cgi_lldp_medPortEdit_get(void);
#endif
static int32 cgi_lldp_local_get(void);
static int32 cgi_lldp_localDetail_get(void);
static int32 cgi_lldp_neighbor_get(void);
static int32 cgi_lldp_neighborDetail_get(void);
static int32 cgi_lldp_statistic_get(void);
static int32 cgi_lldp_overloading_get(void);
static int32 cgi_lldp_overloadingDetail_get(void);

const cgi_cmdmap_t cmdGetLldpTbl[] =
{
    {CGI_CMD_LLDP_PROP,    cgi_lldp_prop_get},
    {CGI_CMD_LLDP_PORT,    cgi_lldp_port_get},
    {CGI_CMD_LLDP_PORTEDIT,    cgi_lldp_portEdit_get},
#ifdef CONFIG_SYS_PROTO_LLDP_MED
    {CGI_CMD_LLDP_MEDPOLICY,    cgi_lldp_medPolicy_get},
    {CGI_CMD_LLDP_MEDPOLICYADD,    cgi_lldp_medPolicyAdd_get},
    {CGI_CMD_LLDP_MEDPOLICYEDIT,    cgi_lldp_medPolicyEdit_get},
    {CGI_CMD_LLDP_MEDPORT,    cgi_lldp_medPort_get},
    {CGI_CMD_LLDP_MEDPORTEDIT,    cgi_lldp_medPortEdit_get},
#endif
    {CGI_CMD_LLDP_LOCAL,    cgi_lldp_local_get},
    {CGI_CMD_LLDP_LOCALDETAIL,    cgi_lldp_localDetail_get},
    {CGI_CMD_LLDP_NEIGHBOR,    cgi_lldp_neighbor_get},
    {CGI_CMD_LLDP_NEIGHBORDETAIL,    cgi_lldp_neighborDetail_get},
    {CGI_CMD_LLDP_STATISTIC,    cgi_lldp_statistic_get},
    {CGI_CMD_LLDP_OVERLOAD,    cgi_lldp_overloading_get},
    {CGI_CMD_LLDP_OVERLOADDETAIL,    cgi_lldp_overloadingDetail_get},
    {NULL, NULL}
};

char *langPolicy[] =
{
    "lblUnknown",
    "lblVoice",
    "lblVoiceSignaling",
    "lblGuestVoice",
    "lblGuestVoiceSignaling",
    "lblSoftphoneVoice",
    "lblVideoConferencing",
    "lblStreamingVideo",
    "lblVideoSignaling",
};

char *langPoEType[] =
{
    "lblPowerSourcingEntity",
    "lblPoweredDevice",
};

char *langPoEPSESource[] =
{
    "lblUnknown",
    "lblPrimaryPowerSource",
    "lblBackupPowerSource",
};

char *langPoEPDSource[] =
{
    "lblUnknown",
    "lblFromPSE",
    "lblLocal",
    "lblLocalAndPSE",
};

char *langPri[] =
{
    "lblUnknown",
    "lblCritical",
    "lblHigh",
    "lblLow",
};

char *langChassisType[] =
{
    "lblUnknown",
    "lblChassisComponent",
    "lblIntfAlias",
    "lblPortComponent",
    "lblMacAddr",
    "lblNetworkAddr",
    "lblIntfName",
    "lblLocal",
};

char *langPortIDType[] =
{
    "lblUnknown",
    "lblIntfAlias",
    "lblPortComponent",
    "lblMacAddr",
    "lblNetworkAddr",
    "lblIntfName",
    "lblAgentCircuitID",
    "lblLocal",
};

char *langManAddrType[] =
{
    "lblOther",
    "lblIPv4",
    "lblIPv6",
    "lblNsap",
    "lblHdlc",
    "lblBbn1822",
    "lblAll802",
    "lblE163",
    "lblE164",
    "lblF69",
    "lblX121",
    "lblIpx",
    "lblAppleTalk",
    "lblDecentIV",
    "lblBanyanVines",
    "lblE164WithNsap",
    "lblDNS",
    "lblDistinguishedName",
    "lblAsNum",
    "lblXtpOverIpv4",
    "lblXtpOverIpv6",
    "lblXtpNativeModeXtp",
    "lblSysPortNum",
};

char *langManAddrIfSubType[] =
{
    "lblUnknown",
    "lblUnknown",
    "lblIfIndex",
    "lblSysPortNum",
};

char *langPhyCapa[] =
{
    "lbl1000BaseTFD",
    "lbl1000BaseT",
    "lbl1000BaseXFD",
    "lbl1000BaseX",
    "lblFdxBPause",
    "lblFdxSPause",
    "lblFdxAPause",
    "lblFdxPause",
    "lbl100BaseT2FD",
    "lbl100BaseT2",
    "lbl100BaseTXFD",
    "lbl100BaseTX",
    "lbl100BaseT4",
    "lbl10BaseTFD",
    "lbl10BaseT",
    "lblOther",
};

char *langPhyOper[] =
{
    "lblOther",
    "lblMauAui",
    "lblMau10Base5",
    "lblMau10Foirl",
    "lblMau10Base2",
    "lblMau10BaseT",
    "lblMau10BaseFP",
    "lblMau10BaseFB",
    "lblMau10BaseFL",
    "lblMau10Broad36",
    "lblMau10BaseTHD",
    "lblMau10BaseTFD",
    "lblMau10BaseFLHD",
    "lblMau10BaseFLFD",
    "lblMau100BaseT4",
    "lblMau100BaseTXHD",
    "lblMau100BaseTXFD",
    "lblMau100BaseFXHD",
    "lblMau100BaseFXFD",
    "lblMau100BaseT2HD",
    "lblMau100BaseT2FD",
    "lblMau1000BaseXHD",
    "lblMau1000BaseXFD",
    "lblMau1000BaseLXHD",
    "lblMau1000BaseLXFD",
    "lblMau1000BaseSXHD",
    "lblMau1000BaseSXFD",
    "lblMau1000BaseCXHD",
    "lblMau1000BaseCXFD",
    "lblMau1000BaseTHD",
    "lblMau1000BaseTFD",
    "lblMau10GigBaseX",
    "lblMau10GigBaseLX4",
    "lblMau10GigBaseR",
    "lblMau10GigBaseER",
    "lblMau10GigBaseLR",
    "lblMau10GigBaseSR",
    "lblMau10GigBaseW",
    "lblMau10GigBaseEW",
    "lblMau10GigBaseLW",
    "lblMau10GigBaseSW",
};

char *langAggCapa[] =
{
    "lblNotCapable",
    "lblCapable",
};

char *langAggState[] =
{
    "lblNotAggr",
    "lblAggr",
};

char *langCapa[] =
{
    "lblOther",
    "lblRepeater",
    "lblBridge",
    "lblWLANAccessPoint",
    "lblRouter",
    "lblTelephone",
    "lblDOCSISCableDevice",
    "lblStationOnly"
};

char *langMedCapa[] =
{
    "lblCapabilities",
    "lblNetworkPolicy",
    "lblLocation",
    "lblExtPSE",
    "lblExtPD",
    "lblInventory"
};

char *langDevClass[] =
{
    "lblNotDef",
    "lblEndClass1",
    "lblEndClass2",
    "lblEndClass3",
    "lblNetCon",
};

char *langPortClass[] =
{
    "lblClassPD",
    "lblClassPSE",
    "lblUnknown",
};

char *langSupport[] =
{
    "lblNotSupported",
    "lblSupported",
};

char *langEnable[] =
{
    "lblDisabled",
    "lblEnabled",
};

char *langOverload[] =
{
    "lblTransmitted",
    "lblOverloading",
};

static int32 cgi_lldp_prop_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    sys_enable_t            enable;
    lldp_disable_action_t   action;
    uint32                  my_tx_interval;
#ifdef CONFIG_SYS_PROTO_LLDP_MED
    uint8                   fastRepeatCount;
#endif
    uint8                   holdtime, reinitDelay;
    uint16                  txDelay;

    CGI_GET_JSON_CREATE(root, data);

    sal_lldp_enable_get(&enable);
    cJSON_AddBoolToObject(data, CGI_LLDP_STATUS, enable);

    sal_lldp_disableAction_get(&action);
    if (LLDP_DISABLE_ACTION_FLOOD_TO_VLAN == action)
        cJSON_AddStringToObject(data, CGI_LLDP_FRMHANDLE, "bridge");
    else if (LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS == action)
        cJSON_AddStringToObject(data, CGI_LLDP_FRMHANDLE, "flood");
    else if (LLDP_DISABLE_ACTION_FILTER == action)
        cJSON_AddStringToObject(data, CGI_LLDP_FRMHANDLE, "filter");

    sal_lldp_tx_interval_get(&my_tx_interval);
    cJSON_AddNumberToObject(data, CGI_LLDP_TLVADVINTV, my_tx_interval);
    cJSON_AddNumberToObject(data, CGI_LLDP_MIN_TLVADVINTV, BOND_LLDP_TX_INTERVAL_MIN);
    cJSON_AddNumberToObject(data, CGI_LLDP_MAX_TLVADVINTV, BOND_LLDP_TX_INTERVAL_MAX);
    cJSON_AddNumberToObject(data, CGI_LLDP_DFLT_TLVADVINTV, DFLT_LLDP_TX_INTERVAL);

    sal_lldp_holdtime_get(&holdtime);
    cJSON_AddNumberToObject(data, CGI_LLDP_HOLDMULTI, holdtime);
    cJSON_AddNumberToObject(data, CGI_LLDP_MIN_HOLDMULTI, BOND_LLDP_HOLDTIME_MIN);
    cJSON_AddNumberToObject(data, CGI_LLDP_MAX_HOLDMULTI, BOND_LLDP_HOLDTIME_MAX);
    cJSON_AddNumberToObject(data, CGI_LLDP_DFLT_HOLDMULTI, DFLT_LLDP_HOLDTIME_MUL);

    sal_lldp_reinit_delay_get(&reinitDelay);
    cJSON_AddNumberToObject(data, CGI_LLDP_REINITDELAY, reinitDelay);
    cJSON_AddNumberToObject(data, CGI_LLDP_MIN_REINITDELAY, BOND_LLDP_REINIT_DELAY_MIN);
    cJSON_AddNumberToObject(data, CGI_LLDP_MAX_REINITDELAY, BOND_LLDP_REINIT_DELAY_MAX);
    cJSON_AddNumberToObject(data, CGI_LLDP_DFLT_REINITDELAY, DFLT_LLDP_REINIT_DELAY);

    sal_lldp_tx_delay_get(&txDelay);
    cJSON_AddNumberToObject(data, CGI_LLDP_TRANSDELAY, txDelay);
    cJSON_AddNumberToObject(data, CGI_LLDP_MIN_TRANSDELAY, BOND_LLDP_TX_DELAY_MIN);
    cJSON_AddNumberToObject(data, CGI_LLDP_MAX_TRANSDELAY, BOND_LLDP_TX_DELAY_MAX);
    cJSON_AddNumberToObject(data, CGI_LLDP_DFLT_TRANSDELAY, DFLT_LLDP_TX_DELAY);

#ifdef CONFIG_SYS_PROTO_LLDP_MED
    sal_lldp_medFastRepeatCount_get(&fastRepeatCount);
    cJSON_AddNumberToObject(data, CGI_LLDP_STARTCNT, fastRepeatCount);
    cJSON_AddNumberToObject(data, CGI_LLDP_MIN_STARTCNT, BOND_LLDP_MED_FAST_REPEAT_COUNT_MIN);
    cJSON_AddNumberToObject(data, CGI_LLDP_MAX_STARTCNT, BOND_LLDP_MED_FAST_REPEAT_COUNT_MAX);
    cJSON_AddNumberToObject(data, CGI_LLDP_DFLT_STARTCNT, DFLT_LLDP_MED_FAST_RETRY);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_port_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;
    cJSON                   *tlvSel = NULL, *tlv = NULL;
    sys_logic_port_t        lp;
    uint8                   lldpPortState;
    lldp_tlv_sel_t          lldpTlvSel;
    sys_enable_t            vidEnable;
    sys_vlanmask_t          vlanmask;
    uint32                  vlanCount = 0;

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_lldp_port_get(lp, &lldpPortState);

        switch(lldpPortState)
        {
            case LLDP_PORT_DISABLE:
                cJSON_AddStringToObject(port, CGI_LLDP_ADMINSTATUS, CGI_LANG("common", "txtDisabled"));
                break;
            case LLDP_PORT_RXONLY:
                cJSON_AddStringToObject(port, CGI_LLDP_ADMINSTATUS, CGI_LANG("lldp", "lblRxOnly"));
                break;
            case LLDP_PORT_TXONLY:
                cJSON_AddStringToObject(port, CGI_LLDP_ADMINSTATUS, CGI_LANG("lldp", "lblTxOnly"));
                break;
            case LLDP_PORT_TXRX:
                cJSON_AddStringToObject(port, CGI_LLDP_ADMINSTATUS, CGI_LANG("lldp", "lblTxAndRx"));
                break;
        }

        sal_lldp_portTlvSel_get(lp, &lldpTlvSel);
        sal_lldp_portTlvPvidEnable_get(lp, &vidEnable);
        sal_lldp_portTlvVlanmask_get(lp, &vlanmask);
        VMSK2CNT(&vlanmask, &vlanCount);

        tlvSel = cJSON_CreateArray();

        if(TRUE == lldpTlvSel.portDesciptionSel)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblPortDescription"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }
        if(TRUE == lldpTlvSel.sysNameSel)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblSysName"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }
        if(TRUE == lldpTlvSel.sysDescriptionSel)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblSysDescription"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }
        if(TRUE == lldpTlvSel.sysCapabilitySel)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblSysCapabilities"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }
        if(TRUE == lldpTlvSel.macPhySel)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblMACPHY"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }
        if(TRUE == lldpTlvSel.linkAggregationSel)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblLinkAggregation"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }
        if(TRUE == lldpTlvSel.maxFrameSizeSel)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblMaxFrameSize"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }
        if(TRUE == lldpTlvSel.managementAddrSel)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblMgmtIpAddr"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }
        if(ENABLED == vidEnable)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblPVIDTLV"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }
        if(0 != vlanCount)
        {
            tlv = cJSON_CreateObject();
            cJSON_AddStringToObject(tlv, CGI_LLDP_TLV, CGI_LANG("lldp", "lblVLANTLV"));
            cJSON_AddItemToArray(tlvSel, tlv);
        }

        if(0 != cJSON_GetArraySize(tlvSel))
        {
            cJSON_AddItemToObject(port, CGI_LLDP_SELOPTS, tlvSel);
        }
        else
        {
            cJSON_Delete(tlvSel);
        }

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_portEdit_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *availTlvs = NULL, *selTlvs = NULL;
    cJSON                   *selVlans = NULL;
    cJSON                   *avaVlans = NULL;
    cJSON                   *entry = NULL;
    sys_logic_port_t        lp;
    char                   *val;
    lldp_tlv_sel_t          lldpTlvSel;
    uint8                   lldpPortState;
    sys_enable_t            vidEnable;
    sys_vid_t               vid;
    char                    tmp[SYS_BUF16_LEN];
    uint8                   hasVlan = FALSE;
    uint8                   hasSelVlan = FALSE;
    sys_vlanmask_t          adminVM;
    sys_vlanmask_t          lldpVlanMask;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, val))
        lp = SYS_STR2UINT(val);
    else
        return SYS_ERR_OK;

    CGI_GET_JSON_CREATE(root, data);

    sal_lldp_port_get(lp, &lldpPortState);
    switch(lldpPortState)
    {
        case LLDP_PORT_DISABLE:
            cJSON_AddStringToObject(data, CGI_LLDP_ADMINSTATUS, "disable");
            break;
        case LLDP_PORT_RXONLY:
            cJSON_AddStringToObject(data, CGI_LLDP_ADMINSTATUS, "rx");
            break;
        case LLDP_PORT_TXONLY:
            cJSON_AddStringToObject(data, CGI_LLDP_ADMINSTATUS, "tx");
            break;
        case LLDP_PORT_TXRX:
            cJSON_AddStringToObject(data, CGI_LLDP_ADMINSTATUS, "txrx");
            break;
    }

    availTlvs = cJSON_CreateArray();
    selTlvs = cJSON_CreateArray();

    sal_lldp_portTlvSel_get(lp, &lldpTlvSel);
    sal_lldp_portTlvPvidEnable_get(lp, &vidEnable);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 0);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblPortDescription"));
    if(TRUE == lldpTlvSel.portDesciptionSel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 1);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblSysName"));
    if(TRUE == lldpTlvSel.sysNameSel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 2);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblSysDescription"));
    if(TRUE == lldpTlvSel.sysDescriptionSel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 3);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblSysCapabilities"));
    if(TRUE == lldpTlvSel.sysCapabilitySel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 4);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblMACPHY"));
    if(TRUE == lldpTlvSel.macPhySel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 5);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblLinkAggregation"));
    if(TRUE == lldpTlvSel.linkAggregationSel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 6);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblMaxFrameSize"));
    if(TRUE == lldpTlvSel.maxFrameSizeSel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 7);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblMgmtIpAddr"));
    if(TRUE == lldpTlvSel.managementAddrSel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 8);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblPVIDTLV"));
    if(ENABLED == vidEnable)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    if(0 != cJSON_GetArraySize(availTlvs))
        cJSON_AddItemToObject(data, CGI_LLDP_AVAILTLVS, availTlvs);
    else
        cJSON_Delete(availTlvs);

    if(0 != cJSON_GetArraySize(selTlvs))
        cJSON_AddItemToObject(data, CGI_LLDP_SELTLVS, selTlvs);
    else
        cJSON_Delete(selTlvs);

    avaVlans = cJSON_CreateArray();
    selVlans = cJSON_CreateArray();
    SYS_MEM_CLEAR(adminVM);
    SYS_MEM_CLEAR(lldpVlanMask);
    sal_vlan_adminEntryExist_get(&adminVM);
    sal_lldp_portTlvVlanmask_get(lp, &lldpVlanMask);
#if 1
    FOR_EACH_VID_IN_VLANMASK(vid, adminVM)
    {
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "VLAN %u", vid);
        entry = cJSON_CreateObject();
        cJSON_AddStringToObject(entry, CGI_NAME, tmp);
        cJSON_AddNumberToObject(entry, CGI_ID, vid);

        if (VLANMASK_IS_VIDSET(lldpVlanMask, vid))
        {
            cJSON_AddItemToArray(selVlans, entry);
            hasSelVlan = TRUE;
        }
        else
        {
            cJSON_AddItemToArray(avaVlans, entry);
            hasVlan = TRUE;
        }
    }

    if (hasVlan)
        cJSON_AddItemToObject(data, CGI_VLAN_AVALIABLE_VLANS, avaVlans);

    if (hasSelVlan)
        cJSON_AddItemToObject(data, CGI_VLAN_SELECT_VLANS, selVlans);
#endif
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED
static int32 cgi_lldp_medPolicy_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *policies = NULL;
    cJSON *entry = NULL;
    uint32 i;
    lldp_med_networkPolicy_t policy[CAPA_LLDP_MED_NETWORK_POLICY_NUM];
    sys_enable_t enable;

    CGI_GET_JSON_CREATE(root, data);

    sal_lldp_medNetworkPolicyAuto_get(&enable);
    cJSON_AddBoolToObject(data, CGI_LLDP_POLICY4VOICE, enable);

    policies = cJSON_CreateArray();
    for(i = 0; i < CAPA_LLDP_MED_NETWORK_POLICY_NUM; i ++)
    {
        sal_lldp_medNetworkPolicy_get(i, &policy[i]);
        if(FALSE == policy[i].valid)
            continue;

        entry = cJSON_CreateObject();

        cJSON_AddNumberToObject(entry, CGI_LLDP_POLICYNUM, i+1);
        cJSON_AddLangStringToObject(entry, CGI_LLDP_APP, "lldp", langPolicy[policy[i].app]);

        cJSON_AddNumberToObject(entry, CGI_LLDP_VID, policy[i].vid);
        if(TRUE == policy[i].vlanTagged)
            cJSON_AddStringToObject(entry, CGI_LLDP_VLANTAG, CGI_LANG("lldp", "lblTagged"));
        else
            cJSON_AddStringToObject(entry, CGI_LLDP_VLANTAG, CGI_LANG("lldp", "lblUntagged"));

        cJSON_AddNumberToObject(entry, CGI_LLDP_PRI, policy[i].userPriority);
        cJSON_AddNumberToObject(entry, CGI_LLDP_DSCP, policy[i].dscpValue);

        cJSON_AddItemToArray(policies, entry);
    }

    if(0 != cJSON_GetArraySize(policies))
        cJSON_AddItemToObject(data, CGI_LLDP_POLICIES, policies);
    else
        cJSON_Delete(policies);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_medPolicyAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *apps = NULL;
    cJSON *entry = NULL;
    uint32 i;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_LLDP_MAX_POLICIESNUM, CAPA_LLDP_MED_NETWORK_POLICY_NUM);
    cJSON_AddNumberToObject(data, CGI_LLDP_VID_MIN, BOND_LLDP_MED_NETWORK_POLICY_VID_MIN);
    cJSON_AddNumberToObject(data, CGI_LLDP_VID_MAX, BOND_LLDP_MED_NETWORK_POLICY_VID_MAX);

    apps = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_LLDP_APPS, apps);

    for(i = LLDP_APP_VOICE; i < LLDP_APP_END; i ++)
    {
        entry = cJSON_CreateObject();
        cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, i);
        cJSON_AddLangStringToObject(entry, CGI_LLDP_APP, "lldp", langPolicy[i]);
        cJSON_AddItemToArray(apps, entry);
    }
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_medPolicyEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *apps = NULL;
    cJSON *entry = NULL;
    uint32 idx = 0, i;
    char *value = NULL;
    lldp_med_networkPolicy_t policy;

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_POLICY, value))
        idx = SYS_STR2UINT(value) - 1;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_LLDP_MAX_POLICIESNUM, CAPA_LLDP_MED_NETWORK_POLICY_NUM);

    sal_lldp_medNetworkPolicy_get(idx, &policy);
    cJSON_AddNumberToObject(data, CGI_LLDP_VID, policy.vid);
    cJSON_AddNumberToObject(data, CGI_LLDP_VID_MIN, BOND_LLDP_MED_NETWORK_POLICY_VID_MIN);
    cJSON_AddNumberToObject(data, CGI_LLDP_VID_MAX, BOND_LLDP_MED_NETWORK_POLICY_VID_MAX);

    if(TRUE == policy.vlanTagged)
        cJSON_AddStringToObject(data, CGI_LLDP_VLANTAG, "tag");
    else
        cJSON_AddStringToObject(data, CGI_LLDP_VLANTAG, "untag");

    cJSON_AddNumberToObject(data, CGI_LLDP_PRI, policy.userPriority);
    cJSON_AddNumberToObject(data, CGI_LLDP_DSCP, policy.dscpValue);
    cJSON_AddNumberToObject(data, CGI_LLDP_SELAPP, policy.app);

    apps = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_LLDP_APPS, apps);
    for(i = LLDP_APP_VOICE; i < LLDP_APP_END; i ++)
    {
        entry = cJSON_CreateObject();
        cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, i);
        cJSON_AddLangStringToObject(entry, CGI_LLDP_APP, "lldp", langPolicy[i]);
        cJSON_AddItemToArray(apps, entry);
    }
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_medPort_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;
    cJSON                   *appSel = NULL;
    cJSON                   *entry = NULL;
    sys_logic_port_t        lp;
    lldp_med_tlv_sel_t      lldpMedTlvSel;
    sys_enable_t            enable;
    lldp_med_networkPolicy_bind_t policyBind;
    uint32                  i;
    lldp_med_networkPolicy_t policy[CAPA_LLDP_MED_NETWORK_POLICY_NUM];

    CGI_GET_JSON_CREATE(root, data);

    SYS_ERR_CHK(sal_lldp_medNetworkPolicyAuto_get(&enable));
    if(ENABLED == enable)
        cJSON_AddStringToObject(data, CGI_LLDP_VOICEAPPMODE, CGI_LANG("lldp", "lblAuto"));
    else
        cJSON_AddStringToObject(data, CGI_LLDP_VOICEAPPMODE, CGI_LANG("lldp", "lblManual"));

    for(i = 0; i < CAPA_LLDP_MED_NETWORK_POLICY_NUM; i ++)
    {
        sal_lldp_medNetworkPolicy_get(i, &policy[i]);
    }

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_lldp_portMedEnable_get(lp,&enable);

        cJSON_AddBoolToObject(port, CGI_LLDP_MEDSTATUS, enable);

        sal_lldp_portMedTlvSel_get(lp, &lldpMedTlvSel);
        cJSON_AddBoolToObject(port, CGI_LLDP_ACTIVE, lldpMedTlvSel.networkPolicySel);

        sal_lldp_portMedNetworkPolicyBind_get(lp, &policyBind);

        if(policyBind.bindMask != 0)
        {
            appSel = cJSON_CreateArray();
            cJSON_AddItemToObject(port, CGI_LLDP_APPS, appSel);
            for(i = 0; i < CAPA_LLDP_MED_NETWORK_POLICY_NUM; i ++)
            {
                if(policyBind.bindMask & (1 << i))
                {
                    entry = cJSON_CreateObject();
                    cJSON_AddLangStringToObject(entry, CGI_LLDP_APP, "lldp", langPolicy[policy[i].app]);
                    cJSON_AddItemToArray(appSel, entry);
                }
            }
        }

        cJSON_AddBoolToObject(port, CGI_LLDP_LOCATION, lldpMedTlvSel.locationSel);
        cJSON_AddBoolToObject(port, CGI_LLDP_INVENTORY, lldpMedTlvSel.inventorySel);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_medPortEdit_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *availTlvs = NULL, *selTlvs = NULL, *avaiPolicies = NULL, *selPolicies = NULL;
    cJSON                   *entry = NULL;
    sys_logic_port_t        lp;
    sys_enable_t            enable;
    char                   *val;
    lldp_med_tlv_sel_t      lldpMedTlvSel;
    uint32                  i;
    lldp_med_networkPolicy_bind_t policyBind;
    lldp_med_networkPolicy_t policy;
    lldp_med_location_t location;
    char strTmp[SYS_BUF512_LEN];

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, val))
        lp = SYS_STR2UINT(val);
    else
        return SYS_ERR_OK;

    CGI_GET_JSON_CREATE(root, data);

    sal_lldp_portMedEnable_get(lp, &enable);
    cJSON_AddBoolToObject(data, CGI_LLDP_MEDSTATUS, enable);

    availTlvs = cJSON_CreateArray();
    selTlvs = cJSON_CreateArray();

    sal_lldp_portMedTlvSel_get(lp, &lldpMedTlvSel);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 0);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblUserDefinedNetworkPolicy"));
    if(TRUE == lldpMedTlvSel.networkPolicySel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 1);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblLocation"));
    if(TRUE == lldpMedTlvSel.locationSel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, CGI_LLDP_VAL, 3);
    cJSON_AddStringToObject(entry, CGI_LLDP_TLV, CGI_LANG("lldp", "lblInventory"));
    if(TRUE == lldpMedTlvSel.inventorySel)
        cJSON_AddItemToArray(selTlvs, entry);
    else
        cJSON_AddItemToArray(availTlvs, entry);

    if(0 != cJSON_GetArraySize(availTlvs))
        cJSON_AddItemToObject(data, CGI_LLDP_AVAITLVS, availTlvs);
    else
        cJSON_Delete(availTlvs);

    if(0 != cJSON_GetArraySize(selTlvs))
        cJSON_AddItemToObject(data, CGI_LLDP_SELTLVS, selTlvs);
    else
        cJSON_Delete(selTlvs);

    avaiPolicies = cJSON_CreateArray();
    selPolicies = cJSON_CreateArray();

    sal_lldp_portMedNetworkPolicyBind_get(lp, &policyBind);

    for(i = 0; i < CAPA_LLDP_MED_NETWORK_POLICY_NUM; i ++)
    {
        sal_lldp_medNetworkPolicy_get(i, &policy);
        if(policy.valid)
        {
            entry = cJSON_CreateObject();
            cJSON_AddNumberToObject(entry, CGI_LLDP_POLICYNUM, i + 1);
            cJSON_AddLangStringToObject(entry, CGI_LLDP_APP, "lldp", langPolicy[policy.app]);

            if(policyBind.bindMask & (1 << i))
                cJSON_AddItemToArray(selPolicies, entry);
            else
                cJSON_AddItemToArray(avaiPolicies, entry);
        }
    }

    if(0 != cJSON_GetArraySize(avaiPolicies))
        cJSON_AddItemToObject(data, CGI_LLDP_AVAIPOLICIES, avaiPolicies);
    else
        cJSON_Delete(avaiPolicies);

    if(0 != cJSON_GetArraySize(selPolicies))
        cJSON_AddItemToObject(data, CGI_LLDP_SELPOLICIES, selPolicies);
    else
        cJSON_Delete(selPolicies);

    sal_lldp_portMedLocation_get(lp, &location);

    osal_memset(strTmp, 0, sizeof(strTmp));
    if(location.coorLen> 0)
    {
        sys_util_byteArray2hexStr(location.coordinate, location.coorLen, strTmp);
        cJSON_AddStringToObject(data, CGI_LLDP_COORDINATE, strTmp);
    }
    cJSON_AddNumberToObject(data, CGI_LLDP_COORDINATE_LEN_MAX, CAPA_LLDP_MED_COORDINATE_LEN);

    osal_memset(strTmp, 0, sizeof(strTmp));
    if(location.civicLen > 0)
    {
        sys_util_byteArray2hexStr(location.civicAddress, location.civicLen, strTmp);
        cJSON_AddStringToObject(data, CGI_LLDP_CIVIC, strTmp);

    }
    cJSON_AddNumberToObject(data, CGI_LLDP_CIVIC_LEN_MIN, BOND_LLDP_MED_CIVIC_LEN_MIN );
    cJSON_AddNumberToObject(data, CGI_LLDP_CIVIC_LEN_MAX, BOND_LLDP_MED_CIVIC_LEN_MAX );

    osal_memset(strTmp, 0, sizeof(strTmp));
    if(location.ecsLen > 0)
    {
        sys_util_byteArray2hexStr(location.ecsElin, location.ecsLen, strTmp);
        cJSON_AddStringToObject(data, CGI_LLDP_ECS, strTmp);
    }
    cJSON_AddNumberToObject(data, CGI_LLDP_ECS_LEN_MIN, BOND_LLDP_MED_ELIN_LEN_MIN);
    cJSON_AddNumberToObject(data, CGI_LLDP_ECS_LEN_MAX, BOND_LLDP_MED_ELIN_LEN_MAX);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif
static int32 cgi_lldp_local_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;
    sys_logic_port_t        lp;
    uint8                   lldpPortState;
    sys_mac_t macaddr;
    char macaddr_str[CAPA_MAC_STR_LEN];
    display_entry_t lldp_entry;

#ifdef CONFIG_SYS_PROTO_LLDP_MED
    sys_enable_t            enable;
#endif

    CGI_GET_JSON_CREATE(root, data);

    sal_sys_macAddr_get(&macaddr);
    sys_util_mac2Str(macaddr.octet, macaddr_str);

    SYS_ERR_CHK(sal_lldp_portLocalEntryInfo_get(0, &lldp_entry));

    cJSON_AddStringToObject(data, CGI_LLDP_CHASSISTYPE, CGI_LANG("lldp", "lblMacAddr"));
    cJSON_AddStringToObject(data, CGI_LLDP_CHASSISID, macaddr_str);
    cJSON_AddStringToObject(data, CGI_LLDP_SYSNAME, lldp_entry.system_name);
    cJSON_AddStringToObject(data, CGI_LLDP_SYSDESCP, lldp_entry.system_description);
#ifdef CONFIG_SYS_L3_ROUTE
    cJSON_AddStringToObject(data, CGI_LLDP_SUPPORTCAP, CGI_LANG("lldp", "lblL3"));
    cJSON_AddStringToObject(data, CGI_LLDP_ENABLECAP, CGI_LANG("lldp", "lblL3"));
#else
    cJSON_AddStringToObject(data, CGI_LLDP_SUPPORTCAP, CGI_LANG("lldp", "lblBridge"));
    cJSON_AddStringToObject(data, CGI_LLDP_ENABLECAP, CGI_LANG("lldp", "lblBridge"));
#endif
    cJSON_AddStringToObject(data, CGI_LLDP_PORTIDTYPE, CGI_LANG("lldp", "lblLocal"));

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_lldp_port_get(lp, &lldpPortState);

        switch(lldpPortState)
        {
            case LLDP_PORT_DISABLE:
                cJSON_AddStringToObject(port, CGI_LLDP_PORTSTATUS, CGI_LANG("common", "txtDisabled"));
                break;
            case LLDP_PORT_RXONLY:
                cJSON_AddStringToObject(port, CGI_LLDP_PORTSTATUS, CGI_LANG("lldp", "lblRxOnly"));
                break;
            case LLDP_PORT_TXONLY:
                cJSON_AddStringToObject(port, CGI_LLDP_PORTSTATUS, CGI_LANG("lldp", "lblTxOnly"));
                break;
            case LLDP_PORT_TXRX:
                cJSON_AddStringToObject(port, CGI_LLDP_PORTSTATUS, CGI_LANG("lldp", "lblTxAndRx"));
                break;
        }

#ifdef CONFIG_SYS_PROTO_LLDP_MED
        sal_lldp_portMedEnable_get(lp, &enable);
        if(ENABLED == enable)
            cJSON_AddStringToObject(port, CGI_LLDP_MEDSTATUS, CGI_LANG("common", "txtEnabled"));
        else
            cJSON_AddStringToObject(port, CGI_LLDP_MEDSTATUS, CGI_LANG("common", "txtDisabled"));
#endif

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_localDetail_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *addr = NULL;
    cJSON                   *negoAdv = NULL;
    cJSON                   *entry = NULL;
    sys_logic_port_t        lp;
    sys_mac_t macaddr;
    uint32 ipaddr;
    uint32 netmask;
    char ipaddr_str[CAPA_IPV4_STR_LEN];
    char netmask_str[CAPA_IPV4_STR_LEN];
    char macaddr_str[CAPA_MAC_STR_LEN];
    uint8 strIpAddr[CAPA_IPV6_STR_LEN];
    display_entry_t lldp_entry;
    char *val;
    uint8 i;
    uint8 isAddrValid = FALSE;
#ifdef CONFIG_SYS_PROTO_LLDP_MED
    display_med_entry_t     lldpMed_entry;
    cJSON                   *capSupport = NULL, *capCurrent = NULL;
    cJSON                   *policies = NULL;
#endif

    CGI_GET_JSON_CREATE(root, data);

    sal_sys_macAddr_get(&macaddr);
    sys_util_mac2Str(macaddr.octet, macaddr_str);
    sal_sys_ip_get(&ipaddr, &netmask);
    sys_util_ip2Str(ipaddr, ipaddr_str);
    sys_util_ip2Str(netmask, netmask_str);

    if (CGI_IS_PARAM_EXIST(CGI_PORT, val))
        STR2LP(val, &lp);
    else
        return SYS_ERR_OK;

    sal_lldp_portLocalEntryInfo_get(lp, &lldp_entry);

    cJSON_AddLangStringToObject(data, CGI_LLDP_CHASSISTYPE, "lldp", langChassisType[lldp_entry.chasis_id.iSubtype]);
    cJSON_AddStringToObject(data, CGI_LLDP_CHASSISID, lldp_entry.chasis_id.id);
    cJSON_AddStringToObject(data, CGI_LLDP_SYSNAME, lldp_entry.system_name);
    cJSON_AddStringToObject(data, CGI_LLDP_SYSDESCP, lldp_entry.system_description);
#ifdef CONFIG_SYS_L3_ROUTE
    cJSON_AddStringToObject(data, CGI_LLDP_SUPPORTCAP, CGI_LANG("lldp", "lblL3"));
    cJSON_AddStringToObject(data, CGI_LLDP_ENABLECAP, CGI_LANG("lldp", "lblL3"));
#else
    cJSON_AddStringToObject(data, CGI_LLDP_SUPPORTCAP, CGI_LANG("lldp", "lblBridge"));
    cJSON_AddStringToObject(data, CGI_LLDP_ENABLECAP, CGI_LANG("lldp", "lblBridge"));
#endif
    cJSON_AddLangStringToObject(data, CGI_LLDP_PORTIDTYPE, "lldp", langPortIDType[lldp_entry.port_id.iSubtype]);
    cJSON_AddStringToObject(data, CGI_LLDP_PORTID, lldp_entry.port_id.id);
    cJSON_AddStringToObject(data, CGI_LLDP_PORTDESCP, lldp_entry.port_description);

    for(i = 0; i < MAX_LLDP_MANAGEMENT_ADDR_NUM; i ++)
    {
        if(lldp_entry.manage_address[i].valid)
        {
            if(FALSE == isAddrValid)
            {
                addr = cJSON_CreateArray();
                cJSON_AddItemToObject(data, CGI_LLDP_ADDRS, addr);
                isAddrValid = TRUE;
            }
            entry = cJSON_CreateObject();
            cJSON_AddLangStringToObject(entry, CGI_LLDP_ADDRTYPE, "lldp", langManAddrType[lldp_entry.manage_address[i].iSubtype]);
            sal_lldp_managementAddressStr_get(lldp_entry.manage_address[i], strIpAddr);
            cJSON_AddStringToObject(entry, CGI_LLDP_ADDR, (char *)strIpAddr);
            cJSON_AddLangStringToObject(entry, CGI_LLDP_INTFTYPE, "lldp", langManAddrIfSubType[lldp_entry.manage_address[i].interfaceSubtype]);
            cJSON_AddNumberToObject(entry, CGI_LLDP_INTFNUM, lldp_entry.manage_address[i].interfaceNum);
            cJSON_AddItemToArray(addr, entry);
        }
    }

    if(lldp_entry.macPhy.valid)
    {
        if(TRUE == lldp_entry.macPhy.support)
            cJSON_AddLangStringToObject(data, CGI_LLDP_NEGOSUPPORT, "lldp", "lblTrue");
        else
            cJSON_AddLangStringToObject(data, CGI_LLDP_NEGOSUPPORT, "lldp", "lblFalse");

        if(TRUE == lldp_entry.macPhy.support)
            cJSON_AddLangStringToObject(data, CGI_LLDP_NEGOENBL, "lldp", "lblTrue");
        else
            cJSON_AddLangStringToObject(data, CGI_LLDP_NEGOENBL, "lldp", "lblFalse");

        negoAdv = cJSON_CreateArray();

        for(i = LLDP_PHY_CAPA_1000BASETFD; i < LLDP_PHY_CAPA_END; i ++)
        {
            if(lldp_entry.macPhy.iCapa & (1 << i))
            {
                entry = cJSON_CreateObject();
                cJSON_AddLangStringToObject(entry, CGI_LLDP_ABILITY, "lldp", langPhyCapa[i]);
                cJSON_AddItemToArray(negoAdv, entry);
            }
        }

        if(0 != cJSON_GetArraySize(negoAdv))
            cJSON_AddItemToObject(data, CGI_LLDP_NEGOADVERTISE, negoAdv);
        else
            cJSON_Delete(negoAdv);

        cJSON_AddLangStringToObject(data, CGI_LLDP_OPERMAUTYPE, "lldp", langPhyOper[lldp_entry.macPhy.iOper]);
    }

    if(TRUE == lldp_entry.maxFrameSize.valid)
        cJSON_AddLangStringArgToObject(data, CGI_LLDP_MAXFRMSIZE, "common", "txtArg", "%d", lldp_entry.maxFrameSize.maxFrameSize);

    if(lldp_entry.trunk.valid)
    {
        cJSON_AddLangStringToObject(data, CGI_LLDP_AGGCAP, "lldp", langAggCapa[lldp_entry.trunk.iCapa]);
        cJSON_AddLangStringToObject(data, CGI_LLDP_AGGSTATUS, "lldp", langAggState[lldp_entry.trunk.iState]);
        cJSON_AddNumberToObject(data, CGI_LLDP_AGGPORTID, lldp_entry.trunk.id);
    }

#ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_ERR_CHK(sal_lldp_portMedLocalEntryInfo_get(lp, &lldpMed_entry));

    if(lldpMed_entry.medCapa.valid)
    {
        capSupport = cJSON_CreateArray();
        capCurrent = cJSON_CreateArray();

        for(i = 0; i < 6; i ++)
        {
            if(lldpMed_entry.medCapa.iCapa & (1 << i))
            {
                entry = cJSON_CreateObject();
                cJSON_AddLangStringToObject(entry, CGI_LLDP_CAP, "lldp", langMedCapa[i]);
                cJSON_AddItemToArray(capSupport, entry);
            }
        }

        for(i = 0; i < 6; i ++)
        {
            if(lldpMed_entry.medCapa.iCapa & (1 << i))
            {
                entry = cJSON_CreateObject();
                cJSON_AddLangStringToObject(entry, CGI_LLDP_CAP, "lldp", langMedCapa[i]);
                cJSON_AddItemToArray(capCurrent, entry);
            }
        }
        if(0 != cJSON_GetArraySize(capSupport))
            cJSON_AddItemToObject(data, CGI_LLDP_CAPSUPPORT, capSupport);
        else
            cJSON_Delete(capSupport);

        if(0 != cJSON_GetArraySize(capCurrent))
            cJSON_AddItemToObject(data, CGI_LLDP_CURRCAP, capCurrent);
        else
            cJSON_Delete(capCurrent);

        cJSON_AddLangStringToObject(data, CGI_LLDP_DEVCLS, "lldp", langDevClass[lldpMed_entry.medCapa.iDevType]);
    }

    if(TRUE == lldpMed_entry.power.valid)
    {
        cJSON_AddLangStringToObject(data, CGI_LLDP_POEDEVTYPE, "lldp", langPoEType[lldpMed_entry.power.iDevType]);
        if(lldpMed_entry.power.iDevType == 1)
            cJSON_AddLangStringToObject(data, CGI_LLDP_POEPWRSRC, "lldp", langPoEPSESource[lldpMed_entry.power.iSrc]);
        else
            cJSON_AddLangStringToObject(data, CGI_LLDP_POEPWRSRC, "lldp", langPoEPDSource[lldpMed_entry.power.iSrc]);

        cJSON_AddLangStringToObject(data, CGI_LLDP_POEPWRPRI, "lldp", langPri[lldpMed_entry.power.iPri]);
        cJSON_AddLangStringArgToObject(data, CGI_LLDP_POEPWRVAL, "lldp","lblWatts", "%d,%d", lldpMed_entry.power.powerVal / 10, lldpMed_entry.power.powerVal % 10);
    }

    if(lldpMed_entry.hardware_v.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_HWVER, lldpMed_entry.hardware_v.strInventory);

    if(lldpMed_entry.firmware_v.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_FWVER, lldpMed_entry.firmware_v.strInventory);

    if(lldpMed_entry.software_v.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_SWVER, lldpMed_entry.software_v.strInventory);

    if(lldpMed_entry.serial_num.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_SN, lldpMed_entry.serial_num.strInventory);

    if(lldpMed_entry.manufacturer.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_MANUFNAME, lldpMed_entry.manufacturer.strInventory);

    if(lldpMed_entry.model_name.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_MODELNAME, lldpMed_entry.model_name.strInventory);

    if(lldpMed_entry.asset.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_ASSETID, lldpMed_entry.asset.strInventory);

    if(lldpMed_entry.locationCivic.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_CIVIC, lldpMed_entry.locationCivic.data);

    if(lldpMed_entry.locationCoor.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_COORDINATE, lldpMed_entry.locationCoor.data);

    if(lldpMed_entry.locationEcs.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_ECS, lldpMed_entry.locationEcs.data);

    policies = cJSON_CreateArray();

    for(i = 0; i < MAX_LLDP_MED_NETWORK_POLICY_RX; i ++)
    {
        if(lldpMed_entry.networkPolicy[i].valid)
        {
            entry = cJSON_CreateObject();

            cJSON_AddLangStringToObject(entry, CGI_LLDP_APP, "lldp", langPolicy[lldpMed_entry.networkPolicy[i].iType]);

            cJSON_AddNumberToObject(entry, CGI_LLDP_VID, lldpMed_entry.networkPolicy[i].vid);
            if(TRUE == lldpMed_entry.networkPolicy[i].isTagged)
                cJSON_AddStringToObject(entry, CGI_LLDP_VLANTYPE, CGI_LANG("lldp", "lblTagged"));
            else
                cJSON_AddStringToObject(entry, CGI_LLDP_VLANTYPE, CGI_LANG("lldp", "lblUntagged"));

            cJSON_AddNumberToObject(entry, CGI_LLDP_PRI, lldpMed_entry.networkPolicy[i].l2Pri);
            cJSON_AddNumberToObject(entry, CGI_LLDP_DSCP, lldpMed_entry.networkPolicy[i].dscpPri);

            cJSON_AddItemToArray(policies, entry);
        }
    }

    if(0 != cJSON_GetArraySize(policies))
        cJSON_AddItemToObject(data, CGI_LLDP_POLICIES, policies);
    else
        cJSON_Delete(policies);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_neighbor_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *neighbor = NULL;
    cJSON *entry = NULL;
    uint32 i;
    display_entry_t lldp_entry;
    char lldpPortStr[CAPA_PORT_STR_LEN];
    sys_logic_port_t logicPort;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, "null", ENABLED);

    neighbor = cJSON_CreateArray();

    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        for(i = 0 ;i < LLDP_MAX_ENTRY_PER_PORT; i++)
        {
            osal_memset(&lldp_entry, 0, sizeof(display_entry_t));
            SYS_ERR_CHK(sal_lldp_portRemoteEntryInfo_get(logicPort, i, &lldp_entry));
            if((lldp_entry.logicPort >= 0) && (lldp_entry.logicPort <= SYS_LOGICPORT_NUM_MAX)
                && ( osal_strlen(lldp_entry.chasis_id.id) != 0 ) && ( lldp_entry.lldp_ttl != 0 ))
            {

                entry = cJSON_CreateObject();

                LP2WEBSTR(logicPort, lldpPortStr);

                cJSON_AddStringToObject(entry, CGI_LLDP_LOCALPORT, lldpPortStr);
                cJSON_AddNumberToObject(entry, CGI_LLDP_INDEX, i);
                cJSON_AddLangStringToObject(entry, CGI_LLDP_CHASSISTYPE, "lldp", langChassisType[lldp_entry.chasis_id.iSubtype]);
                cJSON_AddStringToObject(entry, CGI_LLDP_CHASSISID, lldp_entry.chasis_id.id);
                cJSON_AddLangStringToObject(entry, CGI_LLDP_PORTIDTYPE, "lldp", langPortIDType[lldp_entry.port_id.iSubtype]);
                cJSON_AddStringToObject(entry, CGI_LLDP_PORTID, lldp_entry.port_id.id);
                cJSON_AddStringToObject(entry, CGI_LLDP_SYSNAME, lldp_entry.system_name);
                cJSON_AddNumberToObject(entry, CGI_LLDP_TTL, lldp_entry.lldp_ttl);
                cJSON_AddItemToArray(neighbor, entry);
            }
        }
    }

    if(0 != cJSON_GetArraySize(neighbor))
        cJSON_AddItemToObject(data, CGI_LLDP_NEIGHBORS, neighbor);
    else
        cJSON_Delete(neighbor);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_neighborDetail_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *negoAdv = NULL;
    cJSON                   *addr = NULL;
    cJSON                   *vids = NULL;
    cJSON                   *capCfg = NULL, *capEbl = NULL;
    cJSON                   *entry = NULL;
    sys_logic_port_t        lp;
    sys_mac_t macaddr;
    uint32 ipaddr;
    uint32 netmask;
    char ipaddr_str[CAPA_IPV4_STR_LEN];
    char netmask_str[CAPA_IPV4_STR_LEN];
    char macaddr_str[CAPA_MAC_STR_LEN];
    display_entry_t lldp_entry;
    lldp_vlanName_tlv_t  vlanTlv;
    char *val,*p;
    char  *key = "_";
    uint8 i, index = 0;
    char lldpPortStr[CAPA_PORT_STR_LEN];
    uint8 isAddrValid = FALSE;
    uint8 strIpAddr[CAPA_IPV6_STR_LEN];
#ifdef CONFIG_SYS_PROTO_LLDP_MED
    cJSON                   *capSupport = NULL, *capCurrent = NULL;
    cJSON                   *policies = NULL;
    display_med_entry_t     lldpMed_entry;
#endif

    CGI_GET_JSON_CREATE(root, data);

    sal_sys_macAddr_get(&macaddr);
    sys_util_mac2Str(macaddr.octet, macaddr_str);
    sal_sys_ip_get(&ipaddr, &netmask);
    sys_util_ip2Str(ipaddr, ipaddr_str);
    sys_util_ip2Str(netmask, netmask_str);

    if (CGI_IS_PARAM_EXIST(CGI_PORT, val))
    {
        p = strtok(val, key);
        STR2LP(p, &lp);
        p = strtok(NULL, key);
        index = SYS_STR2UINT(p);
    }
    else
    {
        lp = 0;
        index = 0;
    }

    sal_lldp_portRemoteEntryInfo_get(lp, index, &lldp_entry);

    LP2WEBSTR(lp, lldpPortStr);
    cJSON_AddStringToObject(data, CGI_LLDP_LOCALPORT, lldpPortStr);
    cJSON_AddLangStringToObject(data, CGI_LLDP_CHASSISTYPE, "lldp", langChassisType[lldp_entry.chasis_id.iSubtype]);
    cJSON_AddStringToObject(data, CGI_LLDP_CHASSISID, lldp_entry.chasis_id.id);
    cJSON_AddLangStringToObject(data, CGI_LLDP_PORTIDTYPE, "lldp", langPortIDType[lldp_entry.port_id.iSubtype]);
    cJSON_AddStringToObject(data, CGI_LLDP_PORTID, lldp_entry.port_id.id);
    cJSON_AddStringToObject(data, CGI_LLDP_SYSNAME, lldp_entry.system_name);
    cJSON_AddStringToObject(data, CGI_LLDP_SYSDESCP, lldp_entry.system_description);

    capCfg = cJSON_CreateArray();
    capEbl = cJSON_CreateArray();

    for(i = 0; i < 8; i ++)
    {
        if(lldp_entry.system_capabilities.iSupport & (1 << i))
        {
            entry = cJSON_CreateObject();
            cJSON_AddLangStringToObject(entry, CGI_LLDP_CAP, "lldp", langCapa[i]);
            cJSON_AddItemToArray(capCfg, entry);
        }
    }

    for(i = 0; i < 8; i ++)
    {
        if(lldp_entry.system_capabilities.iEnable & (1 << i))
        {
            entry = cJSON_CreateObject();
            cJSON_AddLangStringToObject(entry, CGI_LLDP_CAP, "lldp", langCapa[i]);
            cJSON_AddItemToArray(capEbl, entry);
        }
    }

    if(0 != cJSON_GetArraySize(capCfg))
        cJSON_AddItemToObject(data, CGI_LLDP_SUPPORTCAP, capCfg);
    else
        cJSON_Delete(capCfg);

    if(0 != cJSON_GetArraySize(capEbl))
        cJSON_AddItemToObject(data, CGI_LLDP_ENABLECAP, capEbl);
    else
        cJSON_Delete(capEbl);

    cJSON_AddLangStringToObject(data, CGI_LLDP_PORTIDTYPE, "lldp", langPortIDType[lldp_entry.port_id.iSubtype]);
    cJSON_AddStringToObject(data, CGI_LLDP_PORTID, lldp_entry.port_id.id);
    cJSON_AddStringToObject(data, CGI_LLDP_PORTDESCP, lldp_entry.port_description);

    for(i = 0; i < MAX_LLDP_MANAGEMENT_ADDR_NUM; i ++)
    {
        if(lldp_entry.manage_address[i].valid)
        {
            if(FALSE == isAddrValid)
            {
                addr = cJSON_CreateArray();
                cJSON_AddItemToObject(data, CGI_LLDP_ADDRS, addr);
                isAddrValid = TRUE;
            }
            entry = cJSON_CreateObject();
            cJSON_AddLangStringToObject(entry, CGI_LLDP_ADDRTYPE, "lldp", langManAddrType[lldp_entry.manage_address[i].iSubtype]);
            sal_lldp_managementAddressStr_get(lldp_entry.manage_address[i], strIpAddr);
            cJSON_AddStringToObject(entry, CGI_LLDP_ADDR, (char *)strIpAddr);
            cJSON_AddLangStringToObject(entry, CGI_LLDP_INTFTYPE, "lldp", langManAddrIfSubType[lldp_entry.manage_address[i].interfaceSubtype]);
            cJSON_AddNumberToObject(entry, CGI_LLDP_INTFNUM, lldp_entry.manage_address[i].interfaceNum);
            cJSON_AddItemToArray(addr, entry);
        }
    }

    if(lldp_entry.macPhy.valid)
    {
        if(TRUE == lldp_entry.macPhy.support)
            cJSON_AddLangStringToObject(data, CGI_LLDP_NEGOSUPPORT, "lldp", "lblTrue");
        else
            cJSON_AddLangStringToObject(data, CGI_LLDP_NEGOSUPPORT, "lldp", "lblFalse");

        if(TRUE == lldp_entry.macPhy.support)
            cJSON_AddLangStringToObject(data, CGI_LLDP_NEGOENBL, "lldp", "lblTrue");
        else
            cJSON_AddLangStringToObject(data, CGI_LLDP_NEGOENBL, "lldp", "lblFalse");

        negoAdv = cJSON_CreateArray();

        for(i = LLDP_PHY_CAPA_1000BASETFD; i < LLDP_PHY_CAPA_END; i ++)
        {
            if(lldp_entry.macPhy.iCapa & (1 << i))
            {
                entry = cJSON_CreateObject();
                cJSON_AddLangStringToObject(entry, CGI_LLDP_ABILITY, "lldp", langPhyCapa[i]);
                cJSON_AddItemToArray(negoAdv, entry);
            }
        }

        if(0 != cJSON_GetArraySize(negoAdv))
            cJSON_AddItemToObject(data, CGI_LLDP_NEGOADVERTISE, negoAdv);
        else
            cJSON_Delete(negoAdv);

        cJSON_AddLangStringToObject(data, CGI_LLDP_OPERMAUTYPE, "lldp", langPhyOper[lldp_entry.macPhy.iOper]);
    }

    if(TRUE == lldp_entry.dot3power.valid)
    {
        cJSON_AddLangStringToObject(data, CGI_LLDP_PWRPORTCLASS, "lldp", langPortClass[lldp_entry.dot3power.iPortClass]);
        cJSON_AddLangStringToObject(data, CGI_LLDP_PWRSUPPORT, "lldp", langSupport[lldp_entry.dot3power.iPowerSupport]);
        cJSON_AddLangStringToObject(data, CGI_LLDP_PWRSTATE, "lldp", langEnable[lldp_entry.dot3power.iPowerState]);
        cJSON_AddLangStringToObject(data, CGI_LLDP_PWRCAP, "lldp", langSupport[lldp_entry.dot3power.iPairControlAbility]);
        cJSON_AddNumberToObject(data, CGI_LLDP_PWRPAIR, lldp_entry.dot3power.iPowerPair);
        cJSON_AddLangStringArgToObject(data, CGI_LLDP_PWRCLASS, "lldp", "lblClass", "%d", lldp_entry.dot3power.iPowerClass-1);
        if(TRUE == lldp_entry.dot3power.extValid)
        {
            cJSON_AddLangStringToObject(data, CGI_LLDP_DOT3DEVTYPE, "lldp", langPoEType[lldp_entry.dot3power.iDevType]);
    #ifdef CONFIG_SYS_PROTO_LLDP_MED
            if(lldpMed_entry.power.iDevType == 1)
                cJSON_AddLangStringToObject(data, CGI_LLDP_DOT3PWRSRC, "lldp", langPoEPSESource[lldp_entry.dot3power.iSrc]);
            else
                cJSON_AddLangStringToObject(data, CGI_LLDP_DOT3PWRSRC, "lldp", langPoEPDSource[lldp_entry.dot3power.iSrc]);
    #endif
            cJSON_AddLangStringToObject(data, CGI_LLDP_DOT3PWRPRI, "lldp", langPri[lldp_entry.dot3power.iPri]);
            cJSON_AddLangStringArgToObject(data, CGI_LLDP_REQUESTPWRVAL, "lldp","lblWatts", "%d,%d", lldp_entry.dot3power.requestPower / 10, lldp_entry.dot3power.requestPower % 10);
            cJSON_AddLangStringArgToObject(data, CGI_LLDP_ALLOCATEPWRVAL, "lldp","lblWatts", "%d,%d", lldp_entry.dot3power.allocatePower / 10, lldp_entry.dot3power.allocatePower % 10);
        }
    }

    if(TRUE == lldp_entry.maxFrameSize.valid)
        cJSON_AddLangStringArgToObject(data, CGI_LLDP_MAXFRMSIZE, "common", "txtArg", "%d", lldp_entry.maxFrameSize.maxFrameSize);

    if(lldp_entry.trunk.valid)
    {
        cJSON_AddLangStringToObject(data, CGI_LLDP_AGGCAP, "lldp", langAggCapa[lldp_entry.trunk.iCapa]);
        cJSON_AddLangStringToObject(data, CGI_LLDP_AGGSTATUS, "lldp", langAggState[lldp_entry.trunk.iState]);
        cJSON_AddNumberToObject(data, CGI_LLDP_AGGPORTID, lldp_entry.trunk.id);
    }

    if(lldp_entry.pvid.valid)
        cJSON_AddNumberToObject(data, CGI_LLDP_PVID, lldp_entry.pvid.pvid);

    sal_lldp_portRemoteEntryInfo_get(lp, 0, &lldp_entry);

    vids = cJSON_CreateArray();
    for(i = 0; i < lldp_entry.vlanNum; i ++)
    {
        sal_lldp_portRemoteVlanNameTlv_get(lp, i, &vlanTlv);

        if(TRUE == vlanTlv.valid)
        {
            entry = cJSON_CreateObject();
            cJSON_AddNumberToObject(entry, CGI_LLDP_VID, vlanTlv.vid);
            cJSON_AddStringToObject(entry, CGI_LLDP_VLANNAME, vlanTlv.name.str);
            cJSON_AddItemToArray(vids, entry);
        }
    }
    if(0 != cJSON_GetArraySize(vids))
        cJSON_AddItemToObject(data, CGI_LLDP_VIDS, vids);
    else
        cJSON_Delete(vids);

#ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_ERR_CHK(sal_lldp_portMedRemoteEntryInfo_get(lp, index, &lldpMed_entry));
    if(lldpMed_entry.medCapa.valid)
    {
        capSupport = cJSON_CreateArray();
        capCurrent = cJSON_CreateArray();

        for(i = 0; i < 6; i ++)
        {
            if(lldpMed_entry.medCapa.iCapa & (1 << i))
            {
                entry = cJSON_CreateObject();
                cJSON_AddLangStringToObject(entry, CGI_LLDP_CAP, "lldp", langMedCapa[i]);
                cJSON_AddItemToArray(capSupport, entry);
            }
        }

        for(i = 0; i < 6; i ++)
        {
            if(lldpMed_entry.medCapa.iCapaCurrent & (1 << i))
            {
                entry = cJSON_CreateObject();
                cJSON_AddLangStringToObject(entry, CGI_LLDP_CAP, "lldp", langMedCapa[i]);
                cJSON_AddItemToArray(capCurrent, entry);
            }
        }
        if(0 != cJSON_GetArraySize(capSupport))
            cJSON_AddItemToObject(data, CGI_LLDP_CAPSUPPORT, capSupport);
        else
            cJSON_Delete(capSupport);

        if(0 != cJSON_GetArraySize(capCurrent))
            cJSON_AddItemToObject(data, CGI_LLDP_CURRCAP, capCurrent);
        else
            cJSON_Delete(capCurrent);

        cJSON_AddLangStringToObject(data, CGI_LLDP_DEVCLS, "lldp", langDevClass[lldpMed_entry.medCapa.iDevType]);
    }

    if(TRUE == lldpMed_entry.power.valid)
    {
        cJSON_AddLangStringToObject(data, CGI_LLDP_POEDEVTYPE, "lldp", langPoEType[lldpMed_entry.power.iDevType]);
        if(lldpMed_entry.power.iDevType == 1)
            cJSON_AddLangStringToObject(data, CGI_LLDP_POEPWRSRC, "lldp", langPoEPSESource[lldpMed_entry.power.iSrc]);
        else
            cJSON_AddLangStringToObject(data, CGI_LLDP_POEPWRSRC, "lldp", langPoEPDSource[lldpMed_entry.power.iSrc]);

        cJSON_AddLangStringToObject(data, CGI_LLDP_POEPWRPRI, "lldp", langPri[lldpMed_entry.power.iPri]);
        cJSON_AddLangStringArgToObject(data, CGI_LLDP_POEPWRVAL, "lldp","lblWatts", "%d,%d", lldpMed_entry.power.powerVal / 10, lldpMed_entry.power.powerVal % 10);
    }

    if(lldpMed_entry.hardware_v.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_HWVER, lldpMed_entry.hardware_v.strInventory);

    if(lldpMed_entry.firmware_v.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_FWVER, lldpMed_entry.firmware_v.strInventory);

    if(lldpMed_entry.software_v.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_SWVER, lldpMed_entry.software_v.strInventory);

    if(lldpMed_entry.serial_num.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_SN, lldpMed_entry.serial_num.strInventory);

    if(lldpMed_entry.manufacturer.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_MANUFNAME, lldpMed_entry.manufacturer.strInventory);

    if(lldpMed_entry.model_name.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_MODELNAME, lldpMed_entry.model_name.strInventory);

    if(lldpMed_entry.asset.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_ASSETID, lldpMed_entry.asset.strInventory);

    if(lldpMed_entry.locationCivic.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_CIVIC, lldpMed_entry.locationCivic.data);

    if(lldpMed_entry.locationCoor.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_COORDINATE, lldpMed_entry.locationCoor.data);

    if(lldpMed_entry.locationEcs.valid)
        cJSON_AddStringToObject(data, CGI_LLDP_ECS, lldpMed_entry.locationEcs.data);

    policies = cJSON_CreateArray();

    for(i = 0; i < MAX_LLDP_MED_NETWORK_POLICY_RX; i ++)
    {
        if(lldpMed_entry.networkPolicy[i].valid)
        {
            entry = cJSON_CreateObject();

            cJSON_AddLangStringToObject(entry, CGI_LLDP_APP, "lldp", langPolicy[lldpMed_entry.networkPolicy[i].iType]);

            cJSON_AddNumberToObject(entry, CGI_LLDP_VID, lldpMed_entry.networkPolicy[i].vid);
            if(TRUE == lldpMed_entry.networkPolicy[i].isTagged)
                cJSON_AddStringToObject(entry, CGI_LLDP_VLANTYPE, CGI_LANG("lldp", "lblTagged"));
            else
                cJSON_AddStringToObject(entry, CGI_LLDP_VLANTYPE, CGI_LANG("lldp", "lblUntagged"));

            cJSON_AddNumberToObject(entry, CGI_LLDP_PRI, lldpMed_entry.networkPolicy[i].l2Pri);
            cJSON_AddNumberToObject(entry, CGI_LLDP_DSCP, lldpMed_entry.networkPolicy[i].dscpPri);

            cJSON_AddItemToArray(policies, entry);
        }
    }

    if(0 != cJSON_GetArraySize(policies))
        cJSON_AddItemToObject(data, CGI_LLDP_POLICIES, policies);
    else
        cJSON_Delete(policies);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_statistic_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;
    sys_logic_port_t        lp;
    lldp_statistic_t        lldp_statistic;

    CGI_GET_JSON_CREATE(root, data);
    sal_lldp_statistic_get(&lldp_statistic);

    cJSON_AddNumberToObject(data, CGI_LLDP_INSERTION , lldp_statistic.insertion);
    cJSON_AddNumberToObject(data, CGI_LLDP_DELETION , lldp_statistic.deletion);
    cJSON_AddNumberToObject(data, CGI_LLDP_DROPS , lldp_statistic.drop);
    cJSON_AddNumberToObject(data, CGI_LLDP_AGEOUTS , lldp_statistic.age_out);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        cJSON_AddNumberToObject(port, CGI_LLDP_TXTOTAL, lldp_statistic.port_statistics[lp].tx_frame);
        cJSON_AddNumberToObject(port, CGI_LLDP_RXTOTAL, lldp_statistic.port_statistics[lp].rx_frame);
        cJSON_AddNumberToObject(port, CGI_LLDP_RXDISCARD, lldp_statistic.port_statistics[lp].rx_discard_frame);
        cJSON_AddNumberToObject(port, CGI_LLDP_RXERR, lldp_statistic.port_statistics[lp].rx_error_frame);
        cJSON_AddNumberToObject(port, CGI_LLDP_RXTLVDISCARD, lldp_statistic.port_statistics[lp].rx_discard_tlv);
        cJSON_AddNumberToObject(port, CGI_LLDP_RXTLVUNRECOG, lldp_statistic.port_statistics[lp].rx_unRecognize_tlv);
        cJSON_AddNumberToObject(port, CGI_LLDP_NEIGHBORDELCNT, lldp_statistic.port_statistics[lp].rx_ageOut);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_overloading_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;
    sys_logic_port_t        lp;
    lldp_overLoading_t      overload;

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_lldp_portTlvOverload_get(lp, &overload);

        cJSON_AddNumberToObject(port, CGI_LLDP_USED, overload.totalSize);
        cJSON_AddNumberToObject(port, CGI_LLDP_AVAILABLE, overload.leftSize);
        if(overload.leftSize > 0)
            cJSON_AddLangStringToObject(port, CGI_LLDP_PACKET_STATUS, "lldp", "lblNotOverloading");
        else
            cJSON_AddLangStringToObject(port, CGI_LLDP_PACKET_STATUS, "lldp", "lblOverloading");

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_overloadingDetail_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    char                    *val;
    sys_logic_port_t        lp;
    lldp_overLoading_t      overload;

    if (CGI_IS_PARAM_EXIST(CGI_PORT, val))
        lp = SYS_STR2UINT(val);
    else
        return SYS_ERR_OK;

    CGI_GET_JSON_CREATE(root, data);

    sal_lldp_portTlvOverload_get(lp, &overload);

    cJSON_AddNumberToObject(data, CGI_LLDP_MANDATORYTLVSIZE, overload.mandatory.byte);
    cJSON_AddLangStringToObject(data, CGI_LLDP_MANDOTORYSTATUS, "lldp", langOverload[overload.mandatory.isOverload]);
#ifdef CONFIG_SYS_PROTO_LLDP_MED
    cJSON_AddNumberToObject(data, CGI_LLDP_CAPSIZE, overload.medCapability.byte);
    cJSON_AddLangStringToObject(data, CGI_LLDP_CAPSTATUS, "lldp", langOverload[overload.medCapability.isOverload]);
    cJSON_AddNumberToObject(data, CGI_LLDP_LOCATIONSIZE, overload.medLocation.byte);
    cJSON_AddLangStringToObject(data, CGI_LLDP_LOCATIONSTATUS, "lldp", langOverload[overload.medLocation.isOverload]);
    cJSON_AddNumberToObject(data, CGI_LLDP_POLICYSIZE, overload.medPolicy.byte);
    cJSON_AddLangStringToObject(data, CGI_LLDP_POLICYSTATUS, "lldp", langOverload[overload.medPolicy.isOverload]);
    cJSON_AddNumberToObject(data, CGI_LLDP_INVENTSIZE, overload.medInventory.byte);
    cJSON_AddLangStringToObject(data, CGI_LLDP_INVENTSTATUS, "lldp", langOverload[overload.medInventory.isOverload]);
    cJSON_AddNumberToObject(data, CGI_LLDP_POWERSIZE, overload.medPoe.byte);
    cJSON_AddLangStringToObject(data, CGI_LLDP_POWERSTATUS, "lldp", langOverload[overload.medPoe.isOverload]);
#endif
    cJSON_AddNumberToObject(data, CGI_LLDP_DOT3TLVSIZE, overload.tlv802dot3.byte);
    cJSON_AddLangStringToObject(data, CGI_LLDP_DOT3TLVSTATUS, "lldp", langOverload[overload.tlv802dot3.isOverload]);
    cJSON_AddNumberToObject(data, CGI_LLDP_OPTTLVSIZE, overload.optional.byte);
    cJSON_AddLangStringToObject(data, CGI_LLDP_OPTTLVSTATUS, "lldp", langOverload[overload.optional.isOverload]);
    cJSON_AddNumberToObject(data, CGI_LLDP_DOT1TLVSIZE, overload.tlv802dot1.byte);
    cJSON_AddLangStringToObject(data, CGI_LLDP_DOT1TLVSTATUS, "lldp", langOverload[overload.tlv802dot1.isOverload]);
    cJSON_AddNumberToObject(data, CGI_LLDP_TOTALBYTESUSED, overload.totalSize);
    cJSON_AddNumberToObject(data, CGI_LLDP_TOTALBYTESLEFT, overload.leftSize);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

