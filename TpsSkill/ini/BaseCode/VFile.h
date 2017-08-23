#ifndef VFile_hxx
#define VFile_hxx
#include "BaseType.h"
#include "MemStream.h"
class VFile
{
public:
	enum SEEKFLAG
	{
		VFILE_BEGIN = 0,
		VFILE_CUR,
		VFILE_END
	};

	enum OPENFLAG
	{
		NONE = 1,
		READ = 2,
		READWRITE = 4,
		APPEND = 8,
		CREATE = 16,
		MODIFY = 32,
	};

public:
	VFile();
	VFile( const char* fullFilePath, int mode=NONE );
	virtual ~VFile();

	bool openFile( const char* fullFilePath, int mode=NONE );
	void closeFile();
	bool seekTo( int to, int orign );
	int read( unsigned char* buf, int len );
	bool readData( unsigned char* buf, int len );
	int write( const unsigned char* buff, uint32 len );
	int write( uint32 len, const unsigned char* buff );
	int write( const char* data );
	int writeLine( const char* date="" );
	bool writeData( const void* buff, uint32 len );
	int readLine( char* buff, int maxlen=1024 );
	int readLine( OUT string& dtLine, int maxlen=1024 );
	void flush();
	inline bool isEof()
	{
		return ( feof( hFile ) != 0 );
	}

	string getFilePath() const;
	string getExtName() const;
	long getFileLen();
	
	static long getFileLen( const char* file );
    static bool chmode( const char* file, const char* mod );
	static bool isFileExist( const char* fullFilePath );
	static bool deleteFile( const char* fullFilePath );
	static bool rename( const char* path, const char* from, const char* to );
	static bool move( const char* from, const char* to, bool replace );
	static bool copyFile( const char* from, const char* to, bool replace=true );
	static bool getTime( const char* path, uint64& create, uint64& access, uint64& write );
	static bool resetAccessTime( const char* path );
	
	long getFilePos();
	bool copyTo( const char* target );
	ref_ptr<MemStream> getAllData();
	bool getAllData( uint8* data );
	bool isOpen()
	{
		return _bOpen;
	}
private:
	string _path;
	bool _bOpen;
	FILE* hFile;

};

#endif
