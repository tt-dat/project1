#include <net.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define BUF_SIZE 65536

void process_packet(char*, int);

void print_ether_frame(char*);
void print_ether_header(char*);

void sniff_and_display(int packet_socket) {
	char buf[BUF_SIZE];
	int count = 0;

	while (1) {
		int byte_reads = recv(packet_socket, buf, BUF_SIZE - 1, 0);
		if (byte_reads == -1) { 
			perror("recv error"); 
			exit(1);
		}

		process_packet(buf, byte_reads);

	}
}

/* TODO: 
 * process to upper protocol
 */
void process_packet(char* ether_frame, int size) {
	struct ether_header* etth = (struct ether_header*)ether_frame;

	uint16_t ether_type = ntohs(etth->ether_type);

	print_ether_frame(ether_frame); // temporary

	// If is not IP packet, dont't process any more
	if (ether_type != ETHERTYPE_IP && ether_type != ETHERTYPE_IPV6) {
		// print_ether_frame(ether_frame);
	} else {

	}

	printf("\n\n");
}

void print_ether_frame(char* ether_frame) {
	
	printf("*** ETHERNET FRAME\n");
	print_ether_header(ether_frame);

}

/* TODO:
 * convert type number to protocol name
 */
void print_ether_header(char* ether_frame) {
	struct ether_header* ethh = (struct ether_header*)ether_frame;

	printf("Ethernet Header\n");
			

	// parse into field
	uint8_t* dst = ethh->ether_dhost;
	uint8_t* src = ethh->ether_shost;


	printf("\t|-Destination MAC: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", dst[0], dst[1], dst[2], dst[3], dst[4], dst[5]);

	
	printf("\t|-Source MAC:      %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", src[0], src[1], src[2], src[3], src[4], src[5]);

	printf("\t|-Type:            %" PRIu16 "\n", ntohs(ethh->ether_type));

}




