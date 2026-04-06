
#include <include/cgi.h>

#include <libsal/sal_sys.h>
#include <libsal/sal_port.h>
#ifdef CONFIG_SYS_POE
#include <libsal/sal_poe.h>
#endif

#define PANEL_BKIMG_24                      "switch_panel.png"
#define PANEL_BKIMG_48                      "switch_panel_48.png"
#define PANEL_BKIMG_EMPTY_24                "switch_panel_empty.png"
#define PANEL_BKIMG_EMPTY_48                "switch_panel_empty_big.png"
#define PANEL_LOGOIMG                       "logo_switch_panel.png"
#define PANEL_COPPER_LED_NOLINK             "Copper_LED.png"
#define PANEL_COPPER_LED_LINKUP_10M         "Copper_LED_linkup_10M.png"
#define PANEL_COPPER_LED_LINKUP_100M        "Copper_LED_linkup_100M.png"
#define PANEL_COPPER_LED_LINKUP_1000M       "Copper_LED_linkup_1000M.png"
#define PANEL_COPPER_LED_LINKUP_2500M       "Copper_LED_linkup_2500M.png"
#define PANEL_COPPER_RJ45_NOLINK_BOT        "RJ45_nolink_bot.png"
#define PANEL_COPPER_RJ45_NOLINK_TOP        "RJ45_nolink_top.png"
#define PANEL_COPPER_RJ45_LINKUP_BOT        "RJ45_linkup_bot.png"
#define PANEL_COPPER_RJ45_LINKUP_TOP        "RJ45_linkup_top.png"
#define PANEL_COPPER_RJ45_LINKUP_10M_BOT    "RJ45_linkup_bot.png"
#define PANEL_COPPER_RJ45_LINKUP_10M_TOP    "RJ45_linkup_top.png"
#define PANEL_COPPER_RJ45_LINKUP_100M_BOT   "RJ45_linkup_bot.png"
#define PANEL_COPPER_RJ45_LINKUP_100M_TOP   "RJ45_linkup_top.png"
#define PANEL_COPPER_RJ45_LINKUP_1000M_BOT  "RJ45_linkup_bot.png"
#define PANEL_COPPER_RJ45_LINKUP_1000M_TOP  "RJ45_linkup_top.png"
#define PANEL_COPPER_RJ45_LINKUP_2500M_BOT  "RJ45_linkup_bot.png"
#define PANEL_COPPER_RJ45_LINKUP_2500M_TOP  "RJ45_linkup_top.png"
#define PANEL_FIBER_NOLINK                  "Fiber_DB.png"
#define PANEL_FIBER_LINKUP                  "Fiber_linkup_DB.png"
#define PANEL_FIBER_LED_NOLINK              "Copper_LED.png"
#define PANEL_FIBER_LED_LINKUP_10M          "Copper_LED_linkup_10M.png"
#define PANEL_FIBER_LED_LINKUP_100M         "Copper_LED_linkup_100M.png"
#define PANEL_FIBER_LED_LINKUP_1000M        "Copper_LED_linkup_1000M.png"
#define PANEL_LED_SINGLE_GRAY               "S_H_LED.png"
#define PANEL_LED_SINGLE_GREER              "S_G_LED.png"
#define PANEL_LED_SINGLE_YELLOW             "S_Y_LED.png"
#define PANEL_CONSOLE                       "Console.png"
#define PANEL_USB                           "USB.png"

typedef enum panel_img_type_e
{
    IMG_TYPE_COPPER_LED = 0,
    IMG_TYPE_COPPER_JACK,
    IMG_TYPE_COPPER_JACK_DOWN,
    IMG_TYPE_FIBER,
    IMG_TYPE_FIBER_LED,
    IMG_TYPE_PORTNUM = 5,
    IMG_TYPE_POE_LED,
    IMG_TYPE_SYS_LED,
    IMG_TYPE_FONT,
    IMG_TYPE_CONSOLE,
    IMG_TYPE_USB = 10,
    IMG_TYPE_END
} panel_img_type_t;

typedef enum panel_align_type_e
{
    ALIGN_TYPE_PLAIN = 0,
    ALIGN_TYPE_STACK,
    ALIGN_TYPE_STACK_REV,
    ALIGN_TYPE_END
} panel_align_type_t;

typedef struct webui_panel_s
{
    uint32 usrPortStart;
    uint32 usrPortEnd;
    int32  imgType;
    int32  imgWidth;
    int32  alignType;
    int32  oddYpos;
    int32  evenYpos;
    int32  baseXpos;
    int32  groupNum;
    int32  gapWidth;
    char   showDes[8];
} webui_panel_t;
#ifdef CONFIG_SYS_KD_BOARD
const webui_panel_t switchPanel_48_4[] =
{
    
    { 1, 48, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 150, 8, 5},
    { 1, 48, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 150, 8, 5},

    {49, 52, IMG_TYPE_FIBER, 30, ALIGN_TYPE_PLAIN, 62, -1, 750, 1, 10},
    {49, 52, IMG_TYPE_PORTNUM, 30, ALIGN_TYPE_PLAIN, 62, -1, 750, 1, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};
#else
const webui_panel_t switchPanel_48_4[] =
{
    { 1, 48, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 80, 8, -1},
    { 1, 48, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 281, 8, 5},
    { 1, 48, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 281, 8, 5},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};
#endif
const webui_panel_t switchPanel_48[] =
{
    { 1, 48, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 80, 8, -1},
    { 1, 48, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 281, 8, 5},
    { 1, 48, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 281, 8, 5},

    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_48_8393_HO[] =
{
    { 1, 48, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 80, 8, -1},
    { 1, 48, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 41, 62, 281, 8, 5},
    { 1, 48, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 281, 8, 5},
    {49, 52, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 860, 4, -1},
    {49, 52, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 41, 62, 880, 4, -1},
    {49, 52, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 880, 4, -1},

    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_48_6[] =
{
    { 1, 48, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 80, 8, -1},
    { 1, 48, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 281, 8, 5},
    { 1, 48, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 281, 8, 5},
    {49, 54, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 860, 4, -1},
    {49, 50, IMG_TYPE_FIBER, 22, ALIGN_TYPE_STACK, 62, 41, 880, 2, 10},
    {49, 50, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 880, 2, 10},
    {51, 52, IMG_TYPE_FIBER, 22, ALIGN_TYPE_STACK, 62, 41, 915, 2, 10},
    {51, 52, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 915, 2, 10},
    {53, 54, IMG_TYPE_FIBER, 22, ALIGN_TYPE_STACK, 62, 41, 950, 2, 10},
    {53, 54, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 950, 2, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_24_4GC[] =
{
    { 1, 24, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 67, 8, -1},
    { 1, 24, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    { 1, 24, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    {25, 28, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 500, 4, -1},
    {25, 28, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 520, 4, -1},
    {25, 28, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 520, 4, -1},
    {25, 28, IMG_TYPE_FIBER, 30, ALIGN_TYPE_PLAIN, 62, -1, 550, 1, 10},
    {25, 28, IMG_TYPE_PORTNUM, 30, ALIGN_TYPE_PLAIN, 62, -1, 555, 1, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_10_4GF[] =
{
    { 1, 10, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 67, 8, -1},
    { 1, 10, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    { 1, 10, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    {11, 14, IMG_TYPE_FIBER, 30, ALIGN_TYPE_PLAIN, 62, -1, 400, 1, 10},
    {11, 14, IMG_TYPE_PORTNUM, 30, ALIGN_TYPE_PLAIN, 62, -1, 400, 1, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_16_2[] =
{
    { 1, 16, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 67, 8, -1},
    { 1, 16, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    { 1, 16, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    {17, 18, IMG_TYPE_FIBER, 30, ALIGN_TYPE_PLAIN, 62, -1, 420, 1, 10},
    {17, 18, IMG_TYPE_PORTNUM, 30, ALIGN_TYPE_PLAIN, 62, -1, 420, 1, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_24_2GF[] =
{
    { 1, 24, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 67, 8, -1},
    { 1, 24, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    { 1, 24, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    {25, 26, IMG_TYPE_FIBER, 30, ALIGN_TYPE_PLAIN, 62, -1, 550, 1, 10},
    {25, 26, IMG_TYPE_PORTNUM, 30, ALIGN_TYPE_PLAIN, 62, -1, 555, 1, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_24_4[] =
{
    { 1, 24, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 67, 8, -1},
    { 1, 24, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    { 1, 24, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    {25, 28, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 500, 4, -1},
    {25, 26, IMG_TYPE_FIBER, 22, ALIGN_TYPE_STACK, 62, 41, 520, 2, 10},
    {25, 26, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 520, 2, 10},
    {27, 28, IMG_TYPE_FIBER, 22, ALIGN_TYPE_STACK, 62, 41, 555, 2, 10},
    {27, 28, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 555, 2, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_9301_24_4[] =
{
    { 1, 24, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 67, 8, -1},
    { 1, 24, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    { 1, 24, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, 5},
    {25, 28, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 500, 4, -1},
    {25, 26, IMG_TYPE_FIBER, 22, ALIGN_TYPE_STACK_REV, 62, 41, 520, 2, 10},
    {25, 26, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK_REV, 62, 41, 520, 2, 10},
    {27, 28, IMG_TYPE_FIBER, 22, ALIGN_TYPE_STACK_REV, 62, 41, 555, 2, 10},
    {27, 28, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK_REV, 62, 41, 555, 2, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_8_2GC[] =
{
    { 1,  8, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 67, 8, -1},
    { 1,  8, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, -1},
    { 1,  8, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 201, 8, -1},
    { 9, 10, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK, 63, 49, 320, 2, -1},
    { 9, 10, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK, 62, 41, 340, 2, -1},
    { 9, 10, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK, 62, 41, 340, 2, -1},
    { 9, 10, IMG_TYPE_FIBER, 30, ALIGN_TYPE_PLAIN, 62, -1, 370, 1, 10},
    { 9, 10, IMG_TYPE_PORTNUM, 30, ALIGN_TYPE_PLAIN, 62, -1, 370, 1, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_4_2GF[] =
{
    { 1,  4, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK_REV, 63, 49, 67, 8, -1},
    { 1,  4, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK_REV, 62, 41, 160, 8, -1},
    { 1,  4, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK_REV, 62, 41, 160, 8, -1},
    { 5,  6, IMG_TYPE_FIBER, 30, ALIGN_TYPE_PLAIN, 62, -1, 250, 1, 10},
    { 5,  6, IMG_TYPE_PORTNUM, 30, ALIGN_TYPE_PLAIN, 62, -1, 250, 1, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_8_2GF[] =
{
    { 1,  8, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_STACK_REV, 63, 49, 67, 8, -1},
    { 1,  8, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_STACK_REV, 62, 41, 201, 8, -1},
    { 1,  8, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_STACK_REV, 62, 41, 201, 8, -1},
    { 9, 10, IMG_TYPE_FIBER, 30, ALIGN_TYPE_PLAIN, 62, -1, 290, 1, 10},
    { 9, 10, IMG_TYPE_PORTNUM, 30, ALIGN_TYPE_PLAIN, 62, -1, 290, 1, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

webui_panel_t switchPanel_12XGF[] =
{
    { 1, 12, IMG_TYPE_FIBER, 30, ALIGN_TYPE_PLAIN, 62, -1, 290, 1, 10},
    { 1, 12, IMG_TYPE_PORTNUM, 30, ALIGN_TYPE_PLAIN, 62, -1, 290, 1, 10},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

const webui_panel_t switchPanel_8_2500MC[] =
{
    { 1,  8, IMG_TYPE_COPPER_LED, 6, ALIGN_TYPE_PLAIN, 63, 49, 67, 4, -1},
    { 1,  8, IMG_TYPE_COPPER_JACK, 22, ALIGN_TYPE_PLAIN, 62, 41, 201, 4, 20},
    { 1,  8, IMG_TYPE_PORTNUM, 22, ALIGN_TYPE_PLAIN, 62, 41, 201, 4, 20},
    
    {0, 0, -1, -1, -1, -1, -1, -1, -1}
};

static int32 cgi_panel_layout_get(void);
static int32 cgi_panel_info_get(void);

const cgi_cmdmap_t cmdGetPanelTbl[] =
{
    {CGI_CMD_PANEL_LAYOUT,          cgi_panel_layout_get},
    {CGI_CMD_PANEL_INFO,            cgi_panel_info_get},
    {NULL, NULL}
};

static uint32 _web_panel_tblSize_ret(webui_panel_t *pPanel)
{
    uint32 count = 0;

    while(!(pPanel[count].usrPortStart == 0 && pPanel[count].usrPortEnd == 0))
    {
        count++;
    }
    return count;
}

static int32 cgi_panel_layout_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *layouts = NULL;
    cJSON *layout = NULL;
    uint32 i = 0;
    sys_boardModel_Id_t boardId;
    webui_panel_t *pCurrSwitchPanelTbl = NULL;

    sal_sys_boardModelId_get(&boardId);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddStringToObject(data, "logoImg", PANEL_LOGOIMG);

    cJSON_AddStringToObject(data, "COPPER_LED_NOLINK", PANEL_LED_SINGLE_GRAY);
    cJSON_AddStringToObject(data, "COPPER_LED_LINKUP_10M", PANEL_LED_SINGLE_GREER);
    cJSON_AddStringToObject(data, "COPPER_LED_LINKUP_100M", PANEL_LED_SINGLE_GREER);
    cJSON_AddStringToObject(data, "COPPER_LED_LINKUP_1000M", PANEL_LED_SINGLE_GREER);
    cJSON_AddStringToObject(data, "COPPER_LED_LINKUP_2500M", PANEL_LED_SINGLE_GREER);
    cJSON_AddStringToObject(data, "COPPER_RJ45_NOLINK_BOT", PANEL_COPPER_RJ45_NOLINK_BOT);
    cJSON_AddStringToObject(data, "COPPER_RJ45_NOLINK_TOP", PANEL_COPPER_RJ45_NOLINK_TOP);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_BOT", PANEL_COPPER_RJ45_LINKUP_BOT);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_TOP", PANEL_COPPER_RJ45_LINKUP_TOP);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_10M_BOT", PANEL_COPPER_RJ45_LINKUP_10M_BOT);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_10M_TOP", PANEL_COPPER_RJ45_LINKUP_10M_TOP);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_100M_BOT", PANEL_COPPER_RJ45_LINKUP_100M_BOT);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_100M_TOP", PANEL_COPPER_RJ45_LINKUP_100M_TOP);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_1000M_BOT", PANEL_COPPER_RJ45_LINKUP_1000M_BOT);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_1000M_TOP", PANEL_COPPER_RJ45_LINKUP_1000M_TOP);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_2500M_BOT", PANEL_COPPER_RJ45_LINKUP_2500M_BOT);
    cJSON_AddStringToObject(data, "COPPER_RJ45_LINKUP_2500M_TOP", PANEL_COPPER_RJ45_LINKUP_2500M_TOP);
    cJSON_AddStringToObject(data, "FIBER_NOLINK", PANEL_FIBER_NOLINK);
    cJSON_AddStringToObject(data, "FIBER_LINKUP", PANEL_FIBER_LINKUP);
    cJSON_AddStringToObject(data, "FIBER_LED_NOLINK", PANEL_LED_SINGLE_GRAY);
    cJSON_AddStringToObject(data, "FIBER_LED_LINKUP_10M", PANEL_LED_SINGLE_GREER);
    cJSON_AddStringToObject(data, "FIBER_LED_LINKUP_100M", PANEL_LED_SINGLE_GREER);
    cJSON_AddStringToObject(data, "FIBER_LED_LINKUP_1000M", PANEL_LED_SINGLE_GREER);
    cJSON_AddStringToObject(data, "CONSOLE", PANEL_CONSOLE);
    cJSON_AddStringToObject(data, "USB", PANEL_USB);

    switch(boardId)
    {
#ifdef CONFIG_SYS_RTL8380
        case SYS_BM_8380_8GE_2GEF:
        case SYS_BM_8380_8GE_2GEF_POE:
        case SYS_BM_8380_8GE_2GEF_POE_BT:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_8_2GF;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_24);
            break;
        case SYS_BM_8382_24GE_4GEC:
        case SYS_BM_8382_24GE_4GEC_POE:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_24_4GC;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_24);
            break;
        case SYS_BM_8382_16GE_2GE:
        case SYS_BM_8382_16GE_2GE_POE:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_16_2;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_24);
            break;
        case SYS_BM_8382_10GE_4GF:
        case SYS_BM_8382_10GE_4GF_POE:
        case SYS_BM_8382_10GE_4GF_POE_BT:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_10_4GF;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_24);
            break;
        case SYS_BM_8380_4GE_2GEF_YFD:
        case SYS_BM_8380_4GE_2GEF_POE:
        case SYS_BM_8380_4GE_2GEF_POE_BT:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_4_2GF;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_24);
            break;
#endif
#ifdef CONFIG_SYS_RTL8390
        case SYS_BM_8353_48FE_2GE_2GEC:
        case SYS_BM_8393_48GE_2GE_2GEC:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_48_4;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_48);
            break;
        case SYS_BM_8393_48GE:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_48;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_48);
            break;
        case SYS_BM_8391_24GE_4GEC:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_24_4GC;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_24);
            break;
        case SYS_BM_8393_48GE_4GEC_HO:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_48_8393_HO;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_48);
            break;
#endif
#ifdef CONFIG_SYS_RTL9300
        case SYS_BM_9301_24GE_4XGEF:
        case SYS_BM_9301_24GE_4XGEF_YFD:
        case SYS_BM_9301_24GE_4XGEF_POE:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_9301_24_4;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_EMPTY_24);
            break;
        case SYS_BM_9303_8X2_5GE:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_8_2500MC;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_24);
            break;
#endif
#ifdef CONFIG_SYS_RTL9310
        case SYS_BM_9311_48GE_6XGEF:
        case SYS_BM_9311_48GE_6XGEF_YFD:
        case SYS_BM_9311_48GE_6XGEF_POE:
        case SYS_BM_9311_48GE_6XGEF_DEMO:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_48_6;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_48);
            break;
        case SYS_BM_9313_12XGEF:
            pCurrSwitchPanelTbl = (webui_panel_t*) switchPanel_12XGF;
            cJSON_AddStringToObject(data, "bgImg", PANEL_BKIMG_48);
            break;
#endif
       default:
           break;
    }

    layouts = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "layout", layouts);

    for (i = 0; i < _web_panel_tblSize_ret(pCurrSwitchPanelTbl); i++)
    {
        layout = cJSON_CreateObject();

        cJSON_AddNumberToObject(layout, "portStart", pCurrSwitchPanelTbl[i].usrPortStart);
        cJSON_AddNumberToObject(layout, "portEnd", pCurrSwitchPanelTbl[i].usrPortEnd);
        cJSON_AddNumberToObject(layout, "imgType", pCurrSwitchPanelTbl[i].imgType);
        cJSON_AddNumberToObject(layout, "imgWidth", pCurrSwitchPanelTbl[i].imgWidth);
        cJSON_AddNumberToObject(layout, "alignType", pCurrSwitchPanelTbl[i].alignType);
        cJSON_AddNumberToObject(layout, "oddYpos", pCurrSwitchPanelTbl[i].oddYpos);
        cJSON_AddNumberToObject(layout, "evenYpos", pCurrSwitchPanelTbl[i].evenYpos);
        cJSON_AddNumberToObject(layout, "baseXpos", pCurrSwitchPanelTbl[i].baseXpos);
        cJSON_AddNumberToObject(layout, "groupNum", pCurrSwitchPanelTbl[i].groupNum);
        cJSON_AddNumberToObject(layout, "gapWidth", pCurrSwitchPanelTbl[i].gapWidth);
        cJSON_AddStringToObject(layout, "showDes", pCurrSwitchPanelTbl[i].showDes);
        cJSON_AddItemToArray(layouts, layout);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_panel_info_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    sys_port_linkStatus_t linkStatus;
    sys_enable_t autoNego;
    sys_port_media_t linkMedia;
    sys_port_speedDuplex_t speedDuplex;
#ifdef CONFIG_SYS_POE
	uint32 poeGood = 0;
#endif

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_port_linkStatus_get(lp, &linkStatus);
        sal_port_linkMedia_get(lp, &linkMedia);
        sal_port_autoNegoEnable_get(lp, &autoNego);
#ifdef CONFIG_SYS_POE
		sal_poe_portPwGood_get(lp, &poeGood);
#endif

        cJSON_AddBoolToObject(port, "autoNego", autoNego);

        if (PORT_LINKUP == linkStatus)
        {
            cJSON_AddBoolToObject(port, "linkup", TRUE);

            sal_port_speedDuplex_get(lp, &speedDuplex);

            switch (speedDuplex.speed)
            {
                case PORT_SPEED_10M:
                    cJSON_AddStringToObject(port, "speed", "10");
                    break;

                case PORT_SPEED_100M:
                    cJSON_AddStringToObject(port, "speed", "100");
                    break;

                case PORT_SPEED_1000M:
                    cJSON_AddStringToObject(port, "speed", "1000");
                    break;

                case PORT_SPEED_2_5G:
                    cJSON_AddStringToObject(port, "speed", "2500");
                    break;

                case PORT_SPEED_10G:
                    cJSON_AddStringToObject(port, "speed", "10000");
                    break;
                default:
                    break;
            }

            if (PORT_HALF_DUPLEX == speedDuplex.duplex)
                cJSON_AddBoolToObject(port, "dupFull", FALSE);
            else
                cJSON_AddBoolToObject(port, "dupFull", TRUE);
        }
        else
        {
            cJSON_AddBoolToObject(port, "linkup", FALSE);
        }

        if (PORT_MEDIA_COPPER != linkMedia)
        {
            cJSON_AddStringToObject(port, "media", "fiber");
        }
#ifdef CONFIG_SYS_POE
		if(IS_POE_PORT(lp))
		{
			if(poeGood)
			{
				cJSON_AddBoolToObject(port, "poelinkup", TRUE);
			}
			else
			{
				cJSON_AddBoolToObject(port, "poelinkup", FALSE);
			}
		}
#endif

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

