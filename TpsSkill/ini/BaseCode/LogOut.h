#pragma once
#include "BaseLog.h"
#include "Referenced.h"

const int LOG_OUTPUT_FILE = 1 << 1;
const int LOG_OUTPUT_LOGSERVER = 1 << 2;
const int LOG_OUTPUT_REPORTSERVER = 1 << 3;

struct LOG_HDR
{
	int lLogMsgSize;
	LOG_SYSTEM sys;
	LOG_TYPE type;
};

typedef struct logpack
{
	LOG_HDR hdr;
	char strLogMsg[ 1024 ];
} LogPack;


class CLogOut : public CReferenced
{
public:
	CLogOut();
	void logOut( LOG_SYSTEM sys, LOG_TYPE type, char* buff );
	void initLogOutBySting( LOG_TYPE type, const char* str );
	bool isUseLogout( LOG_TYPE type );
	void setLogType( LOG_TYPE type );

public:
	int _outputType[MAX_LOG_TYPE];
	LOG_TYPE _startType;
};

void log_init( const char* server=NULL, const char* path=NULL, const char* reportServer=NULL );
void set_log( LOG_SYSTEM sys, LOG_TYPE type, const char* str );
void set_sys_log( LOG_TYPE type, const char* str );
void log_uinit();
void log_flush();
