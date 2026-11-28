#include <libc.h>
#include <net.h>
#include <inner_parse.h>
#include <parse_buffer.h>

/* parse current level header
 * and transport payload and upper protocol to upper level
 */
void parse_link_frame(unsigned char* frame, size_t frame_size) {
	packet_name = "***Ethernet packet***";

	// get header
	struct ether_header* ethh = (struct ether_header*)frame;
	print_ether_header(ethh);
	size_t header_size = sizeof(struct ether_header);

	// dump header content
	dump_string("Ethernet header:\n");
	dump_data(frame, header_size);

	// move payload to upper level
	unsigned char* datagram = frame + header_size;
	size_t datagram_size = frame_size - header_size;
	uint16_t network_proto = ntohs(ethh->ether_type);

	parse_network_datagram(datagram, datagram_size, network_proto);
}

/* TODO: * convert type number to protocol name for ip, ipv6...  * check strange protocol number                                                                 */
void print_ether_header(struct ether_header* ethh) {

	print_header_string("Ethernet Header\n");

	// parse into field                                                             
	uint8_t* dst = ethh->ether_dhost;
	uint8_t* src = ethh->ether_shost;
	uint16_t type = ntohs(ethh->ether_type);

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Destination MAC:         %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", dst[0], dst[1], dst[2], dst[3], dst[4], dst[5]);

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE,  "\t|-Source MAC:              %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", src[0], src[1], src[2], src[3], src[4], src[5]);

	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, "\t|-Type:                    %" PRIu16 "\n", type);

}


