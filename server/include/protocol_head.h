#ifndef __PROTOCOL_HEAD__
#define __PROTOCOL_HEAD__

#include <stdint.h>

struct stProtocolHead
{
	uint8_t cmd_id;
	uint8_t data_len;
	stProtocolHead()
	{
		cmd_id = 0;
		data_len = 0;
	}

	void encode()
	{
		cmd_id = htons(cmd_id);
		data_len = htons(data_len);
	}

	void decode()
	{
		cmd_id = ntohs(cmd_id);
		data_len = ntohs(data_len);
	}
};

struct stProtocolBody
{
	char data[256];

	stProtocolBody()
	{
		memset(data, 0, sizeof(data));
	}
};

struct stLogin
{
	stProtocolHead head;
	stProtocolBody body;
};

enum emLoginsvrCmdID
{
	LG_login = 1,
	LG_login_result = 2,
};

enum emLoginsvrErrorID
{
	LG_login_suc = 0,
	LG_user_no_register = 1,
	LG_passwd_err = 2,
	LG_errorid_max
};

#endif

