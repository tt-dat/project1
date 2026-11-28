#include <libc.h>
#include <net.h>
#include <inner_parse.h>
#include <parse_buffer.h>

void parse_transport_segment(unsigned char* seg, size_t seg_size, uint16_t proto) {
	
	switch (proto) {
		case IPPROTO_TCP:
			packet_name = "***TCP Packet***";

			struct tcphdr* tcph = (struct tcphdr*)seg;
			
			//print_tcp_header(tcph);

		case IPPROTO_UDP:
			packet_name = "***UDP Packet";

			struct udphdr* udph = (struct udphdr*)seg;

			print_udp_header(udph);

		case IPPROTO_ICMP:
		default:
			dump_string("IP Payload\n");
			dump_data(seg, seg_size);
	}

}

void print_udp_header(struct udphdr* udph) {
	print_header_string("UDP Packet\n");

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "/t|-Source Port: %u\n", ntohs(udph->source));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "/t|-Destination Port: %u\n", ntohs(udph->dest));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "/t|-Total length: %u\n", ntohs(udph->len));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "/t|-Checksum:	%u\n", ntohs(udph->check));
}
