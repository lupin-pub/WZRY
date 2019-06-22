#ifndef _LONG_SOLDIER_H_
#define _LONG_SOLDIER_H_

#include "BaseMonster.h"

class LongSoldier : public BaseMonster
{
public:
	//virtual void onEnter();
	static LongSoldier* createMonster(Point point,int color);// std::vector<Point> points, std::vector<std::vector<std::vector<Point>>> sonPath);
	virtual bool initMonster(int color);

};

#endif
