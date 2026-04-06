
#ifndef cJSON__h
#define cJSON__h

#ifdef __cplusplus
extern "C"
{
#endif

#define cJSON_False 0
#define cJSON_True 1
#define cJSON_NULL 2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array 5
#define cJSON_Object 6

#define cJSON_IsReference 256

typedef struct cJSON {
	struct cJSON *next,*prev;	
	struct cJSON *child;		

	int type;					

	char *valuestring;			
	int valueint;				
	double valuedouble;			

	char *string;				
} cJSON;

typedef struct cJSON_Hooks {
      void *(*malloc_fn)(size_t sz);
      void (*free_fn)(void *ptr);
} cJSON_Hooks;

extern void cJSON_InitHooks(cJSON_Hooks* hooks);

extern cJSON *cJSON_Parse(const char *value);

extern char  *cJSON_Print(cJSON *item);

extern char  *cJSON_PrintUnformatted(cJSON *item);

extern void   cJSON_Delete(cJSON *c);

extern int	  cJSON_GetArraySize(cJSON *array);

extern cJSON *cJSON_GetArrayItem(cJSON *array,int item);

extern cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);

extern const char *cJSON_GetErrorPtr(void);

extern cJSON *cJSON_CreateNull(void);
extern cJSON *cJSON_CreateTrue(void);
extern cJSON *cJSON_CreateFalse(void);
extern cJSON *cJSON_CreateBool(int b);
extern cJSON *cJSON_CreateNumber(double num);
extern cJSON *cJSON_CreateString(const char *string);
extern cJSON *cJSON_CreateArray(void);
extern cJSON *cJSON_CreateObject(void);

extern cJSON *cJSON_CreateIntArray(int *numbers,int count);
extern cJSON *cJSON_CreateFloatArray(float *numbers,int count);
extern cJSON *cJSON_CreateDoubleArray(double *numbers,int count);
extern cJSON *cJSON_CreateStringArray(const char **strings,int count);

extern void cJSON_AddItemToArray(cJSON *array, cJSON *item);
extern void	cJSON_AddItemToObject(cJSON *object,const char *string,cJSON *item);

extern void cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item);
extern void	cJSON_AddItemReferenceToObject(cJSON *object,const char *string,cJSON *item);

extern cJSON *cJSON_DetachItemFromArray(cJSON *array,int which);
extern void   cJSON_DeleteItemFromArray(cJSON *array,int which);
extern cJSON *cJSON_DetachItemFromObject(cJSON *object,const char *string);
extern void   cJSON_DeleteItemFromObject(cJSON *object,const char *string);

extern void cJSON_ReplaceItemInArray(cJSON *array,int which,cJSON *newitem);
extern void cJSON_ReplaceItemInObject(cJSON *object,const char *string,cJSON *newitem);

extern void lang_strArg_get(char *str, char *langCat, char *langKey, char* fmt,...);

#define cJSON_AddNullToObject(object,name)	cJSON_AddItemToObject(object, name, cJSON_CreateNull())
#define cJSON_AddTrueToObject(object,name)	cJSON_AddItemToObject(object, name, cJSON_CreateTrue())
#define cJSON_AddFalseToObject(object,name)		cJSON_AddItemToObject(object, name, cJSON_CreateFalse())
#define cJSON_AddNumberToObject(object,name,n)	cJSON_AddItemToObject(object, name, cJSON_CreateNumber(n))
#define cJSON_AddStringToObject(object,name,s)	cJSON_AddItemToObject(object, name, cJSON_CreateString(s))
#define cJSON_AddBoolToObject(object, name, b) cJSON_AddItemToObject(object, name, ((0 == b) ? cJSON_CreateFalse() : cJSON_CreateTrue()))
#define cJSON_AddLangStringToObject(object,name,langCat,langKey) \
do { \
    char _tmp[256]; \
    memset(_tmp,0,256);\
    sprintf(_tmp,"lang('%s','%s')",langCat,langKey); \
    cJSON_AddStringToObject(object, name, _tmp);\
} while (0)
#define cJSON_AddLangStringArgToObject(object,name,langCat,langKey,fmt,args...) \
do { \
    char _tmp[512] = ""; \
    memset(_tmp,0,512);\
    lang_strArg_get(_tmp,langCat,langKey,fmt,## args);\
    cJSON_AddStringToObject(object, name, _tmp);\
} while (0)

#ifdef __cplusplus
}
#endif

#endif
