
#include <libsal/sal_util.h>
#include <libsal/sal_ipmcast.h>

int32 sal_l2McastEntry_getFirst(sys_l2McastEntry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l2McastEntry_getFirst(pEntry));

    return SYS_ERR_OK;
}

int32 sal_l2McastEntry_getNext(sys_l2McastEntry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l2McastEntry_getNext(pEntry));

    return SYS_ERR_OK;
}

int32 sal_ipMcastEntry_getFirst(sys_ipMcastEntry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_ipMcastEntry_getFirst(pEntry));

    return SYS_ERR_OK;
}

int32 sal_ipMcastEntry_getNext(sys_ipMcastEntry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_ipMcastEntry_getNext(pEntry));

    return SYS_ERR_OK;
}

