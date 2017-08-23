

#pragma once
#include <stddef.h>
#undef IN
#define IN

#undef OUT
#define OUT

typedef int					INT;
typedef unsigned int		UINT;
//typedef unsigned long		DWORD;
typedef unsigned int		DWORD;
typedef unsigned short		WORD;
typedef float				FLOAT;
typedef long long			__int64;
typedef unsigned long long	ULONGLONG;
typedef __int64				_INT64;
typedef __int64				INT64;
typedef unsigned long long	UINT64;
typedef unsigned int		UINT32;
typedef unsigned char		UINT8;
typedef unsigned short		UINT16;
typedef long				LONG;
typedef unsigned long		ULONG;
typedef unsigned char		BYTE;
typedef unsigned char		_TUCHAR;
typedef double				DOUBLE;

typedef void *HANDLE;
typedef void VOID;
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

DECLARE_HANDLE(HWND);

#ifndef CONST
#define CONST	const
#endif
typedef char CHAR;
typedef unsigned char UCHAR;
typedef char TCHAR;
typedef short SHORT;
typedef unsigned short USHORT;
//typedef long LONG;

typedef CONST CHAR *LPCSTR, *PCSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;

typedef CHAR *LPSTR, *PSTR;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

#define WM_USER	0x0400

#define __stdcall pascal

#define RtlZeroMemory(Destination, Length) memset((Destination), 0, (Length))
#define ZeroMemory RtlZeroMemory

typedef struct tagRECT
{
	LONG	left;
	LONG	top;
	LONG	right;
	LONG	bottom;
} RECT, *PRECT;

typedef struct tagPOINT
{
	LONG x;
	LONG y;
}POINT, *PPOINT;

char *_itoa(int n, char* pBuffer, int nLength);
char *_i64toa(__int64 n, char* pBuffer, int nLength);
__int64 _atoi64(const char *pBuffer);
char *ltoa(int n, char* pBuffer, int nLength);

#define itoa(n, pBuffer, nLength) _itoa((n), (pBuffer), (nLength))

#define _snprintf snprintf
#define sscanf_s sscanf
#define strncpy_s strncpy

unsigned long timeGetTime();

size_t lstrlen(LPCSTR lpString);
LPTSTR lstrcpy(LPTSTR lpString1, LPCTSTR lpString2);
LPTSTR lstrcat(LPTSTR lpString1, LPCTSTR lpString2);
int lstrcmp(LPCTSTR lpString1, LPCTSTR lpString2);
#define stricmp(p1, p2) strcasecmp((p1), (p2))

#ifndef _stricmp
#define _stricmp(p1, p2) strcasecmp((p1), (p2))
#endif

#define _mbschr(p1, p2) strchr((p1), (p2))
#define _tcschr(p1, p2) strchr((p1), (p2))
#define _tcspbrk(p1, p2) strpbrk((p1), (p2))
#define _tcsstr(p1, p2) strstr((p1), (p2))
#define _tcscmp(p1, p2) strcmp((p1), (p2))
#define _tcsicmp(p1, p2) _stricmp((p1), (p2))


#define __max(a, b) (((a) > (b)) ? (a) : (b))
#define __min(a, b) (((a) < (b)) ? (a) : (b))

#define _MAX_PATH	260
#define _MAX_DRIVE	3
#define _MAX_DIR	256
#define _MAX_FNAME	256
#define _MAX_EXT	256

#if defined(__ANDROID__)
#define SO_NOSIGPIPE MSG_NOSIGNAL
#endif
