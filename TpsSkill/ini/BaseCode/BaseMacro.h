/*********************************************************************\
 Copyright(c) 2010, ***, All Rights Reserved
 Author:	
 Created:	2010-07-23
 Describe:	基础的宏定义头文件
\*********************************************************************/

#pragma once
#include "CryAssert.h"

// ---------------------------------------------------------------------
// 参数宏定义
// ---------------------------------------------------------------------
#undef OUT
#define OUT

#undef IN_OUT
#define IN_OUT

// ---------------------------------------------------------------------
// 变量释放宏定义
// ---------------------------------------------------------------------
#ifndef SAFE_DELETE
	#define SAFE_DELETE(p){ if(p) delete (p); (p) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p){ if(p) delete[] (p); (p) = NULL; }
#endif

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p){ if(p) (p)->Release(); (p) = NULL; }
#endif

// ---------------------------------------------------------------------
// assert
// ---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
// Use like this:
// ASSERT(expression);
// ASSERT_MESSAGE(expression,"Useful message");
// ASSERT_TRACE(expression,("This should never happen because parameter %d named %s is %f",iParameter,szParam,fValue));
//-----------------------------------------------------------------------------------------------------
#ifdef ASSERT
	#undef ASSERT
#endif
#if defined(_DEBUG) || defined(DEBUG)
	#define ASSERT(x) CRY_ASSERT(x)
    #define ASSERT_MESSAGE(condition,message) CRY_ASSERT_MESSAGE(condition,message)
    #define ASSERT_TRACE(condition,message) CRY_ASSERT_TRACE(condition,message)
#else
	#define ASSERT(x) ((void)0)
    #define ASSERT_MESSAGE(condition,message) ((void)0)
    #define ASSERT_TRACE(condition,message) ((void)0)
#endif


#ifndef VERIFY
	#if defined(_DEBUG) || defined(DEBUG)
		#define VERIFY(f) ASSERT(f)
	#else
		#define VERIFY(f) ((void)(f))
	#endif
#endif

#ifndef AssertIf
#define AssertIf(v) ASSERT(!(v))
#endif
#ifndef DebugAssertIf
#define DebugAssertIf(v) AssertIf(v)
#endif

// ---------------------------------------------------------------------
// 内存泄漏查找
// ---------------------------------------------------------------------
/*
// 主入口文件 或 dll 只放置一次即可
#if defined(_DEBUG) | defined(DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include<stdlib.h>
	#include<crtdbg.h>
#endif

#if defined(_DEBUG) | defined(DEBUG)
	#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// main函数里面
#if defined(_DEBUG) | defined(DEBUG)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif

// 需要查找的cpp文件都要放置
#if defined(_DEBUG) || defined(DEBUG)	
	#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


// 实在找不到内存泄漏位置时，使用以下代码调试
//_CrtSetBreakAlloc(52);
*/

// ---------------------------------------------------------------------
// 测试时间
// ---------------------------------------------------------------------
#ifdef WIN32
    #define BEGIN_TIME(name) \
	    LARGE_INTEGER nFreq##name;\
	    LARGE_INTEGER nBeginTime##name;\
	    LARGE_INTEGER nEndTime##name;\
	    QueryPerformanceFrequency(&nFreq##name);\
	    QueryPerformanceCounter(&nBeginTime##name)

    #define END_TIME(name)	\
        QueryPerformanceCounter(&nEndTime##name);\
        double dWasteTime##name = (double)(nEndTime##name.QuadPart-nBeginTime##name.QuadPart)/(double)nFreq##name.QuadPart*1000;\
        CStaticFunc::DebugMsg("%s %fms", #name, dWasteTime##name)

	#define END_TIME_FILTER(name,timefilter)	\
		QueryPerformanceCounter(&nEndTime##name);\
		double dWasteTime##name = (double)(nEndTime##name.QuadPart-nBeginTime##name.QuadPart)/(double)nFreq##name.QuadPart*1000;\
		if(dWasteTime##name >timefilter) CStaticFunc::DebugMsg("%s %fms", #name, dWasteTime##name)

#else

    #define BEGIN_TIME(name) \
        timeval ttBegin##name;\
        gettimeofday(&ttBegin##name, NULL)

    #define END_TIME(name)	\
        timeval ttEnd##name;\
        gettimeofday(&ttEnd##name, NULL);\
        double dWasteTime##name = (ttEnd##name.tv_sec-ttBegin##name.tv_sec)*1000 + (ttEnd##name.tv_usec-ttBegin##name.tv_usec)*0.001;\
        CStaticFunc::DebugMsg("%s %fms", #name, dWasteTime##name)

#endif






