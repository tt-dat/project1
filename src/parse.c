#include <libc.h>
#include <net.h>
#include <inner_parse.h>

// TODO: add COUNTERs
//


// TODO: // -beautify print format // - support packet filter base on packet_name
void parse(unsigned char* packet, size_t size) {
	// initialize buffers for writing
	header_cursor = header_info;
	dump_cursor = dump_info;

	parse_link_frame(packet, size);

	// print packet's infomation to output
	
	// remember to end string
	*header_cursor = *dump_cursor = '\0';
	printf("%s\n%s\n\n*Data Dump\n%s\n\n\n", packet_name, header_info, dump_info);
}




