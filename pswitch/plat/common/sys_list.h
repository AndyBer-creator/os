#ifndef __COMMON_SYS_LIST_H__
#define __COMMON_SYS_LIST_H__
#include <common/sys_type.h>
#define SYS_LIST_NODE_SIZE   CAPA_VLAN_NUM

#define LIST_DECLARE(name) \
    sys_list_array_t name

#define LIST_INIT(list)    \
    sys_list_init(list)

#define LIST_DESTROY(list) \
    sys_list_destroy(list)

#define LIST_ADD_NODE(list, node) \
    sys_list_nodeHead_add(list, node)

#define LIST_DEL_NODE(list, node)   \
    sys_list_node_del(list, node)

#define LIST_UPDATE_NODE(list, node) \
    sys_list_node_update(list, node);

#define LIST_SEARCH_NODE(list, node, idx) \
    sys_list_node_search(list, node, idx)

#define LIST_NODE_INIT(node) \
    sys_list_node_init(node)

typedef enum sys_list_def_e
{
    SYS_LIST_NODE_FOUND = 0xFFF0,
    SYS_LIST_NODE_EMPTY,
    SYS_LIST_DEF_END
} sys_list_def_t;

typedef enum sys_list_key_e
{
    SYS_LIST_KEY_ID = 0,
    SYS_LIST_KEY_NAME,
    SYS_LIST_KEY_ALL,
    SYS_LIST_KEY_END
} sys_list_key_t;

typedef struct sys_list_idx_s
{
    sys_nodeid_t next;
    sys_nodeid_t dnext;
} sys_list_idx_t;

typedef struct sys_list_node_s
{
    uint32 id;
    sys_namestr_t name;
    sys_list_idx_t idx;
} sys_list_node_t;

typedef struct sys_list_idnode_s
{
    uint32 id;
    sys_nodeid_t ptr;
} sys_list_idnode_t;

typedef struct sys_list_namenode_s
{
    sys_namestr_t name;
    sys_nodeid_t ptr;
} sys_list_namenode_t;

typedef struct sys_list_array_s
{
    sys_nodeid_t head;
    sys_nodeid_t tail;
    sys_nodeid_t dhead;
    sys_nodeid_t ptr;
    sys_list_key_t key;
    sys_list_node_t node[SYS_LIST_NODE_SIZE];
    sys_list_idnode_t sortId[SYS_LIST_NODE_SIZE];
    sys_list_namenode_t sortName[SYS_LIST_NODE_SIZE];
} sys_list_array_t;
#define IS_LIST_KEY_INVALID(key)    \
    ((((key) < SYS_LIST_KEY_ID) || ((key) >= SYS_LIST_KEY_END)) ? 1 : 0)
extern int32 sys_list_init(sys_list_array_t *pList);
extern int32 sys_list_destroy(sys_list_array_t *pList);
extern int32 sys_list_node_init(sys_list_node_t *pNode);
extern int32 sys_list_nodeEmpty_get(sys_list_array_t *pList, sys_nodeid_t *pIdx);
extern int32 sys_list_nodeIdExist_get(sys_list_array_t *pList, sys_list_node_t *pNode, sys_nodeid_t *pIdx);
extern int32 sys_list_nodeNameExist_get(sys_list_array_t *pList, sys_list_node_t *pNode, sys_nodeid_t *pIdx);
extern int32 sys_list_node_search(sys_list_array_t *pList, sys_list_node_t *pNode, sys_nodeid_t *pIdx);
extern int32 sys_list_nodeAdd_sort(sys_list_array_t *pList, sys_nodeid_t idx, sys_list_node_t *pNode);
extern int32 sys_list_nodeDel_sort(sys_list_array_t *pList, sys_nodeid_t idx);
extern int32 sys_list_node_set(sys_list_array_t *pList, sys_nodeid_t idx, sys_list_node_t *pNode);
extern int32 sys_list_nodeIdx_reset(sys_list_array_t *pList);
extern int32 sys_list_nodeIdx_del(sys_list_array_t *pList, sys_nodeid_t idx);
extern int32 sys_list_node_del(sys_list_array_t *pList, sys_list_node_t *pNode);
extern int32 sys_list_node_update(sys_list_array_t *pList, sys_list_node_t *pNode);
extern int32 sys_list_nodeHead_add(sys_list_array_t *pList, sys_list_node_t *pNode);
extern int32 sys_list_nodeTail_add(sys_list_array_t *pList, sys_list_node_t *pNode);
extern int32 sys_list_nodeHead_del(sys_list_array_t *pList);
extern int32 sys_list_node_delAll(sys_list_array_t *pList);
extern sys_nodeid_t sys_list_nodeHeadIdx_ret(sys_list_array_t *pList);

#endif 

