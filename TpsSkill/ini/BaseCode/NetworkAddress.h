#ifndef NETWORKADDRESS_HXX_
#define NETWORKADDRESS_HXX_

#include "BaseType.h"

#if defined WIN32
#if defined METRO
#else
#include "winsock.h"
#endif
#else
#include <netinet/in.h>
#endif

#define IPV4_LENGTH 4

class NetworkAddress
{
public:

	NetworkAddress();
	
	NetworkAddress( const char* hostName, uint16 port = 0 );
	
	void setHostName( const char* theAddress );

	void setIp4Addr( unsigned int addr, uint16 port=0 );
	
	void setPort( uint16 port );
	
	string getHostName() const;
	
	/// get IP address as a string in the form "192.168.4.5"
	string getIpName() const;

	string getPortName() const;

	string getIpPortName() const;
	
	/** return the IP4 address packed as an unsigned int, in
	* network byte order. */
	unsigned int getIp4Address() const;

	/* return networkmask in network byte order */
	static unsigned int getNetmask();

	static string getNetmaskName();

	static unsigned int m_netmask;
	
	#if !defined METRO
	/** return the address in a sockaddr.  This value is assigned
	* @param socka    returned address */
	void getSockAddr( struct sockaddr & socka ) const;
	#endif
	
	uint16 getPort() const;
	
	friend bool operator < ( const NetworkAddress & xAddress, const NetworkAddress & yAddress );

	friend bool operator == ( const NetworkAddress & xAddress, const NetworkAddress & yAddress );	
	
	friend bool operator!=( const NetworkAddress & xAddress, const NetworkAddress & yAddress );
	
	friend std::ostream & operator<< ( std::ostream & xStream, const NetworkAddress& rxObj );
	
	NetworkAddress& operator=( const NetworkAddress& x );

	bool isSameNetwork( const NetworkAddress& dtIp );
	
	static bool is_valid_ip_addr( const char* ip_addr, char* retaddr = NULL );

	bool isValid();

private:

	void initIpAddress() const;
	mutable char ipAddress[ IPV4_LENGTH ];  // it's in network byte order
	uint16 aPort;
	string rawHostName;
	mutable bool ipAddressSet;
};

#endif
