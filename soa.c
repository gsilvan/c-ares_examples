#include <stdio.h>
#include <ares.h>
#include <arpa/nameser.h>
#include "util.c"

static void cb_soa(void *arg, int status, int timeouts, unsigned char *abuf, int alen){
	if (status != ARES_SUCCESS){
		printf("%s\n", ares_strerror(status));
        return;
	}
    struct ares_soa_reply *soa;
    status = ares_parse_soa_reply(abuf, alen, &soa);
    if (status != ARES_SUCCESS){
        printf("%s\n", ares_strerror(status));
        return;
    }
    printf("%s %s %d %d %d %d %d\n", 
        soa->nsname, soa->hostmaster, soa->serial, soa->refresh, soa->retry, soa->expire, soa->minttl);
    ares_free_data(soa);
}

int main(int argc, char *argv[]){
    if (ares_library_init(ARES_LIB_INIT_ALL)){
        return -1;
    }
    ares_channel channel;
	if (ares_init(&channel) != ARES_SUCCESS){
        return -1;
	}
	ares_query(channel, argv[1], ns_c_in, ns_t_soa, cb_soa, NULL);
	ares_wait(channel);
    ares_destroy(channel);
	ares_library_cleanup();
}
