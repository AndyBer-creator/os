#include <common/sys_def.h>
#define FIRST_NODE(list)        (list->head)
#define SECOND_NODE(list)       (list->node[list->head].idx.next)
#define NEXT_NODE(list, idx)    (list->node[idx].idx.next)
#define LAST_NODE(list)         (list->tail)
#define NULL_NODE               (-1)

#define LIST_DBG(fmt, args...)   SYS_DBG(DBG_SYSTEM, fmt , ## args)
#define _LIST_DBG(fmt, args...)  do {} while(0)
int32 sys_list_init(sys_list_array_t *pList)
{
    uint32 i;

    for (i = 0; i < SYS_LIST_NODE_SIZE; i++)
    {
        SYS_ERR_CHK(sys_list_node_init(&pList->node[i]));

        pList->sortId[i].id = 0;
        pList->sortId[i].ptr = NULL_NODE;

        pList->sortName[i].name.str[0] = '\0';
        pList->sortName[i].ptr = NULL_NODE;
    }

    SYS_ERR_CHK(sys_list_nodeIdx_reset(pList));

    return SYS_ERR_OK;
}
int32 sys_list_destroy(sys_list_array_t *pList)
{
    SYS_ERR_CHK(sys_list_node_delAll(pList));

    return SYS_ERR_OK;
}
int32 sys_list_node_init(sys_list_node_t *pNode)
{
    pNode->id = 0;
    pNode->name.str[0] = '\0';
    pNode->idx.next = NULL_NODE;
    pNode->idx.dnext = NULL_NODE;

    return SYS_ERR_OK;
}
int32 sys_list_nodeEmpty_get(sys_list_array_t *pList, sys_nodeid_t *pIdx)
{
    uint32 idx;

    if (NULL_NODE == pList->dhead)
    {
        idx = ++(LAST_NODE(pList));

        if (idx >= SYS_LIST_NODE_SIZE)
            return SYS_ERR_OUT_OF_RANGE;
    }
    else
    {
        if (pList->dhead >= SYS_LIST_NODE_SIZE)
            return SYS_ERR_OUT_OF_RANGE;

        idx = pList->dhead;

        if (pList->node[idx].idx.dnext >= SYS_LIST_NODE_SIZE)
            return SYS_ERR_OUT_OF_RANGE;

        pList->dhead = pList->node[idx].idx.dnext;
    }

    *pIdx = idx;

    return SYS_ERR_OK;
}
int32 sys_list_nodeIdExist_get(sys_list_array_t *pList, sys_list_node_t *pNode, sys_nodeid_t *pIdx)
{
    sys_nodeid_t i = FIRST_NODE(pList);

    if (NULL_NODE == i)
        return SYS_ERR_FAILED;

    do
    {
        if (pNode->id == pList->node[i].id)
        {
            *pIdx = i;
            return SYS_ERR_OK;
        }
    } while ((i = pList->node[i].idx.next) != NULL_NODE);

    return SYS_ERR_FAILED;
}
int32 sys_list_nodeNameExist_get(sys_list_array_t *pList, sys_list_node_t *pNode, sys_nodeid_t *pIdx)
{
    sys_nodeid_t i = FIRST_NODE(pList);

    if (NULL_NODE == i)
        return SYS_ERR_FAILED;

    do
    {
        if (!osal_strcmp(pList->node[i].name.str, pNode->name.str))
        {
            *pIdx = i;
            return SYS_ERR_OK;
        }
    } while ((i = pList->node[i].idx.next) != NULL_NODE);

    return SYS_ERR_FAILED;
}
int32 sys_list_node_search(sys_list_array_t *pList, sys_list_node_t *pNode, sys_nodeid_t *pIdx)
{
    int32 min = 0;
    int32 max = pList->ptr - 1;
    int32 mid = 0;
    int32 cmp = 0;

    if (max < 0)
    {
        *pIdx = 0;
        return SYS_ERR_OK;
    }

    do
    {
        mid = (min + max) / 2;

        switch (pList->key)
        {
            case SYS_LIST_KEY_ID:
                cmp = pNode->id - pList->sortId[mid].id;
                break;

            case SYS_LIST_KEY_NAME:
                cmp = osal_strcmp(pNode->name.str, pList->sortName[mid].name.str);
                break;

            default:
                return SYS_ERR_FAILED;
        }

        if (cmp > 0)
            min = mid + 1;
        else if (cmp < 0)
            max = mid - 1;
        else
        {
            *pIdx = mid;
            return SYS_LIST_NODE_FOUND;
        }
    } while (min <= max);

    *pIdx = min;

    return SYS_ERR_OK;
}
int32 sys_list_nodeAdd_sort(sys_list_array_t *pList, sys_nodeid_t idx, sys_list_node_t *pNode)
{
    uint32 i;
    uint32 key;
    uint32 foundId = 0;
    uint32 foundName = 0;
    uint32 restoreKey;
    sys_nodeid_t addIdx;

    restoreKey = pList->key;

    for (key = 0; key < SYS_LIST_KEY_ALL; key++)
    {
        pList->key = key;

        if (SYS_LIST_NODE_FOUND == sys_list_node_search(pList, pNode, &addIdx))
        {
            switch (key)
            {
                case SYS_LIST_KEY_ID: foundId++; break;
                case SYS_LIST_KEY_NAME: foundName++; break;
                default: break;
            }
            continue;
        }

        switch (key)
        {
            case SYS_LIST_KEY_ID:
                for (i = pList->ptr; i > addIdx; i--)
                    pList->sortId[i] = pList->sortId[i - 1];
                pList->sortId[addIdx].id = pList->node[idx].id;
                pList->sortId[addIdx].ptr = idx;
                break;

            case SYS_LIST_KEY_NAME:
                for (i = pList->ptr; i > addIdx; i--)
                    pList->sortName[i] = pList->sortName[i - 1];
                pList->sortName[addIdx].name = pList->node[idx].name;
                pList->sortName[addIdx].ptr = idx;
                break;

            default:
                return SYS_ERR_FAILED;
        }
    }

    pList->key = restoreKey;

    switch (pList->key)
    {
        case SYS_LIST_KEY_ID: if (!foundId) pList->ptr++; break;
        case SYS_LIST_KEY_NAME: if (!foundName) pList->ptr++; break;
        default: break;
    }

    return SYS_ERR_OK;
}
int32 sys_list_nodeDel_sort(sys_list_array_t *pList, sys_nodeid_t idx)
{
    uint32 i;

    for (i = 0; pList->sortId[i].ptr != idx; i++)
        ;
    for ( ; i < pList->ptr; i++)
        pList->sortId[i] = pList->sortId[i + 1];

    for (i = 0; pList->sortName[i].ptr != idx; i++)
        ;
    for ( ; i < pList->ptr; i++)
        pList->sortName[i] = pList->sortName[i + 1];

    pList->ptr--;

    return SYS_ERR_OK;
}
int32 sys_list_node_set(sys_list_array_t *pList, sys_nodeid_t idx, sys_list_node_t *pNode)
{
    pList->node[idx] = *pNode;

    return SYS_ERR_OK;
}
int32 sys_list_nodeIdx_reset(sys_list_array_t *pList)
{
    pList->head = NULL_NODE;
    pList->tail = NULL_NODE;
    pList->dhead = NULL_NODE;
    pList->ptr = 0;

    return SYS_ERR_OK;
}
int32 sys_list_nodeIdx_del(sys_list_array_t *pList, sys_nodeid_t idx)
{
    sys_nodeid_t i = FIRST_NODE(pList);

    if (NULL_NODE == pList->dhead)
        pList->node[idx].idx.dnext = NULL_NODE;
    else
        pList->node[idx].idx.dnext = pList->dhead;

    pList->dhead = idx;

    if (idx == FIRST_NODE(pList))
    {
        FIRST_NODE(pList) = pList->node[idx].idx.next;
    }
    else
    {
        do
        {
            if (idx == pList->node[i].idx.next)
            {
                pList->node[i].idx.next = pList->node[idx].idx.next;
                break;
            }
        } while ((i = pList->node[i].idx.next) != NULL_NODE);
    }

    pList->node[idx].idx.next = NULL_NODE;
    SYS_ERR_CHK(sys_list_nodeDel_sort(pList, idx));

    if (0 == pList->ptr)
    {
        SYS_ERR_CHK(sys_list_nodeIdx_reset(pList));
        return SYS_LIST_NODE_EMPTY;
    }

    return SYS_ERR_OK;
}
int32 sys_list_node_del(sys_list_array_t *pList, sys_list_node_t *pNode)
{
    sys_nodeid_t idx;

    SYS_ERR_CHK(sys_list_nodeIdExist_get(pList, pNode, &idx));
    SYS_ERR_CHK(sys_list_nodeIdx_del(pList, idx));

    return SYS_ERR_OK;
}
static int32 sys_list_nodeName_sort(sys_list_namenode_t nameList[], int32 left, int32 right)
{
    register int32 i, j;
    char *x = nameList[(left+right)/2].name.str;
    sys_list_namenode_t temp;

    i = left;
    j = right;

    do
    {
        while((osal_strcmp(nameList[i].name.str, x) < 0) && (i < right))
            i++;
        while((osal_strcmp(nameList[j].name.str, x) > 0) && (j > left))
            j--;

        if(i <= j)
        {
            temp = nameList[i];
            nameList[i] = nameList[j];
            nameList[j] = temp;
            i++;
            j--;
        }
    } while (i <= j);

    if(left < j)
        sys_list_nodeName_sort(nameList, left, j);

    if(i < right)
        sys_list_nodeName_sort(nameList, i, right);

    return SYS_ERR_OK;
}
int32 sys_list_node_update(sys_list_array_t *pList, sys_list_node_t *pNode)
{
    uint32 i;
    sys_nodeid_t idx;

    if (SYS_LIST_NODE_FOUND != sys_list_node_search(pList, pNode, &idx))
        return SYS_ERR_OK;

    for (i = 0; i < pList->ptr; i++)
    {
        if (!osal_strcmp(pList->node[pList->sortId[idx].ptr].name.str, pList->sortName[i].name.str))
        {
            pList->node[pList->sortId[idx].ptr].name = pNode->name;
            pList->sortName[i].name = pNode->name;
            pList->sortName[i].ptr = pList->sortId[idx].ptr;
            break;
        }
    }

    SYS_ERR_CHK(sys_list_nodeName_sort(pList->sortName, 0, pList->ptr - 1));

#ifdef SYS_LIST_DBG
        for (i = 0; i < pList->ptr; i++)
            LIST_DBG("%s[%d/%d]: nodeId=%d, nodeName=%s",
                __func__, i, pList->ptr, pList->node[i].id, pList->node[i].name.str);
        for (i = 0; i < pList->ptr; i++)
            LIST_DBG("%s[%d/%d]: sortId=%d, ptr=%d",
                __func__, i, pList->ptr, pList->sortId[i].id, pList->sortId[i].ptr);
        for (i = 0; i < pList->ptr; i++)
            LIST_DBG("%s[%d/%d]: sortName=%s, ptr=%d",
                __func__, i, pList->ptr, pList->sortName[i].name.str, pList->sortName[i].ptr);
#endif

    return SYS_ERR_OK;
}
int32 sys_list_nodeHead_add(sys_list_array_t *pList, sys_list_node_t *pNode)
{
    sys_nodeid_t idx;

    if (SYS_LIST_NODE_FOUND == sys_list_node_search(pList, pNode, &idx))
        goto add_end;

    pNode->idx.next = FIRST_NODE(pList);
    SYS_ERR_CHK(sys_list_nodeEmpty_get(pList, &idx));
    FIRST_NODE(pList)= idx;

    _LIST_DBG("%s: FIRST(b)=%d, FIRST(a)=%d, id=%d, name=%s",
        __func__, pNode->idx.next, FIRST_NODE(pList), pNode->id, pNode->name.str);

add_end:
    SYS_ERR_CHK(sys_list_node_set(pList, idx, pNode));
    SYS_ERR_CHK(sys_list_nodeAdd_sort(pList, idx, pNode));

    return SYS_ERR_OK;
}
int32 sys_list_nodeTail_add(sys_list_array_t *pList, sys_list_node_t *pNode)
{
    sys_nodeid_t idx = FIRST_NODE(pList);

    if (NULL_NODE == FIRST_NODE(pList))
        SYS_ERR_CHK(sys_list_nodeHead_add(pList, pNode));
    else
    {
        while (pList->node[idx].idx.next != NULL_NODE)
            idx = pList->node[idx].idx.next;

        SYS_ERR_CHK(sys_list_nodeEmpty_get(pList, &pList->node[idx].idx.next));

        pNode->idx.next = NULL_NODE;
        SYS_ERR_CHK(sys_list_node_set(pList, pList->node[idx].idx.next, pNode));
        SYS_ERR_CHK(sys_list_nodeAdd_sort(pList, pList->node[idx].idx.next, pNode));
    }

    return SYS_ERR_OK;
}
int32 sys_list_nodeHead_del(sys_list_array_t *pList)
{
    if (FIRST_NODE(pList) != NULL_NODE)
    {
        SYS_ERR_CHK(sys_list_nodeIdx_del(pList, FIRST_NODE(pList)));
        FIRST_NODE(pList) = SECOND_NODE(pList);
    }

    return SYS_ERR_OK;
}
int32 sys_list_node_delAll(sys_list_array_t *pList)
{
    while (FIRST_NODE(pList) != NULL_NODE)
        SYS_ERR_CHK(sys_list_nodeHead_del(pList));

    return SYS_ERR_OK;
}
sys_nodeid_t sys_list_nodeHeadIdx_ret(sys_list_array_t *pList)
{
    return FIRST_NODE(pList);
}

