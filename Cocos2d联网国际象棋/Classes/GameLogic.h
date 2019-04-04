#ifndef __GAMELOGIC_H__
#define __GAMELOGIC_H__

#include "def.h"
#include "cocos2d.h"
#include <string>
#include <iostream>

using namespace std;
USING_NS_CC;

#define CHESS_OFFSET_X 37		//偏移量_x
#define CHESS_OFFSET_Y 77		//偏移量_y
#define CHESS_DIAMETER 35		//棋子半径
#define CHESS_NUM	   32		//棋子数量
#define CHESS_COL	   7		//棋盘大小
#define CHESS_ROW	   7		//
#pragma execution_character_set("utf-8")

class Step 
{
public:
	int _moveid;//需要移动的棋子的id
	int _killid;//通过触摸点的位置判断触摸点上是否有棋子
	int _xFrom; //棋子当前的位置的x坐标
	int _yFrom; //棋子当前的位置的y坐标
	int _xTo;   //棋子移动后的位置的x坐标
	int _yTo;   //棋子移动后的位置的y坐标

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
	//创建棋子
	static Chess* create(int id, int color);
	//初始化棋子
	void init(int id, int color);
	//摆放棋子的位置
	void reset(int color);
	//保存棋的初始位置
	static struct InitPos
	{
		int _x;
		int _y;
		int _type;
	}_initPos[32];

protected:
	int _type;									//棋子类型
	int _x;										//棋子坐标x
	int _y;										//棋子坐标y	
	int _id;									//棋子id
	bool _dead;									//是否死亡
	int _color;									//棋字颜色
	int _walkCount;								//移动步数
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