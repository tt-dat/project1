#include <sys/socket.h>		 // for using socket(), send(), recv(), ...
#include <arpa/inet.h>		 // define socklen_t, use inet_ntop()
#include <net/ethernet.h>	 // define ethr_header, include net/if_ether.h for ETH_P_ALL, 
#include <linux/if_packet.h> // for using struct sockaddr_ll, struct packet_mreq
#include <net/if.h>			 // for using if_nametodinex()

// use ip, tcp, udp... header structure
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>



int init_socket();
void sniff(int);
void process_packet(unsigned char*, size_t);
