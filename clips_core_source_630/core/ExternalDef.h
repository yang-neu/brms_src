
#ifndef _H_EXTERNAL_DEF_
#define _H_EXTERNAL_DEF_

typedef  void (*CLIPS_USER_FP)(void *, DATA_OBJECT_PTR);
typedef struct {
	char *templateName;
	char *name;
	char retCode;
	void (*pointer)(void *, DATA_OBJECT_PTR);
	char *name2;
}Ext_FuncInfo, *PExt_FuncInfo;
	
	
#define MAX_EXT_FUNC_CNT 5

	


#endif
