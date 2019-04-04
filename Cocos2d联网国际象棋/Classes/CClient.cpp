#include "CClient.h"

CClient* CClient::instance = nullptr;

CClient::CClient()
{
	client = new ODSocket();
	buf = new Buffer();
	client->Init();
	CCDirector::sharedDirector()->getScheduler()
		->scheduleSelector(schedule_selector(CClient::update), this, 0.1f, false);
}

CClient::~CClient()
{
}

CClient * CClient::getInstance()
{
	if (instance == nullptr) {
		instance = new CClient();
	}
	return instance;
}

void CClient::Connect(const char * ip, unsigned short port)
{
	client->Create(AF_INET, SOCK_STREAM, 0);

	isConnect = false;
	while (!isConnect) {
		isConnect = client->Connect(ip, port);
		if (!isConnect) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			Sleep(2);
#else
			usleep(2);
#endif
		}
	}
	recvThr = thread(&CClient::RecvData, this);
	recvThr.detach();
}

void CClient::Quit()
{
	memset(recvBuf, '\0', sizeof(recvBuf));
	memset(sendBuf, '\0', sizeof(sendBuf));
	//free(recvBuf);
	//free(sendBuf);
	client->Close();
	
}

void CClient::RecvData()
{
	while (true)
	{
		m_pMsg.clear();
		memset(recvBuf, '\0', sizeof(recvBuf));
		int ret = client->Recv(recvBuf, sizeof(recvBuf), 0);
		if (ret <= 0) {
			printf("receive %d, exit.\n", ret);
			break;
		}
		if (buf->buffer_add(recvBuf, ret) == -1) {
			printf("error, too large packet!\n");
			break;
		}

		int n = 0;
		while (1) {
			char *msg = buf->parse_packet();
			if (!msg) {
				break;
			}
			m_pMsg = msg;
			n++;
			if (n > 1) {
				//m_pMsg += msg;
				printf("    [Merged Packet]\n");
			}
			free(msg);
		}
		_msgQueue.push_back(m_pMsg);
		RespondFromServerMsg(_msgQueue[0]);
		buf->buf->size = 0;
		memset(buf->buf->data, '\0', sizeof(buf->buf->data));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		Sleep(20);
#else
		usleep(20);
#endif
	}
}

void CClient::SendData(const char *msg)
{
	memset(sendBuf, '\0', sizeof(sendBuf));
	char *p = buf->encode_packet(msg);
	strcpy(sendBuf, p);
	int len = sizeof(sendBuf);
	client->Send(sendBuf, len, 0);
}

void CClient::update(float dt)
{
	if (_msgQueue.size() > 0) {
		
		switch (msg_recv_type)
		{
		case SUB_C_LOGIN_REQ:
			__NotificationCenter::getInstance()->postNotification(SUB_C_LOGIN_REQ_NOTIFICATION);
			break;
		case SUB_C_REGISTER_REQ:
			__NotificationCenter::getInstance()->postNotification(SUB_C_REGISTER_REQ_NOTIFICATION);
			break;
		case SUB_C_CREAT_ROOM:
			__NotificationCenter::getInstance()->postNotification(SUB_C_CREAT_ROOM_NOTIFICATION);
			break;
		case SUB_C_START_MATCH:
			__NotificationCenter::getInstance()->postNotification(SUB_C_START_MATCH_NOTIFICATION);
			break;
		case SUB_C_DATA_LOAD_SELF:
			__NotificationCenter::getInstance()->postNotification(SUB_C_DATA_LOAD_SELF_NOTIFICATION);
			break;
		case SUB_C_DATA_LOAD_RIVAL:
			__NotificationCenter::getInstance()->postNotification(SUB_C_DATA_LOAD_RIVAL_NOTIFICATION);
			break;
		case SUB_C_MODIF_INFO:
			__NotificationCenter::getInstance()->postNotification(SUB_C_MODIF_INFO_NOTIFICATION);
			break;
		case SUB_C_QUIT_ROOM:
			__NotificationCenter::getInstance()->postNotification(SUB_C_QUIT_ROOM_NOTIFICATION);
			break;
		case SUB_S_GAME_START:
			__NotificationCenter::getInstance()->postNotification(SUB_S_GAME_START_NOTIFICATION);
			break;
		case SUB_S_MOVE_CHESS:
			__NotificationCenter::getInstance()->postNotification(SUB_S_MOVE_CHESS_RIVAL_NOTIFICATION);
			break;
		case SUB_C_MOVE_CHESS:
			__NotificationCenter::getInstance()->postNotification(SUB_S_MOVE_CHESS_SELF_NOTIFICATION);
			break;
		case SUB_S_GAME_END:
			__NotificationCenter::getInstance()->postNotification(SUB_S_GAME_END_NOTIFICATION);
			break;
		case SUB_C_PEACE_REQ:
			__NotificationCenter::getInstance()->postNotification(SUB_S_PEACE_REQ_NOTIFICATION);
			break;
		case SUB_C_PEACE_ANSWER:
			__NotificationCenter::getInstance()->postNotification(SUB_S_PEACE_ANSWER_NOTIFICATION);
			break;
		case SUB_C_READ_RANKING:
			__NotificationCenter::getInstance()->postNotification(SUB_C_READ_RANKINGLIST_NOTIFICATION);
			break;
		default:
			break;
		}
		_msgQueue.clear();
	}
}

const char* CClient::MsgCreateLoginOrRegister(const char name[], const char passwd[])
{
	Document doc;
	doc.SetObject();
	Document::AllocatorType& alloctor = doc.GetAllocator();
	doc.AddMember("msgType", msg_send_type, alloctor);
	rapidjson::Value obj(kObjectType);
	rapidjson::Value value_name(name, alloctor);
	rapidjson::Value value_passwd(passwd, alloctor);
	obj.AddMember("userName", value_name, alloctor);
	obj.AddMember("passworld", value_passwd, alloctor);
	doc.AddMember("CMD_C_LOGIN", obj, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	return buffer.GetString();
}

const char * CClient::MsgCreateLogin(const char name[], const char passwd[])
{
	msg_send_type = SUB_C_LOGIN_REQ;
	return MsgCreateLoginOrRegister(name, passwd);
}

const char * CClient::MsgCreateRegister(const char name[], const char passwd[])
{
	msg_send_type = SUB_C_REGISTER_REQ;
	return MsgCreateLoginOrRegister(name, passwd);

}

const char * CClient::MsgCreateRoom()
{
	msg_send_type = SUB_C_CREAT_ROOM;
	return CreateSimplenesMsgHead();
}

const char * CClient::MsgCreateQuitRoom()
{
	msg_send_type = SUB_C_QUIT_ROOM;
	return CreateSimplenesMsgHead();
}

const char * CClient::MsgCreateMatch()
{
	msg_send_type = SUB_C_START_MATCH;
	return CreateSimplenesMsgHead();
}

const char * CClient::MsgCreateReadUserInfo()
{
	msg_send_type = SUB_C_DATA_LOAD_SELF;
	return CreateSimplenesMsgHead();
}

const char * CClient::MsgCreateReadRivalInfo()
{
	msg_send_type = SUB_C_DATA_LOAD_RIVAL;
	return CreateSimplenesMsgHead();
}

const char * CClient::MsgCreateReady()
{
	msg_send_type = SUB_C_READY;
	return CreateSimplenesMsgHead();
}

const char * CClient::MsgCreatePeace()
{
	msg_send_type = SUB_C_PEACE_REQ;
	return CreateSimplenesMsgHead();
}

const char * CClient::MsgCreateGiveUp()
{
	msg_send_type = SUB_C_GIVEUP_REQ;
	return CreateSimplenesMsgHead();
}

const char * CClient::MsgCreateReadRankingList()
{
	msg_send_type = SUB_C_READ_RANKING;
	return CreateSimplenesMsgHead();
}

const char * CClient::MsgCreateReadRoomList()
{
	msg_send_type = SUB_C_READ_ROOM_LIST;
	return CreateSimplenesMsgHead();
}

const char* CClient::CreateSimplenesMsgHead()
{
	Document doc;
	doc.SetObject();
	Document::AllocatorType& alloctor = doc.GetAllocator();
	doc.AddMember("msgType", msg_send_type, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	return buffer.GetString();
}

const char * CClient::MsgCreatePeaceAnswer(CMD_C_PeaceAnswer data)
{
	msg_send_type = SUB_C_PEACE_ANSWER;

	Document doc;
	doc.SetObject();
	Document::AllocatorType& alloctor = doc.GetAllocator();
	doc.AddMember("msgType", msg_send_type, alloctor);
	rapidjson::Value obj(kObjectType);
	obj.AddMember("cbApprove", data.cbApprove, alloctor);
	doc.AddMember("CMD_C_PeaceAnswer", obj, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	return buffer.GetString();
}

const char * CClient::MsgCreateMoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess)
{
	//CMD_S_MoveChess data;
	CMD_C_MoveChess data;
	data.xSourcePos = xSourcePos;
	data.ySourcePos = ySourcePos;
	data.xTargetPos = xTargetPos;
	data.yTargetPos = yTargetPos;
	data.switchChess = switchChess;
	//data.currentUser = currUser;

	msg_send_type = SUB_C_MOVE_CHESS;
	Document _doc;
	_doc.SetObject();
	Document::AllocatorType& alloctor = _doc.GetAllocator();
	_doc.AddMember("msgType", msg_send_type, alloctor);
	rapidjson::Value obj(kObjectType);
	obj.AddMember("xSourcePos", data.xSourcePos, alloctor);
	obj.AddMember("ySourcePos", data.ySourcePos, alloctor);
	obj.AddMember("xTargetPos", data.xTargetPos, alloctor);
	obj.AddMember("yTargetPos", data.yTargetPos, alloctor);
	obj.AddMember("switchChess", data.switchChess, alloctor);
	//obj.AddMember("currentUser", data.currentUser, alloctor);
	_doc.AddMember("CMD_C_MoveChess", obj, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_doc.Accept(writer);
	return buffer.GetString();
}

const char * CClient::MsgCreateModifUserInfo(CMD_C_MODIFINFO data)
{
	msg_send_type = SUB_C_MODIF_INFO;
	Document _doc;
	_doc.SetObject();
	Document::AllocatorType& alloctor = _doc.GetAllocator();
	_doc.AddMember("msgType", msg_send_type, alloctor);
	rapidjson::Value obj(kObjectType);
	rapidjson::Value value_sex(data.sex, alloctor);
	rapidjson::Value value_sign(data.signature, alloctor);
	rapidjson::Value value_name(data.name, alloctor);
	obj.AddMember("sex", value_sex, alloctor);
	obj.AddMember("name", value_name, alloctor);
	obj.AddMember("signature", value_sign, alloctor);
	obj.AddMember("age", data.age, alloctor);
	obj.AddMember("avta_index", data.avta_index, alloctor);
	_doc.AddMember("CMD_C_MODIFINFO", obj, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_doc.Accept(writer);
	return buffer.GetString();
}

void CClient::RespondFromServerMsg(string msg)
{
	Document doc;
	doc.Parse<0>(msg.c_str());
	if (doc.GetParseError()) {
		cout << "[" << doc.GetParseError() << "]" << endl;
	}
	rapidjson::Value &msgType = doc["msgType"];
	rapidjson::Value &ret = doc["ret"];
	
	if (msgType.IsInt()) {
		msg_recv_type = msgType.GetInt();
	}
	if (ret.IsInt()) {
		respond = ret.GetInt();
	}
	/*if (msg_recv_type == SUB_C_LOGIN_REQ) {
		rapidjson::Value &Vavta = doc["avta_index"];
		__avta_index = Vavta.GetInt();
	}*/
	//return respond;
}

void CClient::AnalysisMsgHead(string msg)
{
	Document doc;
	doc.Parse<0>(msg.c_str());
	if (doc.GetParseError()) {
		//printError(doc.GetParseError());
		std::cout << doc.GetParseError() << std::endl;
	}
	rapidjson::Value &v = doc["msgType"];
	if (v.IsInt()) {
		msg_recv_type = v.GetInt();
	}
}

CMD_C_DATALOAD CClient::MsgReadUserData(string msg)
{
	CMD_C_DATALOAD data;
	Document doc;
	doc.Parse<0>(msg.c_str());
	if (doc.GetParseError()) {
		cout << "[" << doc.GetParseError() << "]" << endl;
	}
	rapidjson::Value &obj = doc["CMD_C_DATALOAD"];
	rapidjson::Value &vSex = obj["sex"];
	rapidjson::Value &vSign = obj["signature"];
	rapidjson::Value &vName = obj["name"];
	rapidjson::Value &vAge = obj["age"];
	rapidjson::Value &vAvta = obj["avta_index"];
	rapidjson::Value &vw1 = obj["W_TotalNum"];
	rapidjson::Value &vw2 = obj["W_TotalWinNum"];
	rapidjson::Value &vb1 = obj["B_TotalNum"];
	rapidjson::Value &vb2 = obj["B_TotalWinNum"];
	
	const char* sex = vSex.GetString();
	const char* signature = vSign.GetString();
	const char* name = vName.GetString();

	strcpy(data.sex, sex);
	strcpy(data.signature, signature);
	strcpy(data.name, name);
	data.age = vAge.GetInt();
	data.avta_index = vAvta.GetInt();
	data.W_TotalNum = vw1.GetInt();
	data.W_TotalWinNum = vw2.GetInt();
	data.B_TotalNum = vb1.GetInt();
	data.B_TotalWinNum = vb2.GetInt();

	__avta_index = data.avta_index;
	__totalPlayNum = data.W_TotalNum + data.B_TotalNum;
	__totalWinNum = data.W_TotalWinNum + data.B_TotalWinNum;
	__totalLoseNum = (__totalPlayNum) - __totalWinNum;
	return data;
}

CMD_S_MoveChess CClient::MsgMoveChess(string msg)
{
	CMD_S_MoveChess data;
	Document doc;
	doc.Parse<0>(msg.c_str());
	if (doc.GetParseError()) {
		cout << "[" << doc.GetParseError() << "]" << endl;
	}
	rapidjson::Value &obj = doc["CMD_S_MoveChess"];
	rapidjson::Value &_vsx = obj["xSourcePos"];
	rapidjson::Value &_vsy = obj["ySourcePos"];
	rapidjson::Value &_vtx = obj["xTargetPos"];
	rapidjson::Value &_vty = obj["yTargetPos"];
	rapidjson::Value &_vswitch = obj["switchChess"];
	rapidjson::Value &_vcurruser = obj["currentUser"];

	data.xSourcePos = _vsx.GetInt();
	data.ySourcePos = _vsy.GetInt();
	data.xTargetPos = _vtx.GetInt();
	data.yTargetPos = _vty.GetInt();
	data.switchChess = _vswitch.GetInt();
	data.currentUser = _vcurruser.GetInt();

	return data;
}

CMD_S_GameEnd CClient::MsgGameOver(string msg)
{
	CMD_S_GameEnd data;
	Document doc;
	doc.Parse<0>(msg.c_str());
	if (doc.GetParseError()) {
		cout << "[" << doc.GetParseError() << "]" << endl;
	}
	rapidjson::Value &obj = doc["CMD_S_GameEnd"];
	rapidjson::Value &_vwinuser = obj["wWinUser"];
	data.wWinUser = _vwinuser.GetInt();
	return data;
}

CMD_S_RankingList CClient::MsgReadRankingList(string msg)
{
	CMD_S_RankingList data;
	Document doc;
	doc.Parse<0>(msg.c_str());
	if (doc.GetParseError()) {
		cout << "[" << doc.GetParseError() << "]" << endl;
	}
	rapidjson::Value &obj = doc["CMD_S_RankingList"];
	rapidjson::Value &vname0 = obj["name0"];
	rapidjson::Value &vname1 = obj["name1"];
	rapidjson::Value &vname2 = obj["name2"];
	rapidjson::Value &vname3 = obj["name3"];
	rapidjson::Value &vname4 = obj["name4"];

	const char* name0 = vname0.GetString();
	const char* name1 = vname1.GetString();
	const char* name2 = vname2.GetString();
	const char* name3 = vname3.GetString();
	const char* name4 = vname4.GetString();

	strcpy(data.name0, name0);
	strcpy(data.name1, name1);
	strcpy(data.name2, name2);
	strcpy(data.name3, name3);
	strcpy(data.name4, name4);

	rapidjson::Value &vwinNum0 = obj["winNum0"];
	rapidjson::Value &vwinNum1 = obj["winNum1"];
	rapidjson::Value &vwinNum2 = obj["winNum2"];
	rapidjson::Value &vwinNum3 = obj["winNum3"];
	rapidjson::Value &vwinNum4 = obj["winNum4"];

	data.winNum0 = vwinNum0.GetInt();
	data.winNum1 = vwinNum1.GetInt();
	data.winNum2 = vwinNum2.GetInt();
	data.winNum3 = vwinNum3.GetInt();
	data.winNum4 = vwinNum4.GetInt();

	rapidjson::Value &vavta_index0 = obj["avta_index0"];
	rapidjson::Value &vavta_index1 = obj["avta_index1"];
	rapidjson::Value &vavta_index2 = obj["avta_index2"];
	rapidjson::Value &vavta_index3 = obj["avta_index3"];
	rapidjson::Value &vavta_index4 = obj["avta_index4"];

	data.avta_index0 = vavta_index0.GetInt();
	data.avta_index1 = vavta_index1.GetInt();
	data.avta_index2 = vavta_index2.GetInt();
	data.avta_index3 = vavta_index3.GetInt();
	data.avta_index4 = vavta_index4.GetInt();

	return data;
}
