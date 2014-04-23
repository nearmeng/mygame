#include <event2/event.h>
#include <event2/util.h>
#include <event2/listener.h>
#include <sys/socket.h>
#include "glog_init.h"

void listener_cb(struct evconnlistener* plistener, int fd, struct sockaddr * addr, int socklen, void * data);

int main(int argc, char* argv[])
{
	daemon(1, 1);
	glog_init(argv[0]);

	sockaddr addr;
	int len = sizeof(sockaddr);
	evutil_parse_sockaddr_port("127.0.0.1:5000", &addr, &len);

	event_base* pbase = event_base_new();
	evconnlistener* plistener 
		= evconnlistener_new_bind(pbase, listener_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 10, &addr, len);

	event_base_dispatch(pbase);

	evconnlistener_free(plistener);
	event_base_free(pbase);

	return 0;
}

void listener_cb(struct evconnlistener* plistener, evutil_socket_t fd, struct sockaddr * addr, int socklen, void * data)
{
	LOG(INFO) << fd;
}

