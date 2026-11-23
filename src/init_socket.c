#include <net.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define NETWORK_INTERFACE "wlan0"

/* 
 * This function will create a PACKET_SOCKET to capture all Link-layer packets
 * Bind this socket to a Network Interface
 * Turn on Promiscuous mode to sniff all packets include that has different
 * address from host (with Ethernet connection, Wi-Fi packet will be discarded automatically by driver)
 */
int init_socket() {

	// socket_type: SOCK_RAW to include Link-layer header
	// ETH_P_ALL match all protocol
	int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (packet_socket == -1) { 
		perror("Create socket fail");
		exit(1);
	}


	// bind this socket with an interface
	struct sockaddr_ll s_addr;
	memset(&s_addr, 0, sizeof(s_addr));		// initialize before use
	s_addr.sll_family = AF_PACKET;			// same as packet_socket
	s_addr.sll_protocol = htons(ETH_P_ALL); // capture all packet from link-layer
	s_addr.sll_ifindex = if_nametoindex(NETWORK_INTERFACE);	// get interface
	socklen_t addrlen = sizeof(struct sockaddr_ll);
	
	int bind_status = bind(packet_socket, (const struct sockaddr*)&s_addr, addrlen);
	if (bind_status == -1) { 
		perror("Bind fail"); 
		exit(1);
	}
	

	// structure to be passed as argument to PACKET_ADD_MEMBERSHIP option
	struct packet_mreq pk_mr;
	memset(&pk_mr, 0, sizeof(pk_mr));
	pk_mr.mr_ifindex = if_nametoindex(NETWORK_INTERFACE);
	pk_mr.mr_type = PACKET_MR_PROMISC; //turn on Promiscuous mode
	socklen_t optlen = sizeof(struct packet_mreq);
									  
	// Set option for socket
	// Setsockopt will make kernel turn off Promiscuous automatically 
	// after close the socket
	// ioctl not
	int set_status = setsockopt(packet_socket, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &pk_mr, optlen);
	if (set_status == -1) {
		perror("Cannot' set socket option");
	}

	return packet_socket;

}


