
#include <stdlib.h>
#include <libipc/lstLib.h>

#define HEAD	node.next		
#define TAIL	node.previous		

void lstLibInit (void)
    {
    return;
    }

void lstInit
    (
    FAST LIST *pList     
    )
    {
    pList->HEAD	 = NULL;
    pList->TAIL  = NULL;
    pList->count = 0;
    }

void lstAdd
    (
    LIST *pList,        
    NODE *pNode         
    )
    {
    lstInsert (pList, pList->TAIL, pNode);
    }

void lstConcat
    (
    FAST LIST *pDstList,        
    FAST LIST *pAddList         
    )
    {
    if (pAddList->count == 0)		
	return;

    if (pDstList->count == 0)
	*pDstList = *pAddList;
    else
	{

	pDstList->TAIL->next     = pAddList->HEAD;
	pAddList->HEAD->previous = pDstList->TAIL;
	pDstList->TAIL           = pAddList->TAIL;

	pDstList->count += pAddList->count;
	}

    lstInit (pAddList);
    }

int lstCount
    (
    LIST *pList         
    )
    {
    return (pList->count);
    }

void lstDelete
    (
    FAST LIST *pList,   
    FAST NODE *pNode    
    )
    {
    if (pNode->previous == NULL)
	pList->HEAD = pNode->next;
    else
	pNode->previous->next = pNode->next;

    if (pNode->next == NULL)
	pList->TAIL = pNode->previous;
    else
	pNode->next->previous = pNode->previous;

    pList->count--;
    }

void lstExtract
    (
    FAST LIST *pSrcList,      
    FAST NODE *pStartNode,    
    FAST NODE *pEndNode,      
    FAST LIST *pDstList       
    )
    {
    FAST int i;
    FAST NODE *pNode;

    if (pStartNode->previous == NULL)
	pSrcList->HEAD = pEndNode->next;
    else
	pStartNode->previous->next = pEndNode->next;

    if (pEndNode->next == NULL)
	pSrcList->TAIL = pStartNode->previous;
    else
	pEndNode->next->previous = pStartNode->previous;

    pDstList->HEAD = pStartNode;
    pDstList->TAIL = pEndNode;

    pStartNode->previous = NULL;
    pEndNode->next       = NULL;

    i = 0;

    for (pNode = pStartNode; pNode != NULL; pNode = pNode->next)
	i++;

    pSrcList->count -= i;
    pDstList->count = i;
    }

NODE *lstFirst
    (
    LIST *pList         
    )
    {
    return (pList->HEAD);
    }

NODE *lstGet
    (
    FAST LIST *pList    
    )
    {
    FAST NODE *pNode = pList->HEAD;

    if (pNode != NULL)                      
	{
	pList->HEAD = pNode->next;          

	if (pNode->next == NULL)            
	    pList->TAIL = NULL;             
	else
	    pNode->next->previous = NULL;   

	pList->count--;                     
	}

    return (pNode);
    }

void lstInsert
    (
    FAST LIST *pList,   
    FAST NODE *pPrev,   
    FAST NODE *pNode    
    )
    {
    FAST NODE *pNext;

    if (pPrev == NULL)
	{				
	pNext = pList->HEAD;
	pList->HEAD = pNode;
	}
    else
	{				
	pNext = pPrev->next;
	pPrev->next = pNode;
	}

    if (pNext == NULL)
	pList->TAIL = pNode;		
    else
	pNext->previous = pNode;	

    pNode->next		= pNext;
    pNode->previous	= pPrev;

    pList->count++;
    }

NODE *lstLast
    (
    LIST *pList         
    )
    {
    return (pList->TAIL);
    }

NODE *lstNext
    (
    NODE *pNode         
    )
    {
    return (pNode->next);
    }

NODE *lstNth
    (
    FAST LIST *pList,           
    FAST int nodenum            
    )
    {
    FAST NODE *pNode;

    if ((nodenum < 1) || (nodenum > pList->count))
	return (NULL);

    if (nodenum < (pList->count >> 1))
	{
	pNode = pList->HEAD;

	while (--nodenum > 0)
	    pNode = pNode->next;
	}

    else
	{
	nodenum -= pList->count;
	pNode = pList->TAIL;

	while (nodenum++ < 0)
	    pNode = pNode->previous;
	}

    return (pNode);
    }

NODE *lstPrevious
    (
    NODE *pNode         
    )
    {
    return (pNode->previous);
    }

NODE *lstNStep
    (
    FAST NODE *pNode,           
    int nStep                   
    )
    {
    int i;

    for (i = 0; i < abs (nStep); i++)
	{
	if (nStep < 0)
	    pNode = pNode->previous;
	else if (nStep > 0)
	    pNode = pNode->next;
	if (pNode == NULL)
	    break;
	}
    return (pNode);
    }

int lstFind
    (
    LIST *pList,                
    FAST NODE *pNode            
    )
    {

    FAST NODE *pNextNode;
    FAST int index = 1;

    pNextNode = lstFirst (pList);

    while ((pNextNode != NULL) && (pNextNode != pNode))
	{
	index++;
	pNextNode = lstNext (pNextNode);
	}

    if (pNextNode == NULL)
		return (-1);
    else
		return (index);
    }

void lstFree
    (
    LIST *pList         
    )
    {
    NODE *p1, *p2;

    if (pList->count > 0)
	{
	p1 = pList->HEAD;
	while (p1 != NULL)
	    {
	    p2 = p1->next;
	    free ((char *)p1);
	    p1 = p2;
	    }
	pList->count = 0;
	pList->HEAD = pList->TAIL = NULL;
	}
    }

