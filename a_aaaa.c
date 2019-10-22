#include <stdio.h>
#include <ares.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "util.c"

static void callback(void *arg, int status, int timeouts, struct hostent *host){
    if(status != ARES_SUCCESS){
        printf("%s\n", ares_strerror(status));
        return;
    }
	char ip[INET6_ADDRSTRLEN];
	int i = 0;
	for (i = 0; host->h_addr_list[i]; i++){
		inet_ntop(host->h_addrtype, host->h_addr_list[i], ip, sizeof(ip));
		printf("%s %s\n", host->h_name, ip);
	}
}

int main(int argc, char *argv[]){
    if (ares_library_init(ARES_LIB_INIT_ALL)){
        return -1;
    }
    ares_channel channel;
	if (ares_init(&channel) != ARES_SUCCESS){
        return -1;
	}
	ares_gethostbyname(channel, argv[1], AF_INET, callback, NULL);
	ares_gethostbyname(channel, argv[1], AF_INET6, callback, NULL);
	ares_wait(channel);
    ares_destroy(channel);
	ares_library_cleanup();
}
