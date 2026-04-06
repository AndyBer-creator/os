#include <authmgr/proto_authmgr.h>
int32 dot1x_sm_timer_check(dot1x_pacp_t *pPacp)
{
    DEC(pPacp->aWhile);      
    DEC(pPacp->quietWhile);  
    DEC(pPacp->reAuthWhen);  

    DEC(pPacp->pae_sm.txWhile); 
    DEC(pPacp->backend_sm.suppWhile); 

    return SYS_ERR_OK;
}
