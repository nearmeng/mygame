#include "libredis.h"
#include <glog/logging.h>


CRedisConnetUint* CRedisMgr::GetDb(int index)
{
	MAP_CREDISCONNETUINT::iterator it = m_map_redis_db_conn.find(index);
	if (it != m_map_redis_db_conn.end())
	{
		return &(it->second);
	}
	else
	{
		return NULL;
	}
}

bool CRedisMgr::InitDb(const char* ip, int port, int max_index)
{
	for (int index = 0; index < max_index; ++index)
	{
		MAP_CREDISCONNETUINT::iterator it = m_map_redis_db_conn.find(index);
		if (it == m_map_redis_db_conn.end())
		{
			DLOG(INFO) << ip << "|" << port << "|" << index;

			if (!m_map_redis_db_conn[index].RedisConnect(ip, port, index))
			{
				LOG(ERROR) << "connect error " << ip << ':' << port;
				return false;
			}
		}
	}

	return true;
}

CRedisMgr::CRedisMgr()
{
}

CRedisMgr::~CRedisMgr()
{
}

CRedisReply::CRedisReply()
{
	m_redis_reply = NULL;
}

CRedisReply::~CRedisReply()
{
	FreeRedisReply();
}

void CRedisReply::SetRedisReply(redisReply* reply)
{
	FreeRedisReply();
	m_redis_reply = reply;
}

void CRedisReply::FreeRedisReply()
{
	if (m_redis_reply != NULL)
	{
		freeReplyObject(m_redis_reply);
		m_redis_reply = NULL;
	}
}

const redisReply* CRedisReply::GetRedisReply() const
{
	return m_redis_reply;
}


CRedisCmdParam::CRedisCmdParam()
{
}

CRedisCmdParam::~CRedisCmdParam()
{
}

void CRedisCmdParam::AddParam(const char* param, const size_t param_len)
{
	m_vec_param.push_back(std::string(param, param_len));
}

void CRedisCmdParam::GetParam(char ** param, size_t* param_len) const
{
	for (size_t i = 0; i < m_vec_param.size(); ++i)
	{
		memcpy(*(param + i), m_vec_param[i].c_str(), m_vec_param[i].size());
		*(param_len + i) = m_vec_param[i].size();
	}
}

size_t CRedisCmdParam::GetMaxParamLen() const
{
	size_t max_len = 0;
	for (VEC_REDIS_CMD_PARAM::const_iterator it = m_vec_param.begin(); it != m_vec_param.end(); ++it)
	{
		max_len = (max_len >= it->size() ? max_len : it->size());
	}
	return max_len;
}

CRedisConnetUint::CRedisConnetUint()
{
	m_redis_context = NULL;
}

CRedisConnetUint::~CRedisConnetUint()
{
	RedisCloseConnect();
}

bool CRedisConnetUint::RedisConnect(const char* ip, int port)
{
	m_redis_context = redisConnect(ip, port);
	if (m_redis_context != NULL && m_redis_context->err)
	{
		LOG(ERROR) << m_redis_context->err << '|' << m_redis_context->errstr << '|'
			<< ip << '|' << port;
		return false;
	}

	return true;
}

bool CRedisConnetUint::RedisSendCmd(const char* cmd, CRedisReply& redis_reply)
{
	void* reply = redisCommand(m_redis_context, cmd);
	if (reply == NULL)
	{
		LOG(ERROR) << "reply is NULL";

		RedisCloseConnect();
		RedisConnect(m_redis_server_ip.c_str(), m_redis_port, m_db_index);

		return false;
	}

	redisReply* presult = reinterpret_cast<redisReply*>(reply);

	redis_reply.SetRedisReply(presult);

	return reply != NULL;
}

bool CRedisConnetUint::RedisSendMultiCmd(const CRedisCmdParam& redis_cmd_param, CRedisReply& redis_reply)
{
	size_t param_size = redis_cmd_param.GetParamSize();
	if (param_size > 0)
	{
		size_t max_param_len = redis_cmd_param.GetMaxParamLen();

		char szparam[param_size][max_param_len];
		size_t paramlen[param_size];

		redis_cmd_param.GetParam(reinterpret_cast<char**>(szparam), reinterpret_cast<size_t*>(paramlen));

		void* reply = redisCommandArgv(m_redis_context, param_size, reinterpret_cast<const char**>(szparam), reinterpret_cast<const size_t*>(paramlen));
		if (reply == NULL)
		{
			LOG(ERROR) << "reply is NULL";
			return false;
		}

		redisReply* presult = reinterpret_cast<redisReply*>(reply);

		redis_reply.SetRedisReply(presult);

		return reply != NULL;
	}

	return false;
}

void CRedisConnetUint::RedisCloseConnect()
{
	if (m_redis_context != NULL)
	{
		redisFree(m_redis_context);
		m_redis_context = NULL;
	}
}

bool CRedisConnetUint::RedisConnect(const char* ip, int port, int db_index)
{
	if (RedisConnect(ip, port))
	{
		m_redis_server_ip = ip;
		m_redis_port = port;

		CRedisReply select_reply;
		char szcmd[128];
		snprintf(szcmd, sizeof(szcmd), "SELECT %d", db_index);
		if (RedisSendCmd(szcmd, select_reply))
		{
			DLOG(INFO) << select_reply.GetRedisReply()->type;
			if (select_reply.GetRedisReply()->type == REDIS_REPLY_STATUS)
			{
				if (strcasecmp(select_reply.GetRedisReply()->str, "OK") == 0)
				{
					m_db_index = db_index;
					return true;
				}
			}
		}
	}

	return false;
}

