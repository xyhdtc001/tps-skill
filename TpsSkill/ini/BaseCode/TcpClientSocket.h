#ifndef TcpClientSocket_hxx
#define TcpClientSocket_hxx

#if defined WIN32
#else
    #ifdef __ANDROID__
        #include <sys/endian.h>
    #endif
    #include <errno.h>
    #include <unistd.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

#include "Connection.h"

class NetworkAddress;

class TcpClientSocket
{
public:
	TcpClientSocket( const char* hostName, uint16 servPort, bool blocking=true );
	
	TcpClientSocket( const NetworkAddress& server, bool blocking=true );
	
	TcpClientSocket( const TcpClientSocket& );
	
	TcpClientSocket& operator=( TcpClientSocket& other );

	TcpClientSocket( bool blocking=true );
	
	~TcpClientSocket();

	bool connect( int32 timeout=-1 );

	void setServer( const char* server, uint16 serverPort, bool blocking=true );

	#ifndef METRO
	void setServer( struct sockaddr, bool blocking=true );

	inline SOCKET getConnId()
	{
		return _connId;
	}
	#endif

	void close();

	inline Connection* getConn()
	{
		return _conn.get();
	};


private:
	ref_ptr<Connection> _conn;

	#if !defined METRO
	struct sockaddr _addr;
	SOCKET _connId;
	#endif

	string _hostName;
	uint16 _serverPort;
	bool _blocking;
	bool _bAddr;
};

#endif
