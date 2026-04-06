
#include <lib/libcgiutil/cgi_util.h>
#include <lib/libcJSON/cJSON.h>

#include <include/cgi_error.h>
#include <include/cgi_map_cmd.h>
#include <include/cgi_map_param.h>

#define CGI_GET_DATA_KEY "data"
#define JS_FILELOAD_VER "1.0.0.6"

#define CGI_GET_JSON_CREATE(_root, _data) \
do{ \
  _root = cJSON_CreateObject(); \
  _data = cJSON_CreateObject(); \
  cJSON_AddItemToObject(_root, CGI_GET_DATA_KEY, _data); \
}while(0)

#define CGI_GET_JSON_RESPONSE_AND_DESTROY(_root) \
do { \
  CGI_RESPONSE_JSON(_root); \
  cJSON_Delete(_root); \
}while(0)

#define CGI_LANG(category, item) "lang('"category"','"item"')"
#define CGI_LANG_ARG(category, item, arg) "lang('"category"','"item"',["arg"])"

#define CGI_SET_ERR_HDL(op, ret) \
do {                                    \
    if ((ret = (op)) != SYS_ERR_OK){   \
        goto cgi_end;}                \
} while(0)

#define CGI_SET_PORT_ERR_CONTINUE(op, lp)           \
{                                                   \
    int32 _ret = SYS_ERR_FAILED;                    \
    if ((_ret = (op)) != SYS_ERR_OK){               \
        char _portStr[CAPA_PORT_STR_LEN];            \
        char _msg[SYS_BUF512_LEN];                  \
        SYS_MEM_CLEAR(_portStr);                    \
        SYS_MEM_CLEAR(_msg);                        \
        LP2WEBSTR(lp, _portStr);                    \
        osal_sprintf(_msg, "%s: %s", _portStr, SYS_ERR_DES(_ret));\
        cgi_set_respErrMsgs_add(_msg);              \
        continue;                                   \
    }                                               \
}

#define CGI_SET_VID_ERR_CONTINUE(op, vid)            \
{                                                   \
    int32 _ret = SYS_ERR_FAILED;                    \
    if ((_ret = (op)) != SYS_ERR_OK){               \
        char _msg[SYS_BUF512_LEN];                  \
        SYS_MEM_CLEAR(_msg);                        \
        osal_sprintf(_msg, "VLAN %u: %s", vid, SYS_ERR_DES(_ret));\
        cgi_set_respErrMsgs_add(_msg);              \
        continue;                                   \
    }                                               \
}

#define CGI_SET_ENTRY_ERR_CONTINUE(op, entryDescp)  \
{                                                   \
    int32 _ret = SYS_ERR_FAILED;                    \
    if ((_ret = (op)) != SYS_ERR_OK){               \
        char _msg[SYS_BUF512_LEN];                  \
        SYS_MEM_CLEAR(_msg);                        \
        osal_sprintf(_msg, "%s: %s", entryDescp, SYS_ERR_DES(_ret));\
        cgi_set_respErrMsgs_add(_msg);              \
        continue;                                   \
    }                                               \
}

#define CGI_SET_ERR_CONTINUE(op, ret) if ((ret = (op)) != SYS_ERR_OK) continue;

extern void cgi_set_respGeneral_send(int32 ret);
extern void cgi_set_respLangErrWithArg_send(char *langCat, char *langKey, char* fmt, ...);
extern void cgi_set_respErrMsgs_add(char *msg);
