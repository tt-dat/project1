#include <libc.h>
#include <parse_helpers.h>


// TODO: move dump functions to other files
void dump_data(unsigned char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		// first element of line
		if (i % 16 == 0) { 
			if (i != 0) { dump_string("\n"); }

			dump_string("    "); 
		} 


		dump_cursor += snprintf(dump_cursor, MAX_PRINT_SIZE,  "%02X ", data[i]);

		// last element of line or last line
		// then print the character representation of data
		if (i % 16 == 15 || i == size - 1) {
			// align with upper lines
			int pad_size = (15 - i%16)*3 + 1;
			unsigned char pad[pad_size];
			memset(pad, ' ', sizeof(pad));
			pad[pad_size - 1] = '\0';
			
			dump_cursor += snprintf(dump_cursor, MAX_PRINT_SIZE,  "%s\t", pad);

			int start = (i >> 4) << 4;
			for (size_t j = start; j <= i; j++) {
				unsigned char c = (data[j] >= 32 && data[j] <= 127)? data[j] : '.';
				dump_cursor += snprintf(dump_cursor, MAX_PRINT_SIZE,  "%c", c);
			}

		}
	}

	dump_string("\n");
}


void dump_string(const char* string) {
	dump_cursor += snprintf(dump_cursor, MAX_PRINT_SIZE, string);
}

void print_header_string(const char* string) {
	header_cursor += snprintf(header_cursor, MAX_PRINT_SIZE, string);
}


