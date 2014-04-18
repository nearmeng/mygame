#ifndef __LIBREDIS__
#define __LIBREDIS__


#include <map>
#include <string>
#include <vector>
using namespace std;

#include <hiredis/hiredis.h>

class CRedisReply
{
public:
	CRedisReply();

	~CRedisReply();

	void SetRedisReply(redisReply* reply);

	const redisReply* GetRedisReply() const;

private:
	redisReply* m_redis_reply;

	void FreeRedisReply();
};

class CRedisCmdParam
{
public:

	typedef std::vector<std::string> VEC_REDIS_CMD_PARAM;

	CRedisCmdParam();
	~CRedisCmdParam();

	void AddParam(const char* param, const size_t param_len);

	void GetParam(char ** param, size_t* param_len) const;

	size_t GetParamSize() const
	{
		return m_vec_param.size();
	}

	size_t GetMaxParamLen() const;

private:
	VEC_REDIS_CMD_PARAM m_vec_param;
};

class CRedisConnetUint
{
public:
	CRedisConnetUint();
	~CRedisConnetUint();

	bool RedisConnect(const char* ip, int port, int db_index);

	bool  RedisSendCmd(const char* cmd, CRedisReply& redis_reply);

	bool RedisSendMultiCmd(const CRedisCmdParam& redis_cmd_param, CRedisReply& redis_reply);

	void RedisCloseConnect();

private:
	bool RedisConnect(const char* ip, int port);

	redisContext* m_redis_context;
	std::string m_redis_server_ip;
	int m_redis_port;
	int m_db_index;
};



class CRedisMgr
{
public:
	CRedisConnetUint* GetDb(int index);

	bool InitDb(const char* ip, int port, int max_index);

	CRedisMgr();

	~CRedisMgr();
private:


	std::map<int, CRedisConnetUint> m_map_redis_db_conn;

	typedef std::map<int, CRedisConnetUint> MAP_CREDISCONNETUINT;
};

#endif





#endif
