#ifndef __CRYASSERT_H__
#define __CRYASSERT_H__
#pragma once
#define USE_CRY_ASSERT

//-----------------------------------------------------------------------------------------------------
// Use like this:
// CRY_ASSERT(expression);
// CRY_ASSERT_MESSAGE(expression,"Useful message");
// CRY_ASSERT_TRACE(expression,("This should never happen because parameter %d named %s is %f",iParameter,szParam,fValue));
//-----------------------------------------------------------------------------------------------------
#if defined(USE_CRY_ASSERT) && defined(WIN32) && !defined(_WIN64)
    //#pragma message("CryAssert enabled.")
    void CryAssertTrace(const char *,...);
    bool CryAssert(const char *,const char *,unsigned int,bool *);
    #define DEBUG_BREAK _asm { int 3 }
    #define CRY_ASSERT(condition) CRY_ASSERT_MESSAGE(condition,NULL)
    #define CRY_ASSERT_MESSAGE(condition,message) CRY_ASSERT_TRACE(condition,(message))
 
    #define CRY_ASSERT_TRACE(condition,parenthese_message)			    \
    do														            \
    {															        \
        static bool s_bIgnoreAssert = false;					        \
        if(!s_bIgnoreAssert && !(condition))				            \
        {														        \
            CryAssertTrace parenthese_message;					        \
            if(CryAssert(#condition,__FILE__,__LINE__,&s_bIgnoreAssert))    \
            {									                        \
                 DEBUG_BREAK;								            \
            }												            \
        }														        \
    } while(0)

    #undef assert
    #define assert CRY_ASSERT
#else

#ifdef __APPLE__
    #define chSTR2(x) #x
    #define chSTR(x) chSTR2(x)
    #define CRY_ASSERT(condition) if(!(condition)) printf("[Assertion Failed] CONDITION:(" #condition")" " FILE:" __FILE__ " LINE:" chSTR(__LINE__) "\n")
    #define CRY_ASSERT_MESSAGE(condition,message) if(!(condition)) printf("[Assertion Failed] CONDITION:(" #condition")" " FILE:" __FILE__ " LINE:" chSTR(__LINE__) " MESSAGE:" message "\n")
    #define CRY_ASSERT_TRACE(condition,parenthese_message) if(!(condition)) {printf("[Assertion Failed] CONDITION:(" #condition")" " FILE:" __FILE__ " LINE:" chSTR(__LINE__) " MESSAGE:"); printf parenthese_message; printf("\n");}
#else
    #include <assert.h>
    #define CRY_ASSERT(condition) assert(condition)
    #define CRY_ASSERT_MESSAGE(condition,message) assert(condition)
    #define CRY_ASSERT_TRACE(condition,parenthese_message) assert(condition)
#endif

#endif

#endif