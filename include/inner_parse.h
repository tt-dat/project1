#include <parse_buffer.h>

#define MAX_PRINT_SIZE 1600

/* interface to parse.c */
// shared data dump function
void dump_data(unsigned char*, size_t);

// wrapper of snprintf
void dump_string(const char*);
void print_header_string(const char*);

// different layer's parse logic
void parse_link_frame(unsigned char*, size_t);
void parse_network_datagram(unsigned char*, size_t, uint16_t);
void parse_transport_segment(unsigned char*, size_t, uint16_t);

// header print function for each protocol
void print_ether_header(struct ether_header*);
void print_ip_header(struct iphdr*);
void print_ip6_header(struct ip6_hdr*);
void print_tcp_header(struct tcphdr*);
	


