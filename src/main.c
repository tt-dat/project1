#include <net.h>

/* TODO:
 * implement argument passing and help
 */
int main() {
	int packet_socket = init_socket();

	sniff_and_display(packet_socket);
}
