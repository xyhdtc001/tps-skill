#pragma once

#include <map>
#include "stringdata.h"
#include "VFile.h"

#define VER_FILE "lof.dat"
#define VER_FILE_INFO "fileVerInfo.dat"

#ifdef X64
#define CONFIG_FILE "s3ver64.dat"
#else
#define CONFIG_FILE "s3ver.dat"
#endif

#define LOADING_PIC "loading.jpg"
#define WAITING_PIC "waiting.gif"
#define REMOTE_VER_FILE "rmf.dat"
#define VER_FILE_CONFIG_FILE "config.xml"

#pragma pack(1)

#define FILE_OK 0
#define FILE_OPERATING 1


struct S3_SAFE_FILE_HDR
{
	uint16 identity;
	uint16 version;
	uint32 fileNum;
	uint16 status;
	uint8 resev[64];
};

struct S3_VERFILE_ITEM
{
	uint32 hashID;
	uint16 zip : 1;
	uint16 fileVer : 15;
};

struct S3_VERFILE_INFO
{
	char name[255];
	uint32 hashID;
};

struct PACK_FILE_HDR
{
	uint32 fileLen;	
};

// struct SCRIPT_ITEM
// {
// 	uint16 pathLen;
// 	uint16 fileLen;
// 	uint8 path[0];
// };

const int MAX_FILE_LEN = 10000000;

#pragma pack()


class VerControl
{
public:
	VerControl(void);
	~VerControl(void);
	bool hashFolder( const Data& folder, uint32 ver );
	bool packFolder( const Data& srcFolder, const Data& tarFolder );
	bool mergFolder( const Data& srcDir, const Data& tarDir );
	void update( const Data& server );
	bool mergFile( const Data& filename, const Data& filepath, const Data& relaDir );
	bool EnumFile( const char* filename, const char* filepath, const char* relaDir );
	bool packFile( const char* filename, const char* filepath, const char* relaDir );
	static bool parseVerFile( const Data& name, std::map<uint32,uint16>& mapVer );

protected:
	VFile _file;
	VFile _infoFile;
	Data _mergPackDir;
	Data _mergTarDir;
	std::map<uint32,uint16> _mapFileHash;
	Data _tarFolder;
	uint32 _ver;
};
