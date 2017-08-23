#ifndef UDP_HXX
#define UDP_HXX

#ifdef WIN32
#else
#include <sys/select.h>
#include <netinet/in.h>
#endif

#include <string>
#include "NetworkAddress.h"
#include "Connection.h"

typedef enum udpmode
{
    inactive,
    sendonly,
    recvonly,
    sendrecv
} UdpMode;

class UdpStackPrivateData;

class UdpStack
{
public:
	UdpStack();

	bool init( const char* server, uint16 clientPort=0, uint16 servPort=0,
							bool block=true, UdpMode udpMode=sendrecv, bool isMulticast=false );

	void close();

	/** ���ش���ģʽ **/
	UdpMode getMode();
	
	/** ��ñ��ؽ��ܶ˿� **/
	unsigned short getLocalPort();
	
	/** ���Ŀ�Ķ˿� **/
	unsigned short getDestinationPort ();
	
	NetworkAddress getDestinationHost() const;

	NetworkAddress getLocalHost() const;
	
	/** ���÷��ͽ���ģʽ **/
	void setMode ( const UdpMode theMode );
	
	#ifndef METRO
	/* ���socket��� **/
	SOCKET getSocketFD ();
	
	/** ����ǰsocket����fd_set **/
	void addToFdSet ( fd_set* set );

	/** ��⵱ǰ����Ƿ���fd_set�� **/
	bool checkIfSet ( fd_set* set );
	int receiveFrom ( const void* buffer, const int bufSize, sockaddr_in* sender, long millsecond=-1 );
	int transmitTo ( const void* buffer, const int length, const sockaddr_in dest, long millsecond=-1 );
	#endif


	/*
	/// Find the max of any file descripts in this stack and the passed value
	int getMaxFD ( int prevMax = 0 );
	*/
	
	/// Recive a datagram into the specifed buffer - returns size of mesg.
	/// Once connectPorts() is called, can't receive from other ports
	int receive ( const void* buffer, const int bufSize, long millsecond );
	
	/** ���������յ�������,���÷��Ͷ˵�ַ,���ؽ��ܳ��� -1 �����ж� 0 ������ **/
	int receiveFrom ( const void* buffer, const int bufSize, NetworkAddress* sender, long millsecond=-1 );
	
	/** �������ݰ� **/
	int transmit ( const void* data, size_t len, long millsecond=-1 );
	
	/** ��ָ����ַ�������ݰ� **/
	int transmitTo ( const void* buffer, const int length, const NetworkAddress dest, long millsecond=-1 );
	
	virtual ~UdpStack ();

	/** ��������ģʽ | true ����, false ������ **/
	void setBlock( bool bBlock );
	
	/**  ����Ѿ����͵����ݳ��� **/
	int getBytesTransmitted ();
	
	/** ����Ѿ����͵����ݰ� **/
	int getPacketsTransmitted ();
	
	/** ��ý��յ������ݳ��� **/
	int getBytesReceived ();
	
	/** ��ý��յ������ݰ� **/
	int getPacketsReceived ();

	bool doServer ( uint16 port );
	
	bool doClient ( const char* server, uint16 port );
	
private:
	
	UdpStack( const UdpStack& )
	{
		assert ( 0 );
	};

	bool isReadReady( int mSeconds ) const;

	bool isWriteReady( int mSeconds=500 ) const;
	
private:

	int localPort;										// port the stack is currently using
	bool bBlockMode;
	UdpMode mode;
	bool _block;
	
	#ifdef METRO
	DataWriter^ _dataWriter;
	#else
	/// ���ط��ͽ������ݵĵ�ַ
	struct sockaddr_in _localAddr;

	/// receive from��Զ�ص�ַ doClient()���ú����
	struct sockaddr_in _remoteAddr;

	//socket���
	SOCKET _socketFd;
	#endif

	int rcvCount ;
	int sndCount ;
};


#endif
