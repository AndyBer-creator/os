
#ifndef __CMD_EXT_H__
#define __CMD_EXT_H__

extern int32 cmd_ext_parserOnly_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc);

extern int32 cmd_ext_condHidden_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc);

extern int32 cmd_ext_nosort_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc);

extern int32 cmd_ext_dynDef_translate(cmd_set_func cmdset, char *cmd, char *desc);

extern int32 cmd_ext_dynDef_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc);

extern int32 cmd_ext_deprecated_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc);
#endif 

