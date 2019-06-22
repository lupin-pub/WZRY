#pragma once
#ifndef _PAO_CAR_H_
#define _PAO_CAR_H_

#include "BaseMonster.h"

class paoCar : public BaseMonster
{
public:
	//virtual void onEnter();
	static paoCar* createMonster(Point point, int color);// std::vector<Point> points, std::vector<std::vector<std::vector<Point>>> sonPath);
	virtual bool initMonster(int color);
	virtual void shoot(float delta);

};

#endif
