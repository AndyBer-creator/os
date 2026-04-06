
#include <include/cgi.h>

static int32 cgi_vlan_surveillance_prop_set(void);
static int32 cgi_vlan_surveillance_portEdit_set(void);
static int32 cgi_vlan_surveillance_oui_set(void);
static int32 cgi_vlan_surveillance_ouiDel_set(void);

const cgi_cmdmap_t cmdSetVlanSurveillanceTbl[] =
{
    {CGI_CMD_VLAN_SURVEILLANCEVLAN_PROP,          cgi_vlan_surveillance_prop_set},
    {CGI_CMD_VLAN_SURVEILLANCEVLAN_PORTEDIT,      cgi_vlan_surveillance_portEdit_set},
    {CGI_CMD_VLAN_SURVEILLANCEVLAN_OUI,           cgi_vlan_surveillance_oui_set},
    {CGI_CMD_VLAN_SURVEILLANCEVLAN_OUIDEL,        cgi_vlan_surveillance_ouiDel_set},
    {NULL, NULL}
};

static int32 cgi_vlan_surveillance_prop_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    int32 aging;
    sys_vid_t vid;
    sys_pri_t pri;
    sys_enable_t remark = DISABLED;
    sys_enable_t state = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        state = ENABLED;
    else
        state = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_VLAN, value))
        vid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_COS_REMARK, value))
        remark = ENABLED;
    else
        remark = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_COS, value))
        pri = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_AGING, value))
        aging = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (DISABLED == state)
    {
        CGI_SET_ERR_HDL(sal_surveillance_vlanState_set(state), ret);
        CGI_SET_ERR_HDL(sal_surveillance_vlanVid_set(vid), ret);
    }
    else
    {
        CGI_SET_ERR_HDL(sal_surveillance_vlanVid_set(vid), ret);
        CGI_SET_ERR_HDL(sal_surveillance_vlanState_set(state), ret);
    }

    CGI_SET_ERR_HDL(sal_surveillance_vlanPri_set(pri), ret);
    CGI_SET_ERR_HDL(sal_surveillance_vlan1pRemarkEnable_set(remark), ret);
    CGI_SET_ERR_HDL(sal_surveillance_vlanAging_set(aging), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_surveillance_portEdit_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_enable_t portEnable = DISABLED;
    surveillance_vlan_port_mode_t portMode;
    surveillance_vlan_cos_mode_t cosMode;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        portEnable = ENABLED;
    else
        portEnable = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_PORT_MODE, value))
        portMode = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_COS_MODE, value))
        cosMode = SYS_STR2UINT(value);
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_surveillance_vlan_portEnable_set(lp, portEnable), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_surveillance_vlan_portMode_set(lp, portMode), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_surveillance_vlan_portCosMode_set(lp, cosMode), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_surveillance_oui_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    surveillance_vlan_oui_t ouicfg;

    SYS_MEM_CLEAR(ouicfg);

    if (CGI_IS_PARAM_EXIST(CGI_IDX, value))
        CGI_SET_ERR_HDL(sal_surveillance_vlanOuiByIdx_set(SYS_STR2UINT(value), &ouicfg), ret);

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_OUI1, value))
    {
        SYS_MEM_CLEAR(ouicfg.ouiName);
        osal_sprintf(ouicfg.ouiName.str, "0x%s", value);
        ouicfg.oui[0] = SYS_STR2UINT(ouicfg.ouiName.str);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_OUI2, value))
    {
        SYS_MEM_CLEAR(ouicfg.ouiName);
        osal_sprintf(ouicfg.ouiName.str, "0x%s", value);
        ouicfg.oui[1] = SYS_STR2UINT(ouicfg.ouiName.str);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_OUI3, value))
    {
        SYS_MEM_CLEAR(ouicfg.ouiName);
        osal_sprintf(ouicfg.ouiName.str, "0x%s", value);
        ouicfg.oui[2] = SYS_STR2UINT(ouicfg.ouiName.str);
    }
    else
        goto cgi_end;

    SYS_MEM_CLEAR(ouicfg.ouiName);

    if (CGI_IS_PARAM_EXIST(CGI_SURVEILLANCEVLAN_OUT_DESP, value))
        strlcpy(ouicfg.ouiName.str, value, sizeof(ouicfg.ouiName.str));

    CGI_SET_ERR_HDL(sal_surveillance_vlanOui_add(&ouicfg), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_surveillance_ouiDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i;
    char **array = NULL;

    surveillance_vlan_oui_t ouicfg;

    SYS_MEM_CLEAR(ouicfg);

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array, &len))
    {
        for(i = 0; i < len; i++)
            CGI_SET_ENTRY_ERR_CONTINUE(sal_surveillance_vlanOuiByIdx_set(SYS_STR2UINT(array[i]), &ouicfg), "Delete");
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

