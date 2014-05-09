#include <sys/socket.h>
#include <event2/event.h>
#include <event2/util.h>
#include <string.h>
#include <iostream>
#include "include/protocol_head.h"
#include "proto/loginsvr.pb.h"
#include "src/libredis/libredis.h"
#include "src/libbase/md5.h"
#include "include/glog_helper.h"

#include <string>
using namespace std;


void call_back(evutil_socket_t, short, void *);
int check_passwd(const pb_loginsvr::Login& pblogin);

int main(int argc, char** argv)
{
	daemon(1, 1);
	glog_init(*argv);

	event_config* pconfig = event_config_new();
	//event_config_avoid_method(pconfig, "epoll");
	//event_config_set_flag(pconfig, EVENT_BASE_FLAG_NOLOCK);

	event_base* pbase = event_base_new_with_config(pconfig);

	evutil_socket_t fd = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr sock_addr;
	int sockaddr_len = sizeof(sock_addr);

	evutil_parse_sockaddr_port("127.0.0.1:4999", &sock_addr, &sockaddr_len);
	evutil_make_socket_nonblocking(fd);
//	evutil_make_listen_socket_reuseable(fd);

	bind(fd, &sock_addr, sizeof(sock_addr));

	char test_new[32] = "new data";

	event* pevent = event_new(pbase, fd, EV_READ | EV_PERSIST, call_back, reinterpret_cast<void*>(test_new));
//	event_base_set(pbase, pevent);
	event_add(pevent, NULL);
	event_base_dispatch(pbase);

	event_del(pevent);
	event_base_free(pbase);
	event_config_free(pconfig);
	return 1;
}


void call_back(evutil_socket_t fd, short event_id, void * pdata)
{
	LOG(INFO) << fd << "|" << event_id << reinterpret_cast<char*>(pdata);
	switch (event_id)
	{
	case EV_READ:
	{
					sockaddr addr;
					socklen_t len = sizeof(addr);

					stLogin stlogin;
					recvfrom(fd, &stlogin, sizeof(stlogin), 0, &addr, &len);
					stlogin.head.decode();

					switch (stlogin.head.cmd_id)
					{
					case LG_login:
					{
									 if (stlogin.head.data_len > 0)
									 {
										 pb_loginsvr::Login pblogin;
										 pblogin.ParseFromArray(stlogin.body.data, stlogin.head.data_len);
										 pb_loginsvr::LoginReuslt pbloginresult;
										 pbloginresult.set_result(check_passwd(pblogin));
										 pbloginresult.set_msg("");
										 stlogin.head.cmd_id = LG_login_result;
										 string tmp;
										 pbloginresult.SerializeToString(&tmp);
										 stlogin.head.data_len = tmp.length();
										 memcpy(stlogin.body.data, tmp.c_str(), tmp.length());
										 stlogin.head.encode();
										 sendto(fd, &stlogin, sizeof(stlogin), 0, &addr, len);
									 }
					}
						break;
					default:
						break;
					}
					break;
	}
	default:
		break;
	}
}



int check_passwd(const pb_loginsvr::Login& pblogin)
{
	static CRedisMgr redismgr;
	if (redismgr.InitDb("127.0.0.1", 6379, 1))
	{
		CRedisReply reply;
		CRedisConnetUint* redisuint = redismgr.GetDb(0);
		stringstream ss;
		ss << "GET " << pblogin.name();
		redisuint->RedisSendCmd(ss.str().c_str(), reply);
		switch (reply.GetRedisReply()->integer)
		{
		case REDIS_REPLY_NIL:
			return LG_user_no_register;
			break;
		case REDIS_REPLY_STRING:
		{
								   ss.str().clear();
								   ss << reply.GetRedisReply()->str << pblogin.timestamp();
								   unsigned char* ptmp = reinterpret_cast<unsigned char*>(const_cast<char*>(ss.str().c_str()));
								   string md5 = calc_md5_string(ptmp, ss.str().length());
								   const string& tmp = pblogin.passwd();
								   if (strncasecmp(md5.c_str(), tmp.c_str(), md5.length() > tmp.length() ? tmp.length() : md5.length()) == 0)
								   {
									   return LG_login_suc;
								   }
								   else
								   {
									   return LG_passwd_err;
								   }
		}
			break;
		default:
			break;
		}
	}

	return LG_errorid_max;
}