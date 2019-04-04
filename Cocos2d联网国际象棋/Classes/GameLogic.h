#ifndef __GAMELOGIC_H__
#define __GAMELOGIC_H__

#include "def.h"
#include "cocos2d.h"
#include <string>
#include <iostream>

using namespace std;
USING_NS_CC;

#define CHESS_OFFSET_X 37		//ƫ����_x
#define CHESS_OFFSET_Y 77		//ƫ����_y
#define CHESS_DIAMETER 35		//���Ӱ뾶
#define CHESS_NUM	   32		//��������
#define CHESS_COL	   7		//���̴�С
#define CHESS_ROW	   7		//
#pragma execution_character_set("utf-8")

class Step 
{
public:
	int _moveid;//��Ҫ�ƶ������ӵ�id
	int _killid;//ͨ���������λ���жϴ��������Ƿ�������
	int _xFrom; //���ӵ�ǰ��λ�õ�x����
	int _yFrom; //���ӵ�ǰ��λ�õ�y����
	int _xTo;   //�����ƶ����λ�õ�x����
	int _yTo;   //�����ƶ����λ�õ�y����

	static Step* create(int moveid, int killid, int xFrom, int yFrom, int xTo, int yTo)
	{
		Step* step = new Step;

		step->_moveid = moveid;
		step->_killid = killid;
		step->_xFrom = xFrom;
		step->_yFrom = yFrom;
		step->_xTo = xTo;
		step->_yTo = yTo;
		return step;
	}
};

class Chess : public Sprite
{

public:
	Chess();
	~Chess();
	//��������
	static Chess* create(int id, int color);
	//��ʼ������
	void init(int id, int color);
	//�ڷ����ӵ�λ��
	void reset(int color);
	//������ĳ�ʼλ��
	static struct InitPos
	{
		int _x;
		int _y;
		int _type;
	}_initPos[32];

protected:
	int _type;									//��������
	int _x;										//��������x
	int _y;										//��������y	
	int _id;									//����id
	bool _dead;									//�Ƿ�����
	int _color;									//������ɫ
	int _walkCount;								//�ƶ�����
public:
	void setType(int type) { _type = type; }
	int  getType() const { return _type; }
	void setX(int x) { _x = x; }
	int  getX() const { return _x; }
	void setY(int y) { _y = y; }
	int  getY() const { return _y; }
	void setID(int ID) { _id = ID; }
	int  getID() const { return _id; }
	void setDead(bool dead) { _dead = dead; }
	bool getDead() const { return _dead; }
	void setRed(int color) { _color = color; }
	int  getRed() const { return _color; }
	void setWalkCount(int waloCount) { _walkCount = waloCount; }
	int  getWalkCount() const { return _walkCount; }
};

class GameLogic
{

};

#endif //__GAMELOGIC_H__