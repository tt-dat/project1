#include <net.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define BUF_SIZE 65536

// TODO: add COUNTERs


char packet_name[256]; 
char header_info[BUF_SIZE];	
char dump_info[BUF_SIZE];
char *header_cursor, *dump_cursor; // write position of header_info, dump_info

void process_packet(char*, size_t);
void process_link_frame(char*, size_t);
void process_network_datagram(char*, size_t, uint16_t);

void print_ether_frame(char*);
void print_ether_header(struct ether_header*);


/* This function read raw bytes from network card
 * and call process_packet() to fill information
 * then output the result
 */
void sniff_and_display(int packet_socket) {
	char buf[BUF_SIZE]; // very large buffer
	int count = 0;

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

void process_packet(char* packet, size_t size) {
	// initialize buffers for writing
	header_cursor = header_info;
	dump_cursor = dump_info;

	process_link_frame(packet, size);
	
	// print packet's infomation to output
	// printf("%s%s%s", packet_name, header_info, dump_info);
	printf("%s\n%s\n\n", packet_name, header_info);
}


/* Parse current level header
 * and transport payload and upper protocol to upper level
 */
void process_link_frame(char* frame, size_t frame_size) {
	strcpy(packet_name, "***Ethernet Packet***");
	
	// get header 
	struct ether_header* ethh = (struct ether_header*)frame;
	print_ether_header(ethh);
	size_t header_size = sizeof(ethh);
	
	// dump header content
	dump_cursor += sprintf(dump_cursor, "Ethernet Header:\n");
	//print_data(frame, header_size);

	// move payload to upper level
	char* datagram = frame + header_size;
	size_t datagram_size = frame_size - header_size;
	uint16_t network_proto = ntohs(ethh->ether_type);

	process_network_datagram(datagram, datagram_size, network_proto);
}


void process_network_datagram(char* dgram, size_t dgram_size, uint16_t proto) {

	size_t header_size;
	uint8_t transport_proto;

	switch (proto) {
		case ETHERTYPE_IP:
			strcpy(packet_name, "***IP Packet***");
			
			struct iphdr* iph = (struct iphdr*)dgram;

			// parse header
			//print_ip_header(iph);
			header_size = sizeof(iph);

			// print header type
			dump_cursor += sprintf(dump_cursor, "IP Header:\n");

			transport_proto = iph->protocol;
			break;

		case ETHERTYPE_IPV6:
			strcpy(packet_name, "***IPv6 Packet***");

			struct ip6_hdr* ip6_h = (struct ip6_hdr*)dgram;

			//print_ip6_header(ip6_h);
			header_size = sizeof(ip6_h);

			dump_cursor += sprintf(dump_cursor, "IPv6 Header:\n");

			// transport_proto = (ip6_h->ip6_ctlun).ip6_hdrctl.ip6_un1_nxt;
			break;

		default:	// don't analyze further
			dump_cursor += sprintf(dump_cursor, "Ethernet Payload:\n");
			//print_data(dgram, dgram_size);
			return;
	}

	// dump header content
	//print_data(dgram, header_size);

	char* segment = dgram + header_size;
	size_t segment_size = dgram_size - header_size;
	
	// process_transport_segment(segment, segment_size, transport_proto);
	
}


//void process_transport_segment(char* seg, size_t seg_size, uint16_t proto) {
//	switch(proto) {
//		case 
//	}
//}





/* TODO:
 * convert type number to protocol name
 */
void print_ether_header(struct ether_header* ethh) {

	header_cursor += sprintf(header_cursor, "Ethernet Header\n");
	

	// parse into field
	uint8_t* dst = ethh->ether_dhost;
	uint8_t* src = ethh->ether_shost;
	uint16_t type = ntohs(ethh->ether_type);


	header_cursor += sprintf(header_cursor, "\t|-Destination MAC: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", dst[0], dst[1], dst[2], dst[3], dst[4], dst[5]);
	
	
	header_cursor += sprintf(header_cursor, "\t|-Source MAC:      %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", src[0], src[1], src[2], src[3], src[4], src[5]);
	

	header_cursor += sprintf(header_cursor,"\t|-Type:            %" PRIu16 "\n", type);
	

}

void print_data(char* data, size_t size) {
	for (int i = 0; i < size; i++) {

	}
}



