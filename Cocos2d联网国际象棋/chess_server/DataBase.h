#pragma once

//#pragma comment(lib,"libmySQL.lib")
#include <mysql.h>
#include <winsock.h>
#include <string>
#include "def.h"

//#pragma comment(lib,"libmysql.lib")


class DataBase
{
public:
	~DataBase();
	//DataBase();
	static DataBase* getInstance();
	//ע��
	int user_login(CMD_C_LOGIN userInfo);						
	//��½
	int user_register(CMD_C_LOGIN userInfo);					
	//�޸�
	int user_modif_info(CMD_C_MODIFINFO userInfo);	
	//��ȡ�û���¼
	CMD_C_DATALOAD user_load_game_data(const char name[]);
public:
	//char* msq_result;
	char* msq_result;
	int avta_index;
private:
	static DataBase *instace;
	MYSQL m_sqlCon;

	const char* host = "120.78.155.232";
	const char* user = "ChessDB";
	const char* passwd = "123456";
	const char* table = "ChessDB";												//���ݿ���
	int  port = 3306;

private:
	DataBase();
	//�������캯��
	DataBase(DataBase&);					
	//��ʼ������
	bool initConnection();
	//��ѯ�û���
	int user_query_by_name(char name[]);
	//�û�����
	int user_insert(CMD_C_LOGIN userInfo);
	//���ݸ���
	int user_update(CMD_C_MODIFINFO userInfo);
	//ɾ������
	int user_delete(const char name[]);

	string getTime();

	void printError(int errcode);
	void printMsg(string msg);
};

