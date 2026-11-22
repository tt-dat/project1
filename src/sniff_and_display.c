#include <net.h>
#include <stdio.h>

#define BUF_SIZE 65536

void sniff_and_display(int packet_socket) {
	char buf[BUF_SIZE];

	while (1) {
		int byte_reads = recv(packet_socket, buf, BUF_SIZE - 1, 0);
		if (byte_read == -1) { perror("recv error"); }
		buf[byte_reads] = 0;
		puts(buf);
	}
}
