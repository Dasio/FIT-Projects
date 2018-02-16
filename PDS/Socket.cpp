#include "Socket.h"
#include <stdexcept>
#include <sys/socket.h> 
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <linux/if_ether.h>  
#include <netinet/ip.h>  
#include <ifaddrs.h>
#include <iomanip>
#include <sstream>
#include <list>
#include <numeric>

#include <string>
#include <cstring>
#include <iostream>

using namespace std;

Socket::Socket(int protocol)
{
	 if ((sd = socket (PF_PACKET, SOCK_RAW, htons (protocol))) < 0) {
	 	throw std::runtime_error(std::string("Socket::Socket() - ")
	 		.append(strerror(errno)));
	 }
	 buffer.reserve(IP_MAXPACKET);

}

void Socket::loadInterface(std::string& interface)
{
	struct ifreq ifr;

	if (interface.length() > sizeof(ifr.ifr_name))
		throw std::runtime_error(std::string("Socket::loadInterface) - interface name too long")
			.append(strerror(errno)));

	std::strcpy (ifr.ifr_name, interface.c_str());
	// Get mac address
	if (ioctl (sd, SIOCGIFHWADDR, &ifr) < 0)
		throw std::runtime_error(std::string("Socket::loadInterface() - ioctl(mac) - ")
			.append(strerror(errno)));
	memcpy (src_mac, ifr.ifr_hwaddr.sa_data, 6 * sizeof (uint8_t));

	// Get ipv4
	if (ioctl (sd, SIOCGIFADDR, &ifr) < 0)
		throw std::runtime_error(std::string("Socket::loadInterface() - ioctl(src_ip) - ")
			.append(strerror(errno)));
	memcpy (src_ip,  &((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr, 4 * sizeof (uint8_t));

	

	memset (&device, 0, sizeof (device));
	if ((device.sll_ifindex = if_nametoindex (ifr.ifr_name)) == 0)
	   throw std::runtime_error(std::string("Socket::loadInterface() - if_nametoindex")
			.append(strerror(errno)));


	if (ioctl (sd, SIOCGIFNETMASK, &ifr) < 0)
		throw std::runtime_error(std::string("Socket::loadInterface() - ioctl(dst_ip) - ")
			.append(strerror(errno)));
	memcpy (netmask, &((struct sockaddr_in*)&ifr.ifr_netmask)->sin_addr, 4 * sizeof (uint8_t));

  // Fill out sockaddr_ll.
  device.sll_family = PF_PACKET;
  memcpy (device.sll_addr, src_mac, 6 * sizeof (uint8_t));
  device.sll_halen = 6;

  getRange();
  getIPv6(interface);
}

std::pair<uint32_t, uint32_t> Socket::getRange()
{
	IPv4 first_ip, last_ip;
	for(int i=0; i<4; ++i)
	{
		first_ip[i] = src_ip[i] & netmask[i];
		last_ip[i] = src_ip[i] | ~netmask[i];
	}
	uint32_t first = first_ip[0] << 24 | first_ip[1] << 16 | first_ip[2] << 8 | first_ip[3];
	uint32_t last = last_ip[0] << 24 | last_ip[1] << 16 | last_ip[2] << 8 | last_ip[3];

	return std::make_pair(first, last);
}

int Socket::send(Packet &packet)
{
	int bytes = sendto (sd, packet.getBuffer().data(), packet.getBuffer().size(),
	 						0, (struct sockaddr *) &device, sizeof (device));
	if (bytes <= 0)
	{
	    throw std::runtime_error(std::string("Socket::send() - sendto - ")
			.append(strerror(errno)));
	}
	return bytes;
}

uint16_t Socket::recvPacket(Packet *packet)
{
	fd_set set;
	struct timeval timeout;
	FD_ZERO(&set); /* clear the set */
	FD_SET(sd, &set); /* add our file descriptor to the set */
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	int rv = select(sd + 1, &set, NULL, NULL, &timeout);

	int recv_size;

	if (rv == 0)
	{
	    // timeout, socket does not have anything to read
	    return 0;
	}

	recv_size = recv(sd, buffer.data(), IP_MAXPACKET, 0); 
	packet->modify(buffer.data(), recv_size);
	return recv_size;
}

Arp::arpHeader* Socket::receive()
{
	fd_set set;
	struct timeval timeout;
	FD_ZERO(&set); /* clear the set */
	FD_SET(sd, &set); /* add our file descriptor to the set */
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	int rv = select(sd + 1, &set, NULL, NULL, &timeout);

	int recv_size;

	if (rv == 0)
	{
	    // timeout, socket does not have anything to read
	    return nullptr;
	}

	recv_size = recv(sd, buffer.data(), IP_MAXPACKET, 0);
	if (recv_size <= 0)
		return nullptr;

	return reinterpret_cast<Arp::arpHeader*>(buffer.data() + 6 + 6 + 2);
}

void Socket::getIPv6(std::string &interface)
{
    vector<string> addresses;

    struct ifaddrs *ifa=NULL,*ifEntry=NULL;
    void *addPtr = NULL;
    int rc = 0;
    char addressBuffer[INET6_ADDRSTRLEN];

    rc = getifaddrs(&ifa);
    if (rc==0) {
        for(ifEntry=ifa; ifEntry!=NULL; ifEntry=ifEntry->ifa_next)
        {
	    	if(ifEntry->ifa_addr->sa_data == NULL)
                continue;
            if (strcmp(ifEntry->ifa_name, interface.data()) != 0)
            	continue;
			if(ifEntry->ifa_addr->sa_family==AF_INET6)
			{
				addPtr = &((struct sockaddr_in6 *)ifEntry->ifa_addr)->sin6_addr;
            }
            else 
            	continue;
            const char *a = inet_ntop(ifEntry->ifa_addr->sa_family,
                          addPtr,
                          addressBuffer,
                          sizeof(addressBuffer));
	    	if(a != NULL)
	    		addresses.push_back(string(a));
        }
    }
    freeifaddrs(ifa);
    // Save only the first one
    if(addresses.size() > 0)
    	ipv6.assign(addresses[0]);
}
