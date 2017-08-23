// VHttp.h: interface for the VHttp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined VHTTP_H__
#define VHTTP_H__

#include <map>
#include "TcpClientSocket.h"
#include "MemStream.h"

class IHttpCallback
{
public:
	virtual void progress( uint32 percent ) = 0;
};


class VHttp  
{
public:
	VHttp();
	virtual ~VHttp();

	static string UrlEncode( const string& url );
	int GetHttpResponseData( const char* fullUrl, char* pData, int len, bool bGet=true, bool bGetResp=true );
	int GetHttpResponseData( const char* ip, const char* url, char* pData,
								int len, bool bGet = true, bool bGetResp = true );

    void setHttpProxy( const char* proxy );
	void setCookie( const char* cookie );
	int getHttpFile( const char* fullUrl, const char* savefile, int startpos=0 );
	int getHttpFile( const char* server, const char* httpfile, const char* savefile, int startpos=0 );
	ref_ptr<MemStream> getHttpFileStream( const char* fullUrl );
	ref_ptr<MemStream> getHttpFileStream( const char* server, const char* httpfile );
	ref_ptr<MemStream> getHttpStream( const char* server, const char* httpfile, int32 len=-1, uint32 retryNum=1 );
	ref_ptr<MemStream> getHttpStream( const char* file, uint32 retryNum=1 );
	void setNotifyCallback( IHttpCallback* callback );
	void clear();
	void setUserAgent( const char* agent );
	void debug();
	const char* getRespFieldValue( const char* field );

	void processCallback();
	uint32 getSpeed();

	int getWriteLen();
	int getFileLen();
	void enableSpeedCheck();
	void close();

protected:
	IHttpCallback* _callback;
	TcpClientSocket _clientSock;
	string	m_dtCookie;
	string	m_dtUserAgent;
    string    m_dtHttpProxy;
	uint32	m_iNotifyPos;
	uint32	m_iWriteLen;
	uint32	m_iLength;
	uint32	m_iStatusCode;
	unsigned long	_lastTime;
	uint32	_iWrited;
	uint32	_speed;
	bool	_bCheckSpeed;
	bool _bClose;

public:
	void clearParam();
	void addParam( const char* name, const char* val );
	std::map<string,string> _paramMap;
	std::map<string,string> _respHeadMap;
};

#endif
