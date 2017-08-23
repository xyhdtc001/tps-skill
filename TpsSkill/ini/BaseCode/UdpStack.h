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

	/** 返回传输模式 **/
	UdpMode getMode();
	
	/** 获得本地接受端口 **/
	unsigned short getLocalPort();
	
	/** 获得目的端口 **/
	unsigned short getDestinationPort ();
	
	NetworkAddress getDestinationHost() const;

	NetworkAddress getLocalHost() const;
	
	/** 设置发送接受模式 **/
	void setMode ( const UdpMode theMode );
	
	#ifndef METRO
	/* 获得socket句柄 **/
	SOCKET getSocketFD ();
	
	/** 将当前socket加入fd_set **/
	void addToFdSet ( fd_set* set );

	/** 检测当前句柄是否在fd_set中 **/
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
	
	/** 接受所有收到的数据,设置发送端地址,返回接受长度 -1 连接中断 0 无数据 **/
	int receiveFrom ( const void* buffer, const int bufSize, NetworkAddress* sender, long millsecond=-1 );
	
	/** 发送数据包 **/
	int transmit ( const void* data, size_t len, long millsecond=-1 );
	
	/** 向指定地址发送数据包 **/
	int transmitTo ( const void* buffer, const int length, const NetworkAddress dest, long millsecond=-1 );
	
	virtual ~UdpStack ();

	/** 设置阻塞模式 | true 阻塞, false 非阻塞 **/
	void setBlock( bool bBlock );
	
	/**  获得已经发送的数据长度 **/
	int getBytesTransmitted ();
	
	/** 获得已经发送的数据包 **/
	int getPacketsTransmitted ();
	
	/** 获得接收到的数据长度 **/
	int getBytesReceived ();
	
	/** 获得接收到的数据包 **/
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
	/// 本地发送接收数据的地址
	struct sockaddr_in _localAddr;

	/// receive from的远地地址 doClient()调用后产生
	struct sockaddr_in _remoteAddr;

	//socket句柄
	SOCKET _socketFd;
	#endif

	int rcvCount ;
	int sndCount ;
};


#endif
