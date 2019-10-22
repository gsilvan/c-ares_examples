#include <ares.h>

static void ares_wait(ares_channel channel){
	int nfds;
	fd_set readers, writers;
	struct timeval tv, *tvp;
	 
	while (1) {
		FD_ZERO(&readers);
		FD_ZERO(&writers);
		nfds = ares_fds(channel, &readers, &writers);
		if (nfds == 0)
			break;
		tvp = ares_timeout(channel, NULL, &tv);
		select(nfds, &readers, &writers, NULL, tvp);
		ares_process(channel, &readers, &writers);
	}
}
