#include <libc.h>
#include <net.h>
#include <init_socket.h>
#include <sniff.h>

#define NOBODY 65534

int packet_count = 0;
int packet_dropped = 0;
int packet_logged = 0;

/* TODO: * implement argument passing and help * output redirection with flag -o (use freopen)
 */
int main() {
	int packet_socket = init_socket();

	// drop privilege
	//int u_st = setuid(NOBODY);
	// int g_st = setgid(NOBODY);

	// if (u_st < 0 || g_st < 0) {
	// 	perror("set ID");
	// 	exit(1);
	// }
	
	sniff(packet_socket);
}
