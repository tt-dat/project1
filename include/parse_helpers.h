#include <parse_buffer.h>

#define MAX_PRINT_SIZE 1600


// wrapper of snprintf
void dump_string(const char*);
void print_header_string(const char*);

void dump_data(unsigned char*, size_t);
