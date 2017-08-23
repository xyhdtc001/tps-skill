#if !defined VDIRECTORY_H__
#define VDIRECTORY_H__

#ifdef WIN32
#ifdef METRO
#else
#endif
#else
#include <sys/types.h>
#include <dirent.h>
#endif

#include <vector>
#include <set>
#include "VFile.h"

typedef bool (*EnumFunc)( const char* filename, const char* filepath, const char* relaDir, void* pParam );
typedef bool (*FolderFunc)( const char* folderPath, const char* folder, void* pParam );

class VDirectory
{
public:
	VDirectory();
	VDirectory( const char* dir, bool create = false );
	bool open( const char* directory, bool create = false );
	bool EnumFiles( EnumFunc func, void* pParam=NULL );
	bool EnumDirs( FolderFunc func, void* pParam=NULL );

	const char* getPath();

	void getFiles();
	void getFilesVec( std::vector<string>* vecData );
	void getSubDirs();
	void getSubDirsSet( std::set<string>* pSet );
	const char* getFileName( unsigned int index );
	const char* getDirName( unsigned int index );
	static bool removeAll( const char* path, bool recursive=true );
	static bool removeAllBySubfix( const char* path, const char* subfix, bool recursive=true );

	uint32 getFileNum();
	uint32 getDirNum();

	void setIncludeFile( const char* data );
	void setExcludeFile( const char* data );
	void setExcludeDir( const char* data );

	bool isIncludeFile( const char* file );
	bool isExcludeFile( const char* file );
	bool isExcludeDir( const char* dir );

	static bool rename( const char* from, const char* to );
	static bool EnumAllFunc( const char* fullDir, const char* relaDir, EnumFunc func, FolderFunc folderFunc=NULL, void* pParam=NULL );
	static bool removeDir( const char* dir );
	static bool isDirectoryExist( const char* dir );
	static bool createDir( const char* dir );
	static bool createFileDir( const char* dir );
	static bool chDir( const char* dir );
	static bool copyDir( const char* srcDir, const char* tarDir );
	static bool isDirEmpty( const char* fullDir );

public:
	std::vector< string > _fileVec;
	std::vector< string > _dirVec;

protected:
	string _directory;
	std::vector< string > _vecIncludeFile;
	std::vector< string > _vecExcludeFile;
	std::vector< string > _vecExcludeDir;
	bool _bIncludeFile;
	bool _bExcludeFile;
	bool _bExcludeDir;
};

#endif
