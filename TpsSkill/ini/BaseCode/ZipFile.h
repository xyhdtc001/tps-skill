#pragma once

#include "ref_ptr.h"
#include "memstream.h"
#include "StringData.h"
#include "VFile.h"

//#define USE_ZIP_ZLIB

#define USE_ZIP_LZMA

class ZipFile
{
public:
	static bool zipFile( VFile& inFile, VFile& outFile );
	static ref_ptr<MemStream> zipFileToMem( VFile& inFile );
	static ref_ptr<MemStream> unZipStream( ref_ptr<MemStream> memStream );
	static bool unZipFile( VFile& inFile, VFile& outFile );
};