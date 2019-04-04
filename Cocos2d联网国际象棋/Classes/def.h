#ifndef __DEF_H__
#define __DEF_H__

//---------------返回值定义---------------
//登陆
#define ERROR_LOGIN_NAME_NULL			1							//用户名不存在
#define ERROR_LOGIN_PASSWD_NOTMATCH		2							//密码不匹配
#define LOGIN_SUCCEED					3							//登陆成功
//注册
#define ERROR_REGISTER_NAME_NULL		4							//用户名为空
#define ERROR_REGISTER_NAME_EXIST		6							//用户已存在
#define REGISTER_SUCCEED				7							//注册成功
//修改个人信息
#define ERROR_MODIF_NAME_NULL			8							//用户名为空
#define ERROR_MODIF_PASSWD_NULL			9							//密码名为空
#define MODIF_SUCCEED					10							//修改成功
#define MODIF_FAILED					11							//修改失败
//数据库操作
#define ERROR_DB_EXECUTE_FAILED			-1							//语句执行失败
#define DB_EXECUTE_SUCCEED				0							//语句执行成功
#define ERROR_DB_QUERY_FAILED			12							//查询失败
//创建房间 匹配
#define CREAT_ROOM_SUCCEED				20							//创建房间成功
#define CREAT_ROOM_FAILED				21							//创建房间失败
#define MATCH_SUCCEED					22							//匹配成功
#define MATCH_FAILED					23							//匹配失败
#define QUIT_ROOM_SUCCEED				26							//退出房间成功
#define QUIT_ROOM_FAILED				27							//退出房间失败
//读取信息
#define DATALOAD_FAILED					24							//读取信息失败
#define DATALOAD_SUCCEED				25							//读取信息成功
//走棋信息
#define MOVECHESS_SUCCEED				30							//走棋成功
#define MOVECHESS_FAILED				31							//走棋失败
//游戏结束
#define GAMEOVER_NORMAL					33							//正常结束
#define GAMEOVER_USER_LEFT				34							//用户强退
#define GAMEOVER_GIVEUP					35							//认输
//求和事件
#define PEACE_SUCCEED					41							//同意求和
#define PEACE_FAILED					42							//拒绝求和
//悔棋事件
#define REGRET_SUCCEED					44							//同意悔棋
#define REGRET_FAILED					45							//拒绝悔棋
//other
#define NO_RESPOND						50							//无返回值

//---------------返回值定义---------------

//棋子颜色
#define NO_CHESS					0									//没有棋子
#define BLACK_CHESS					1									//黑色棋子/
#define WHITE_CHESS					2									//白色棋子/
//玩家定义
#define BLACK_PLAYER				0									//黑棋玩家
#define WHITE_PLAYER				1									//白棋玩家
//棋子定义
#define CHESS_KING					1									//王
#define CHESS_QUEEN					2									//后
#define CHESS_ROOK					3									//车
#define CHESS_ELEPHANT				4									//象
#define CHESS_HORSE					5									//马
#define CHESS_SOLDIER				6									//兵
//走棋类型
//#define MOVETYPE_ERROR,				0									//走棋错误
//#define MOVETYPE_NORMAL,				1									//普通走棋
//#define MOVETYPE_SOLDIERUP,			2									//兵的升变
//#define MOVETYPE_KINGROOK,			3									//王车易位
//棋字定义
enum enMoveType
{
	enMoveType_Error,					//走棋错误
	enMoveType_Normal,					//普通走棋
	enMoveType_SoldierUp,				//兵的升变
	enMoveType_KingRook,				//王车易位
	enMoveType_EatPassingSoldier		//吃过路兵
};

struct chessIten
{
	int							xPos;								//棋子坐标
	int							yPos;								//棋子坐标
	int							color;								//棋子颜色
	int							chess;								//棋子权位
	int							chessID;							//棋子标识
	int							walkCount;							//行走步数
};

//移动棋子
struct CMD_S_MoveChess
{
	int							xSourcePos;							//棋子位置
	int							ySourcePos;							//棋子位置
	int							xTargetPos;							//棋子位置
	int							yTargetPos;							//棋子位置
	int							switchChess;						//变换棋子
	int				 			currentUser;						//当前玩家
};

//记录棋字移动的步(棋谱)
struct chessManual
{
	//移动棋子
	int							sourceID;							//棋子标识
	int							sourceColor;						//棋子颜色
	int							sourceChess;						//棋子权位
	int							xSourceChessPos;					//棋子位置
	int							ySourceChessPos;					//棋子位置
	int							xTargetChessPos;					//棋子位置
	int							yTargetChessPos;					//棋子位置

	//关联棋子
	int							uniteID;							//棋子标识
	int							uniteColor;							//棋子颜色
	int							uniteChess;							//棋子权位
	int							switchChess;						//升变棋子
	int							xUniteChessPos;						//棋子位置
	int							yUniteChessPos;						//棋子位置
};
//游戏结束
struct CMD_S_GameEnd
{
	int							wWinUser;							//胜利玩家
};


//服务器命令
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_MOVE_CHESS			101									//移动棋子
#define SUB_S_REGRET_REQ			102									//悔棋请求
#define SUB_S_REGRET_FAILE			103									//悔棋失败
#define SUB_S_REGRET_RESULT			104									//悔棋结果
#define SUB_S_PEACE_REQ				105									//和棋请求
#define SUB_S_PEACE_ANSWER			106									//和棋应答
#define SUB_S_GAME_END				107									//游戏结束
#define SUB_S_MATCH_FINISH			108									//对局找到

//客户端命令
#define SUB_C_LOGIN_REQ				0									//登    陆
#define SUB_C_REGISTER_REQ			1									//注	    册
#define SUB_C_CREAT_ROOM			2									//创建房间
#define SUB_C_START_MATCH			3									//开始匹配
#define SUB_C_MOVE_CHESS			4									//移动棋子
#define SUB_C_REGRET_REQ			5									//悔棋请求
#define SUB_C_REGRET_ANSWER			6									//悔棋应答
#define SUB_C_PEACE_REQ				7									//和棋请求
#define SUB_C_PEACE_ANSWER			8									//和棋应答
#define SUB_C_GIVEUP_REQ			9									//认输请求
#define SUB_C_MODIF_INFO			10									//修改个人信息
#define SUB_C_DATA_LOAD_SELF		11									//读取自己数据
#define SUB_C_DATA_LOAD_RIVAL		12									//读取别人手数据
#define SUB_C_QUIT_ROOM				13									//退出房间
#define SUB_C_READY					14									//准备
#define SUB_C_READ_RANKING			15									//排行
#define SUB_C_READ_ROOM_LIST		16									//读取房间列表


struct CMD_S_RankingList
{
	char name0[10];
	int avta_index0;
	int  winNum0;

	char name1[10];
	int avta_index1;
	int  winNum1;

	char name2[10];
	int avta_index2;
	int  winNum2;

	char name3[10];
	int avta_index3;
	int  winNum3;

	char name4[10];
	int avta_index4;
	int  winNum4;
};

//登陆结构 注册结构
struct CMD_C_LOGIN {
	char userName[10];													//用户名
	char passworld[8];													//密码
};

//修改个人信息
struct CMD_C_MODIFINFO
{
	char name[10];														//名字
	char sex[4];														//性别
	int	 age;															//年龄
	char signature[256];												//签名
	int  avta_index;													//头像索引
};

//读取信息
//读取信息
struct CMD_C_DATALOAD
{
	char sex[4];														//性别
	int	 age;															//年龄
	char signature[256];												//签名
	char name[10];
	int W_TotalNum;														//白棋总场次
	int B_TotalNum;
	int W_TotalWinNum;													//白棋总赢场次
	int B_TotalWinNum;
	int avta_index;														//头像索引
};
//移动棋子
struct CMD_C_MoveChess
{
	int							xSourcePos;						//棋子位置
	int							ySourcePos;						//棋子位置
	int							xTargetPos;						//棋子位置
	int							yTargetPos;						//棋子位置
	int							switchChess;					//变换棋子
};
//悔棋响应
struct CMD_C_RegretAnswer
{
	int							cbApprove;							//同意标志
};

//和棋响应
struct CMD_C_PeaceAnswer
{
	int							cbApprove;							//同意标志
};


#endif