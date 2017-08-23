/*********************************************************************\
 Copyright(c) 2010, ***, All Rights Reserved
 Author:	
 Created:	2010-07-23
 Describe:	基础的类型定义头文件
\*********************************************************************/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include <time.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 
#include "Windows.h"
#else
#include "glWindows.h"
#endif

// warning C4786: 'Some STL template class' : identifier was truncated to '255' characters in the debug information
// #pragma warning(disable: 4786)
#pragma warning(disable: 4503)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
// #pragma warning(error:4715)

// warning C4996: 'sprintf': This function or variable may be unsafe. 
// Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
#pragma warning(disable: 4996)

#include <map>
#include <list>
#include <deque>
#include <stack>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;

// ---------------------------------------------------------------------
// stl定义
// ---------------------------------------------------------------------
typedef std::vector<int> VEC_INT;
typedef std::vector<DWORD> VEC_DWORD;
typedef std::vector<std::string> VEC_STR;
typedef std::map<int, int> MAP_INT_INT;
typedef std::map<int, std::string> MAP_INT_STR;
typedef std::map<std::string, std::string> MAP_STR_STR;
typedef std::set<int> SET_INT;
typedef std::set<std::string> SET_STR;

// ---------------------------------------------------------------------
// int
// ---------------------------------------------------------------------
#ifdef WIN32
typedef char int8;
#else
typedef __signed char int8;
#endif

typedef short int16;
typedef int int32;
typedef long long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char UTF8;
typedef unsigned short UTF16;
typedef unsigned int UTF32;

typedef float float32;
typedef double float64;

#ifdef WIN32
typedef int socklen_t;
#endif

#if !defined(SOCKET) && !defined(_WIN64)
typedef unsigned int SOCKET;
#endif

static const char SP[] = " ";
static const char EMPTY[] = "";
static const char CRLF[] = "\r\n";
static const char COMMA[] = ",";
static const char SEMICOLON[] = ";";
static const char EQUAL[] = "=";
static const char COLON[] = ":";

static const char CR[] = "\r";
static const char LF[] = "\n";
static const char TCP[] = "TCP";
static const char CRLF2[] = "\r\n\r\n";
static const char CRLF3[] = "\n\r\n";
static const char CRLF4[] = "\r\r\n";
static const char CRLF5[] = "\n\n";
static const char QUOTE[] = "\"";
static const char MC[] = "-";

static const int8  Int8_MIN  = int8(-128);                            
static const int8  Int8_MAX  = int8(127);   

static const uint8  Uint8_MIN  = uint8(0); 
static const uint8  Uint8_MAX  = uint8(255);                               

static const int16 Int16_MIN = int16(-32768);                           
static const int16 Int16_MAX = int16(32767);   

static const uint16 Uint16_MIN = uint16(0);  
static const uint16 Uint16_MAX = uint16(65535);                            

static const int32 Int32_MIN = int32(-2147483647 - 1);                 
static const int32 Int32_MAX = int32(2147483647);    

static const uint32 Uint32_MIN = uint32(0);  
static const uint32 Uint32_MAX = uint32(0xffffffff);                      

static const float32 Float32_MIN = float32(1.175494351e-38F);                 
static const float32 Float32_MAX = float32(3.402823466e+38F);   

static const float32 Float32_Sqrt2 = float32(1.41421356237309504880f);          
static const float32 Float32_SqrtHalf = float32(0.7071067811865475244008443f);  

extern const float32 Float32_Inf;
static const float32 Float32_One  = float32(1.0);                          
static const float32 Float32_Half = float32(0.5);                           
static const float32 Float32_Zero = float32(0.0);    

static const float32 Float32_PI   = float32(3.14159265358979323846);        
static const float32 Float32_2PI  = float32(2.0 * 3.14159265358979323846);  
static const float32 Float32_InversePI = float32(1.0 / 3.14159265358979323846);
static const float32 Float32_HalfPI = float32(0.5 * 3.14159265358979323846);   
static const float32 Float32_2InversePI = float32(2.0 / 3.14159265358979323846);
static const float32 Float32_Inverse2PI = float32(0.5 / 3.14159265358979323846);


#ifndef MTU
#define MTU 1500 //以太网标准MTU
#endif

#if defined(_MSC_VER) && _MSC_VER < 1800
#define va_copy(a,z) ((void)((a)=(z)))
#endif