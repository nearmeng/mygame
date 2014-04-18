#ifndef __PROTOCOL_HEAD__
#define __PROTOCOL_HEAD__
struct stProtocolHead
{
	uint8 cmd_id;
	uint8 data_len;
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

