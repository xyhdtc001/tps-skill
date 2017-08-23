#ifndef _STRINGDATA_H_
#define _STRINGDATA_H_

#include <vector>
#include <string>

#include "BaseType.h"

#define NOT_FOUND -1
#define FIRST -2
#define FOUND 0
static const char SPACE[] = " ";

class StringData
{
public:
	StringData();
	StringData( const char* str );
	StringData( const char* buffer, int length );
	StringData( const StringData& data );
	StringData( const std::string& str);
	 ~StringData();

	static StringData ToString( const char *str, ... );
	static StringData ToString( int32 val ){ return ToString( "%d", val ); }
	static StringData ToString( uint32 val ){ return ToString( "%u", val ); }
	static StringData ToString( float32 val ){ return ToString( "%g", val ); }
	static StringData ToString( const char val ){ return ToString( "%c", val ); }
	static StringData ToString( unsigned short val ){ return ToString( "%d", val ); }

	bool operator>( const StringData& ) const ;
	bool operator<( const StringData& ) const;
	StringData& operator=( const char* str );
	StringData& operator=( const StringData& data );
	int HexToInt();
	const char* c_str() const;

	StringData substr( int start, int length = -1 ) const;
	char getChar( unsigned int i ) const;
	void setchar( unsigned int i, char c );
	char operator[]( unsigned int i ) const;
	uint32 length() const;
	
	bool operator==( const char* str ) const;
	bool operator==( const StringData& data ) const;
	bool operator!=( const char* str ) const;
	bool operator!=( const StringData& data ) const;

	StringData operator+( const StringData& data ) const;
	StringData operator+( const char* str ) const;

	StringData& operator+=( const StringData& );
	StringData& operator+=( const char* );
	StringData& operator+=( int val );
	StringData& operator+=( unsigned int val );
	StringData& operator+=( char ch );

	void clear();

	void ToLower();

	//operator const char*() const;

	int32 toInt32() const;
	int64 toInt64() const;

	float toFloat() const;

	bool match( const StringData& match, StringData* data = NULL, bool replace = false, StringData replaceWith = "" );

	void replace( int startpos, int len, const StringData& replaceStr );

	void replace( const StringData& from, const StringData to );

	int findlast( const StringData& match, int stop=-1 ) const;

	int findlastNoCase( const StringData& match, int stop=-1 ) const;

	void removeChar( char ch );

	void removeSpaces();

	void removeTab();

	void clearSpaces();

	void clearAllSpaces();

	void inflate( int size, const StringData& fillval, bool before = true );

	void removeQuo();

	void expand( StringData startFrom, StringData findstr, StringData replstr, StringData delimiter );

	friend bool isEqualNoCase( const StringData& left, const StringData& right );

	bool isEqualNoCase( const StringData& data ) const;

	int32 find( const StringData& match, int start = 0 ) const;

	int findNoCase( const StringData& match, int star=0 );

	bool isEmpty() const;

	void removeCRLF();

	void split( const StringData& sp, std::vector<StringData>& outElements ) const;

	static StringData xorString( const StringData& str1, const StringData& str2 );

	//bool tobcd( char fillch );

	bool isNumeric();

	bool isEndWith( const StringData& str, bool bCase=true ) const;

	StringData getFilePath() const;

	void makePath();

	StringData getFileName() const;

	StringData getExtName() const;

	void lowerCase();

	void formatPath( bool url=true );

private:
	std::string buf;
};


typedef StringData Data;

#endif
