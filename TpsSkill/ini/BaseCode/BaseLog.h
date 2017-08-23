#if  !defined LOG_OUT_H__
#define LOG_OUT_H__

enum LOG_SYSTEM
{
	MAIN = 0,
	GRAPHIC,
	ROLE,
	ANIMATION,
	SCRIPT,
	NETWORK,
	PHYX,
	PLUGIN,
	RES,
	GUI,
	SOUND,
	MAX_LOG_SYSTEM,
};

#define LogOut log_out

#if defined LOG_SUPPORT
#define LogDebug log_debug
#else
#define LogDebug(...)
#endif

enum LOG_TYPE
{
	LOG_FATAL=0,		// ��ʾ�������־��һ������ϵͳ�������ش��� 
	LOG_ERR,			// ��ʾ�������־��һ��ϵͳ���� 
	LOG_WARN,			// ��ʾ�������־��һ��������Ϣ 
	LOG_DEBUG,			// ��ʾ�������־Ϊһ��������Ϣ 
	LOG_INFO,			// ��ʾ�������־��һ��ϵͳ��ʾ 
	MAX_LOG_TYPE,
};

void log_out( LOG_SYSTEM sys, LOG_TYPE pri, const char* fmt, ... );
void log_debug(LOG_SYSTEM sys, const char* fmt, ... );

typedef void ( *LogOutFunc )( LOG_SYSTEM sys, LOG_TYPE type, char* buff );

typedef bool ( *IsLogOutUsedFunc )( LOG_SYSTEM sys, LOG_TYPE type );

extern const char* logSysNames[];
extern const char* logTypeNames[];

extern LogOutFunc g_logFunc;
extern IsLogOutUsedFunc g_isLogUsedFunc;

/*PLATFORM_API*/ void SetLogOutFunc( LogOutFunc func );
/*PLATFORM_API*/ void SetIsLogOutUsedFunc( IsLogOutUsedFunc func );

#endif