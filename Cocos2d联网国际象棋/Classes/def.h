#ifndef __DEF_H__
#define __DEF_H__

//---------------����ֵ����---------------
//��½
#define ERROR_LOGIN_NAME_NULL			1							//�û���������
#define ERROR_LOGIN_PASSWD_NOTMATCH		2							//���벻ƥ��
#define LOGIN_SUCCEED					3							//��½�ɹ�
//ע��
#define ERROR_REGISTER_NAME_NULL		4							//�û���Ϊ��
#define ERROR_REGISTER_NAME_EXIST		6							//�û��Ѵ���
#define REGISTER_SUCCEED				7							//ע��ɹ�
//�޸ĸ�����Ϣ
#define ERROR_MODIF_NAME_NULL			8							//�û���Ϊ��
#define ERROR_MODIF_PASSWD_NULL			9							//������Ϊ��
#define MODIF_SUCCEED					10							//�޸ĳɹ�
#define MODIF_FAILED					11							//�޸�ʧ��
//���ݿ����
#define ERROR_DB_EXECUTE_FAILED			-1							//���ִ��ʧ��
#define DB_EXECUTE_SUCCEED				0							//���ִ�гɹ�
#define ERROR_DB_QUERY_FAILED			12							//��ѯʧ��
//�������� ƥ��
#define CREAT_ROOM_SUCCEED				20							//��������ɹ�
#define CREAT_ROOM_FAILED				21							//��������ʧ��
#define MATCH_SUCCEED					22							//ƥ��ɹ�
#define MATCH_FAILED					23							//ƥ��ʧ��
#define QUIT_ROOM_SUCCEED				26							//�˳�����ɹ�
#define QUIT_ROOM_FAILED				27							//�˳�����ʧ��
//��ȡ��Ϣ
#define DATALOAD_FAILED					24							//��ȡ��Ϣʧ��
#define DATALOAD_SUCCEED				25							//��ȡ��Ϣ�ɹ�
//������Ϣ
#define MOVECHESS_SUCCEED				30							//����ɹ�
#define MOVECHESS_FAILED				31							//����ʧ��
//��Ϸ����
#define GAMEOVER_NORMAL					33							//��������
#define GAMEOVER_USER_LEFT				34							//�û�ǿ��
#define GAMEOVER_GIVEUP					35							//����
//����¼�
#define PEACE_SUCCEED					41							//ͬ�����
#define PEACE_FAILED					42							//�ܾ����
//�����¼�
#define REGRET_SUCCEED					44							//ͬ�����
#define REGRET_FAILED					45							//�ܾ�����
//other
#define NO_RESPOND						50							//�޷���ֵ

//---------------����ֵ����---------------

//������ɫ
#define NO_CHESS					0									//û������
#define BLACK_CHESS					1									//��ɫ����/
#define WHITE_CHESS					2									//��ɫ����/
//��Ҷ���
#define BLACK_PLAYER				0									//�������
#define WHITE_PLAYER				1									//�������
//���Ӷ���
#define CHESS_KING					1									//��
#define CHESS_QUEEN					2									//��
#define CHESS_ROOK					3									//��
#define CHESS_ELEPHANT				4									//��
#define CHESS_HORSE					5									//��
#define CHESS_SOLDIER				6									//��
//��������
//#define MOVETYPE_ERROR,				0									//�������
//#define MOVETYPE_NORMAL,				1									//��ͨ����
//#define MOVETYPE_SOLDIERUP,			2									//��������
//#define MOVETYPE_KINGROOK,			3									//������λ
//���ֶ���
enum enMoveType
{
	enMoveType_Error,					//�������
	enMoveType_Normal,					//��ͨ����
	enMoveType_SoldierUp,				//��������
	enMoveType_KingRook,				//������λ
	enMoveType_EatPassingSoldier		//�Թ�·��
};

struct chessIten
{
	int							xPos;								//��������
	int							yPos;								//��������
	int							color;								//������ɫ
	int							chess;								//����Ȩλ
	int							chessID;							//���ӱ�ʶ
	int							walkCount;							//���߲���
};

//�ƶ�����
struct CMD_S_MoveChess
{
	int							xSourcePos;							//����λ��
	int							ySourcePos;							//����λ��
	int							xTargetPos;							//����λ��
	int							yTargetPos;							//����λ��
	int							switchChess;						//�任����
	int				 			currentUser;						//��ǰ���
};

//��¼�����ƶ��Ĳ�(����)
struct chessManual
{
	//�ƶ�����
	int							sourceID;							//���ӱ�ʶ
	int							sourceColor;						//������ɫ
	int							sourceChess;						//����Ȩλ
	int							xSourceChessPos;					//����λ��
	int							ySourceChessPos;					//����λ��
	int							xTargetChessPos;					//����λ��
	int							yTargetChessPos;					//����λ��

	//��������
	int							uniteID;							//���ӱ�ʶ
	int							uniteColor;							//������ɫ
	int							uniteChess;							//����Ȩλ
	int							switchChess;						//��������
	int							xUniteChessPos;						//����λ��
	int							yUniteChessPos;						//����λ��
};
//��Ϸ����
struct CMD_S_GameEnd
{
	int							wWinUser;							//ʤ�����
};


//����������
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_MOVE_CHESS			101									//�ƶ�����
#define SUB_S_REGRET_REQ			102									//��������
#define SUB_S_REGRET_FAILE			103									//����ʧ��
#define SUB_S_REGRET_RESULT			104									//������
#define SUB_S_PEACE_REQ				105									//��������
#define SUB_S_PEACE_ANSWER			106									//����Ӧ��
#define SUB_S_GAME_END				107									//��Ϸ����
#define SUB_S_MATCH_FINISH			108									//�Ծ��ҵ�

//�ͻ�������
#define SUB_C_LOGIN_REQ				0									//��    ½
#define SUB_C_REGISTER_REQ			1									//ע	    ��
#define SUB_C_CREAT_ROOM			2									//��������
#define SUB_C_START_MATCH			3									//��ʼƥ��
#define SUB_C_MOVE_CHESS			4									//�ƶ�����
#define SUB_C_REGRET_REQ			5									//��������
#define SUB_C_REGRET_ANSWER			6									//����Ӧ��
#define SUB_C_PEACE_REQ				7									//��������
#define SUB_C_PEACE_ANSWER			8									//����Ӧ��
#define SUB_C_GIVEUP_REQ			9									//��������
#define SUB_C_MODIF_INFO			10									//�޸ĸ�����Ϣ
#define SUB_C_DATA_LOAD_SELF		11									//��ȡ�Լ�����
#define SUB_C_DATA_LOAD_RIVAL		12									//��ȡ����������
#define SUB_C_QUIT_ROOM				13									//�˳�����
#define SUB_C_READY					14									//׼��
#define SUB_C_READ_RANKING			15									//����
#define SUB_C_READ_ROOM_LIST		16									//��ȡ�����б�


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

//��½�ṹ ע��ṹ
struct CMD_C_LOGIN {
	char userName[10];													//�û���
	char passworld[8];													//����
};

//�޸ĸ�����Ϣ
struct CMD_C_MODIFINFO
{
	char name[10];														//����
	char sex[4];														//�Ա�
	int	 age;															//����
	char signature[256];												//ǩ��
	int  avta_index;													//ͷ������
};

//��ȡ��Ϣ
//��ȡ��Ϣ
struct CMD_C_DATALOAD
{
	char sex[4];														//�Ա�
	int	 age;															//����
	char signature[256];												//ǩ��
	char name[10];
	int W_TotalNum;														//�����ܳ���
	int B_TotalNum;
	int W_TotalWinNum;													//������Ӯ����
	int B_TotalWinNum;
	int avta_index;														//ͷ������
};
//�ƶ�����
struct CMD_C_MoveChess
{
	int							xSourcePos;						//����λ��
	int							ySourcePos;						//����λ��
	int							xTargetPos;						//����λ��
	int							yTargetPos;						//����λ��
	int							switchChess;					//�任����
};
//������Ӧ
struct CMD_C_RegretAnswer
{
	int							cbApprove;							//ͬ���־
};

//������Ӧ
struct CMD_C_PeaceAnswer
{
	int							cbApprove;							//ͬ���־
};


#endif