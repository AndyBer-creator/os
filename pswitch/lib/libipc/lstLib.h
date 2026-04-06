
#ifndef __INClstLibh
#define __INClstLibh

#define FAST register

typedef struct node		
    {
    struct node *next;		
    struct node *previous;	
    } NODE;

typedef struct			
    {
    NODE node;			
    int count;			
    } LIST;

extern void lstLibInit (void);
extern NODE *	lstFirst (LIST *pList);
extern NODE *	lstGet (FAST LIST *pList);
extern NODE *	lstLast (LIST *pList);
extern NODE *	lstNStep (FAST NODE *pNode, int nStep);
extern NODE *	lstNext (NODE *pNode);
extern NODE *	lstNth (FAST LIST *pList, FAST int nodenum);
extern NODE *	lstPrevious (NODE *pNode);
extern int 	lstCount (LIST *pList);
extern int 	lstFind (LIST *pList, FAST NODE *pNode);
extern void 	lstAdd (LIST *pList, NODE *pNode);
extern void 	lstConcat (FAST LIST *pDstList, FAST LIST *pAddList);
extern void 	lstDelete (FAST LIST *pList, FAST NODE *pNode);
extern void 	lstExtract (FAST LIST *pSrcList, FAST NODE *pStartNode, FAST NODE *pEndNode, FAST LIST *pDstList);
extern void 	lstFree (LIST *pList);
extern void 	lstInit (FAST LIST *pList);
extern void 	lstInsert (FAST LIST *pList, FAST NODE *pPrev, FAST NODE *pNode);

#endif

