#include "stdafx.h"
#include "CClient.h"
#include "RoomManage.h"
#include "CClientManage.h"


CClient::CClient(SOCKET m_sock)
{
	_sClient = m_sock;
	buf = new Buffer();
}

CClient::~CClient()
{
	buf->~Buffer();
	delete buf;
	buf = nullptr;
}

int CClient::SendData(const char * msg)
{
	char data[512] = { 0 };
	char *p = buf->encode_packet(msg);
	strcpy_s(data, p);
	int len = sizeof(data);
	int ret = send(_sClient, data, len, 0);
	buf->buf->size = 0;
	memset(buf->buf->data, '\0', sizeof(buf->buf->data));
	return ret;
}

int CClient::RecvData(SOCKET client)
{
	m_pMsg.clear();
	char data[512] ;
	int ret = recv(client, data, sizeof(data), 0);
	if (ret <= 0) {
		printf("receive %d, exit.\n", ret);
		exit(0);
	}
	if (buf->buffer_add(data, ret) == -1) {
		printf("error, too large packet!\n");
		exit(0);
	}

	int n = 0;
	
	while (1) {
		char *msg = buf->parse_packet();
		if (!msg) {
			break;
		}
		printMsg(msg);
		m_pMsg = msg;
		n++;
		if (n > 1) {
			m_pMsg += msg;
			printMsg("Merged Packet");
		}
		free(msg);
	}
	buf->buf->size = 0;
	memset(buf->buf->data, '\0', sizeof(buf->buf->data));
	
	DeCode();
	return ret;
}

void CClient::EnCode()
{

}

void CClient::DeCode()
{
	int msgType = AnalysisMsgHead();

	switch (msgType)
	{
	case SUB_C_LOGIN_REQ:
	case SUB_C_REGISTER_REQ:
		ProcesLoginOrRegisterMsg(msgType);
		break;
	case SUB_C_CREAT_ROOM:
	case SUB_C_START_MATCH:
		ProcesCreatRoomOrMatchMsg(msgType);
		break;
	case SUB_C_DATA_LOAD_SELF:
	case SUB_C_DATA_LOAD_RIVAL:
		ProcesReadGameDataMsg(msgType);
		break;
	case SUB_C_MODIF_INFO:
		ProcesModifInfoMsg(msgType);
		break;
	case SUB_C_QUIT_ROOM:
		ProcesQuitRoomMsg(msgType);
		break;
	case SUB_C_READY:
		ProcesReadyMsg(msgType);
		break;
	case SUB_C_MOVE_CHESS:
		ProcesMoveChessMsg(msgType);
		break;
	case SUB_C_GIVEUP_REQ:
		ProcesGiveUpMsg(msgType);
		break;
	case SUB_C_PEACE_REQ:
	case SUB_C_PEACE_ANSWER:
		ProcesPeaceMsg(msgType);
		break;
	}
}

int CClient::AnalysisMsgHead()
{
	Document doc;
	doc.Parse<0>(m_pMsg.c_str());
	if (doc.GetParseError()) {
		printError(doc.GetParseError());
	}
	Value &v = doc["msgType"];
	int msgType = -1;
	if (v.IsInt()) {
		msgType = v.GetInt();
	}
	return msgType;
}

void CClient::AfterProcesMsg(int msgType, int ret)
{
	Document _doc;
	_doc.SetObject();
	Document::AllocatorType& alloctor = _doc.GetAllocator();
	_doc.AddMember("msgType", msgType, alloctor);
	//if (ret != NO_RESPOND)	_doc.AddMember("ret", ret, alloctor);
	_doc.AddMember("ret", ret, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_doc.Accept(writer);
	const char* msg = buffer.GetString();
	SendData(msg);
}

void CClient::ProcesLoginOrRegisterMsg(int msgType)
{
	Document doc;
	doc.Parse<0>(m_pMsg.c_str());
	if (doc.GetParseError()) {
		printError(doc.GetParseError());
	}
	CMD_C_LOGIN userInfo;
	Value &obj = doc["CMD_C_LOGIN"];
	Value &_vName = obj["userName"];
	Value &_vPasswd = obj["passworld"];
	const char* _name = _vName.GetString();
	const char* _paswd = _vPasswd.GetString();
	strcpy_s(__userName, _name);
	strcpy_s(userInfo.userName, _name);
	strcpy_s(userInfo.passworld, _paswd);
	//setClientName(_name);
	
	int ret;
	if (msgType == SUB_C_LOGIN_REQ) {
		ret = DataBase::getInstance()->user_login(userInfo);
	}
	else {
		ret = DataBase::getInstance()->user_register(userInfo);
	}

	AfterProcesMsg(msgType, ret);
}

void CClient::ProcesCreatRoomOrMatchMsg(int msgType)
{
	int ret;
	if (msgType == SUB_C_CREAT_ROOM) {
		ret = RoomManage::getInstance()->createRoom(this);
		AfterProcesMsg(msgType, ret);
	}
	else{
		CClientManage::getInstance()->addToMatchList(this);
		CClientManage::getInstance()->StartMatch(this, msgType);
	}
}

void CClient::ProcesReadGameDataMsg(int msgType)
{
	DataBase* db = DataBase::getInstance();

	//判断当前消息类型是否为SUB_C_DATA_LOAD_RIVAL，是则读取房间另个玩家的信息
	char* rivalName = nullptr;
	if (msgType == SUB_C_DATA_LOAD_RIVAL) {
		//找到所在房间号
		int _roomId = RoomManage::getInstance()->playeIsJoinRoom(this);
		//找到房间
		auto _room = RoomManage::getInstance()->getRoomById(_roomId);
		for (int i = 0; i < _room->_player_List.size(); i++) {
			if (_room->_player_List[i] != this) {
				rivalName = _room->_player_List[i]->__userName;
				break;
			}
		}
	}
	
	//数据库返回的数据
	CMD_C_DATALOAD data;
	if (msgType == SUB_C_DATA_LOAD_RIVAL) {
		data = db->user_load_game_data(rivalName);
	}
	else{
		data = db->user_load_game_data(__userName);
	}
	Document _doc;
	_doc.SetObject();
	Document::AllocatorType& alloctor = _doc.GetAllocator();
	_doc.AddMember("msgType", msgType, alloctor);
	_doc.AddMember("ret", DATALOAD_SUCCEED, alloctor);
	Value obj(kObjectType);
	rapidjson::Value value_sex(data.sex, alloctor);
	rapidjson::Value value_sign(data.signature, alloctor);
	rapidjson::Value value_name(data.name, alloctor);
	obj.AddMember("sex", value_sex, alloctor);
	obj.AddMember("signature", value_sign, alloctor);
	obj.AddMember("name", value_name, alloctor);
	obj.AddMember("age", data.age, alloctor);
	obj.AddMember("W_TotalNum", data.W_TotalNum, alloctor);
	obj.AddMember("B_TotalNum", data.B_TotalNum, alloctor);
	obj.AddMember("W_TotalWinNum", data.W_TotalWinNum, alloctor);
	obj.AddMember("B_TotalWinNum", data.B_TotalWinNum, alloctor);
	obj.AddMember("avta_index", data.avta_index, alloctor);
	_doc.AddMember("CMD_C_DATALOAD", obj, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_doc.Accept(writer);
	const char* msg = buffer.GetString();

	SendData(msg);
	//清理数据缓存区
	memset(db->msq_result, '\0', sizeof(db->msq_result));
}

void CClient::ProcesModifInfoMsg(int msgType)
{
	Document doc;
	doc.Parse<0>(m_pMsg.c_str());
	if (doc.GetParseError()) {
		printError(doc.GetParseError());
	}
	CMD_C_MODIFINFO userInfo;
	Value &obj = doc["CMD_C_MODIFINFO"];
	Value &_vName = obj["name"];
	Value &_vAge = obj["age"];
	Value &_vSex = obj["sex"];
	Value &_vSignature = obj["signature"];
	Value &_vAvta_index = obj["avta_index"];

	const char* _name = _vName.GetString();
	const char* _sex = _vSex.GetString();
	const char* _signature = _vSignature.GetString();
	strcpy_s(userInfo.name, _name);
	strcpy_s(userInfo.sex, _sex);
	strcpy_s(userInfo.signature, _signature);
	userInfo.age = _vAge.GetInt();
	userInfo.avta_index = _vAvta_index.GetInt();

	int ret = DataBase::getInstance()->user_modif_info(userInfo);
	AfterProcesMsg(msgType, ret);
}

void CClient::ProcesQuitRoomMsg(int msgType)
{
	int ret = RoomManage::getInstance()->quitFromRoom(this);
	AfterProcesMsg(msgType, ret);
}

void CClient::ProcesReadyMsg(int msgType)
{
	this->setIsReady(true);
	int roomId = RoomManage::getInstance()->playeIsJoinRoom(this);
	Room* room = RoomManage::getInstance()->getRoomById(roomId);
	//CClientManage::getInstance()->SendUserInfoMsgToRoomPlayers(room);
	//room->OnEventGameStart();
	CClientManage::getInstance()->SendStartGame(room);
}

void CClient::ProcesMoveChessMsg(int msgType)
{
	Document doc;
	doc.Parse<0>(m_pMsg.c_str());
	if (doc.GetParseError()) {
		printError(doc.GetParseError());
	}
	CMD_C_MoveChess data;
	Value &obj = doc["CMD_C_MoveChess"];
	Value &_vsx = obj["xSourcePos"];
	Value &_vsy = obj["ySourcePos"];
	Value &_vtx = obj["xTargetPos"];
	Value &_vty = obj["yTargetPos"];
	Value &_vswitch = obj["switchChess"];
	//Value &_vcurruser = obj["currentUser"];

	data.xSourcePos = _vsx.GetInt();
	data.ySourcePos = _vsy.GetInt();
	data.xTargetPos = _vtx.GetInt();
	data.yTargetPos = _vty.GetInt();
	data.switchChess = _vswitch.GetInt();
	//data.currentUser = _vcurruser.GetInt();

	int roomId = RoomManage::getInstance()->playeIsJoinRoom(this);
	Room* room = RoomManage::getInstance()->getRoomById(roomId);
	int ret = room->OnUserMoveChess(data.xSourcePos, data.ySourcePos, data.xTargetPos, data.yTargetPos, data.switchChess, this);
	AfterProcesMsg(msgType, ret);
	//room->TranspondMoveChessMsg(data, this, msgType);
}

void CClient::ProcesPeaceMsg(int msgType)
{
	int roomId = RoomManage::getInstance()->playeIsJoinRoom(this);
	Room* room = RoomManage::getInstance()->getRoomById(roomId);
	int approve = NO_RESPOND;
	if (msgType == SUB_C_PEACE_ANSWER) {
		Document doc;
		doc.Parse<0>(m_pMsg.c_str());
		if (doc.GetParseError()) {
			printError(doc.GetParseError());
		}
		CMD_C_PeaceAnswer data;
		Value &obj = doc["CMD_C_PeaceAnswer"];
		Value &_valpe = obj["cbApprove"];
		approve = _valpe.GetInt();
	}
	
	room->OnUserPeaceReq(this, msgType, approve);
}

void CClient::ProcesGiveUpMsg(int msgType)
{
	int roomId = RoomManage::getInstance()->playeIsJoinRoom(this);
	Room* room = RoomManage::getInstance()->getRoomById(roomId);
	room->OnUserGiveUp();
}

void CClient::SendMatchFinishMsg(int msgType, int ret)
{
	Document _doc;
	_doc.SetObject();
	Document::AllocatorType& alloctor = _doc.GetAllocator();
	_doc.AddMember("msgType", msgType, alloctor);
	_doc.AddMember("ret", ret, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_doc.Accept(writer);
	const char* msg = buffer.GetString();
	SendData(msg);
}

void CClient::SendUserinfoToRoomPlayer(CMD_C_DATALOAD data)
{
	Document _doc;
	_doc.SetObject();
	Document::AllocatorType& alloctor = _doc.GetAllocator();
	_doc.AddMember("msgType", SUB_C_DATA_LOAD_SELF, alloctor);
	_doc.AddMember("ret", DATALOAD_SUCCEED, alloctor);
	
	
	Value obj(kObjectType);
	rapidjson::Value value_sex(data.sex, alloctor);
	rapidjson::Value value_sign(data.signature, alloctor);
	rapidjson::Value value_name(data.name, alloctor);
	obj.AddMember("sex", value_sex, alloctor);
	obj.AddMember("name", value_name, alloctor);
	obj.AddMember("signature", value_sign, alloctor);

	obj.AddMember("age", data.age, alloctor);
	obj.AddMember("W_TotalNum", data.W_TotalNum, alloctor);
	obj.AddMember("B_TotalNum", data.B_TotalNum, alloctor);
	obj.AddMember("W_TotalWinNum", data.W_TotalWinNum, alloctor);
	obj.AddMember("B_TotalWinNum", data.B_TotalWinNum, alloctor);
	obj.AddMember("avta_index", data.avta_index, alloctor);
	_doc.AddMember("CMD_C_DATALOAD", obj, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_doc.Accept(writer);
	const char* msg = buffer.GetString();

	SendData(msg);
	//清理数据缓存区
	auto db = DataBase::getInstance();
	memset(db->msq_result, '\0', sizeof(db->msq_result));
}

void CClient::SendStartGameMsg(int msgType, int ret)
{
	AfterProcesMsg(msgType, ret);
}

void CClient::SendGameOverMsg(CMD_S_GameEnd data, int msgType, int ret)
{
	Document _doc;
	_doc.SetObject();
	Document::AllocatorType& alloctor = _doc.GetAllocator();
	_doc.AddMember("msgType", msgType, alloctor);
	_doc.AddMember("ret", ret, alloctor);
	Value obj(kObjectType);
	obj.AddMember("wWinUser", data.wWinUser, alloctor);
	_doc.AddMember("CMD_S_GameEnd", obj, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_doc.Accept(writer);
	const char* msg = buffer.GetString();
	SendData(msg);
}

string CClient::getTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char temp[64] = { 0 };
	sprintf_s(temp, "[%4d-%02d-%02d %02d:%02d:%02d]", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return temp;
}

void CClient::printError(int errcode)
{
	cout << getTime() << "[ErrorCode :" << errcode << "]" << endl;
}

void CClient::printMsg(string msg)
{
	cout << getTime() << "[" << msg << "]" << endl;
}