#include <sys/socket.h>
#include <event2/event.h>
#include <string.h>
#include <iostream>
using namespace std;


void call_back(evutil_socket_t, short, void *);


int main(int argc, char** argv)
{
	event_config* pconfig = event_config_new();
	event_config_avoid_method(pconfig, "epoll");
	event_config_set_flag(pconfig, EVENT_BASE_FLAG_NOLOCK);
	event_base* pbase = event_base_new_with_config(pconfig);

	evutil_socket_t fd = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(4999);
	evutil_inet_pton(AF_INET, "127.0.0.1", &sock_addr.sin_addr);

	bind(fd, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr));

	event* pevent = event_new(pbase, fd, EV_READ | EV_PERSIST, call_back, NULL);
	event_add(pevent, NULL);
	event_base_dispatch(pbase);

	event_del(pevent);
	event_base_free(pbase);
	event_config_free(pconfig);
	return 1;
}


void call_back(evutil_socket_t fd, short event_id, void * pdata)
{
	cout << fd 
		<< "|"
		<< event_id
		<< endl;
}