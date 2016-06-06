   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.30  08/16/14          */
   /*                                                     */
   /*                USER FUNCTIONS MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Created file to seperate UserFunctions and     */
/*            EnvUserFunctions from main.c.                  */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*************************************************************/

/***************************************************************************/
/*                                                                         */
/* Permission is hereby granted, free of charge, to any person obtaining   */
/* a copy of this software and associated documentation files (the         */
/* "Software"), to deal in the Software without restriction, including     */
/* without limitation the rights to use, copy, modify, merge, publish,     */
/* distribute, and/or sell copies of the Software, and to permit persons   */
/* to whom the Software is furnished to do so.                             */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT   */
/* OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY  */
/* CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES */
/* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN   */
/* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF */
/* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
/*                                                                         */
/***************************************************************************/

#include "clips.h"
#include "ExternalDef.h"
typedef  void (*CLIPS_USER_FP)(void *, DATA_OBJECT_PTR);

void UserFunctions(void);
void EnvUserFunctions(void *);
void SetupClipsUserFunc(CLIPS_USER_FP newEntryPoint,Ext_FuncInfo *extFuncTable, int cnt);


int g_extFuncTable_cnt;
CLIPS_USER_FP NewEntryPoint_clips;
Ext_FuncInfo *g_extFuncTable_clips;


/*********************************************************/
/* UserFunctions: Informs the expert system environment  */
/*   of any user defined functions. In the default case, */
/*   there are no user defined functions. To define      */
/*   functions, either this function must be replaced by */
/*   a function with the same name within this file, or  */
/*   this function can be deleted from this file and     */
/*   included in another file.                           */
/*********************************************************/
void UserFunctions()
  {
   // Use of UserFunctions is deprecated.
   // Use EnvUserFunctions instead.
  }
  
/***********************************************************/
/* EnvUserFunctions: Informs the expert system environment */
/*   of any user defined functions. In the default case,   */
/*   there are no user defined functions. To define        */
/*   functions, either this function must be replaced by   */
/*   a function with the same name within this file, or    */
/*   this function can be deleted from this file and       */
/*   included in another file.                             */
/***********************************************************/
void EnvUserFunctions(
  void *environment)
  {
#if MAC_XCD
#pragma unused(environment)
#endif
	//extern double entryPoint(void *);
//	EnvDefineFunction(environment,"entryPoint",'d',PTIEF entryPoint,"entryPoint");
	//	extern void* entryPoint(void *);
		//	EnvDefineFunction(environment,"entryPoint",'y',PTIEF entryPoint,"entryPoint");
	int i = 0;
	PExt_FuncInfo p;
	
	EnvDefineFunction(environment,"entry-point",'b',PTIEF NewEntryPoint_clips,"NewEntryPoint");
	for ( i=0; i<g_extFuncTable_cnt; i++ )
	{
		p = &g_extFuncTable_clips[i];
		//EnvDefineFunction(environment,"entry-point",'y',PTIEF entry_point,"entry-point");
		EnvDefineFunction(environment,p->name, p->retCode, PTIEF p->pointer, p->name2);
	}

  }
void SetupClipsUserFunc(CLIPS_USER_FP newEntryPoint,  Ext_FuncInfo *extFuncTable, int cnt)
{
	NewEntryPoint_clips = newEntryPoint;
	g_extFuncTable_clips = extFuncTable;
	
	g_extFuncTable_cnt = cnt;
}
