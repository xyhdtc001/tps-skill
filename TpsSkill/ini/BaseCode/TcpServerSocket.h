#ifndef TcpServerSocket_hxx
#define TcpServerSocket_hxx

#if defined WIN32
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
#endif

#include "Connection.h"


class TcpServerSocket
{
public:
	TcpServerSocket();

	TcpServerSocket( const TcpServerSocket& );
	
	TcpServerSocket& operator=( TcpServerSocket& other );

	virtual ~TcpServerSocket();

	bool init( int servPort=-1, int lisQ=5 );

	/* Accept socket connection
	Return -1 if timeout or error */
	bool accept( Connection* conn, int timeout=-1 );

	Connection* getServerConn()
	{
		return _serverConn.get();
	};

	short getServerPort();

private:
	ref_ptr<Connection> _serverConn;
	int m_listenQ;
	short m_srvPort;
};

#endif
