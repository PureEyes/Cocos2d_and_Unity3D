#include "GameLogic.h"

//--------------------������----------------
Chess::Chess()
{}
Chess::~Chess()
{}

Chess * Chess::create(int id, int color)
{
	Chess* c = new Chess();

	c->init(id, color);

	c->autorelease();

	return c;
}

void Chess::init(int id, int color)
{
	_id = id;

	//_color = (id < 16) ? BLACK_CHESS : WHITE_CHESS;
	_color = (id < 16) ? WHITE_CHESS : BLACK_CHESS;

	//��ʼ���ڰ����ӣ���ͬ����֮�����16
	if (_id < 16)
		_type = _initPos[_id]._type;
	else
		_type = _initPos[_id - 16]._type;

	setWalkCount(0);

	string fileName = StringUtils::format("menu/Chess/one/chess_%d_%d.png", _color, _type);

	Sprite::initWithFile(fileName);

	setScale(0.45f, 0.4f);

	reset(color);
}

void Chess::reset(int color)
{
	this->setDead(false);

	//���
	if (color == WHITE_PLAYER) {
		//�ڰ���
		if (_id < 16) {
			this->setX(_initPos[_id]._x);
			this->setY(_initPos[_id]._y);
		}
		//�ں���
		else {
			this->setX(7 - _initPos[_id - 16]._x);
			this->setY(7 - _initPos[_id - 16]._y);
		}
	}
	else {
		//�ں���
		if (_id < 16) {
			this->setX(7 - _initPos[_id]._x);
			this->setY(7 - _initPos[_id]._y);
		}
		//�ڰ���
		else {
			this->setX(_initPos[_id - 16]._x);
			this->setY(_initPos[_id - 16]._y);
		}
	}
}
//��������
Chess::InitPos Chess::_initPos[32] =
{
	{ 0, 0, CHESS_ROOK },
	{ 1, 0, CHESS_HORSE },
	{ 2, 0, CHESS_ELEPHANT },
	{ 3, 0, CHESS_KING },
	{ 4, 0, CHESS_QUEEN },
	{ 5, 0, CHESS_ELEPHANT },
	{ 6, 0, CHESS_HORSE },
	{ 7, 0, CHESS_ROOK },
	{ 0, 1, CHESS_SOLDIER },
	{ 1, 1, CHESS_SOLDIER },
	{ 2, 1, CHESS_SOLDIER },
	{ 3, 1, CHESS_SOLDIER },
	{ 4, 1, CHESS_SOLDIER },
	{ 5, 1, CHESS_SOLDIER },
	{ 6, 1, CHESS_SOLDIER },
	{ 7, 1, CHESS_SOLDIER },


};