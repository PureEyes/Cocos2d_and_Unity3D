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
	//注册
	int user_login(CMD_C_LOGIN userInfo);						
	//登陆
	int user_register(CMD_C_LOGIN userInfo);					
	//修改
	int user_modif_info(CMD_C_MODIFINFO userInfo);	
	//读取用户记录
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
	const char* table = "ChessDB";												//数据库名
	int  port = 3306;

private:
	DataBase();
	//拷贝构造函数
	DataBase(DataBase&);					
	//初始化连接
	bool initConnection();
	//查询用户名
	int user_query_by_name(char name[]);
	//用户插入
	int user_insert(CMD_C_LOGIN userInfo);
	//数据更新
	int user_update(CMD_C_MODIFINFO userInfo);
	//删除操作
	int user_delete(const char name[]);

	string getTime();

	void printError(int errcode);
	void printMsg(string msg);
};

