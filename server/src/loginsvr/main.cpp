#include <sys/socket.h>
#include <event2/event.h>
#include <string.h>
#include <iostream>
#include <glog/logging.h>
#include "include/protocol_head.h"
#include "proto/loginsvr.pb.h"
#include "src/libredis/libredis.h"
#include "src/libbase/md5.h"
#include <string>

using namespace std;


void call_back(evutil_socket_t, short, void *);
void glog_init(const char* argv0);
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
	sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(4999);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	//evutil_inet_pton(AF_INET, "127.0.0.1", &sock_addr.sin_addr);

	bind(fd, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr));

	char test_new[32] = "new data";

	event* pevent = event_new(pbase, fd, EV_READ | EV_PERSIST, call_back, reinterpret_cast<void*>(test_new));
	event_base_set(pbase, pevent);
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
					stLogin stlogin;
					recv(fd, &stlogin, sizeof(stlogin), 0);
					stlogin.head.decode();

					switch (stlogin.head.cmd_id)
					{
					case LG_login:
					{
									 if (stlogin.head.data_len > 0)
									 {
										 pb_loginsvr::Login pblogin;
										 pblogin.ParseFromArray(stlogin.body.data, stlogin.head.data_len);
										 LOG(INFO) << check_passwd(pblogin);
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

void glog_init(const char* argv0)
{
	//FLAGS_log_dir = log_path;//设置log输出目录
	FLAGS_stderrthreshold = google::ERROR;//高于等于ERROR级别的才会输出到控制台
	FLAGS_stop_logging_if_full_disk = 1; //磁盘没空间了停止写log
	FLAGS_logbufsecs = 0;//有log就会写道磁盘上面，不做缓存
	google::InitGoogleLogging(argv0);
}

int check_passwd(const pb_loginsvr::Login& pblogin)
{
	static CRedisMgr redismgr;
	if (redismgr.InitDb("127.0.0.1", 6379, 1))
	{
		CRedisReply reply;
		CRedisConnetUint* redisuint = redismgr.GetDb(0);
		char sztmp[256];
		const string& tmpname = pblogin.name();
		snprintf(sztmp, sizeof(sztmp), "GET %s", tmpname.c_str());
		redisuint->RedisSendCmd(sztmp, reply);
		switch (reply.GetRedisReply()->integer)
		{
		case REDIS_REPLY_NIL:
			return LG_user_no_register;
			break;
		case REDIS_REPLY_STRING:
		{
								   snprintf(sztmp, sizeof(sztmp), "%s|%"PRIu64"", reply.GetRedisReply()->str, pblogin.timestamp());
								   string md5 = calc_md5_string(reinterpret_cast<unsigned char*>(sztmp), strlen(sztmp));
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

	return LG_errorID_max;
}