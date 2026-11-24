#include <net.h>
#include <libc.h>

#define INFO_SIZE 16384
#define MAX_PRINT_SIZE 1600

// TODO: add COUNTERs

static unsigned char packet_name[256]; 
static unsigned char header_info[INFO_SIZE], dump_info[INFO_SIZE];
static unsigned char *header_cursor, *dump_cursor; // write position of header_info, dump_info

void process_link_frame(unsigned char*, size_t);
void process_network_datagram(unsigned char*, size_t, uint16_t);

void print_ether_header(struct ether_header*);
void print_ip_header(struct iphdr*);
void print_ip6_header(struct ip6_hdr*);

void print_data(unsigned char*, size_t);


// TODO:
// -beautify print format
// - support packet filter base on packet_name
void process_packet(unsigned char* packet, size_t size) {
	// initialize buffers for writing
	header_cursor = header_info;
	dump_cursor = dump_info;

	process_link_frame(packet, size);

	// print packet's infomation to output
	
	// remember to end string
	*header_cursor = *dump_cursor = '\0';
	printf("%s\n%s\n\n*Data Dump\n%s\n\n\n", packet_name, header_info, dump_info);
}


/* Parse current level header
 * and transport payload and upper protocol to upper level
 */
void process_link_frame(unsigned char* frame, size_t frame_size) {
	strcpy(packet_name, "***Ethernet Packet***");

	// get header 
	struct ether_header* ethh = (struct ether_header*)frame;
	print_ether_header(ethh);
	size_t header_size = sizeof(struct ether_header);

	// dump header content
	dump_cursor += sprintf(dump_cursor, "Ethernet Header:\n");
	print_data(frame, header_size);

	// move payload to upper level
	unsigned char* datagram = frame + header_size;
	size_t datagram_size = frame_size - header_size;
	uint16_t network_proto = ntohs(ethh->ether_type);

	process_network_datagram(datagram, datagram_size, network_proto);
}


void process_network_datagram(unsigned char* dgram, size_t dgram_size, uint16_t proto) {

	size_t header_size;
	uint8_t transport_proto;

	switch (proto) {
		case ETHERTYPE_IP:
			strcpy(packet_name, "***IP Packet***");

			struct iphdr* iph = (struct iphdr*)dgram;

			// parse header
			print_ip_header(iph);
			header_size = sizeof(struct iphdr);

			// print header type
			dump_cursor += sprintf(dump_cursor, "IP Header:\n");

			transport_proto = iph->protocol;
			break;

		case ETHERTYPE_IPV6:
			strcpy(packet_name, "***IPv6 Packet***");

			struct ip6_hdr* ip6_h = (struct ip6_hdr*)dgram;

			//print_ip6_header(ip6_h);
			header_size = sizeof(struct ip6_hdr);

			dump_cursor += sprintf(dump_cursor, "IPv6 Header:\n");

			// transport_proto = (ip6_h->ip6_ctlun).ip6_hdrctl.ip6_un1_nxt;
			break;

		default:	// don't analyze further
			if (dgram_size < MAX_PRINT_SIZE) { // print if payload is small
				dump_cursor += sprintf(dump_cursor, "Ethernet Payload\n");
				print_data(dgram, dgram_size);

			}
			return;
	}

	// dump header content
	print_data(dgram, header_size);

	unsigned char* segment = dgram + header_size;
	size_t segment_size = dgram_size - header_size;

	// process_transport_segment(segment, segment_size, transport_proto);

}


//void process_transport_segment(unsigned char* seg, size_t seg_size, uint16_t proto) {
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

	header_cursor += sprintf(header_cursor, "\t|-Destination MAC:         %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", dst[0], dst[1], dst[2], dst[3], dst[4], dst[5]);

	header_cursor += sprintf(header_cursor, "\t|-Source MAC:              %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", src[0], src[1], src[2], src[3], src[4], src[5]);

	header_cursor += sprintf(header_cursor,"\t|-Type:                    %" PRIu16 "\n", type);

}

void print_ip_header(struct iphdr* iph) {

	header_cursor += sprintf(header_cursor, "IP Header\n");

	struct sockaddr_in src_in, dest_in;
	char s_re[INET_ADDRSTRLEN], d_re[INET_ADDRSTRLEN];
	const char *s_chk, *d_chk;	// check non-NULL dest

	src_in.sin_addr.s_addr = iph->saddr;
	dest_in.sin_addr.s_addr = iph->daddr;

	// translate Network Order Address to Presentation
	s_chk = inet_ntop(AF_INET, &(src_in.sin_addr), s_re, INET_ADDRSTRLEN);
	d_chk = inet_ntop(AF_INET, &(dest_in.sin_addr), d_re, INET_ADDRSTRLEN);
	if (!s_chk || !d_chk) {
		perror("inet_ntop");
		exit(1);
	}
	


	// parse into field
	header_cursor += sprintf(header_cursor, "\t|-Version:              %u\n", iph->version);
	header_cursor += sprintf(header_cursor, "\t|-Header length:        %u Bytes\n", iph->ihl << 2);
	header_cursor += sprintf(header_cursor, "\t|-Type of service:      %u\n", iph->tos);
	header_cursor += sprintf(header_cursor, "\t|-Datagram length:      %u Bytes\n", ntohs(iph->tot_len));
	header_cursor += sprintf(header_cursor, "\t|-Identifier:           %u\n", ntohs(iph->id));
	header_cursor += sprintf(header_cursor, "\t|-Fragmentation offset: %u\n", ntohs(iph->frag_off));
	header_cursor += sprintf(header_cursor, "\t|-Time-to-live:         %u\n", iph->ttl);
	header_cursor += sprintf(header_cursor, "\t|-Upper protocol:       %u\n", iph->protocol);
	header_cursor += sprintf(header_cursor, "\t|-Header checksum:      0x%X\n", ntohs(iph->check));
	header_cursor += sprintf(header_cursor, "\t|-Source Address:       %s\n", s_re);
	header_cursor += sprintf(header_cursor, "\t|-Destination Address:  %s\n", d_re);
}


void print_data(unsigned char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		// first element of line
		if (i % 16 == 0) { 
			if (i != 0) { dump_cursor += sprintf(dump_cursor, "\n"); }
			dump_cursor += sprintf(dump_cursor, "    "); 
		} 


		dump_cursor += sprintf(dump_cursor, "%02X ", data[i]);

		// last element of line or last line
		// then print the character representation of data
		if (i % 16 == 15 || i == size - 1) {
			// align with upper lines
			int pad_size = (15 - i%16)*3 + 1;
			unsigned char pad[pad_size];
			memset(pad, ' ', sizeof(pad));
			pad[pad_size - 1] = '\0';
			
			dump_cursor += sprintf(dump_cursor, "%s\t", pad);

			int start = (i >> 4) << 4;
			for (size_t j = start; j <= i; j++) {
				unsigned char c = (data[j] >= 32 && data[j] <= 127)? data[j] : '.';
				dump_cursor += sprintf(dump_cursor, "%c", c);
			}

		}
	}

	dump_cursor += sprintf(dump_cursor, "\n");
}



