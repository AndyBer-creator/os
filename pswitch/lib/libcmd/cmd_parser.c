
#include <libcmd/cmd.h>

extern const cmd_inst_tbl_t          cmdSetTbl[];
extern const cmd_save_tbl_t          cmdSaveTbl[];
extern const cmd_save_intfPort_tbl_t cmdSaveIntfPortTbl[];
#ifdef CONFIG_SYS_L3_ROUTE
extern const cmd_save_intfVlan_tbl_t cmdSaveIntfVlanTbl[];
extern const cmd_save_intfLo_tbl_t   cmdSaveIntfLoTbl[];
#endif
extern const cmd_save_line_tbl_t     cmdSaveLineTbl[];
uint32 g_cmdIsDiff;
uint32 g_isAllDiff;
uint32 g_isAllIdentical;
uint32 g_cmdIsParserRun = FALSE;
uint32 g_isCopy2Running = FALSE;

int32 cmd_parser_init(void)
{
    uint32 i;
    uint32 node;
    uint32 nodemask;
    struct cmd_element *cmd;
    char cmdstr[MAX_CMD_STRLEN];

    cmd_sal_init();

    i = 0;
    while (NULL != cmdSetTbl[i].cmdset)
    {
        if (FALSE == cmd_util_parserInstall_check(i))
        {
            i++;
            continue;
        }

        cmd = malloc(sizeof(struct cmd_element));
        osal_memset(cmd, 0, sizeof(struct cmd_element));

        if (cmd_ext_dynDef_install == cmdSetTbl[i].cmdext)
        {
            SYS_MEM_CLEAR(cmdstr);
            osal_strcpy(cmdstr, cmdSetTbl[i].cmdstr);
            cmd_ext_dynDef_translate(cmdSetTbl[i].cmdset, cmdstr, "");
            cmd->string = cmdstr;
        }
        else
        {
            cmd->string = cmdSetTbl[i].cmdstr;
        }
        cmd->func = cmdSetTbl[i].cmdset;

        nodemask = cmdSetTbl[i].nodemask;
        for (node = CONFIG_NODE; node < END_NODE; node++)
        {
            if (SYS_SHIFT_BIT(node) & (nodemask))
            {
                sal_install_element(node, cmd);

                nodemask = ~(SYS_SHIFT_BIT(node)) & nodemask;
            }

            if (0 == nodemask)
                break;
        }

        free(cmd);

        i++;
    }

#ifdef CONFIG_SYS_LIB_CUSTOM
    
    i = 0;
    while (NULL != cmdCustomSetTbl[i].cmdset)
    {
        cmd = malloc(sizeof(struct cmd_element));
        osal_memset(cmd, 0, sizeof(struct cmd_element));

        cmd->string = cmdCustomSetTbl[i].cmdstr;
        cmd->func = cmdCustomSetTbl[i].cmdset;

        nodemask = cmdCustomSetTbl[i].nodemask;
        for (node = CONFIG_NODE; node < END_NODE; node++)
        {
            if (SYS_SHIFT_BIT(node) & (nodemask))
            {
                sal_install_element(node, cmd);
                    break;
            }
        }

        free(cmd);

        i++;
    }
#endif 

    return SYS_ERR_OK;
}

int32 cmd_parser_run(FILE *fp)
{
    int32 ret;
    struct vty *vty;

    vty = vty_new();
    
    vty->fd = 0;
    vty->type = VTY_TERM;
    vty->node = CONFIG_NODE;

    ret = config_init_from_file(vty, fp);

    if (!((CMD_SUCCESS == ret) || (CMD_ERR_NOTHING_TODO == ret)))
    {
        switch(ret)
        {
            case CMD_ERR_AMBIGUOUS:
                SYS_PRINTF("Ambiguous command.\n");
                break;

            case CMD_ERR_NO_MATCH:
                SYS_PRINTF("There is no such command.\n");
                break;

            default:
                break;
        }

        SYS_PRINTF("Error occured during reading below line.\n%s\n", vty->buf);

        if (NULL != vty->buf)
        {
            free(vty->buf);
        }

        free(vty);

        return SYS_ERR_FAILED;
    }

    if (NULL != vty->buf)
    {
        free(vty->buf);
    }

    free(vty);

#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
    {
        FILE *tmpfp;

        if (NULL != (tmpfp = fopen(SYS_AAA_CMDPRIV_EXECCONF_TMPFILE, "w")))
        {
            fprintf(fp, "1");

            fclose(tmpfp);
        }
    }
#endif 

    return SYS_ERR_OK;
}

int32 cmd_configFile2Shm_copy(char *filename)
{
    int32 ret = SYS_ERR_FAILED;
    FILE *fp = NULL;

    fp = fopen(filename, "r");

    if (NULL == fp)
    {
        SYS_PRINTF("File %s is not exist\n", filename);

        return ret;
    }

    g_cmdIsParserRun = TRUE;

    cmd_parser_init();

    cmd_parser_run(fp);

    cmd_terminate();

    g_cmdIsParserRun = FALSE;

    fclose(fp);

    return SYS_ERR_OK;
}

int32 cmd_configValidFile_convert(char *filename)
{
    int32 ret = SYS_ERR_FAILED;

    g_cmdIsParserRun = TRUE;

    cmd_parser_init();

    ret = config_valid_file_convert(filename);

    cmd_terminate();

    g_cmdIsParserRun = FALSE;

    return ret;
}

uint32 cmd_configValidFileCheck_ret(char *filename)
{
    uint32 valid = FALSE;

    g_cmdIsParserRun = TRUE;

    cmd_parser_init();

    valid = config_valid_file_check(filename);

    cmd_terminate();

    g_cmdIsParserRun = FALSE;

    return valid;
}

uint32 cmd_configValidFileHdrCheck_ret(char *filename)
{
    FILE *fp = NULL;
    char cmdstr[SYS_BUF1024_LEN];
    uint32 headerEndChk = FALSE;

    if (strlen(DFLT_SYS_CFG_HEAD_START) == 0)
        return TRUE;

    if (strlen(DFLT_SYS_CFG_HEAD_END) == 0)
        headerEndChk = TRUE;

    if (NULL == (fp = fopen(filename, "r")))
        return FALSE;

    SYS_MEM_CLEAR(cmdstr);
    while (fgets(cmdstr, SYS_BUF1024_LEN, fp))
    {
/*
*	�޸�˵����ԭʼ����ͨ���Ƚ������ļ�ͷ����ͷ�ͽ�β�ַ��������������ж��ģ�
*		���ִ���һ��bug,��������ļ���winϵͳ�Ͻ��б༭֮����ᵼ��ͷ���ַ���
*		�ĳ�������һ���ֽڣ�
*   ��ܷ�����ͨ���жϲ�������ͷ���ַ��������жϣ�
*/
#if 0
        if ((strlen(cmdstr) - 1) != strlen(DFLT_SYS_CFG_HEAD_START))
        {
            fclose(fp);
            return FALSE;
        }

        if (0 == strncmp(cmdstr, DFLT_SYS_CFG_HEAD_START, strlen(cmdstr) - 1))
        {
            if (TRUE == headerEndChk)
            {
                while (fgets(cmdstr, VTY_BUFSIZ, fp))
                {
                    if (0 == strncmp(cmdstr, DFLT_SYS_CFG_HEAD_END, strlen(cmdstr) - 1))
                    {
                        fclose(fp);
                        return TRUE;
                    }
                }

                fclose(fp);
                return FALSE;
            }
            else
            {
                break;
            }
        }
        else
        {
            fclose(fp);
            return FALSE;
        }
#else  
		if(osal_strstr(cmdstr, DFLT_SYS_CFG_HEAD_START) == NULL)
		{
			fclose(fp);
            return FALSE;
		}
		else 
		{
			if (TRUE == headerEndChk)
            {
                while (fgets(cmdstr, VTY_BUFSIZ, fp))
                {
                    if(osal_strstr(cmdstr, DFLT_SYS_CFG_HEAD_END))
                    {
                        fclose(fp);
                        return TRUE;
                    }
                    SYS_MEM_CLEAR(cmdstr);
                }

                fclose(fp);
                return FALSE;
            }
            else
            {
                break;
            }
		}
#endif
        SYS_MEM_CLEAR(cmdstr);
    }

    fclose(fp);

    return TRUE;
}

static int32 _cmd_tmp_randomFilename_get(char *filename)
{
    uint32 fileno = 0;

    srand(time(NULL));
    fileno = rand();

    osal_sprintf(filename, "%s_%d", CMD_TMP_DIFF_FILE_BASENAME, fileno);

    return SYS_ERR_OK;
}

static int32 _cmd_tmp_file_append(FILE *fp, char *filename)
{
    int32 ret = SYS_ERR_OK;
    char *buf = NULL;
    FILE *fp_tmp = NULL;
    uint32 size = 0;

    if (NULL == (fp_tmp = fopen(filename, "r")))
    {
        
        return SYS_ERR_FILE_OPEN;
    }

    if (0 != fseek(fp_tmp, 0, SEEK_END))
    {
        ret = SYS_ERR_FILE_SEEK;

        goto done;
    }

    size = ftell(fp_tmp);

    if ((size <= 0) || (size >= MAX_FILECOPY_BUF_SIZE))
    {
        
        ret = SYS_ERR_FILE_SIZE;

        goto done;
    }

    if (0 != fseek(fp_tmp, 0L, SEEK_SET))
    {
        ret = SYS_ERR_FILE_SEEK;

        goto done;
    }

    buf = malloc(size);
    osal_memset(buf, 0, size);

    if (fread(buf, 1, size, fp_tmp) != size)
    {
        ret = SYS_ERR_FILE_READ;

        goto done;
    }

    if (fwrite(buf, 1, size, fp) != size)
    {
        ret = SYS_ERR_FILE_WRITE;

        goto done;
    }

done:
    fclose(fp_tmp);

    if (NULL != buf)
        free(buf);

    return ret;
}

int32 cmd_parser_intfPortCmd_save(uint32 catid, sys_logic_port_t port, FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char portstr[CAPA_PORT_STR_LEN];
    uint32 i = 0;

    i = 0;
    SYS_MEM_CLEAR(cmd);
    SYS_MEM_CLEAR(portstr);
    LP2STR(port, portstr);
    osal_sprintf(cmd, "interface %s\n", portstr);
    CMD_WRITE_TO_FILE(fp, cmd);

    g_isAllIdentical = TRUE;
    g_isAllDiff = TRUE;
    while (NULL != cmdSaveIntfPortTbl[i].cmdsave)
    {
        if ((catid != cmdSaveIntfPortTbl[i].id) && (catid != CAT_ID_END))
        {
            i++;
            continue;
        }

        g_cmdIsDiff = FALSE;
        cmdSaveIntfPortTbl[i].cmdsave(fp, port);

        if (FALSE == g_cmdIsDiff)
            g_isAllDiff = FALSE;
        else
            g_isAllIdentical = FALSE;

        i++;
    }

#ifdef CONFIG_SYS_LIB_CUSTOM
    i = 0;
    while (NULL != cmdCustomSaveIntfPortTbl[i].cmdsave)
    {
        if ((catid != cmdCustomSaveIntfPortTbl[i].id) && (catid != CAT_ID_END))
        {
            i++;
            continue;
        }

        g_cmdIsDiff = FALSE;
        cmdCustomSaveIntfPortTbl[i].cmdsave(fp, port);

        if (FALSE == g_cmdIsDiff)
            g_isAllDiff = FALSE;
        else
            g_isAllIdentical = FALSE;

        i++;
    }
#endif

    if (FALSE == g_isAllDiff)
        CMD_WRITE_TO_FILE(fp, "!\n");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_L3_ROUTE

int32 cmd_parser_intfVlanCmd_save(uint32 catid, sys_vid_t vid, FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 i = 0;

    i = 0;
    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "interface vlan%d\n", vid);
    CMD_WRITE_TO_FILE(fp, cmd);

    g_isAllIdentical = TRUE;
    g_isAllDiff = TRUE;
    while (NULL != cmdSaveIntfVlanTbl[i].cmdsave)
    {
        if ((catid != cmdSaveIntfVlanTbl[i].id) && (catid != CAT_ID_END))
        {
            i++;
            continue;
        }

        g_cmdIsDiff = FALSE;
        cmdSaveIntfVlanTbl[i].cmdsave(fp, vid);

        if (FALSE == g_cmdIsDiff)
            g_isAllDiff = FALSE;
        else
            g_isAllIdentical = FALSE;

        i++;
    }

    if (FALSE == g_isAllDiff)
        CMD_WRITE_TO_FILE(fp, "!\n");

    return SYS_ERR_OK;
}

int32 cmd_parser_intfLoCmd_save(uint32 catid, uint32 id, FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 i = 0;

    i = 0;
    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "interface loopback%d\n", (id + 1));
    CMD_WRITE_TO_FILE(fp, cmd);

    g_isAllIdentical = TRUE;
    g_isAllDiff = TRUE;
    while (NULL != cmdSaveIntfLoTbl[i].cmdsave)
    {
        if ((catid != cmdSaveIntfLoTbl[i].id) && (catid != CAT_ID_END))
        {
            i++;
            continue;
        }

        g_cmdIsDiff = FALSE;
        cmdSaveIntfLoTbl[i].cmdsave(fp, id);

        if (FALSE == g_cmdIsDiff)
            g_isAllDiff = FALSE;
        else
            g_isAllIdentical = FALSE;

        i++;
    }

    if (FALSE == g_isAllDiff)
        CMD_WRITE_TO_FILE(fp, "!\n");

    return SYS_ERR_OK;
}
#endif

static int32 _cmd_parser_intfPort_save(uint32 catid, FILE *fp)
{
    sys_logic_port_t lp;

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        cmd_parser_intfPortCmd_save(catid, lp, fp);
    }

    return SYS_ERR_OK;
}

static int32 _cmd_parser_intfTrkPort_save(uint32 catid, FILE *fp)
{
    char tmpDiffFname[CAPA_FILE_NAME_LEN];
    FILE *fp_tmp = NULL;
    sys_logic_port_t lp;

    FOR_EACH_TRUNK_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(tmpDiffFname);
        _cmd_tmp_randomFilename_get(tmpDiffFname);

        if (NULL == (fp_tmp = fopen(tmpDiffFname, "w")))
            return SYS_ERR_FILE_OPEN;

        cmd_parser_intfPortCmd_save(catid, lp, fp_tmp);

        fclose(fp_tmp);

        if (FALSE == g_isAllIdentical)
            _cmd_tmp_file_append(fp, tmpDiffFname);

        unlink(tmpDiffFname);
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_L3_ROUTE
static int32 _cmd_parser_intfVlan_save(uint32 catid, FILE *fp)
{
    char tmpDiffFname[CAPA_FILE_NAME_LEN];
    FILE *fp_tmp = NULL;
    uint32 i = 0;
    sys_vid_t vid;
    sys_vlan_adminEntry_t adminEntry;

    FOR_EACH_VLAN_STATIC_ENTRY_IDX(i)
    {
        SYS_MEM_CLEAR(tmpDiffFname);
        _cmd_tmp_randomFilename_get(tmpDiffFname);

        if (NULL == (fp_tmp = fopen(tmpDiffFname, "w")))
            return SYS_ERR_FILE_OPEN;

        sal_vlan_adminEntrySort_get(i, &vid, &adminEntry);
        cmd_parser_intfVlanCmd_save(catid, vid, fp_tmp);

        fclose(fp_tmp);

        if (FALSE == g_isAllIdentical)
            _cmd_tmp_file_append(fp, tmpDiffFname);

        unlink(tmpDiffFname);
    }

    return SYS_ERR_OK;
}

static int32 _cmd_parser_intfLo_save(uint32 catid, FILE *fp)
{
    char tmpDiffFname[CAPA_FILE_NAME_LEN];
    FILE *fp_tmp = NULL;
    uint32 i = 0;
    uint32 id = 0;

    for (i = 0; i < CAPA_L3_INTFLO_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpDiffFname);
        _cmd_tmp_randomFilename_get(tmpDiffFname);

        if (NULL == (fp_tmp = fopen(tmpDiffFname, "w")))
            return SYS_ERR_FILE_OPEN;

        cmd_parser_intfLoCmd_save(catid, id, fp_tmp);

        fclose(fp_tmp);

        if (FALSE == g_isAllIdentical)
            _cmd_tmp_file_append(fp, tmpDiffFname);

        unlink(tmpDiffFname);
    }

    return SYS_ERR_OK;
}
#endif

static int32 _cmd_parser_line_save(uint32 catid, FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char tmpDiffFname[CAPA_FILE_NAME_LEN];
    uint32 i = 0;
    FILE *fp_tmp = NULL;
    sys_line_cliType_t lineType = 0;

    for (lineType = 0; lineType < SYS_LINE_CLI_END; lineType++)
    {
        g_isAllIdentical = TRUE;

        SYS_MEM_CLEAR(tmpDiffFname);
        _cmd_tmp_randomFilename_get(tmpDiffFname);

        if (NULL == (fp_tmp = fopen(tmpDiffFname, "w")))
            return SYS_ERR_FILE_OPEN;

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "line %s\n", text_line_cli[lineType].text);
        CMD_WRITE_TO_FILE(fp_tmp, cmd);

        i = 0;
        while (NULL != cmdSaveLineTbl[i].cmdsave)
        {
            if ((catid != cmdSaveLineTbl[i].id) && (catid != CAT_ID_END))
            {
                i++;
                continue;
            }

            g_cmdIsDiff = FALSE;
            cmdSaveLineTbl[i].cmdsave(fp_tmp, lineType);

            if (TRUE == g_cmdIsDiff)
                g_isAllIdentical = FALSE;

            i++;
        }

#ifdef CONFIG_SYS_LIB_CUSTOM
        i = 0;
        while (NULL != cmdCustomSaveLineTbl[i].cmdsave)
        {
            if ((catid != cmdCustomSaveLineTbl[i].id) && (catid != CAT_ID_END))
            {
                i++;
                continue;
            }

            g_cmdIsDiff = FALSE;
            cmdCustomSaveLineTbl[i].cmdsave(fp_tmp, lineType);

            if (TRUE == g_cmdIsDiff)
                g_isAllIdentical = FALSE;

            i++;
        }
#endif

        fclose(fp_tmp);

        if (FALSE == g_isAllIdentical)
            _cmd_tmp_file_append(fp, tmpDiffFname);

        unlink(tmpDiffFname);
    }

    return SYS_ERR_OK;
}

int32 cmd_parser_runningFile_print(uint32 catid, char *toFile)
{
    char header[SYS_CMD_CONFIGFILEHDR_LEN];
    FILE *fp = NULL;
    uint32 i = 0;
    uint32 j = 0;

    fp = fopen(toFile, "w");

    if (NULL == fp)
    {
        SYS_PRINTF("Open %s fail\n", toFile);

        return SYS_ERR_FAILED;
    }

    osal_memset(header, 0, sizeof(header));

    cmd_util_cfgFileHdr_print(header);

    fwrite(header, 1, osal_strlen(header), fp);
    
    CMD_WRITE_TO_FILE(fp, "!\n");
    CMD_WRITE_TO_FILE(fp, "!\n");
    CMD_WRITE_TO_FILE(fp, "!\n");

    i = 0;
    while (1)
    {
        
        if (cmdSaveTbl[i].id < CAT_ID_SPECIAL_END)
        {
            switch (cmdSaveTbl[i].id)
            {
                case CAT_ID_IF_PORT:
                    _cmd_parser_intfTrkPort_save(catid, fp);
                    _cmd_parser_intfPort_save(catid, fp);
                    break;

#ifdef CONFIG_SYS_L3_ROUTE
                case CAT_ID_IF_VLAN:
                    _cmd_parser_intfVlan_save(catid, fp);
                    break;

                case CAT_ID_IF_LO:
                    _cmd_parser_intfLo_save(catid, fp);
                    break;
#endif

                case CAT_ID_LINE:
                    _cmd_parser_line_save(catid, fp);
                    break;

#ifdef CONFIG_SYS_LIB_CUSTOM
                case CAT_ID_CUSTOM:
                    j = 0;
                    
                    while (1)
                    {
                        if (NULL == cmdCustomSaveTbl[j].cmdsave)
                            break;

                        g_cmdIsDiff = FALSE;
                        cmdCustomSaveTbl[j].cmdsave(fp);

                        if (FALSE == g_cmdIsDiff)
                            CMD_WRITE_TO_FILE(fp, "!\n");

                        j++;
                    }
                    break;
#endif 

                default:
                    break;
            }
        }
        
        else if (NULL == cmdSaveTbl[i].cmdsave)
        {
            break;
        }
        
        else if ((catid == cmdSaveTbl[i].id) || (catid == CAT_ID_END))
        {
          
            cmdSaveTbl[i].cmdsave(fp);
            g_cmdIsDiff = FALSE;
            if (FALSE == g_cmdIsDiff)
                CMD_WRITE_TO_FILE(fp, "!\n");
        }

        i++;
    }

#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
    {
        char cmd[MAX_CMD_STRLEN];
        FILE *tmpfp;

        system("cli -g");

        fwrite("\n# Command Privilege\n", 1, osal_strlen("\n# Command Privilege\n"), fp);

        if ((tmpfp = fopen(SYS_AAA_CMDPRIV_CONF_TMPFILE, "r")) != NULL)
        {
            osal_memset(cmd, 0, sizeof(cmd));

            while (fgets(cmd, sizeof(cmd), tmpfp))
            {
                fwrite(cmd, 1, osal_strlen(cmd), fp);
            }

            fclose(tmpfp);

            unlink(SYS_AAA_CMDPRIV_CONF_TMPFILE);
        }
    }
#endif 

    fwrite("\n", 1, osal_strlen("\n"), fp);

    fclose(fp);

    return SYS_ERR_OK;
}

int32 cmd_catName_get(cat_id_t id, char *catname)
{
    int32 i;

    i = 0;
    while (-1 != catNameTbl[i].id)
    {
        if (id == catNameTbl[i].id)
        {
            if (NULL != catNameTbl[i].name)
            {
                osal_strcpy(catname, catNameTbl[i].name);

                return SYS_ERR_OK;
            }
        }

        i++;
    }

    return SYS_ERR_FAILED;
}

uint32 cmd_isParserRun_ret(void)
{
    if (g_cmdIsParserRun || g_isCopy2Running)
        return TRUE;

    return FALSE;
}

int32 cmd_parser_common_running_clone_config_File_print(uint32 catid, char *toFile)
{
    char header[SYS_CMD_CONFIGFILEHDR_LEN];
    FILE *fp = NULL;
    uint32 i = 0;
    uint32 j = 0;

    fp = fopen(toFile, "w");

    if (NULL == fp)
    {
        SYS_PRINTF("Open %s fail\n", toFile);

        return SYS_ERR_FAILED;
    }

    osal_memset(header, 0, sizeof(header));
    cmd_util_cfgFileHdr_print(header);
    fwrite(header, 1, osal_strlen(header), fp);

    i = 0;
    while (1)
    {
    	
    	switch(cmdSaveTbl[i].id)
    	{
			case CAT_ID_SYSTEM :
				i++;
				continue;
			default:
                    break;
			
    	}

        if (cmdSaveTbl[i].id < CAT_ID_SPECIAL_END)
        {
            switch (cmdSaveTbl[i].id)
            {
                case CAT_ID_IF_PORT:
                    _cmd_parser_intfTrkPort_save(catid, fp);
                    _cmd_parser_intfPort_save(catid, fp);
                    break;

#ifdef CONFIG_SYS_L3_ROUTE
                case CAT_ID_IF_VLAN:
                    _cmd_parser_intfVlan_save(catid, fp);
                    break;

                case CAT_ID_IF_LO:
                    _cmd_parser_intfLo_save(catid, fp);
                    break;
#endif

                case CAT_ID_LINE:
                    _cmd_parser_line_save(catid, fp);
                    break;

#ifdef CONFIG_SYS_LIB_CUSTOM
                case CAT_ID_CUSTOM:
                    j = 0;
                    
                    while (1)
                    {
                        if (NULL == cmdCustomSaveTbl[j].cmdsave)
                            break;

                        g_cmdIsDiff = FALSE;
                        cmdCustomSaveTbl[j].cmdsave(fp);

                        if (FALSE == g_cmdIsDiff)
                            CMD_WRITE_TO_FILE(fp, "!\n");

                        j++;
                    }
                    break;
#endif 

                default:
                    break;
            }
        }
        
        else if (NULL == cmdSaveTbl[i].cmdsave)
        {
            break;
        }
        
        else if ((catid == cmdSaveTbl[i].id) || (catid == CAT_ID_END))
        {
            g_cmdIsDiff = FALSE;
            cmdSaveTbl[i].cmdsave(fp);

            if (FALSE == g_cmdIsDiff)
                CMD_WRITE_TO_FILE(fp, "!\n");
        }

        i++;
    }

#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
    {
        char cmd[MAX_CMD_STRLEN];
        FILE *tmpfp;

        system("cli -g");

        fwrite("\n# Command Privilege\n", 1, osal_strlen("\n# Command Privilege\n"), fp);

        if ((tmpfp = fopen(SYS_AAA_CMDPRIV_CONF_TMPFILE, "r")) != NULL)
        {
            osal_memset(cmd, 0, sizeof(cmd));

            while (fgets(cmd, sizeof(cmd), tmpfp))
            {
                fwrite(cmd, 1, osal_strlen(cmd), fp);
            }

            fclose(tmpfp);

            unlink(SYS_AAA_CMDPRIV_CONF_TMPFILE);
        }
    }
#endif 

    fwrite("\n", 1, osal_strlen("\n"), fp);

    fclose(fp);

    return SYS_ERR_OK;
}

#define CFG_FILE_MODULE_HDR_LINE(hdr, fmt, args...)    \
do {                                                  \
    char __line[256];                                 \
    osal_memset(__line, 0, sizeof(__line));           \
    osal_sprintf(__line, "! "fmt"\n", ## args); \
    osal_strcat(hdr, __line);                         \
} while (0)

int32 cmd_parser_config_by_module_print(char* pkey, char *toFile)
{
    char header[SYS_CMD_CONFIGFILEHDR_LEN];
    FILE *fp = NULL;
    uint32 i = 0;
    uint32 j = 0;
	char catName[32];
	int catId = 0;

	i = 0;
	osal_memset(catName, 0, sizeof(catName));
	
    while (-1 != catSaveNameTbl[i].id && NULL != catSaveNameTbl[i].name)
    {
        if (osal_strncmp(catSaveNameTbl[i].name, pkey, osal_strlen(catSaveNameTbl[i].name)) == 0)
        {
            osal_strcpy(catName, catSaveNameTbl[i].name); 
            catId = catSaveNameTbl[i].id;
            break;
        }

        i++;
    }
	if(osal_strlen(catName) == 0 && catId == 0)
		return SYS_ERR_FAILED;
		
    fp = fopen(toFile, "w");

    if (NULL == fp)
    {
        SYS_PRINTF("Open %s fail\n", toFile);

        return SYS_ERR_FAILED;
    }

    osal_memset(header, 0, sizeof(header));

	CFG_FILE_MODULE_HDR_LINE(header, "[%s running-config]", catName);

    fwrite(header, 1, osal_strlen(header), fp);

    i = 0;
    while (1)
    {
        
        if (cmdSaveTbl[i].id < CAT_ID_SPECIAL_END)
        {
            switch (cmdSaveTbl[i].id)
            {
                case CAT_ID_IF_PORT:
                    _cmd_parser_intfTrkPort_save(catId, fp);
                    _cmd_parser_intfPort_save(catId, fp);
                    break;

#ifdef CONFIG_SYS_L3_ROUTE
                case CAT_ID_IF_VLAN:
                    _cmd_parser_intfVlan_save(catId, fp);
                    break;

                case CAT_ID_IF_LO:
                    _cmd_parser_intfLo_save(catId, fp);
                    break;
#endif

                case CAT_ID_LINE:
                    _cmd_parser_line_save(catId, fp);
                    break;

#ifdef CONFIG_SYS_LIB_CUSTOM
                case CAT_ID_CUSTOM:
                    j = 0;
                    
                    while (1)
                    {
                        if (NULL == cmdCustomSaveTbl[j].cmdsave)
                            break;

                        g_cmdIsDiff = FALSE;
                        cmdCustomSaveTbl[j].cmdsave(fp);

                        if (FALSE == g_cmdIsDiff)
                            CMD_WRITE_TO_FILE(fp, "!\n");

                        j++;
                    }
                    break;
#endif 

                default:
                    break;
            }
        }
        
        else if (NULL == cmdSaveTbl[i].cmdsave)
        {
            break;
        }
        
        else if ((catId == cmdSaveTbl[i].id) || (catId == CAT_ID_END))
        {
            g_cmdIsDiff = FALSE;
            cmdSaveTbl[i].cmdsave(fp);

            if (FALSE == g_cmdIsDiff)
                CMD_WRITE_TO_FILE(fp, "!\n");
        }

        i++;
    }

    fwrite("\n", 1, osal_strlen("\n"), fp);

    fclose(fp);

    return SYS_ERR_OK;
}

