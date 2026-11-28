#include <libc.h>
#include <net.h>
#include <inner_parse.h>
#include <parse_buffer.h>

void parse_transport_segment(unsigned char* seg, size_t seg_size, uint16_t proto) {

	size_t header_length;
	
	switch (proto) {
		case IPPROTO_TCP:
			packet_name = "***TCP Packet***";

			struct tcphdr* tcph = (struct tcphdr*)seg;
			
			print_tcp_header(tcph);

			dump_string("TCP Header:\n");

			header_length = tcph->doff << 2;
			break;

		case IPPROTO_UDP:
			packet_name = "***UDP Packet";

			struct udphdr* udph = (struct udphdr*)seg;

			print_udp_header(udph);

			dump_string("UDP Header:\n");

			header_length = 8;
			break;

		case IPPROTO_ICMP:
			break;
		default:
			dump_string("IP Payload\n");
			dump_data(seg, seg_size);
	}

	// dump transport protocols header
	dump_data(seg, header_length);

	// TODO:check seg_size >= header_length
	dump_string("Transport Payload\n");
	unsigned char* payload = seg + header_length;
	size_t payload_size = seg_size - header_length;
	dump_data(payload, payload_size);
}

void print_tcp_header(struct tcphdr* tcph) {
	print_header_string("TCP Packet\n");

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Source Port: %u\n", ntohs(tcph->source));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Destination Port: %u\n", ntohs(tcph->dest));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Sequence Number: %u\n", ntohl(tcph->seq));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Acknowledment number: %u\n", ntohl(tcph->ack_seq));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Header length: %u Bytes\n", tcph->doff << 2);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Urgent flag: %u\n", tcph->urg);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Acknowledment flag: %u\n", tcph->ack);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Push flag: %u\n", tcph->psh);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Reset flag: %u\n", tcph->rst);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Synchronize Flag: %u\n", tcph->syn);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Finish Flag: %u\n", tcph->rst);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Window: %u\n", ntohs(tcph->window));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Total Checksum: %u\n", ntohs(tcph->check));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Urgent Pointer: %u\n", ntohs(tcph->urg_ptr));
}

void print_udp_header(struct udphdr* udph) {
	print_header_string("UDP Packet\n");

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Source Port: %u\n", ntohs(udph->source));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Destination Port: %u\n", ntohs(udph->dest));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Total length: %u\n", ntohs(udph->len));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Checksum:	%u\n", ntohs(udph->check));
}
