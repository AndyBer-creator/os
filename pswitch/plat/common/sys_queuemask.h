#ifndef __COMMON_SYS_QUEUEMASK_H__
#define __COMMON_SYS_QUEUEMASK_H__
#include <common/sys_bitmap.h>
typedef struct sys_queuemask_s
{
    uint32 bits[SYS_BITS_TO_LONGS(CAPA_QOS_QUEUE_NUM)];
} sys_queuemask_t;
#define QUEUEMASK_SET_QID(queuemask, qid) \
do{ \
    if((qid) > CAPA_QOS_QUEUE_NUM) return SYS_ERR_QUEUE_ID; \
    SYS_BITMAP_SET_BIT((queuemask).bits, (qid)); \
}while(0)

#define QUEUEMASK_IS_QIDSET(queuemask, qid) \
    (((qid) < CAPA_QOS_QUEUE_NUM) ? SYS_BITMAP_IS_BITSET((queuemask).bits, (qid)) : 0)

#define FOR_EACH_QID(qid) \
    for((qid) = 0; (qid) < CAPA_QOS_QUEUE_NUM; (qid)++)

#define FOR_EACH_QID_IN_QUEUEMASK(qid, queuemask) \
    for((qid) = 0; (qid) < CAPA_QOS_QUEUE_NUM; (qid)++) \
        if(QUEUEMASK_IS_QIDSET((queuemask), (qid)))
#endif 

