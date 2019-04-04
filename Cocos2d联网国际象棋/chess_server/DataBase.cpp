#include "stdafx.h"
#include "DataBase.h"
#include <cstdlib> 

DataBase* DataBase::instace = nullptr;

DataBase::DataBase()
{
	initConnection();
}


DataBase::~DataBase()
{
}

DataBase* DataBase::getInstance()
{
	if (instace == nullptr) {
		instace = new DataBase();
	}
	return instace;
}

bool DataBase::initConnection()
{
	try
	{
		if (mysql_init(&m_sqlCon) == nullptr) {
			printMsg("初始化数据库失败");
			return false;
		}

		if (!mysql_real_connect(&m_sqlCon, host, user, passwd, table, port, nullptr, 0)) {
			printMsg("数据库连接失败");

			const char* errMsg = mysql_error(&m_sqlCon);
			std::cout << mysql_error(&m_sqlCon) << std::endl;
			return false;
		}

		if (mysql_set_character_set(&m_sqlCon, "gbk"))
		{
			printMsg("设置字符集失败");
		}
		printMsg("数据库连接成功");
		return true;

	}
	catch (const std::exception&) {
		printMsg("出错了...");
	}

}

int DataBase::user_login(CMD_C_LOGIN userInfo)
{
	char sql[64] = { 0 };
	char con[] = "SELECT *FROM USERINFOR WHERE NAME = \"%s\"";
	sprintf_s(sql, sizeof(sql), con, userInfo.userName);

	//语句执行成功返回0，失败则返回非零
	if (mysql_query(&m_sqlCon, sql) != 0) {
		printError(ERROR_DB_EXECUTE_FAILED);
		return ERROR_DB_EXECUTE_FAILED;
	}

	MYSQL_RES *result = mysql_store_result(&m_sqlCon);
	MYSQL_ROW sql_row = mysql_fetch_row(result);

	int row_num = mysql_num_rows(result);
	//用户不存在
	if (row_num <= 0) {
		printError(ERROR_LOGIN_NAME_NULL);
		mysql_free_result(result);
		return ERROR_LOGIN_NAME_NULL;
	}
	//密码不匹配
	
	char msg[12] = { 0 };
	strcpy_s(msg, sql_row[3]);
	avta_index = atoi(sql_row[10]);
	if (strcmp(msg,userInfo.passworld)!=0) {
		printError(ERROR_LOGIN_PASSWD_NOTMATCH);
		mysql_free_result(result);
		return ERROR_LOGIN_PASSWD_NOTMATCH;
	}
	mysql_free_result(result);
	printMsg("登陆成功!");

	return LOGIN_SUCCEED;
}

int DataBase::user_register(CMD_C_LOGIN userInfo)
{
	//用户名为空
	int nameLen = sizeof(userInfo.userName);
	if (nameLen <= 0) {
		printError(ERROR_REGISTER_NAME_NULL);
		return ERROR_REGISTER_NAME_NULL;
	}

	char sql[64] = { 0 };
	char con[] = "SELECT *FROM USERINFOR WHERE NAME = \"%s\"";
	sprintf_s(sql, sizeof(sql), con, userInfo.userName);

	//语句执行成功返回0，失败则返回非零
	if (mysql_query(&m_sqlCon, sql) != 0) {
		printError(ERROR_DB_EXECUTE_FAILED);
		return ERROR_DB_EXECUTE_FAILED;
	}
	MYSQL_RES *result = mysql_store_result(&m_sqlCon);
	MYSQL_ROW sql_row = mysql_fetch_row(result);
	//用户存在
	int row_num = mysql_num_rows(result);
	if (row_num > 0) {
		printError(ERROR_REGISTER_NAME_EXIST);
		mysql_free_result(result);
		return ERROR_REGISTER_NAME_EXIST;
	}
	mysql_free_result(result);
	//插入数据
	user_insert(userInfo);
	return REGISTER_SUCCEED;
}

int DataBase::user_modif_info(CMD_C_MODIFINFO userInfo)
{
	int nameLen = strlen(userInfo.name);

	if (nameLen <= 0) {
		printError(ERROR_REGISTER_NAME_EXIST);
		return ERROR_MODIF_NAME_NULL;
	}
	
	 return user_update(userInfo);
}

CMD_C_DATALOAD DataBase::user_load_game_data(const char name[])
{
	char sql[64] = { 0 };
	char con[] = "SELECT *FROM USERINFOR WHERE NAME = \"%s\"";
	sprintf_s(sql, sizeof(sql), con, name);

	//语句执行成功返回0，失败则返回非零
	if (mysql_query(&m_sqlCon, sql) != 0) {
		printError(ERROR_DB_EXECUTE_FAILED);
	}

	MYSQL_RES *result = mysql_store_result(&m_sqlCon);
	MYSQL_ROW sql_row = mysql_fetch_row(result);
	//用户不存在
	int row_num = mysql_num_rows(result);
	if (row_num <= 0) {
		printError(ERROR_DB_QUERY_FAILED);
	}

	CMD_C_DATALOAD data;
	data.age = atoi(sql_row[4]);
	strcpy_s(data.sex, sql_row[2]);
	strcpy_s(data.name, sql_row[1]);
	strcpy_s(data.signature, sql_row[5]);
	data.W_TotalNum = atoi(sql_row[6]);
	data.B_TotalNum = atoi(sql_row[7]);
	data.W_TotalWinNum = atoi(sql_row[8]);
	data.B_TotalWinNum = atoi(sql_row[9]);
	data.avta_index = atoi(sql_row[10]);
	//memset(msq_result, '\0', sizeof(msq_result));
	//char* temp = (char*)&data;
	msq_result = (char*)&data;
	
	mysql_free_result(result);
	//return DATALOAD_FAILED;
	return data;
}

int DataBase::user_query_by_name(char name[])
{
	char sql[64] = { 0 };
	char con[] = "SELECT *FROM USERINFOR WHERE NAME = \"%s\"";
	sprintf_s(sql, sizeof(sql), con, name);

	//语句执行成功返回0，失败则返回非零
	if (mysql_query(&m_sqlCon, sql) != 0) {
		printError(ERROR_DB_EXECUTE_FAILED);
		return ERROR_DB_EXECUTE_FAILED;
	}

	MYSQL_RES *result = mysql_store_result(&m_sqlCon);
	MYSQL_ROW sql_row = mysql_fetch_row(result);
	//用户不存在
	int row_num = mysql_num_rows(result);
	if (row_num <= 0) {
		printError(ERROR_DB_QUERY_FAILED);
		return ERROR_DB_QUERY_FAILED;
	}
	mysql_free_result(result);

	return DB_EXECUTE_SUCCEED;
}

int DataBase::user_insert(CMD_C_LOGIN userInfo)
{
	char sql[64] = { 0 };
	//char sql[64] = "INSERT INTO user (NAME,PASSWD) VALUES('hhg12','123456')";
	char con[] = "INSERT INTO USERINFOR (NAME,PASSWD) VALUES('%s','%s')";
	sprintf_s(sql, sizeof(sql),con, userInfo.userName, userInfo.passworld);
	if (mysql_query(&m_sqlCon, sql)) {
		printError(ERROR_DB_EXECUTE_FAILED);
		return ERROR_DB_EXECUTE_FAILED;
	}
	else {
		printMsg("插入数据成功!");
		return DB_EXECUTE_SUCCEED;
	}
}

int DataBase::user_update(CMD_C_MODIFINFO userInfo)
{
	char sql[300] = { 0 };
	char con[] = "UPDATE USERINFOR SET SEX = \"%s\", AGE = %d, SIGNATURE = \"%s\", AVTA_INDEX = %d WHERE NAME = \"%s\"";
	sprintf_s(sql, sizeof(sql), con, userInfo.sex, userInfo.age, userInfo.signature, userInfo.avta_index, userInfo.name);
	if (mysql_query(&m_sqlCon, sql)) {
		printError(MODIF_FAILED);
		return MODIF_FAILED;
	}
	else {
		printMsg("更新数据成功!");
		return MODIF_SUCCEED;
	}
}

int DataBase::user_delete(const char name[])
{
	char sql[64] = { 0 };
	char con[] = "DELETE FROM USERINFOR WHERE NAME = \"%s\" ";
	sprintf_s(sql, sizeof(sql), con, name);
	if (mysql_query(&m_sqlCon, sql)) {
		printError(ERROR_DB_EXECUTE_FAILED);
		return ERROR_DB_EXECUTE_FAILED;
	}
	else {
		printMsg("删除数据成功!");
		return DB_EXECUTE_SUCCEED;
	}
}

string DataBase::getTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char temp[64] = { 0 };
	sprintf_s(temp, "[%4d-%02d-%02d %02d:%02d:%02d]", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return temp;
}

void DataBase::printError(int errcode)
{
	cout << getTime() << "[ErrorCode :" << errcode  << "]" << endl;
}

void DataBase::printMsg(string msg)
{
	cout << getTime() << "[" << msg << "]" << endl;
}
