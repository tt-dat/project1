#include <stdlib.h>

#define INFO_SIZE 65356

const unsigned char *packet_name; // name of highest layer decoded
unsigned char *header_info; // buffer stores header info
unsigned char *dump_info;     // buffer stores data dump
							  
// write position of header_info, dump_info
unsigned char *header_cursor, *dump_cursor;

void alloc_parse_buffer() {
	header_info = malloc(INFO_SIZE);
	dump_info = malloc(INFO_SIZE);
}

