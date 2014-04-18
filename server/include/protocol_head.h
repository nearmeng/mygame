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

enum emLoginsvrCmdID
{
	LG_login = 1,
};

#endif

