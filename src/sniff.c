#include <net.h>
#include <libc.h>

#define BUF_SIZE ETH_MAX_MTU

/* This function read raw bytes from network card
 * and call process_packet() to fill information
 * then output the result
 */
void sniff(int packet_socket) {
	
	unsigned char buf[BUF_SIZE];

	while (1) {
		// read packet
		int byte_reads = recv(packet_socket, buf, BUF_SIZE, 0);
		if (byte_reads < 0) { 
			perror("recv error"); 
			exit(1);
		}

		process_packet(buf, byte_reads);

	}
}

