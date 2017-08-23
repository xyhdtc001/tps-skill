#if !defined WENSDY_BASEFUNC_H
#define WENSDY_BASEFUNC_H

#include "basehdr.h"
//#include "StringData.h"
//#include "str.h"


inline uint32 getMin(uint32 a, uint32 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline uint16 getMin(uint16 a, uint16 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline uint8 getMin(uint8 a, uint8 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline int32 getMin(int32 a, int32 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline int16 getMin(int16 a, int16 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline int8 getMin(int8 a, int8 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline float getMin(float a, float b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline double getMin(double a, double b)
{
	return a>b ? b : a;
}

/// Returns the greater of the two parameters: a & b.
inline uint32 getMax(uint32 a, uint32 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline uint16 getMax(uint16 a, uint16 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline uint8 getMax(uint8 a, uint8 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline int32 getMax(int32 a, int32 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline int16 getMax(int16 a, int16 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline int8 getMax(int8 a, int8 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline float getMax(float a, float b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline double getMax(double a, double b)
{
	return a>b ? a : b;
}

/********************* 算MAC ********************************/
void MacData(unsigned char *from,unsigned char *to,int len,unsigned char *key);

bool ascBcd( const char* ascstr,unsigned char* bcdstr,int bcdlen );

int compareNoCase( const char *str1, const char *str2, int len );

bool equalNoCase( const char *str1, const char *str2 );

/**************************************************************
说明: bcd码转Asc码
参数: cbuf Asc码字符串; ubuf:目标bcd缓冲; len:要转bcd数据长度
情况: 2002.10 by wensdy
***************************************************************/
void bcdAsc( char *cbuf,const unsigned char *ubuf,int len );

/***************** 删除字符中的空格 ***************************/
char* DelSpace(char* strConvert);

int HexToInt(char* hexstr);

/***************** 计算校验和(TCP/IP) *************************/
unsigned short checksum( unsigned short *buffer,int size );

int AsccharToInt( unsigned char asc );
void AscstrToBcdstr( unsigned char *ascstr, unsigned char *bcdstr, int asclen );
void BcdstrToAscstr( unsigned char *bcdstr, unsigned char *ascstr, int bcdlen );
int BcdcharToInt( unsigned char bcdchar );
int strnatcmp0( const char* a, const char* b, int fold_case );
int compare_left( const char* a, const char* b );
int compare_right( const char* a, const char* b );
void expandEscape( char *dest, const char *src );
bool collapseEscape( char *buf );

int getHexDigit( char c );
int charConv( int in );

double	GetDistanceBy3D( float fX0, float fY0, float fZ0, float fX1, float fY1, float fZ1 );

uint32 fastHash( const char * data, int len, uint32 hashSoFar = 0 );



double UInt64ToDouble( uint64 uValue );

void vsleep( long millsec );

std::string URLEncode( const std::string &sIn );

unsigned long getSysMillsec();

unsigned long getSysSecond();

unsigned long getTimeMillsec();
void resetTimeMillsec();
unsigned long getSysTimeMillsec();

void my_gcc_srand(int i);
int my_gcc_rand();
void my_win_srand(int i);
int my_win_rand();

#endif

