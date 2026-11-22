#include <sys/socket.h> // for using socket(), send(), recv(), ...
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h> // for using struct sockaddr_ll, struct packet_mreq
#include <net/if.h>


int init_socket();
void sniff_and_display(int);
