#include <net.h>

/* TODO:
 * implement argument passing and help
 * output redirection with flag -o (use freopen)
 */
int main() {
	int packet_socket = init_socket();
	sniff(packet_socket);
}
