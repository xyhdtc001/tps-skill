#if !defined FTP_CONN_H__
#define FTP_CONN_H__

#include <list>
#include "TcpClientSocket.h"

typedef void (*FTP_TRANS_CALLBACK)( unsigned int progress, void* param );

struct FtpFileItem
{
	string name;
	bool bDir;
	int fileLen;
};

class FtpConn
{
public:
	FtpConn(void);
	~FtpConn(void);
	bool connect( const char* server, short port );
	bool login( const char* user, const char* pasw );
	bool mkdir( const char* dir );
	bool chdir( const char* dir );
	bool ftpCmd( const char* cmd, char ch );
	void setTimeout( long millsec );
	bool getFile( const char* localFile, const char* remoteFile );
	bool putFile(const char* localFile, const char* remoteFile);
	bool openPort( TcpClientSocket& sock );
	bool modDate( const char* path, OUT string& date );
	bool dir();
	void close();
	bool isConnected();
	void setCallbackFunc( FTP_TRANS_CALLBACK func, void* param );
	void setNotifyPercent( unsigned int percent );
	FtpFileItem* getFtpItem( const char* name );

public:
	std::list< FtpFileItem > _fileList;

private:
	TcpClientSocket _clientSock;
	ref_ptr<Connection> _conn;
	char _cmdBuff[ 1024 ];
	long _timeout;
	bool _bConnected;
	unsigned int m_pace;
	FTP_TRANS_CALLBACK m_callBack;
	void *m_param;
};

#endif
