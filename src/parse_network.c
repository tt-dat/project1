#include <libc.h>
#include <net.h>
#include <inner_parse.h>
#include <parse_buffer.h>


void parse_network_datagram(unsigned char* dgram, size_t dgram_size, uint16_t proto) {

	size_t header_size;
	uint8_t transport_proto;

	switch (proto) {
		case ETHERTYPE_IP:	// IPv4 packet	
			packet_name = "***IPv4 Packet***";

			struct iphdr* iph = (struct iphdr*)dgram;

			// inner_parse.header
			print_ip_header(iph);

			// print header type
			dump_string("IPv4 Header:\n");

			header_size = sizeof(struct iphdr);
			transport_proto = iph->protocol;
			break;

		case ETHERTYPE_IPV6:	// IPv6 packet
			packet_name = "***IPv6 Packet***";

			struct ip6_hdr* ip6_h = (struct ip6_hdr*)dgram;

			print_ip6_header(ip6_h);

			dump_string("IPv6 Header:\n");

			// update header_size and transport_proto
			header_size = sizeof(struct ip6_hdr);
			transport_proto = ip6_h->ip6_nxt;
			break;

		default:	// don't analyze further
			if (dgram_size < MAX_PRINT_SIZE) { // print if payload is small
				dump_string("Ethernet Payload:\n");
				dump_data(dgram, dgram_size);

			}
			return;
	}

	// dump header content
	dump_data(dgram, header_size);

	unsigned char* segment = dgram + header_size;
	size_t segment_size = dgram_size - header_size;

	//parse_transport_segment(segment, segment_size, transport_proto);

}

void print_ip_header(struct iphdr* iph) {

	print_header_string("IPv4 Header\n");

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
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Version:              %u\n", iph->version);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Header length:        %u Bytes\n", iph->ihl << 2);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Type of service:      %u\n", iph->tos);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Datagram length:      %u Bytes\n", ntohs(iph->tot_len));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Identifier:           %u\n", ntohs(iph->id));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Fragmentation offset: %u\n", ntohs(iph->frag_off));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Time-to-live:         %u\n", iph->ttl);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Upper protocol:       %u\n", iph->protocol);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Header checksum:      0x%X\n", ntohs(iph->check));
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Source Address:       %s\n", s_re);
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Destination Address:  %s\n", d_re);
}

// TODO: // ipv6 version is reversed?
void print_ip6_header(struct ip6_hdr* ip6_h) {
	print_header_string("IPv6 Header\n");

	struct sockaddr_in6 src_in6, dst_in6;
	
	char s_re[INET6_ADDRSTRLEN], d_re[INET6_ADDRSTRLEN];
	const char *s_chk, *d_chk;

	src_in6.sin6_addr = ip6_h->ip6_src;
	dst_in6.sin6_addr = ip6_h->ip6_dst;

	s_chk = inet_ntop(AF_INET6, &(src_in6.sin6_addr), s_re, INET6_ADDRSTRLEN);
	d_chk = inet_ntop(AF_INET6, &(dst_in6.sin6_addr), d_re, INET6_ADDRSTRLEN);

	if (!s_chk || !d_chk) {
		perror("inet6_ntop");
		exit(1);
	}


	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Version:              %u\n", ip6_h->ip6_vfc >> 4);

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Payload length:       %u\n", ntohs(ip6_h->ip6_plen));

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Next header:          %u\n", ip6_h->ip6_nxt);

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Hop limit:            %u\n", ip6_h->ip6_hlim);

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Source Address:       %s\n", s_re);
	
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Destination Address:  %s\n", d_re);
}

