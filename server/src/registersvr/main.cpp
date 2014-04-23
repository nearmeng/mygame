#include <event2/event.h>
#include <event2/util.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <sys/socket.h>
#include "include/glog_helper.h"

void listener_cb(struct evconnlistener* plistener, evutil_socket_t fd, struct sockaddr * addr, int socklen, void * data);
void callback_fn(evutil_socket_t fd, short eventid, void * data);
void data_read_cb(struct bufferevent *bev, void *ctx);
void data_write_cb(struct bufferevent *bev, void *ctx);
void event_cb(struct bufferevent *bev, short what, void *ctx);

int main(int argc, char* argv[])
{
	daemon(1, 1);
	glog_init(argv[0]);

	sockaddr addr;
	int len = sizeof(sockaddr);
	evutil_parse_sockaddr_port("127.0.0.1:5000", &addr, &len);

	event_base* pbase = event_base_new();
	char test[32] = "test data";
	evconnlistener* plistener 
		= evconnlistener_new_bind(pbase, listener_cb, reinterpret_cast<void*>(test), LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 10, &addr, len);

	LOG(INFO) << evconnlistener_get_fd(plistener);

	event_base_dispatch(pbase);

	evconnlistener_free(plistener);
	event_base_free(pbase);

	return 0;
}

void listener_cb(struct evconnlistener* plistener, evutil_socket_t fd, struct sockaddr * addr, int socklen, void * data)
{
	LOG(INFO) << fd << "|" << reinterpret_cast<char*>(data);
	
	struct event_base *pbase = evconnlistener_get_base(plistener);
	struct bufferevent *bev = bufferevent_socket_new(pbase, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, data_read_cb, NULL, event_cb, data);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void callback_fn(evutil_socket_t fd, short eventid, void * data)
{
	LOG(INFO) << fd << "|" << eventid;
}

void data_read_cb(struct bufferevent *bev, void *ctx)
{
	LOG(INFO) << reinterpret_cast<char*>(ctx);
	struct evbuffer *input = bufferevent_get_input(bev);
	struct evbuffer *output = bufferevent_get_output(bev);
	evbuffer_add_buffer(output, input);
}

void data_write_cb(struct bufferevent *bev, void *ctx)
{
	LOG(INFO) << reinterpret_cast<char*>(ctx);
}

void event_cb(struct bufferevent *bev, short what, void *ctx)
{
	LOG(INFO) << reinterpret_cast<char*>(ctx) << "|" << what;
	//BEV_EVENT_CONNECTED
}
