#include <net.h>
#include <libc.h>
#include <parse.h>
#include <parse_buffer.h>

#define PACKET_SIZE ETH_MAX_MTU

/* This function read raw bytes from network card
 * and call process_packet() to fill information
 * then output the result
 */
void sniff(int packet_socket) {
	
	unsigned char pack[PACKET_SIZE];

	// init parse buffer for use during sniffing
	alloc_parse_buffer();

	while (1) {
		// read packet
		int byte_reads = recv(packet_socket, pack, PACKET_SIZE, 0);
		if (byte_reads < 0) { 
			perror("recv error"); 
			exit(1);
		}

		parse(pack, byte_reads);

	}

	free_parse_buffer();
}

