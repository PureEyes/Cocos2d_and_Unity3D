#pragma once
class ProcessMessage
{
public:
	ProcessMessage();
	~ProcessMessage();
	static void MsgEncode();
	static char* MsgDecode(char* msg);
};

