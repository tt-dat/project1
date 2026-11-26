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
		case IPPROTO_ICMP:
		default:
			dump_string("IP Payload");
			dump_data(seg, seg_size);
	}

}
