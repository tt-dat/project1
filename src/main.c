#include <net.h>

int main() {
	int packet_socket = init_socket();

	sniff_and_display(packet_socket);
}
